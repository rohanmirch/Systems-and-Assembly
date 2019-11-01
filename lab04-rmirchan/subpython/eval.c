#include "eval.h"

#include <assert.h>
#include <math.h>
#include <string.h>

#include "alloc.h"
#include "ast.h"
#include "global.h"

/* Global variable information. */

#define MAX_DEPTH 4

struct GlobalVariable {
    char *name;
    Reference ref;
} *global_vars = NULL;

int num_vars = 0;
int max_vars = 0;

//////////// EVALUATION ENGINE ////////////

typedef enum EvaluationStatus {
    EVAL_NORMAL,
    EVAL_RETURN,
    EVAL_BREAK,
    EVAL_CONTINUE
} EvaluationStatus;

typedef struct EvaluationResult {
    EvaluationStatus status;
    Reference result;
} EvaluationResult;

//// PRIVATE VARIABLE DECLARATIONS ////

/* These are references held by the evaluation engine to the singletons
 * None, True, and False. These references should never be collected
 * since they are always considered globals and should never changes because
 * reference numbers should never change! */
static Reference NONE_REF = NULL_REF;
static Reference TRUE_REF = NULL_REF;
static Reference FALSE_REF = NULL_REF;


bool ref_is_none(Reference r) {
    return r == NONE_REF;
}
bool ref_is_true(Reference r) {
    return r == TRUE_REF;
}
bool ref_is_false(Reference r) {
    return r == FALSE_REF;
}

Reference get_bool_ref(bool value) {
    return value ? TRUE_REF : FALSE_REF;
}

//// LOCAL FUNCTION DECLARATIONS ////

Reference *add_global_variable(const char *name, Reference value);
Reference *get_global_variable(const char *name, bool create);
void delete_global_variable(const char *name);

int add_temporary_global(Reference value);
void remove_temporary_global(size_t glob);

EvaluationResult eval_main(Node *node);
EvaluationResult eval_del(NodeStmtDel *node);

Reference eval_expr(Node *node);
Reference *eval_expr_lval(Node *node, bool create);

static bool eval_generic_comp(NodeExprBuiltinType type,
                              Reference l, Reference r);

Reference make_reference_none();
Reference make_reference_bool();
Reference make_reference_int(long int v);
Reference make_reference_float(double f);
Reference make_reference_string(const char *value);
Reference make_reference_string_concat(const char *v1, const char *v2);
Reference make_reference_list_node(Reference value);
Reference make_reference_dict_node(Reference key, Reference value);


//// HELPER FUNCTIONS ////

typedef enum Promotion {
    NO_PROMOTE,
    TO_FLOAT,
    TO_INTEGER,
} Promotion;

static bool is_numeric(Reference r) {
    ValueType type = deref(r)->type;
    return type == VAL_INTEGER || type == VAL_FLOAT;
}
static bool is_int(Reference r) {
    return deref(r)->type == VAL_INTEGER;
}
static bool is_float(Reference r) {
    return deref(r)->type == VAL_FLOAT;
}


static const char *get_typestr(Reference r) {
    switch (deref(r)->type) {
        case VAL_NONE:      return "NoneType";
        case VAL_BOOL:      return "bool";
        case VAL_INTEGER:   return "int";
        case VAL_FLOAT:     return "float";
        case VAL_STRING:    return "str";
        case VAL_LIST_NODE: return "list";
        case VAL_DICT_NODE: return "dict";
        default:            return "<unknown>";
    }
}

Promotion get_promotion(Reference l, Reference r) {
    if (r == NULL_REF) {
        if (is_float(l)) {
            return TO_FLOAT;
        } else if (is_int(l)) {
            return TO_INTEGER;
        } else {
            return NO_PROMOTE;
        }
    } else if (is_numeric(l) && is_numeric(r)) {
        return is_float(l) || is_float(r) ? TO_FLOAT : TO_INTEGER;
    } else {
        return NO_PROMOTE;
    }
}

ListValue *to_list_value(Value *v) {
    assert(v == NULL || v->type == VAL_LIST_NODE);
    return (ListValue *) v;
}

ListValue *deref_to_list_value(Reference ref) {
    return to_list_value(deref(ref));
}


DictValue *to_dict_value(Value *v) {
    assert(v == NULL || v->type == VAL_DICT_NODE);
    return (DictValue *) v;
}

DictValue *deref_to_dict_value(Reference ref) {

    return to_dict_value(deref(ref));
}


/*!
 * Returns the length of the list, not including the dummy start value.
 */
long int list_get_length(Reference ref) {
    ListValue *elem = deref_to_list_value(ref);

    /* Skip over the dummy value. */
    elem = deref_to_list_value(elem->list_node.next);

    long int length = 0;
    while (elem) {
        elem = deref_to_list_value(elem->list_node.next);
        length++;
    }

    return length;
}

/*!
 * Returns the element of the list at index idx, or reports an error if the
 * list doesn't have an element at that index.
 */
ListValue *list_get_elem(Reference ref, long int idx) {
    /* If the index is negative, then figure out the length of the list
     * and compute the true index. */
    if (idx < 0) {
        idx += list_get_length(ref);
    }

    /* If the index is still negative, then we are out of bounds. */
    if (idx < 0) {
        error("list index out of range");
    }

    /* The first node in a list is always a dummy value, so we can ignore it.
     * (Also, verify that it actually doesn't hold anything.) */
    ListValue *elem = deref_to_list_value(ref);
    assert(elem->list_node.value == NULL_REF);

    /* Now traverse (from -1 to skip the dummy element) to the element we
     * want. */
    for (int i = -1; elem && i < idx; i++) {
        ref = elem->list_node.next;
        elem = deref_to_list_value(ref);
    }

    if (elem == NULL) {
        error("list index out of range");
    }

    return elem;
}

