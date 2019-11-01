#ifndef TYPES_H
#define TYPES_H


/*!
 * An opaque Reference that can be used to indirectly access a Value.  The
 * Reference itself is not a pointer; rather, it is an index into a table of
 * references maintained in the eval.c state.  It can be dereferenced into a
 * Value pointer using the deref() function.
 */
typedef int Reference;


/*! This value is used to represent a "null" reference.
 *  
 *  Note that this is different than a reference to `None`. A reference to
 *  `None` will be a real reference with a valid integer value. A null
 *  reference is invalid. */
#define NULL_REF (-1)


/*!
 * An enumeration of all types of values supported by the interpreter.
 */
typedef enum ValueType {
    VAL_NONE,           /*!< The None value. There is only one! */
    VAL_BOOL,           /*!< A bool value. There is only True and False! */
    VAL_INTEGER,        /*!< An integer value. */
    VAL_FLOAT,          /*!< A float value */
    VAL_STRING,         /*!< A string value */
    VAL_LIST_NODE,      /*!< A node in a list */
    VAL_DICT_NODE       /*!< A node (key/value pair) in a dictionary */
} ValueType;

/*! This is a single element of a linked list. */
typedef struct ListNode {
    /* Tell us if the memory is linked to a global variable - 0 or 1. */ 
    int marked;

    /*! The value at this index of the list. */
    Reference value;

    /*! The next node in the list, or NULL_REF if this is the last node. */
    Reference next;

} ListNode;


/*! This is a single entry in a dictionary. */
typedef struct DictNode {
    /* Tell us if the memory is linked to a global variable - 0 or 1. */ 
    int marked;

    /*! The key for this dictionary entry. */
    Reference key;

    /*! The value associated with the key. */
    Reference value;

    /*! The next key/value pair in the dictionary. */
    Reference next;

} DictNode;


/*!
 * A Value type that represents all possible kinds of values used within the
 * interpreter.
 *
 *  - All numbers are floats
 *  - All strings are '\0' terminated
 *  - Lists are represented as a singly-linked list of ListNode values,
 *    which are themselves allocated from the memory pool
 *  - Dictionaries are represented as a singly-linked list of DictNode
 *    key-value pairs, which are themselves allocated from the memory pool
 */
typedef struct Value {
    /*!
     * Every Value knows the Reference associated with it, so that we don't
     * have to search for what reference goes with a particular value in the
     * reference table.
     */
    Reference ref;

	/*! This specifies what kind of value is actually represented. */
    ValueType type;

    /*!
     * This is the size of the data in the value.  For fixed-size types, this
     * is as expected - e.g. floats are 4 bytes, and so forth.  For strings,
     * this is the size of the string data including NUL-terminator.  For
     * lists and dictionaries, this will be the size of the node details.
     *
     * This means we can always compute the size of a Value by the expression
     * sizeof(Value) + value->data_size, regardless of which kind of Value
     * it is.
     */
    int data_size;

    /* Tell us if the memory is linked to a global variable - 0 or 1. */ 
    int marked;
} Value;

/*!
 * A "integer value" type that represents long integers.  It is a
 * subtype of Value.  This means that we can cast a IntegerValue* to a Value*
 * and still access all the Value components.  And, if a Value has a type
 * of VAL_INTEGER, we can cast the Value* back to a IntegerValue* to get at
 * the integer it stores.
 */
typedef struct IntegerValue {
    /*!
     * Every Value knows the Reference associated with it, so that we don't
     * have to search for what reference goes with a particular value in the
     * reference table.
     */
    Reference ref;

    /*! This specifies what kind of value is actually represented. */
    ValueType type;

    /* Tell us if the memory is linked to a global variable - 0 or 1. */ 
    int marked;

    /*!
     * This is the size of the data in the value.  For fixed-size types, this
     * is as expected - e.g. integers are 4 bytes, and so forth.  For strings,
     * this is the size of the string data including NUL-terminator.  For
     * lists and dictionaries, this will be the size of the node.
     */
    int data_size;

    /*! The integer value this IntegerValue represents. */
    int integer_value;

} IntegerValue;

