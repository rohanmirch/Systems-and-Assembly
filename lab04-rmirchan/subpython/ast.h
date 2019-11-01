#ifndef AST_H
#define AST_H

#include <stdbool.h>
#include <stddef.h>

#include "types.h"

typedef enum NodeType {
    STMT_SEQUENCE,

    STMT_ASSIGN,
    STMT_DEL,
    STMT_IF,
    STMT_WHILE,

    STMT_SENTRY_LAST,       /*!< This is here to make implementing
                             *   `is_expression` simpler. */

    EXPR_LITERAL_STRING,
    EXPR_LITERAL_INTEGER,
    EXPR_LITERAL_FLOAT,
    EXPR_LITERAL_LIST,
    EXPR_LITERAL_DICT,
    EXPR_LITERAL_PAIR,
    EXPR_LITERAL_SINGLETON,

    EXPR_IDENTIFIER,
    EXPR_BUILTIN,
    EXPR_CALL,
    EXPR_SUBSCRIPT
} NodeType;

static inline bool is_statement(NodeType type) {
    return type <= STMT_SENTRY_LAST;
}

typedef struct Node {
    NodeType type;
} Node;

typedef struct NodeListEntry NodeListEntry;
struct NodeListEntry {
    NodeListEntry *next;
    Node *node;
    Reference reference; /* For temporarily storing evalulation results. */
    size_t idx;          /* For temporarily storing global indexes. */
};

typedef struct NodeList {
    NodeListEntry *head;
    NodeListEntry *tail;
} NodeList;

typedef struct NodeStmtSequence {
    NodeType type;
    NodeList *statements;
} NodeStmtSequence;

typedef struct NodeStmtAssign {
    NodeType type;
    Node *left;
    Node *right;
} NodeStmtAssign;

typedef struct NodeStmtDel {
    NodeType type;
    Node *arg;
} NodeStmtDel;

typedef struct NodeStmtIf {
    NodeType type;
    Node *cond;
    Node *left;
    Node *right;
} NodeStmtIf;

typedef struct NodeStmtWhile {
    NodeType type;
    Node *cond;
    Node *body;
} NodeStmtWhile;

typedef struct NodeExprLiteralString {
    NodeType type;
    const char *value;
} NodeExprLiteralString;

typedef struct NodeExprLiteralInteger {
    NodeType type;
    long int value;
} NodeExprLiteralInteger;

typedef struct NodeExprLiteralFloat {
    NodeType type;
    double value;
} NodeExprLiteralFloat;

typedef struct NodeExprLiteralList {
    NodeType type;
    NodeList *values;
} NodeExprLiteralList;

typedef struct NodeExprLiteralDict {
    NodeType type;
    NodeList *values;
} NodeExprLiteralDict;

typedef struct NodeExprLiteralPair {
    NodeType type;
    Node *key;
    Node *value;
} NodeExprLiteralPair;

typedef enum SingletonType {
    S_NONE,
    S_TRUE,
    S_FALSE
} SingletonType;

typedef struct NodeExprLiteralSingleton {
    NodeType type;
    SingletonType singleton;
} NodeExprLiteralSingleton;


typedef struct NodeExprIdentifier {
    NodeType type;
    const char *name;
} NodeExprIdentifier;

typedef enum NodeExprBuiltinType {
    UOP_NEGATE,
    UOP_IDENTITY,
    UOP_NOT,

    COMP_EQUALS,
    COMP_LT,
    COMP_GT,
    COMP_LE,
    COMP_GE,

    OP_OR,
    OP_AND,

    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_MODULO,

    N_BUILTINS
} NodeExprBuiltinType;

static inline bool is_unary_builtin(NodeExprBuiltinType type) {
    return type == UOP_NEGATE || type == UOP_IDENTITY || type == UOP_NOT;
}

typedef struct NodeExprBuiltin {
    NodeType type;
    NodeExprBuiltinType builtin_type;
    Node *left;
    Node *right;
} NodeExprBuiltin;

typedef struct NodeExprCall {
    NodeType type;
    Node *func;
    NodeList *args;
} NodeExprCall;

typedef struct NodeExprSubscript {
    NodeType type;
    Node *obj;
    Node *index;
} NodeExprSubscript;

void *ast_create_pool();
void  ast_free_pool(void *pool);
void *ast_pool_alloc(void *pool, size_t sz);

NodeList *ast_alloc_nodelist(void *pool);

void ast_nodelist_append(void *pool, NodeList *list, Node *node);
size_t ast_nodelist_length(NodeList *list);

Node *ast_alloc_sequence(void *pool, NodeList *statements);
Node *ast_alloc_assign(void *pool, Node *left, Node *right);
Node *ast_alloc_del(void *pool, Node *arg);
Node *ast_alloc_if(void *pool, Node *cond, Node *left, Node *right);
Node *ast_alloc_while(void *pool, Node *cond, Node *body);

Node *ast_alloc_literal_string(void *pool, const char *value);
Node *ast_alloc_literal_integer(void *pool, long int value);
Node *ast_alloc_literal_float(void *pool, double value);
Node *ast_alloc_literal_list(void *pool, NodeList *values);
Node *ast_alloc_literal_dict(void *pool, NodeList *values);
Node *ast_alloc_literal_pair(void *pool, Node *key, Node *value);
Node *ast_alloc_literal_singleton(void *pool, SingletonType type);

Node *ast_alloc_identifier(void *pool, const char *name);
Node *ast_alloc_builtin(void *pool, NodeExprBuiltinType type, Node *left, Node *right);
Node *ast_alloc_call(void *pool, Node *func, NodeList *args);
Node *ast_alloc_subscript(void *pool, Node *obj, Node *index);

#endif /* AST_H */