void list_delete_elem(Reference ref, long int idx) {
    /* If the index is negative, then figure out the length of the list
     * and compute the true index. */
    if (idx < 0) {
        idx += list_get_length(ref);
    }

    /* If the index is still negative, then we are out of bounds. */
    if (idx < 0) {
        error("list index out of range");
    }

    /* The first node in a list is always a dummy value, so we can ignore it.
     * (Also, verify that it actually doesn't hold anything.) */
    ListValue *elem = deref_to_list_value(ref);
    assert(elem->list_node.value == NULL_REF);

    /* Now traverse (from -1 to skip the dummy element) to the element right
     * before then one we want. */
    for (int i = -1; elem && i < idx - 1; i++) {
        ref = elem->list_node.next;
        elem = deref_to_list_value(ref);
    }

    if (elem == NULL) {
        error("list index out of range");
    }

    /* Now get the actual element we are trying to delete. */
    ListValue *next = deref_to_list_value(elem->list_node.next);

    if (next == NULL) {
        error("list index out of range");
    }

    /* Remove this element from the list. */
    elem->list_node.next = next->list_node.next;
}


/*!
 * Returns the length of the dict, not including the dummy start value.
 */
long int dict_get_length(Reference ref) {
    DictValue *elem = deref_to_dict_value(ref);

    /* Skip over the dummy value. */
    elem = deref_to_dict_value(elem->dict_node.next);

    long int length = 0;
    while (elem) {
        elem = deref_to_dict_value(elem->dict_node.next);
        length++;
    }

    return length;
}


DictValue *dict_get_entry(Reference ref, Reference key, bool create) {

    /* The first node in a dictionary is always a dummy value, so we can
     * ignore it.  (Also, verify that it actually doesn't hold anything.)
     */
    DictValue *start = deref_to_dict_value(ref);
    DictValue *entry = start;
    DictValue *prev;
    assert(start->dict_node.key == NULL_REF);
    assert(start->dict_node.value == NULL_REF);

    /* Move past the first entry. */
    entry = deref_to_dict_value(entry->dict_node.next);
    prev = start;

    /* Iterate until we find our key, or until we reach the end of the
     * dictionary's entries. */
    while (entry != NULL) {
        if (eval_generic_comp(COMP_EQUALS, entry->dict_node.key, key)) {
            break;
        }

        prev = entry;
        entry = deref_to_dict_value(entry->dict_node.next);
    }

    /* If we got NULL, then that means our key is missing. */
    if (entry == NULL) {
        if (!create) {
            /* The caller wants us to report an error. */
            error("key not found");
        }
        else {
            /* The caller wants us to create a new entry.  Tack it onto the
             * end of the dictionary.  Set the key, but we don't know what
             * value it should have yet.
             */

            Reference entry_ref = make_reference_dict_node(key, NULL_REF);
            entry = (DictValue *) deref(entry_ref);

            assert(prev != NULL);
            assert(prev->dict_node.next == NULL_REF);
            prev->dict_node.next = entry_ref;
        }
    }

    return entry;
}



void dict_delete_entry(Reference ref, Reference key) {
    (void) ref;
    (void) key;

    /* The first node in a dictionary is always a dummy value, so we can
     * ignore it.  (Also, verify that it actually doesn't hold anything.) */
    DictValue *prev;
    DictValue *entry = deref_to_dict_value(ref);
    assert(entry->dict_node.key == NULL_REF);
    assert(entry->dict_node.value == NULL_REF);

    /* Move past the first entry. */
    prev = entry;
    entry = deref_to_dict_value(entry->dict_node.next);

    /* Iterate until we find our key, or until we reach the end of the
     * dictionary's entries. */
    while (entry != NULL) {
        if (eval_generic_comp(COMP_EQUALS, entry->dict_node.key, key)) {
            break;
        }

        prev = entry;
        entry = deref_to_dict_value(entry->dict_node.next);
    }

    /* If we got NULL, then that means our key is missing. */
    if (entry == NULL) {
        error("key not found");
    }

    /* Otherwise, remove the entry. */
    prev->dict_node.next = entry->dict_node.next;
}


//// REFERENCE COERCION ////

static inline bool coerce_ref_to_bool(Reference l) {
    Value *v = deref(l);

    switch (v->type) {
        case VAL_NONE:
            return false;
        case VAL_BOOL:
            return l == TRUE_REF;
        case VAL_INTEGER:
            return ((IntegerValue *) v)->integer_value;
        case VAL_FLOAT:
            return ((FloatValue *) v)->float_value;
        case VAL_STRING:
            return strlen(((StringValue *) v)->string_value) > 0;
        case VAL_LIST_NODE:
            return ((ListValue *) v)->list_node.next != NULL_REF;
        case VAL_DICT_NODE:
            return ((DictValue *) v)->dict_node.next != NULL_REF;
        default:
            error("cannot coerce '%s' to bool", get_typestr(l));
    }
}
static inline double coerce_ref_to_float(Reference l) {
    Value *v = deref(l);

    switch (v->type) {
        case VAL_INTEGER:
            return (double) ((IntegerValue *) v)->integer_value;
        case VAL_FLOAT:
            return ((FloatValue *) v)->float_value;
        default:
            error("cannot coerce '%s' to float", get_typestr(l));
    }
}
static inline long int coerce_ref_to_int(Reference l) {
    Value *v = deref(l);

    switch (v->type) {
        case VAL_INTEGER:
            return ((IntegerValue *) v)->integer_value;
        case VAL_FLOAT:
            return (long int) ((FloatValue *) v)->float_value;
        default:
            error("cannot coerce '%s' to int", get_typestr(l));
    }
}


//// PRINTING CODE ////

void ref_print_ext(FILE *os, Reference ref, bool newline, int depth);

void list_print(FILE *os, Reference ref, int depth) {
    bool first = true;

    /* The first element is a dummy element.  Skip it. */
    ListValue *lv = deref_to_list_value(ref);
    assert(lv->list_node.value == NULL_REF);
    lv = deref_to_list_value(lv->list_node.next);

    while (lv != NULL) {
        if (first) {
            first = false;
        } else {
            fprintf(os, ", ");
        }

        if (depth != 0) {
            ref_print_ext(os, lv->list_node.value, false, depth - 1);
        } else {
            fprintf(os, "...");
        }

        lv = deref_to_list_value(lv->list_node.next);
    }
}

