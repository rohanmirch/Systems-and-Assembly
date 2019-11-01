#include "ast.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define AST_POOL_PGSIZE 4096

typedef struct AstPoolPage AstPoolPage;
struct AstPoolPage {
    AstPoolPage *prev;
    size_t offset;
    char data[];
};

typedef struct AstPool {
    AstPoolPage *page;
} AstPool;

void *ast_create_pool() {
    return calloc(1, sizeof(AstPool));
}
void ast_free_pool(void *ptr) {
    AstPool *pool = ptr;

    if (pool) {
        AstPoolPage *page = pool->page;
        while (page) {
            void *temp = page->prev;
            free(page);
            page = temp;
        }
        free(pool);
    }
}
void *ast_pool_alloc(void *ptr, size_t sz) {
    assert(ptr != NULL);

    AstPool *pool = ptr;
    AstPoolPage *page = pool->page;

    if (!page || AST_POOL_PGSIZE - page->offset < sz) {
        size_t req = sizeof(AstPoolPage) + sz;
        size_t min = sizeof(AstPoolPage) + AST_POOL_PGSIZE;

        AstPoolPage *new = calloc(1, req < min ? min : req);
        if (new) {
            new->offset = sz;
            if (req > min && page) {
                new->prev = page->prev;
                page->prev = new;
            } else {
                new->prev = page;
                pool->page = page = new;
            }

            return new->data;
        } else {
            return NULL;
        }
    } else {
        void *alloc = page->data + page->offset;
        page->offset += sz;
        return alloc;
    }
}


static char *ast_pool_strcpy(void *ptr, const char *str) {
    assert(str != NULL);

    size_t len = strlen(str);
    char *dest = ast_pool_alloc(ptr, len + 1);
    if (dest) {
        memcpy(dest, str, len + 1);
    }
    return dest;
}

NodeList *ast_alloc_nodelist(void *pool) {
    NodeList *list = ast_pool_alloc(pool, sizeof(*list));
    if (list) {
        memset(list, 0, sizeof(*list));
    }
    return list;
}

static NodeListEntry *ast_alloc_nodelistentry(void *pool, Node *node) {
    NodeListEntry *entry = ast_pool_alloc(pool, sizeof(*entry));
    if (entry) {
        entry->next = NULL;
        entry->node = node;
    }
    return entry;
}

void ast_nodelist_append(void *pool, NodeList *list, Node *node) {
    assert(list != NULL);

    NodeListEntry *entry = ast_alloc_nodelistentry(pool, node);
    if (list->tail) {
        list->tail->next = entry;
        list->tail = entry;
    } else {
        list->head = list->tail = entry;
    }
}

size_t ast_nodelist_length(NodeList *list) {
    assert(list != NULL);

    size_t length = 0;
    for (NodeListEntry *entry = list->head;
            entry;
            entry = entry->next, length++);

    return length;
}

#define AST_NODE_DECL(TYPENAME, TYPE) \
    TYPENAME *node = (TYPENAME *) ast_alloc_node(pool, sizeof(TYPENAME), TYPE)

static Node *ast_alloc_node(void *pool, size_t size, NodeType type) {
    Node *node = ast_pool_alloc(pool, size);
    if (node) {
        node->type = type;
    }
    return node;
}

Node *ast_alloc_sequence(void *pool, NodeList *statements) {
    AST_NODE_DECL(NodeStmtSequence, STMT_SEQUENCE);
    if (node) {
        node->statements = statements;
    }
    return (Node *) node;
}

Node *ast_alloc_assign(void *pool, Node *left, Node *right) {
    AST_NODE_DECL(NodeStmtAssign, STMT_ASSIGN);
    if (node) {
        node->left = left;
        node->right = right;
    }
    return (Node *) node;
}
Node *ast_alloc_del(void *pool, Node *arg) {
    AST_NODE_DECL(NodeStmtDel, STMT_DEL);
    if (node) {
        node->arg = arg;
    }
    return (Node *) node;
}
Node *ast_alloc_if(void *pool, Node *cond, Node *left, Node *right) {
    AST_NODE_DECL(NodeStmtIf, STMT_IF);
    if (node) {
        node->cond = cond;
        node->left = left;
        node->right = right;
    }
    return (Node *) node;
}
Node *ast_alloc_while(void *pool, Node *cond, Node *body) {
    AST_NODE_DECL(NodeStmtWhile, STMT_WHILE);
    if (node) {
        node->cond = cond;
        node->body = body;
    }
    return (Node *) node;
}

Node *ast_alloc_literal_string(void *pool, const char *value) {
    AST_NODE_DECL(NodeExprLiteralString, EXPR_LITERAL_STRING);
    if (node) {
        node->value = ast_pool_strcpy(pool, value);
    }
    return (Node *) node;
}
Node *ast_alloc_literal_integer(void *pool, long int value) {
    AST_NODE_DECL(NodeExprLiteralInteger, EXPR_LITERAL_INTEGER);
    if (node) {
        node->value = value;
    }
    return (Node *) node;
}
Node *ast_alloc_literal_float(void *pool, double value) {
    AST_NODE_DECL(NodeExprLiteralFloat, EXPR_LITERAL_FLOAT);
    if (node) {
        node->value = value;
    }
    return (Node *) node;
}
Node *ast_alloc_literal_list(void *pool, NodeList *values) {
    AST_NODE_DECL(NodeExprLiteralList, EXPR_LITERAL_LIST);
    if (node) {
        node->values = values;
    }
    return (Node *) node;
}
Node *ast_alloc_literal_dict(void *pool, NodeList *values) {
    AST_NODE_DECL(NodeExprLiteralDict, EXPR_LITERAL_DICT);
    if (node) {
        node->values = values;
    }
    return (Node *) node;
}
Node *ast_alloc_literal_pair(void *pool, Node *key, Node *value) {
    AST_NODE_DECL(NodeExprLiteralPair, EXPR_LITERAL_PAIR);
    if (node) {
        node->key = key;
        node->value = value;
    }
    return (Node *) node;
}
Node *ast_alloc_literal_singleton(void *pool, SingletonType type) {
    AST_NODE_DECL(NodeExprLiteralSingleton, EXPR_LITERAL_SINGLETON);
    if (node) {
        node->singleton = type;
    }
    return (Node *) node;
}

Node *ast_alloc_identifier(void *pool, const char *name) {
    AST_NODE_DECL(NodeExprIdentifier, EXPR_IDENTIFIER);
    if (node) {
        node->name = ast_pool_strcpy(pool, name);
    }
    return (Node *) node;
}
Node *ast_alloc_builtin(void *pool, NodeExprBuiltinType type, Node *left, Node *right) {
    AST_NODE_DECL(NodeExprBuiltin, EXPR_BUILTIN);
    if (node) {
        node->builtin_type = type;
        node->left = left;
        node->right = right;
    }
    return (Node *) node;
}

Node *ast_alloc_call(void *pool, Node *func, NodeList *args) {
    AST_NODE_DECL(NodeExprCall, EXPR_CALL);
    if (node) {
        node->func = func;
        node->args = args;
    }
    return (Node *) node;
}

Node *ast_alloc_subscript(void *pool, Node *obj, Node *index) {
    AST_NODE_DECL(NodeExprSubscript, EXPR_SUBSCRIPT);
    if (node) {
        node->obj = obj;
        node->index = index;
    }
    return (Node *) node;
}