/*!
 * A "float value" type that represents double-precision floats.  It is a
 * subtype of Value.  This means that we can cast a FloatValue* to a Value*
 * and still access all the Value components.  And, if a Value has a type
 * of VAL_FLOAT, we can cast the Value* back to a FloatValue* to get at
 * the float it stores.
 */
typedef struct FloatValue {
    /*!
     * Every Value knows the Reference associated with it, so that we don't
     * have to search for what reference goes with a particular value in the
     * reference table.
     */
    Reference ref;

    /*! This specifies what kind of value is actually represented. */
    ValueType type;

    /*!
     * This is the size of the data in the value.  For fixed-size types, this
     * is as expected - e.g. floats are 8 bytes, and so forth.  For strings,
     * this is the size of the string data including NUL-terminator.  For
     * lists and dictionaries, this will be the size of the node.
     */
    int data_size;

    /*! The float value this FloatValue represents. */
    double float_value;

    /* Tell us if the memory is linked to a global variable - 0 or 1. */ 
    int marked;
} FloatValue;


/*!
 * A "string value" type that represents strings.  It is a subtype of
 * Value.  This means that we can cast a StringValue* to a Value* and 
 * still access all the Value components.  And, if a Value has a type of
 * VAL_STRING, we can cast the Value* back to a StringValue* to get at
 * the string it stores.
 */
typedef struct StringValue {
    /*!
     * Every Value knows the Reference associated with it, so that we don't
     * have to search for what reference goes with a particular value in the
     * reference table.
     */
    Reference ref;

    /*! This specifies what kind of value is actually represented. */
    ValueType type;

    /*!
     * This is the size of the data in the value.  For fixed-size types, this
     * is as expected - e.g. floats are 4 bytes, and so forth.  For strings,
     * this is the size of the string data including NUL-terminator.  For
     * lists and dictionaries, this will be the size of the node.
     */
    int data_size;

    /* Tell us if the memory is linked to a global variable - 0 or 1. */ 
    int marked;

    /*!
     * The string value this StringValue represents.  We use the undimensioned
     * array syntax so that the string data can immediately follow the Value
     * part of the struct, and reference it easily from C code.
     */
    char string_value[];

    
} StringValue;


/*!
 * A "list value" type that represents list elements.  It is a subtype
 * of Value.  This means that we can cast a ListValue* to a Value* and
 * still access all the Value components.  And, if a Value has a type of
 * VAL_LIST_NODE, we can cast the Value* back to a ListValue* to get at
 * all the list-related details.
 */
typedef struct ListValue {
    /*!
     * Every Value knows the Reference associated with it, so that we don't
     * have to search for what reference goes with a particular value in the
     * reference table.
     */
    Reference ref;

    /*! This specifies what kind of value is actually represented. */
    ValueType type;

    /*!
     * This is the size of the data in the value.  For fixed-size types, this
     * is as expected - e.g. floats are 4 bytes, and so forth.  For strings,
     * this is the size of the string data including NUL-terminator.  For
     * lists and dictionaries, this will be the size of the node.
     */
    int data_size;

    /*! The details of the list element stored in this ListValue. */
    ListNode list_node;
    /* Tell us if the memory is linked to a global variable - 0 or 1. */ 
    int marked;
} ListValue;


/*!
 * A "dictionary value" type that represents dictionary entries.  It is a
 * subtype of Value.  This means that we can cast a DictValue* to a Value*
 * and still access all the Value components.  And, if a Value has a type
 * of VAL_DICT_NODE, we can cast the Value* back to a DictValue* to get at
 * all the dictionary-related details.
 */
typedef struct DictValue {
    /*!
     * Every Value knows the Reference associated with it, so that we don't
     * have to search for what reference goes with a particular value in the
     * reference table.
     */
    Reference ref;

    /*! This specifies what kind of value is actually represented. */
    ValueType type;

    /*!
     * This is the size of the data in the value.  For fixed-size types, this
     * is as expected - e.g. floats are 4 bytes, and so forth.  For strings,
     * this is the size of the string data including NUL-terminator.  For
     * lists and dictionaries, this will be the size of the node.
     */
    int data_size;

    /*! The details of the dictonary entry stored in this DictValue. */
    DictNode dict_node;

    /* Tell us if the memory is linked to a global variable - 0 or 1. */ 
    int marked;
} DictValue;


#endif /* TYPES_H */