void dict_print(FILE *os, Reference ref, int depth) {
    bool first = true;

    /* The first entry is a dummy entry.  Skip it. */
    DictValue *dv = deref_to_dict_value(ref);
    assert(dv->dict_node.key == NULL_REF);
    assert(dv->dict_node.value == NULL_REF);
    dv = deref_to_dict_value(dv->dict_node.next);

    while (dv != NULL) {
        if (first) {
            first = false;
        } else {
            fprintf(os, ", ");
        }

        /* depth irrelevant for keys */
        ref_print_ext(os, dv->dict_node.key, false, 0);

        fprintf(os, ": ");

        if (depth != 0) {
            ref_print_ext(os, dv->dict_node.value, false, depth - 1);
        } else {
            fprintf(os, "...");
        }

        dv = deref_to_dict_value(dv->dict_node.next);
    }
}

void ref_print_ext(FILE *os, Reference ref, bool newline, int depth) {
    Value *v = deref(ref);
    switch (v->type) {
        case VAL_NONE:
            fprintf(os, "None");
            break;

        case VAL_BOOL:
            fprintf(os, ref == TRUE_REF ? "True" : "False");
            break;

        case VAL_INTEGER:
            fprintf(os, "%d", ((IntegerValue *) v)->integer_value);
            break;

        case VAL_FLOAT:
            fprintf(os, "%f", ((FloatValue *) v)->float_value);
            break;

        case VAL_STRING:
            fprintf(os, "\"%s\"", ((StringValue *) v)->string_value);
            break;

        case VAL_LIST_NODE:
            fprintf(os, "[");
            list_print(os, ref, depth);
            fprintf(os, "]");
            break;

        case VAL_DICT_NODE:
            fprintf(os, "{");
            dict_print(os, ref, depth);
            fprintf(os, "}");
            break;

        default:
            fprintf(os, "Unrecognized value type\n");
            break;
    }

    if (newline) {
        fprintf(os, "\n");
    }
}

void ref_print(FILE *os, Reference ref) {
    ref_print_ext(os, ref, false, MAX_DEPTH);
}
void ref_println(FILE *os, Reference ref) {
    ref_print_ext(os, ref, true, MAX_DEPTH);
}

//// COMPARISON SYSTEM ////
static const char *eval_generic_str(NodeExprBuiltinType type) {
    switch (type) {
        case COMP_EQUALS:   return "==";
        case COMP_LT:       return "<";
        case COMP_GT:       return ">";
        case COMP_LE:       return "<=";
        case COMP_GE:       return ">=";
        case OP_ADD:        return "+";
        case OP_SUBTRACT:   return "-";
        case OP_MULTIPLY:   return "*";
        case OP_DIVIDE:     return "/";
        case OP_MODULO:     return "%";
        default:            return "?";
    }
}
noreturn static Reference eval_generic_error(NodeExprBuiltinType type,
                                             Reference l, Reference r) {
    error("unsupported operand type(s) for %s: '%s' and '%s'",
            eval_generic_str(type), get_typestr(l), get_typestr(r));
}

static bool eval_generic_comp_float(NodeExprBuiltinType type,
                                    Reference l, Reference r) {

    double lval = coerce_ref_to_float(l);
    double rval = coerce_ref_to_float(r);

    switch (type) {
        case COMP_EQUALS:   return lval == rval;
        case COMP_LT:       return lval < rval;
        case COMP_GT:       return lval > rval;
        case COMP_LE:       return lval <= rval;
        case COMP_GE:       return lval >= rval;
        default:
            eval_generic_error(type, l, r);
    }
}

static bool eval_generic_comp_int(NodeExprBuiltinType type,
                                  Reference l, Reference r) {

    long int lval = coerce_ref_to_int(l);
    long int rval = coerce_ref_to_int(r);

    switch (type) {
        case COMP_EQUALS:   return lval == rval;
        case COMP_LT:       return lval < rval;
        case COMP_GT:       return lval > rval;
        case COMP_LE:       return lval <= rval;
        case COMP_GE:       return lval >= rval;
        default:
            eval_generic_error(type, l, r);
    }
}

static bool eval_generic_comp_string(NodeExprBuiltinType type,
                                     Reference l, Reference r) {

    const char *lval = ((StringValue *) deref(l))->string_value;
    const char *rval = ((StringValue *) deref(r))->string_value;
    int res = strcmp(lval, rval);

    switch (type) {
        case COMP_EQUALS:   return res == 0;
        case COMP_LT:       return res < 0;
        case COMP_GT:       return res > 0;
        case COMP_LE:       return res <= 0;
        case COMP_GE:       return res >= 0;
        default:
            eval_generic_error(type, l, r);
    }
}

static bool eval_generic_comp_list(NodeExprBuiltinType type,
                                   Reference l, Reference r) {
    if (l == NULL_REF) {
        switch (type) {
            case COMP_EQUALS:   return r == NULL_REF;
            case COMP_LT:       return r != NULL_REF;
            case COMP_GT:       return false;
            case COMP_LE:       return true;
            case COMP_GE:       return r == NULL_REF;
            default:
                eval_generic_error(type, l, r);
        }
    } else if (r == NULL_REF) {
        switch (type) {
            case COMP_EQUALS:   return false;
            case COMP_LT:       return false;
            case COMP_GT:       return true;
            case COMP_LE:       return false;
            case COMP_GE:       return true;
            default:
                eval_generic_error(type, l, r);
        }
    }

    ListValue *llist = deref_to_list_value(l);
    ListValue *rlist = deref_to_list_value(r);

    Reference lval = llist->list_node.value;
    Reference rval = rlist->list_node.value;
    Reference lnext = llist->list_node.next;
    Reference rnext = rlist->list_node.next;

    /* Advance past the dummy element at the start of all lists. */
    if (lval == NULL_REF || rval == NULL_REF) {
        return eval_generic_comp_list(type, lnext, rnext);
    }

    /* Otherwise compare the current element and potentially check the
     * next in accordance to lexicographical sorting. */
    switch (type) {
        case COMP_EQUALS:
            return eval_generic_comp(type, lval, rval) &&
                        eval_generic_comp_list(type, lnext, rnext);
        case COMP_LT:
        case COMP_GT:
        case COMP_LE:
        case COMP_GE:
            return eval_generic_comp(type, lval, rval) ||
                        eval_generic_comp_list(type, lnext, rnext);
        default:
            eval_generic_error(type, l, r);
    }
}

static bool eval_generic_comp(NodeExprBuiltinType type,
                              Reference l, Reference r) {

    Promotion promo = get_promotion(l, r);

    switch (promo) {
        case TO_FLOAT:
            return eval_generic_comp_float(type, l, r);

        case TO_INTEGER:
            return eval_generic_comp_int(type, l, r);

        default: {
            Value *lv = deref(l);
            Value *rv = deref(r);

            if (lv->type == rv->type) {
                switch (lv->type) {
                    case VAL_STRING:
                        return eval_generic_comp_string(type, l, r);

                    case VAL_LIST_NODE:
                        return eval_generic_comp_list(type, l, r);

                    default:
                        eval_generic_error(type, l, r);
                }
            } else {
                return false;
            }
        }
    }
}

static Reference eval_generic_comp_ref(NodeExprBuiltinType type,
                                       Reference l, Reference r) {
    return get_bool_ref(eval_generic_comp(type, l, r));
}

static Reference eval_generic_comp_nodes(NodeExprBuiltinType type,
                                         Node *l, Node *r) {
    Reference lref = eval_expr(l);

    /* Save the left size to a temporary so that it doesn't get collected
     * by the right side evauation. */
    size_t tglob_idx = add_temporary_global(lref);

    /* Now attempt to comparison. */
    Reference result = eval_generic_comp_ref(type, lref, eval_expr(r));

    /* Clean up the left hand size. */
    remove_temporary_global(tglob_idx);

    return result;
}



//// EVALUATION IMPLEMENTATION ////


/*! Initialize the evaluation engine.
 *
 *  This function initializes the standard Python global singletons
 *  None, True and False, which are tied to to respective names and cannot
 *  be deleted. */
void eval_init() {
    add_global_variable("None",  NONE_REF = make_reference_none());
    add_global_variable("True",  TRUE_REF = make_reference_bool(true));
    add_global_variable("False", FALSE_REF = make_reference_bool(false));
}

/*! Entry point to the evaluation system. */
Reference eval_root(Node *root) {
    return eval_main(root).result;
}

EvaluationResult eval_main(Node *node) {
    assert(node != NULL);

    if (is_statement(node->type)) {
        switch (node->type) {
            case STMT_SEQUENCE: {
                NodeStmtSequence *sequence = (NodeStmtSequence *) node;

                for (NodeListEntry *current = sequence->statements->head;
                        current;
                        current = current->next) {
                    eval_main(current->node); // TODO: Status?
                }
                break;
            }

            case STMT_ASSIGN: {
                NodeStmtAssign *assign = (NodeStmtAssign *) node;

                Reference rref = eval_expr(assign->right);
                Reference *lref = eval_expr_lval(assign->left, true);

                /* Checking for invalid assignments should have been
                 * done in `eval_expr_lval` which will refuse to evalate
                 * non-lval eligible expressions so we should be fine
                 * just updating here. */
                *lref = rref;
                break;
            }

            case STMT_DEL:
                eval_del((NodeStmtDel *) node);
                break;

            case STMT_IF: {
                NodeStmtIf *ifnode = (NodeStmtIf *) node;

                if (coerce_ref_to_bool(eval_expr(ifnode->cond))) {
                    eval_main(ifnode->left); // TODO: Status?
                } else if (ifnode->right) {
                    eval_main(ifnode->right);
                }
                break;
            }

            case STMT_WHILE: {
                NodeStmtWhile *wnode = (NodeStmtWhile *) node;

                while (coerce_ref_to_bool(eval_expr(wnode->cond))) {
                    eval_main(wnode->body);
                }
                break;
            }

            default:
                error("unimplemented: %d", node->type);
        }

        return (EvaluationResult) {
            .result = NULL_REF
        };
    } else {
        Reference result = eval_expr(node);
        if (result == NULL_REF) {
            error("unexpected NULL reference!");
        } else if (result != NONE_REF) {
            ref_println(stdout, result);
        }

        return (EvaluationResult) {
            .result = result
        };
    }
}

EvaluationResult eval_del(NodeStmtDel *node) {
    /* For the deletion statement, we need to check the type of the right
     * hand parse in order to know what to do. */
    switch (node->arg->type) {
        case EXPR_LITERAL_STRING:
        case EXPR_LITERAL_INTEGER:
        case EXPR_LITERAL_FLOAT:
        case EXPR_LITERAL_SINGLETON:
            error("cannot delete literal");

        case EXPR_LITERAL_DICT:
        case EXPR_LITERAL_LIST:
            error("deletion destructuring not implemented");

        case EXPR_LITERAL_PAIR:
            error("unexpected pair");

        case EXPR_IDENTIFIER:
            delete_global_variable(((NodeExprIdentifier *) node->arg)->name);
            break;

        case EXPR_BUILTIN:
            error("cannot delete result of expression");

        case EXPR_CALL:
            error("cannot delete result of call");

        case EXPR_SUBSCRIPT: {
            NodeExprSubscript *subscript = (NodeExprSubscript *) node->arg;
            Reference keyref = eval_expr(subscript->index);
            Reference objref = *eval_expr_lval(subscript->obj, false);
            Value *objv = deref(objref);

            switch (objv->type) {
                case VAL_LIST_NODE:
                    list_delete_elem(objref, coerce_ref_to_int(keyref));
                    break;

                case VAL_DICT_NODE:
                    dict_delete_entry(objref, keyref);
                    break;

                default:
                    error("'%s' does not support item deletion",
                            get_typestr(objref));
            }
            break;
        }

        default:
            error("unimplemented expr lval `%d`", node->type);
    }

    return (EvaluationResult) {
        .result = NULL_REF
    };
}

/* Here are many definitions for builtin functions that implement
 * basic operations like `not` or `+`. Unfortunately, because these operations
 * are all slightly different, there is alot of duplicated code here. :( */

static Reference eval_builtin_negate(Node *l, Node *r) {
    (void) r;

    Reference lref = eval_expr(l);
    Value *lv = deref(lref);

    switch (lv->type) {
        case VAL_FLOAT:
            return make_reference_float(-((FloatValue *) lv)->float_value);
        case VAL_INTEGER:
            return make_reference_int(-((IntegerValue *) lv)->integer_value);

        default:
            error("unsupported operand type(s) for unary -: '%s'",
                            get_typestr(lref));
    }
}

static Reference eval_builtin_identity(Node *l, Node *r) {
    (void) r;

    Reference lref = eval_expr(l);
    Value *lv = deref(lref);

    switch (lv->type) {
        case VAL_FLOAT:
        case VAL_INTEGER:
            return lref;

        default:
            error("unsupported operand type(s) for unary +: '%s'",
                            get_typestr(lref));
    }
}

static Reference eval_builtin_not(Node *l, Node *r) {
    (void) r;

    return get_bool_ref(!coerce_ref_to_bool(eval_expr(l)));
}

static Reference eval_builtin_eq(Node *l, Node *r) {
    return eval_generic_comp_nodes(COMP_EQUALS, l, r);
}
static Reference eval_builtin_lt(Node *l, Node *r) {
    return eval_generic_comp_nodes(COMP_LT, l, r);
}
static Reference eval_builtin_gt(Node *l, Node *r) {
    return eval_generic_comp_nodes(COMP_GT, l, r);
}
static Reference eval_builtin_le(Node *l, Node *r) {
    return eval_generic_comp_nodes(COMP_LE, l, r);
}
static Reference eval_builtin_ge(Node *l, Node *r) {
    return eval_generic_comp_nodes(COMP_GE, l, r);
}


static Reference eval_builtin_or(Node *l, Node *r) {
    Reference lref = eval_expr(l);
    return coerce_ref_to_bool(lref) ? lref : eval_expr(r);
}

static Reference eval_builtin_and(Node *l, Node *r) {
    Reference lref = eval_expr(l);
    return coerce_ref_to_bool(lref) ? eval_expr(r) : lref;
}

static Reference eval_builtin_add(Node *l, Node *r) {
    Reference lref = eval_expr(l);
    size_t tglob_idx = add_temporary_global(lref);

    Reference rref = eval_expr(r);
    Promotion promo = get_promotion(lref, rref);
    Reference result;

    switch (promo) {
        case TO_FLOAT:
            result = make_reference_float(coerce_ref_to_float(lref) +
                                          coerce_ref_to_float(rref));
            break;

        case TO_INTEGER:
            result = make_reference_int(coerce_ref_to_int(lref) +
                                        coerce_ref_to_int(rref));
            break;

        default: {
            Value *lv = deref(lref);
            Value *rv = deref(rref);

            if (lv->type == rv->type) {
                switch (lv->type) {
                    case VAL_STRING:
                        result = make_reference_string_concat(
                                ((StringValue *) lv)->string_value,
                                ((StringValue *) rv)->string_value);
                        break;

                    /* case VAL_LIST_NODE: */
                    /* case VAL_DICT_NODE: */

                    default:
                        eval_generic_error(OP_ADD, lref, rref);
                }
            } else {
                eval_generic_error(OP_ADD, lref, rref);
            }
        }
    }

    remove_temporary_global(tglob_idx);
    return result;
}

static Reference eval_builtin_subtract(Node *l, Node *r) {
    Reference lref = eval_expr(l);
    size_t tglob_idx = add_temporary_global(lref);

    Reference rref = eval_expr(r);
    Promotion promo = get_promotion(lref, rref);
    Reference result;

    switch (promo) {
        case TO_FLOAT:
            result = make_reference_float(coerce_ref_to_float(lref) -
                                          coerce_ref_to_float(rref));
            break;

        case TO_INTEGER:
            result = make_reference_int(coerce_ref_to_int(lref) -
                                        coerce_ref_to_int(rref));
            break;

        default:
            eval_generic_error(OP_SUBTRACT, lref, rref);
    }

    remove_temporary_global(tglob_idx);
    return result;
}

static Reference eval_builtin_multiply(Node *l, Node *r) {
    Reference lref = eval_expr(l);
    size_t tglob_idx = add_temporary_global(lref);

    Reference rref = eval_expr(r);
    Promotion promo = get_promotion(lref, rref);
    Reference result;

    switch (promo) {
        case TO_FLOAT:
            result = make_reference_float(coerce_ref_to_float(lref) *
                                          coerce_ref_to_float(rref));
            break;

        case TO_INTEGER:
            result = make_reference_int(coerce_ref_to_int(lref) *
                                        coerce_ref_to_int(rref));
            break;

        default:
            eval_generic_error(OP_MULTIPLY, lref, rref);
    }

    remove_temporary_global(tglob_idx);
    return result;
}

static Reference eval_builtin_divide(Node *l, Node *r) {
    Reference lref = eval_expr(l);
    size_t tglob_idx = add_temporary_global(lref);

    Reference rref = eval_expr(r);
    Promotion promo = get_promotion(lref, rref);
    Reference result;

    switch (promo) {
        case TO_FLOAT:
        case TO_INTEGER:
            result = make_reference_float(coerce_ref_to_float(lref) /
                                          coerce_ref_to_float(rref));
            break;

        default:
            eval_generic_error(OP_DIVIDE, lref, rref);
    }

    remove_temporary_global(tglob_idx);
    return result;
}

static Reference eval_builtin_modulo(Node *l, Node *r) {
    Reference lref = eval_expr(l);
    size_t tglob_idx = add_temporary_global(lref);

    Reference rref = eval_expr(r);
    Promotion promo = get_promotion(lref, rref);
    Reference result;

    switch (promo) {
        case TO_FLOAT:
            result = make_reference_float(
                    fmod(coerce_ref_to_float(lref),
                         coerce_ref_to_float(rref)));
            break;

        case TO_INTEGER:
            result = make_reference_int(coerce_ref_to_int(lref) %
                                        coerce_ref_to_int(rref));
            break;

        default:
            eval_generic_error(OP_MODULO, lref, rref);
    }

    remove_temporary_global(tglob_idx);
    return result;
}

typedef Reference (*builtin_op)(Node *, Node *);
static const builtin_op builtins[N_BUILTINS] = {
    eval_builtin_negate,   /* UOP_NEGATE */
    eval_builtin_identity, /* UOP_IDENTITY */
    eval_builtin_not,      /* UOP_NOT */

    eval_builtin_eq,       /* COMP_EQUALS */
    eval_builtin_lt,       /* COMP_LT */
    eval_builtin_gt,       /* COMP_GT */
    eval_builtin_le,       /* COMP_LE */
    eval_builtin_ge,       /* COMP_GE */

    eval_builtin_or,       /* OP_OR */
    eval_builtin_and,      /* OP_AND */

    eval_builtin_add,      /* OP_ADD */
    eval_builtin_subtract, /* OP_SUBTRACT */
    eval_builtin_multiply, /* OP_MULTIPLY */
    eval_builtin_divide,   /* OP_DIVIDE */
    eval_builtin_modulo    /* OP_MODULO */
};

Reference eval_expr_builtin(NodeExprBuiltin *node) {
    return builtins[node->builtin_type](node->left, node->right);
}


static Reference eval_builtin_exit(size_t arity, NodeList *args) {
    if (arity > 1) {
        error("exit() takes from 0 to 1 positional arguments "
                    "but %d were given", arity);
    }

    int code = 0;
    if (arity == 1) {
        Reference coderef = args->head->reference;
        Value *codeval = deref(coderef);

        if (codeval->type == VAL_INTEGER) {
            code = ((IntegerValue *) codeval)->integer_value;
        } else {
            ref_println(stdout, coderef);
        }
    }

    exit(code);
}

static Reference eval_builtin_mem(size_t arity, NodeList *args) {
    (void) args;

    if (arity > 0) {
        error("mem() takes 0 positional arguments but %d were given", arity);
    }

    printf("%d\n", memuse());

    return NONE_REF;
}

static Reference eval_builtin_gc(size_t arity, NodeList *args) {
    (void) args;

    if (arity > 0) {
        error("gc() takes 0 positional arguments but %d were given", arity);
    }

    collect_garbage();

    return NONE_REF;
}

static Reference eval_builtin_print(size_t arity, NodeList *args) {
    if (arity > 0) {
        ref_print(stdout, args->head->reference);
        for (NodeListEntry *entry = args->head->next;
                entry; entry = entry->next) {
            fprintf(stdout, " ");
            ref_print(stdout, entry->reference);
        }
    }

    fprintf(stdout, "\n");

    return NONE_REF;
}

static Reference eval_builtin_len(size_t arity, NodeList *args) {
    if (arity != 1) {
        error("len() takes 1 positional argument but %d were given", arity);
    }

    Reference r = args->head->reference;
    Value *v = deref(args->head->reference);
    switch (v->type) {
        case VAL_STRING:
            return make_reference_int(v->data_size);

        case VAL_LIST_NODE:
            return make_reference_int(list_get_length(r));

        case VAL_DICT_NODE:
            return make_reference_int(dict_get_length(r));

        default:
            error("cannot get len() of '%s", get_typestr(r));
    }
}

Reference eval_expr_call(NodeExprCall *node) {
    // TODO: If a gc() is called during a function evaluation, then the
    // references to the function arguments will not be considered globals!
    // This means that the function arguments stored in the NodeList
    // will end up dangling, which is not desirable.

    /* Compute function arity and arguments. */
    size_t arity = 0;
    if (node->args) {
        for (NodeListEntry *entry = node->args->head;
                entry; entry = entry->next, arity++) {
            entry->reference = eval_expr(entry->node);
            entry->idx = add_temporary_global(entry->reference);
        }
    }

    if (node->func->type != EXPR_IDENTIFIER) {
        error("calling non-identifiers not yet supported");
    }

    const char *name = ((NodeExprIdentifier *) node->func)->name;
    Reference result;

    if (strcmp(name, "exit") == 0 || strcmp(name, "quit") == 0) {
        result = eval_builtin_exit(arity, node->args);
    } else if (strcmp(name, "mem") == 0) {
        result = eval_builtin_mem(arity, node->args);
    } else if (strcmp(name, "gc") == 0) {
        result = eval_builtin_gc(arity, node->args);
    } else if (strcmp(name, "print") == 0) {
        result = eval_builtin_print(arity, node->args);
    } else if (strcmp(name, "len") == 0) {
        result = eval_builtin_len(arity, node->args);
    } else {
        error("calling user-defined functions not yet supported");
    }

    /* Cleanup */
    if (node->args) {
        for (NodeListEntry *entry = node->args->head;
                entry; entry = entry->next, arity++) {
            remove_temporary_global(entry->idx);
        }
    }

    return result;
}

Reference eval_expr_subscript(NodeExprSubscript *node) {
    Reference idxref = eval_expr(node->index);

    size_t tglob_idx = add_temporary_global(idxref);

    Reference objref = eval_expr(node->obj);
    Reference result;

    Value *objv = deref(objref);
    switch (objv->type) {
        case VAL_STRING: {
            const char *str = ((StringValue *) objv)->string_value;

            long int len = strlen(str);
            long int idx = coerce_ref_to_int(idxref);

            long int actual = idx;
            if (actual < 0) {
                actual += len;
            }
            if (actual >= len) {
                error("string index out of range");
            }

            char buf[2] = { str[actual], 0 };
            result = make_reference_string(buf);
            break;
        }

        case VAL_LIST_NODE:
            result = list_get_elem(objref,
                            coerce_ref_to_int(idxref))->list_node.value;
            break;

        case VAL_DICT_NODE:
            result = dict_get_entry(objref, idxref, false)->dict_node.value;
            break;

        default:
            error("'%s' object is not subscriptable", get_typestr(objref));
    }

    remove_temporary_global(tglob_idx);
    return result;
}

static bool is_hashable(ValueType type) {
    return type != VAL_LIST_NODE && type != VAL_DICT_NODE;
}

Reference eval_expr(Node *node) {
    switch (node->type) {
        case EXPR_LITERAL_STRING:
            return make_reference_string(
                        ((NodeExprLiteralString *) node)->value);

        case EXPR_LITERAL_INTEGER:
            return make_reference_int(
                        ((NodeExprLiteralInteger *) node)->value);

        case EXPR_LITERAL_FLOAT:
            return make_reference_float(
                        ((NodeExprLiteralFloat *) node)->value);

        case EXPR_LITERAL_LIST: {
            /* Lists start with a dummy element so we construct this first. */
            Reference list = make_reference_list_node(NULL_REF);

            /* Add the list to the set of temporary globals so that it
             * does not end up getting garbage collected. */
            size_t tglob_idx = add_temporary_global(list);

            NodeList *exprs = ((NodeExprLiteralList *) node)->values;
            if (exprs) {
                /* Now iterate through the expression list and construct
                 * the list. */
                Reference tail = list;
                for (NodeListEntry *entry = exprs->head; entry;
                        entry = entry->next) {

                    Reference next = make_reference_list_node(NONE_REF);
                    deref_to_list_value(tail)->list_node.next = next;

                    Reference elem = eval_expr(entry->node);
                    deref_to_list_value(next)->list_node.value = elem;

                    tail = next;
                }
            }

            /* Now remove it from the globals list. */
            remove_temporary_global(tglob_idx);

            return list;
        }

        case EXPR_LITERAL_DICT: {
             /* Similar to list code. Almost identical, but s/List/Dict, and
              * there are both keys and values... */
            Reference dict = make_reference_dict_node(NULL_REF, NULL_REF);

            /* Add the dict to the set of temporary globals so that it
             * does not end up getting garbage collected. */
            size_t tglob_idx = add_temporary_global(dict);

            NodeList *exprs = ((NodeExprLiteralDict *) node)->values;
            if (exprs) {
                /* Now iterate through the expression list and construct
                 * the dict. Each element of the list should be a
                 * NodeExprLiteralPair. */
                Reference tail = dict;
                for (NodeListEntry *entry = exprs->head; entry;
                        entry = entry->next) {

                    if (entry->node->type != EXPR_LITERAL_PAIR) {
                        error("expected pair in dict literal");
                    }

                    NodeExprLiteralPair *pair =
                        (NodeExprLiteralPair *) entry->node;

                    Reference next = make_reference_dict_node(NONE_REF, NONE_REF);
                    deref_to_dict_value(tail)->dict_node.next = next;

                    Reference valueref = eval_expr(pair->value);
                    Reference keyref = eval_expr(pair->key);
                    if (!is_hashable(deref(keyref)->type)) {
                        error("dictionary keys must be hashable");
                    }

                    DictValue *elem = deref_to_dict_value(next);
                    elem->dict_node.key = keyref;
                    elem->dict_node.value = valueref;

                    tail = next;
                }
            }

            /* Now remove it from the globals list. */
            remove_temporary_global(tglob_idx);

            return dict;
        }

        case EXPR_LITERAL_SINGLETON:
            switch (((NodeExprLiteralSingleton *) node)->singleton) {
                case S_NONE:  return NONE_REF;
                case S_TRUE:  return TRUE_REF;
                case S_FALSE: return FALSE_REF;
                default:
                    error("unknown singleton type");
            }

        case EXPR_LITERAL_PAIR:
            error("unexpected pair");

        case EXPR_IDENTIFIER:
            return *get_global_variable(
                        ((NodeExprIdentifier *) node)->name, false);

        case EXPR_BUILTIN:
            return eval_expr_builtin((NodeExprBuiltin *) node);

        case EXPR_CALL:
            return eval_expr_call((NodeExprCall *) node);

        case EXPR_SUBSCRIPT:
            return eval_expr_subscript((NodeExprSubscript *) node);

        default:
            error("unimplemented expr `%d`", node->type);
    }
}

/*!
 * Evaluates an expression that can be the target of an assignment.  This is why
 * the method returns a pointer to a Reference - so that we can change the
 * Reference itself to refer to something else.
 */
Reference *eval_expr_lval(Node *node, bool create) {
    switch (node->type) {
        case EXPR_LITERAL_STRING:
        case EXPR_LITERAL_INTEGER:
        case EXPR_LITERAL_FLOAT:
        case EXPR_LITERAL_SINGLETON:
            error("cannot assign to literal");

        case EXPR_LITERAL_DICT:
        case EXPR_LITERAL_LIST:
            error("assignment destructuring not implemented");

        case EXPR_LITERAL_PAIR:
            error("unexpected pair");

        case EXPR_IDENTIFIER:
            return get_global_variable(
                        ((NodeExprIdentifier *) node)->name, create);

        case EXPR_BUILTIN:
            error("cannot assign to result of expression");

        case EXPR_CALL:
            error("cannot assign to result of call");

        case EXPR_SUBSCRIPT: {
            NodeExprSubscript *subscript = (NodeExprSubscript *) node;
            Reference keyref = eval_expr(subscript->index);

            size_t tglob_idx = add_temporary_global(keyref);

            Reference objref = *eval_expr_lval(subscript->obj, false);
            Value *objv = deref(objref);
            Reference *result;

            switch (objv->type) {
                case VAL_LIST_NODE: {
                    ListValue *elem = list_get_elem(objref,
                            coerce_ref_to_int(keyref));
                    result = &(elem->list_node.value);
                    break;
                }

                case VAL_DICT_NODE: {
                    /* Find entry with key or, if applicable, create it. */
                    DictValue *lhs_dict = dict_get_entry(objref,
                            keyref, create);
                    result = &(lhs_dict->dict_node.value);
                    break;
                }

                default:
                    error("'%s' does not support item assignment",
                            get_typestr(objref));
            }

            remove_temporary_global(tglob_idx);

            return result;
        }

        default:
            error("unimplemented expr lval `%d`", node->type);
    }
}


//// GLOBAL VAR FUNCTIONS ////

/*! Adds a new global variable with the provided name. */
Reference *add_global_variable(const char *name, Reference value) {
    if (global_vars == NULL) {
        /* If our global vars array is NULL, let's make a new one. */
        global_vars = calloc(sizeof(struct GlobalVariable), INITIAL_SIZE);
        max_vars = INITIAL_SIZE;
    } else if (num_vars == max_vars) {
        /* Otherwise, double its size (the JVM internal source said this
         * was a good resizing semantic, don't sue me!), and zero it out. */
        max_vars *= 2;
        global_vars = realloc(global_vars,
                              sizeof(struct GlobalVariable) * max_vars);
        // Zero so our loop searching for a new spot doesn't fail.
        memset(global_vars + num_vars, 0,
               sizeof(struct GlobalVariable) * num_vars);
    }

    if (global_vars == NULL) {
        error("%s", "Allocation failed!");
    }

    /* Search for a new variable. */
    for (int i = 0; i < max_vars; i++) {
        if (global_vars[i].name == NULL) {
            num_vars++;
            global_vars[i].name = strndup(name, strlen(name));
            global_vars[i].ref = value;
            return &global_vars[i].ref;
        }
    }

    /* We made space for at least 1 new var up there,
     * so this can't happen..! I swear! */
    UNREACHABLE();
}

/*! Tries to retrieve a global variable's reference, creating it if `create`
    is true. */
Reference *get_global_variable(const char *name, bool create) {
    for (int i = 0; i < num_vars; i++) {
        if (global_vars[i].name != NULL &&
                strcmp(name, global_vars[i].name) == 0) {
            return &global_vars[i].ref;
        }
    }

    if (create) {
        return add_global_variable(name, NULL_REF);
    } else {
        error("name '%s' is not defined", name);
    }
}

/*! Delete the global variable with name `name`. Error if no such variable
    exists. */
void delete_global_variable(const char *name) {
    int i;
    for (i = 0; i < num_vars; i++) {
        if (strcmp(name, global_vars[i].name) == 0) {
            break;
        }
    }

    if (i < num_vars) {
        // Found the variable.  Remove it by sliding the whole array down.
        free(global_vars[i].name);

        // REVIEW:  Would be faster to do this with a memmove(), but whatever
        for (int j = i; j < num_vars - 1; j++) {
            global_vars[j].name = global_vars[j + 1].name;
            global_vars[j].ref = global_vars[j + 1].ref;
        }

        num_vars--;
        global_vars[num_vars].name = NULL;
        global_vars[num_vars].ref = NULL_REF;

        // TODO: resize array if too small. We'd need to compact it too.
    } else {
        error("Could not delete variable `%s`", name);
    }
}

/*! The current index of the next global variable. We assume that this will
 *  not overflow during the execution of this program. */
static size_t tglob_next = 0;

/*!
 * Adds a new temporary global variable that cannot be referred to by the code
 * so that it is a root during execution. These temporary global variables
 * have the naming scheme `$t<id>`.
 *
 * All temporary globals are removed by `clear_temporary_globals` which is
 * called by `error` when an error occurs. */
int add_temporary_global(Reference value) {
    char buffer[32];
    snprintf(buffer, 31, "$t%zu", tglob_next);

    add_global_variable(buffer, value);
    return tglob_next++;
}

/*!
 * Removes the specified temporary global value.
 */
void remove_temporary_global(size_t glob) {
    char buffer[32];
    snprintf(buffer, 31, "$t%zu", glob);

    delete_global_variable(buffer);
}

/*!
 * Removes all temporary global values found in the global variable list.
 */
void clear_temporary_globals() {
    int new_num_vars = 0;
    for (int i = 0; i < num_vars; i++) {
        const char *var_name = global_vars[i].name;

        if (var_name[0] == '$' && var_name[1] == 't') {
            free(global_vars[i].name);
        } else {
            if (i != new_num_vars) {
                global_vars[new_num_vars].name = global_vars[i].name;
                global_vars[new_num_vars].ref  = global_vars[i].ref;
            }
            new_num_vars++;
        }
    }

    memset(&global_vars[new_num_vars], 0,
            sizeof(struct GlobalVariable) * (num_vars - new_num_vars));
    num_vars = new_num_vars;
}

/*!
 * Invokes a function for each global in the global environment.  Returns the
 * number of globals found.
 */
int foreach_global(void (*f)(const char *name, Reference Ref)) {

    /* Call the callback on each global. */
    for (int i = 0; i < num_vars; i++) {
        f(global_vars[i].name, global_vars[i].ref);
    }

    return num_vars;
}

void print_global_helper(const char *name, Reference ref) {
    fprintf(stdout, "%s = ref %d; value ", name, ref);
    ref_print_ext(stdout, ref, true, MAX_DEPTH);
}

void print_globals(void) {
    // Just so we can make the text reflect the number of globals.
    if (num_vars == 0)
        fprintf(stdout, "0 Globals\n");
    else if (num_vars == 1)
        fprintf(stdout, "1 Global:\n");
    else
        fprintf(stdout, "%d Globals:\n", num_vars);

    foreach_global(print_global_helper);
}


//// NEW REFERENCE FUNCTIONS ////

/*! Creates a new None reference. This should only be called once. */
Reference make_reference_none() {
    Value *v = mm_malloc(VAL_NONE, 0);
    return v->ref;
}

/*! Creates a new Bool reference. This should only be called twice. */
Reference make_reference_bool() {
    Value *v = mm_malloc(VAL_BOOL, 0);
    return v->ref;
}

/*! Assigns a long int to a new reference in the ref_table. */
Reference make_reference_int(long int i) {
    IntegerValue *iv = (IntegerValue *) mm_malloc(VAL_INTEGER, /* ignored */ 0);
    iv->integer_value = i;
    return iv->ref;
}


/*! Assigns a double to a new reference in the ref_table. */
Reference make_reference_float(double f) {
    FloatValue *fv = (FloatValue *) mm_malloc(VAL_FLOAT, /* ignored */ 0);
    fv->float_value = f;
    return fv->ref;
}

/*! Assigns a string to a new reference in the ref_table. */
Reference make_reference_string(const char *value) {
    StringValue *sv = (StringValue *) mm_malloc(VAL_STRING, strlen(value) + 1);
    strcpy(sv->string_value, value);
    return sv->ref;
}

/*! Assigns a concatenated string to a new referecne in the ref_table. */
Reference make_reference_string_concat(const char *v1, const char *v2) {
    int len1 = strlen(v1), len2 = strlen(v2);
    StringValue *sv = (StringValue *) mm_malloc(VAL_STRING, len1 + len2 + 1);
    strcpy(sv->string_value, v1);
    strcpy(sv->string_value + len1, v2);
    return sv->ref;
}

/*! ListNode allocation helper. */
Reference make_reference_list_node(Reference value) {
    ListValue *lv = (ListValue *) mm_malloc(VAL_LIST_NODE, /* ignored */ 0);
    lv->list_node.value = value;
    lv->list_node.next = NULL_REF;
    return lv->ref;
}

/*! DictNode allocation helper. */
Reference make_reference_dict_node(Reference key, Reference value) {
    DictValue *dv = (DictValue *) mm_malloc(VAL_DICT_NODE, /* ignored */ 0);
    dv->dict_node.key = key;
    dv->dict_node.value = value;
    dv->dict_node.next = NULL_REF;
    return dv->ref;
}

