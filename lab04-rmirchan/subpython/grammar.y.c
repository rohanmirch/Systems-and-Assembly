/* A Bison parser, made by GNU Bison 3.3.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef __clang_analyzer__

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.3.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* "%code top" blocks.  */
#line 7 "grammar.y" /* yacc.c:315  */

    #include <stdio.h>

#line 71 "grammar.y.c" /* yacc.c:315  */




# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "grammar.y.h".  */
#ifndef YY_YY_GRAMMAR_Y_H_INCLUDED
# define YY_YY_GRAMMAR_Y_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 13 "grammar.y" /* yacc.c:352  */

    #include <assert.h>
    #include <stdbool.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>

    #include <unistd.h>

#ifndef NREADLINE
    #include <readline/history.h>
    #include <readline/readline.h>
#endif

    #include "ast.h"
    #include "global.h"

    typedef void *yyscan_t;

    #define TOKEN_QUEUE_MAX  1024
    #define INDENT_STACK_MAX 1024

    typedef struct subpy_udata_t {
        int input_type;
        struct {
            char *buffer;
            size_t length;
            size_t position;
        } input;
        FILE *stream;

        bool interactive;

        void *pool;
        Node *tree;

        int token_queue[TOKEN_QUEUE_MAX];
        size_t token_queue_pos;
        size_t token_queue_len;

        size_t indent_stack[INDENT_STACK_MAX];
        size_t indent_stack_pos;
    } subpy_udata_t;

    void token_queue_push(subpy_udata_t *d, int token);

    void subpy_udata_init(subpy_udata_t *d, FILE *file);
    void subpy_udata_destroy(subpy_udata_t *d);

    int subpy_udata_read(subpy_udata_t *d, char *buf, int *bytes, int len);
    int subpy_udata_wrap(subpy_udata_t *d);

    size_t subpy_udata_indent_cur(const subpy_udata_t *d);
    void subpy_udata_indent_push(subpy_udata_t *d, size_t level);
    void subpy_udata_indent_pop(subpy_udata_t *d);

#line 165 "grammar.y.c" /* yacc.c:352  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INVALID = 258,
    START_SINGLE = 259,
    START_FILE = 260,
    SEMICOLON = 261,
    LINE_END = 262,
    INPUT_END = 263,
    INDENT = 264,
    DEDENT = 265,
    INDENT_ERROR = 266,
    IF = 267,
    ELIF = 268,
    ELSE = 269,
    DO = 270,
    WHILE = 271,
    CONTINUE = 272,
    BREAK = 273,
    DEL = 274,
    NONE = 275,
    TRUE = 276,
    FALSE = 277,
    ASSIGN = 278,
    EQUALS = 279,
    LT = 280,
    GT = 281,
    LE = 282,
    GE = 283,
    OR = 284,
    AND = 285,
    NOT = 286,
    PLUS = 287,
    MINUS = 288,
    ASTERISK = 289,
    FSLASH = 290,
    PERCENT = 291,
    LPAREN = 292,
    RPAREN = 293,
    LBRACKET = 294,
    RBRACKET = 295,
    LBRACE = 296,
    RBRACE = 297,
    COMMA = 298,
    COLON = 299,
    STRING = 300,
    INTEGER = 301,
    FLOAT = 302,
    IDENT = 303
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 185 "grammar.y" /* yacc.c:352  */

    Node *node_value;
    NodeList *node_list;

    const char *string_value;
    long int int_value;
    double float_value;

#line 235 "grammar.y.c" /* yacc.c:352  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



int yyparse (yyscan_t scanner);
/* "%code provides" blocks.  */
#line 10 "grammar.y" /* yacc.c:352  */

    void yyerror(YYLTYPE *yylloc, yyscan_t scanner, const char* msg);

#line 265 "grammar.y.c" /* yacc.c:352  */

#endif /* !YY_YY_GRAMMAR_Y_H_INCLUDED  */


/* Unqualified %code blocks.  */
#line 69 "grammar.y" /* yacc.c:355  */

    #include "grammar.l.h"
    #include "ast.h"

    extern int yylex(YYSTYPE* yylvalp, YYLTYPE* yyllocp, yyscan_t scanner);

    #define yypool (yyget_extra(scanner)->pool)

    
    void token_queue_push(subpy_udata_t *d, int token) {
        assert(d->token_queue_len + 1 < TOKEN_QUEUE_MAX);

        d->token_queue[d->token_queue_len] = token;
        d->token_queue_len++;
    }

    void subpy_udata_init(subpy_udata_t *d, FILE *file) {
        assert(d != NULL);

        memset(d, 0, sizeof(*d));

        d->interactive = stdin == file && isatty(fileno(file));
        d->stream = file;
        d->pool = ast_create_pool();
    }
    void subpy_udata_destroy(subpy_udata_t *d) {
        assert(d != NULL);

        free(d->input.buffer);
        ast_free_pool(d->pool);
    }

    int subpy_udata_read(subpy_udata_t *d, char *buf, int *bytes, int len) {
        *bytes = 0;
        if (d->interactive) {
            if (d->input.position < d->input.length) {
                int avail = (int) d->input.length - d->input.position;
                int to_copy = avail > len ? len : avail;

                memcpy(buf, d->input.buffer + d->input.position, to_copy);
                d->input.position += to_copy;
                *bytes = to_copy;

                return 0;
            } else {
                return 1;
            }
        } else {
            *bytes = (int) fread(buf, 1, len, d->stream);
            return *bytes == 0 && feof(d->stream);
        }
    }

    int subpy_udata_wrap(subpy_udata_t *d) {
        if (d->interactive) {
            free(d->input.buffer);

            const char *prompt = d->input.buffer == NULL ? ">>> " : "... ";

#ifdef NREADLINE
            fprintf(stdout, "%s", prompt);

            d->input.buffer = NULL;
            size_t size;
            ssize_t len = getline(&d->input.buffer, &size, d->stream);

            if (len == -1) {
                return 1;
            }

            d->input.position = 0;
            d->input.length = len;
#else
            d->input.buffer = readline(prompt);

            if (d->input.buffer == NULL) {
                return 1;
            }

            size_t length = strlen(d->input.buffer);

            if (length) {
                /* Not a blank line, so record it in history. */
                add_history(d->input.buffer);

                /* This is wasteful; we'd prefer to just write the most recent
                 * command to history, but append_history() isn't always
                 * available. */
                write_history(SUBPYTHON_HISTORY);
            }

            d->input.buffer[length] = '\n';
            d->input.position = 0;
            d->input.length = length + 1;
#endif

            return 0;
        } else {
            return 1;
        }
    }

    size_t subpy_udata_indent_cur(const subpy_udata_t *d) {
        return d->indent_stack[d->indent_stack_pos];
    }
    void subpy_udata_indent_push(subpy_udata_t *d, size_t level) {
        assert(d->indent_stack_pos + 1 < INDENT_STACK_MAX);
        d->indent_stack_pos++;
        d->indent_stack[d->indent_stack_pos] = level;
    }
    void subpy_udata_indent_pop(subpy_udata_t *d) {
        assert(d->indent_stack_pos > 0);
        d->indent_stack_pos--;
    }

#line 387 "grammar.y.c" /* yacc.c:355  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  49
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   333

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  49
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  31
/* YYNRULES -- Number of rules.  */
#define YYNRULES  77
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  131

#define YYUNDEFTOK  2
#define YYMAXUTOK   303

/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   239,   239,   240,   241,   243,   244,   245,   246,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   258,   259,
     260,   261,   262,   263,   265,   266,   267,   269,   270,   271,
     272,   274,   276,   277,   278,   279,   280,   281,   282,   283,
     284,   285,   286,   287,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   307,   308,   310,   311,   312,   314,   315,   316,   317,
     318,   319,   320,   321,   322,   323,   324,   325
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INVALID", "START_SINGLE", "START_FILE",
  "SEMICOLON", "LINE_END", "INPUT_END", "INDENT", "DEDENT", "INDENT_ERROR",
  "IF", "ELIF", "ELSE", "DO", "WHILE", "CONTINUE", "BREAK", "DEL", "NONE",
  "TRUE", "FALSE", "ASSIGN", "EQUALS", "LT", "GT", "LE", "GE", "OR", "AND",
  "NOT", "PLUS", "MINUS", "ASTERISK", "FSLASH", "PERCENT", "LPAREN",
  "RPAREN", "LBRACKET", "RBRACKET", "LBRACE", "RBRACE", "COMMA", "COLON",
  "STRING", "INTEGER", "FLOAT", "IDENT", "$accept", "input",
  "single_input", "statement_seq", "statement_seq_list", "statement",
  "simple_statement", "simple_statement_list", "small_statement",
  "compound_statement", "suite", "expr_statement", "delete_statement",
  "if_statement", "elif_statement", "while_statement", "or_test",
  "and_test", "not_test", "comparison", "expr_arith", "expr_term",
  "expr_factor", "expr_atom", "atom", "arguments", "pair_arguments",
  "pair", "literal", "literal_list", "literal_dict", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303
};
# endif

#define YYPACT_NINF -78

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-78)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      17,     8,   131,     9,   -78,   -78,   263,   263,   263,   -78,
     -78,   -78,   263,   285,   285,   263,   190,   212,   -78,   -78,
     -78,   -78,   -78,   -78,    45,   -78,    24,   -78,   -78,   -78,
     -78,    13,     5,   -78,   -78,   133,    38,   -78,    49,   -78,
     -78,   -78,   -78,   -78,    67,   168,   -78,   -78,   -78,   -78,
     -19,   -12,    60,   -78,   -78,   -78,   -15,   -78,    60,    22,
     -78,   -11,    25,   -78,    59,   -78,   -78,   263,   263,   263,
     285,   285,   285,   285,   285,   285,   285,   285,   285,   285,
     241,   263,   -78,   -78,    90,    90,   -78,   -78,   263,   263,
     -78,   263,   -78,   -78,    60,     5,   -78,    50,    50,    50,
      50,    50,    38,    38,   -78,   -78,   -78,   -78,     0,    -3,
      37,   -78,    80,   -78,    60,    60,   -78,   -78,   -78,   168,
     263,    51,   -78,    89,   -10,    90,   -78,    90,   -78,    80,
     -78
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     5,     6,     0,     0,     0,    69,
      70,    71,     0,     0,     0,     0,     0,     0,    66,    67,
      68,    58,     2,     7,     0,    16,     0,    18,    19,    20,
      21,    24,    32,    34,    36,    38,    44,    47,    51,    54,
      59,    72,    73,     3,     0,     9,    10,    12,    13,     1,
       0,     0,    26,    37,    52,    53,     0,    74,    61,     0,
      76,     0,     0,    63,     0,    14,     8,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,    11,     0,     0,    60,    75,     0,     0,
      77,     0,    15,    17,    25,    33,    35,    39,    40,    41,
      42,    43,    45,    46,    48,    49,    50,    55,     0,     0,
       0,    22,    30,    31,    62,    65,    64,    56,    57,     0,
       0,     0,    27,     0,     0,     0,    23,     0,    29,    30,
      28
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -78,   -78,   -78,   -16,   -78,    63,    -1,   -78,    53,   112,
     -77,   -78,   -78,   -78,   -14,   -78,    -4,    46,    -5,   -78,
     -13,    26,    -8,   -78,   -78,    39,   -78,    29,   -78,   -78,
     -78
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     3,    22,    44,    45,    46,   111,    24,    25,    48,
     112,    27,    28,    29,   122,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    59,    62,    63,    40,    41,
      42
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      23,    47,    50,    51,    52,    54,    55,    53,   113,    49,
      68,    56,    58,    61,    68,     4,     5,    68,    68,    68,
       6,     1,     2,    86,     7,    84,    68,     8,     9,    10,
      11,    66,    85,    89,   127,    69,    67,   118,   117,    12,
      13,    14,    68,    88,    47,    15,   119,    16,   128,    17,
     129,    64,    65,    18,    19,    20,    21,    97,    98,    99,
     100,   101,    87,    94,    96,    88,    92,    90,    91,   104,
     105,   106,    77,    78,    79,    82,    58,   109,     8,     9,
      10,    11,    75,    76,   114,   115,    80,    61,    81,    68,
      12,    13,    14,   120,   121,   125,    15,   110,    16,   126,
      17,   102,   103,   123,    18,    19,    20,    21,    83,     8,
       9,    10,    11,    26,    95,   130,   124,    93,    47,   108,
     116,    12,    13,    14,     0,     0,     0,    15,     0,    16,
       0,    17,     0,     0,     0,    18,    19,    20,    21,    43,
       0,     0,     0,     6,     0,     0,     0,     7,     0,     0,
       8,     9,    10,    11,     0,     0,     0,    70,    71,    72,
      73,    74,    12,    13,    14,    75,    76,     0,    15,     0,
      16,     0,    17,     0,     0,     0,    18,    19,    20,    21,
       6,     0,     0,     0,     7,     0,     0,     8,     9,    10,
      11,     0,     0,     0,     0,     0,     0,     0,     0,    12,
      13,    14,     0,     0,     0,    15,     0,    16,     0,    17,
       9,    10,    11,    18,    19,    20,    21,     0,     0,     0,
       0,    12,    13,    14,     0,     0,     0,    15,     0,    16,
      57,    17,     9,    10,    11,    18,    19,    20,    21,     0,
       0,     0,     0,    12,    13,    14,     0,     0,     0,    15,
       0,    16,     0,    17,    60,     0,     0,    18,    19,    20,
      21,     9,    10,    11,     0,     0,     0,     0,     0,     0,
       0,     0,    12,    13,    14,     0,     0,     0,    15,   107,
      16,     0,    17,     9,    10,    11,    18,    19,    20,    21,
       0,     0,     0,     0,    12,    13,    14,     0,     0,     0,
      15,     0,    16,     0,    17,     9,    10,    11,    18,    19,
      20,    21,     0,     0,     0,     0,     0,    13,    14,     0,
       0,     0,    15,     0,    16,     0,    17,     0,     0,     0,
      18,    19,    20,    21
};

static const yytype_int16 yycheck[] =
{
       1,     2,     6,     7,     8,    13,    14,    12,    85,     0,
      29,    15,    16,    17,    29,     7,     8,    29,    29,    29,
      12,     4,     5,    38,    16,    44,    29,    19,    20,    21,
      22,     7,    44,    44,    44,    30,    23,    40,    38,    31,
      32,    33,    29,    43,    45,    37,     9,    39,   125,    41,
     127,     6,     7,    45,    46,    47,    48,    70,    71,    72,
      73,    74,    40,    67,    69,    43,     7,    42,    43,    77,
      78,    79,    34,    35,    36,     8,    80,    81,    19,    20,
      21,    22,    32,    33,    88,    89,    37,    91,    39,    29,
      31,    32,    33,    13,    14,    44,    37,     7,    39,    10,
      41,    75,    76,   119,    45,    46,    47,    48,    45,    19,
      20,    21,    22,     1,    68,   129,   120,    64,   119,    80,
      91,    31,    32,    33,    -1,    -1,    -1,    37,    -1,    39,
      -1,    41,    -1,    -1,    -1,    45,    46,    47,    48,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    16,    -1,    -1,
      19,    20,    21,    22,    -1,    -1,    -1,    24,    25,    26,
      27,    28,    31,    32,    33,    32,    33,    -1,    37,    -1,
      39,    -1,    41,    -1,    -1,    -1,    45,    46,    47,    48,
      12,    -1,    -1,    -1,    16,    -1,    -1,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    33,    -1,    -1,    -1,    37,    -1,    39,    -1,    41,
      20,    21,    22,    45,    46,    47,    48,    -1,    -1,    -1,
      -1,    31,    32,    33,    -1,    -1,    -1,    37,    -1,    39,
      40,    41,    20,    21,    22,    45,    46,    47,    48,    -1,
      -1,    -1,    -1,    31,    32,    33,    -1,    -1,    -1,    37,
      -1,    39,    -1,    41,    42,    -1,    -1,    45,    46,    47,
      48,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    33,    -1,    -1,    -1,    37,    38,
      39,    -1,    41,    20,    21,    22,    45,    46,    47,    48,
      -1,    -1,    -1,    -1,    31,    32,    33,    -1,    -1,    -1,
      37,    -1,    39,    -1,    41,    20,    21,    22,    45,    46,
      47,    48,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,
      -1,    -1,    37,    -1,    39,    -1,    41,    -1,    -1,    -1,
      45,    46,    47,    48
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,     5,    50,     7,     8,    12,    16,    19,    20,
      21,    22,    31,    32,    33,    37,    39,    41,    45,    46,
      47,    48,    51,    55,    56,    57,    58,    60,    61,    62,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      77,    78,    79,     8,    52,    53,    54,    55,    58,     0,
      65,    65,    65,    67,    71,    71,    65,    40,    65,    74,
      42,    65,    75,    76,     6,     7,     7,    23,    29,    30,
      24,    25,    26,    27,    28,    32,    33,    34,    35,    36,
      37,    39,     8,    54,    44,    44,    38,    40,    43,    44,
      42,    43,     7,    57,    65,    66,    67,    69,    69,    69,
      69,    69,    70,    70,    71,    71,    71,    38,    74,    65,
       7,    55,    59,    59,    65,    65,    76,    38,    40,     9,
      13,    14,    63,    52,    65,    44,    10,    44,    59,    59,
      63
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    49,    50,    50,    50,    51,    51,    51,    51,    52,
      53,    53,    54,    54,    55,    55,    56,    56,    57,    57,
      58,    58,    59,    59,    60,    60,    61,    62,    63,    63,
      63,    64,    65,    65,    66,    66,    67,    67,    68,    68,
      68,    68,    68,    68,    69,    69,    69,    70,    70,    70,
      70,    71,    71,    71,    72,    72,    72,    72,    73,    73,
      73,    74,    74,    75,    75,    76,    77,    77,    77,    77,
      77,    77,    77,    77,    78,    78,    79,    79
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     3,     1,     1,     1,     2,     1,
       1,     2,     1,     1,     2,     3,     1,     3,     1,     1,
       1,     1,     1,     4,     1,     3,     2,     5,     5,     3,
       0,     4,     1,     3,     1,     3,     1,     2,     1,     3,
       3,     3,     3,     3,     1,     3,     3,     1,     3,     3,
       3,     1,     2,     2,     1,     3,     4,     4,     1,     1,
       3,     1,     3,     1,     3,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     3,     2,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (&yylloc, scanner, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, yyscan_t scanner)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (yylocationp);
  YYUSE (scanner);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, yyscan_t scanner)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yytype, yyvaluep, yylocationp, scanner);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, yyscan_t scanner)
{
  unsigned long yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, scanner); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return (YYSIZE_T) (yystpcpy (yyres, yystr) - yyres);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, yyscan_t scanner)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (yyscan_t scanner)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yynewstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  *yyssp = (yytype_int16) yystate;

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = (YYSIZE_T) (yyssp - yyss + 1);

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex (&yylval, &yylloc, scanner);
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 239 "grammar.y" /* yacc.c:1652  */
    { yyget_extra(scanner)->tree = (yyvsp[0].node_value);   YYACCEPT; }
#line 1709 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 3:
#line 240 "grammar.y" /* yacc.c:1652  */
    { yyget_extra(scanner)->tree = NULL; YYACCEPT; }
#line 1715 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 4:
#line 241 "grammar.y" /* yacc.c:1652  */
    { yyget_extra(scanner)->tree = (yyvsp[-1].node_value);   YYACCEPT; }
#line 1721 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 5:
#line 243 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = NULL; }
#line 1727 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 6:
#line 244 "grammar.y" /* yacc.c:1652  */
    { yyresult = 3; goto yyreturn; }
#line 1733 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 9:
#line 248 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_sequence(yypool, (yyvsp[0].node_list)); }
#line 1739 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 10:
#line 249 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_list) = ast_alloc_nodelist(yypool); ast_nodelist_append(yypool, (yyval.node_list), (yyvsp[0].node_value)); }
#line 1745 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 11:
#line 250 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_list) = (yyvsp[-1].node_list); ast_nodelist_append(yypool, (yyvsp[-1].node_list), (yyvsp[0].node_value)); }
#line 1751 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 14:
#line 253 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_sequence(yypool, (yyvsp[-1].node_list)); }
#line 1757 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 15:
#line 254 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_sequence(yypool, (yyvsp[-2].node_list)); }
#line 1763 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 16:
#line 255 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_list) = ast_alloc_nodelist(yypool); ast_nodelist_append(yypool, (yyval.node_list), (yyvsp[0].node_value)); }
#line 1769 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 17:
#line 257 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_list) = (yyvsp[-2].node_list); ast_nodelist_append(yypool, (yyvsp[-2].node_list), (yyvsp[0].node_value)); }
#line 1775 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 23:
#line 263 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = (yyvsp[-1].node_value); }
#line 1781 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 25:
#line 266 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_assign(yypool, (yyvsp[-2].node_value), (yyvsp[0].node_value)); }
#line 1787 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 26:
#line 267 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_del(yypool, (yyvsp[0].node_value)); }
#line 1793 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 27:
#line 269 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_if(yypool, (yyvsp[-3].node_value), (yyvsp[-1].node_value), (yyvsp[0].node_value)); }
#line 1799 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 28:
#line 270 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_if(yypool, (yyvsp[-3].node_value), (yyvsp[-1].node_value), (yyvsp[0].node_value)); }
#line 1805 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 29:
#line 271 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = (yyvsp[0].node_value); }
#line 1811 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 30:
#line 272 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = NULL; }
#line 1817 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 31:
#line 274 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_while(yypool, (yyvsp[-2].node_value), (yyvsp[0].node_value)); }
#line 1823 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 33:
#line 277 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_builtin(yypool, OP_OR, (yyvsp[-2].node_value), (yyvsp[0].node_value)); }
#line 1829 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 35:
#line 279 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_builtin(yypool, OP_AND, (yyvsp[-2].node_value), (yyvsp[0].node_value)); }
#line 1835 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 37:
#line 281 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_builtin(yypool, UOP_NOT, (yyvsp[0].node_value), NULL); }
#line 1841 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 39:
#line 283 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_builtin(yypool, COMP_EQUALS, (yyvsp[-2].node_value), (yyvsp[0].node_value)); }
#line 1847 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 40:
#line 284 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_builtin(yypool, COMP_LT, (yyvsp[-2].node_value), (yyvsp[0].node_value)); }
#line 1853 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 41:
#line 285 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_builtin(yypool, COMP_GT, (yyvsp[-2].node_value), (yyvsp[0].node_value)); }
#line 1859 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 42:
#line 286 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_builtin(yypool, COMP_LE, (yyvsp[-2].node_value), (yyvsp[0].node_value)); }
#line 1865 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 43:
#line 287 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_builtin(yypool, COMP_GE, (yyvsp[-2].node_value), (yyvsp[0].node_value)); }
#line 1871 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 45:
#line 290 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_builtin(yypool, OP_ADD, (yyvsp[-2].node_value), (yyvsp[0].node_value)); }
#line 1877 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 46:
#line 291 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_builtin(yypool, OP_SUBTRACT, (yyvsp[-2].node_value), (yyvsp[0].node_value)); }
#line 1883 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 48:
#line 293 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_builtin(yypool, OP_MULTIPLY, (yyvsp[-2].node_value), (yyvsp[0].node_value)); }
#line 1889 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 49:
#line 294 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_builtin(yypool, OP_DIVIDE, (yyvsp[-2].node_value), (yyvsp[0].node_value)); }
#line 1895 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 50:
#line 295 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_builtin(yypool, OP_MODULO, (yyvsp[-2].node_value), (yyvsp[0].node_value)); }
#line 1901 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 52:
#line 297 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_builtin(yypool, UOP_IDENTITY, (yyvsp[0].node_value), NULL); }
#line 1907 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 53:
#line 298 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_builtin(yypool, UOP_NEGATE, (yyvsp[0].node_value), NULL); }
#line 1913 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 55:
#line 300 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_call(yypool, (yyvsp[-2].node_value), NULL); }
#line 1919 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 56:
#line 301 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_call(yypool, (yyvsp[-3].node_value), (yyvsp[-1].node_list)); }
#line 1925 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 57:
#line 302 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_subscript(yypool, (yyvsp[-3].node_value), (yyvsp[-1].node_value)); }
#line 1931 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 58:
#line 303 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_identifier(yypool, (yyvsp[0].string_value)); }
#line 1937 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 60:
#line 305 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = (yyvsp[-1].node_value); }
#line 1943 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 61:
#line 307 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_list) = ast_alloc_nodelist(yypool); ast_nodelist_append(yypool, (yyval.node_list), (yyvsp[0].node_value)); }
#line 1949 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 62:
#line 308 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_list) = (yyvsp[-2].node_list); ast_nodelist_append(yypool, (yyval.node_list), (yyvsp[0].node_value)); }
#line 1955 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 63:
#line 310 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_list) = ast_alloc_nodelist(yypool); ast_nodelist_append(yypool, (yyval.node_list), (yyvsp[0].node_value)); }
#line 1961 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 64:
#line 311 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_list) = (yyvsp[-2].node_list); ast_nodelist_append(yypool, (yyval.node_list), (yyvsp[0].node_value)); }
#line 1967 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 65:
#line 312 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_literal_pair(yypool, (yyvsp[-2].node_value), (yyvsp[0].node_value)); }
#line 1973 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 66:
#line 314 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_literal_string(yypool, (yyvsp[0].string_value)); }
#line 1979 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 67:
#line 315 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_literal_integer(yypool, (yyvsp[0].int_value)); }
#line 1985 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 68:
#line 316 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_literal_float(yypool, (yyvsp[0].float_value)); }
#line 1991 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 69:
#line 317 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_literal_singleton(yypool, S_NONE); }
#line 1997 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 70:
#line 318 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_literal_singleton(yypool, S_TRUE); }
#line 2003 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 71:
#line 319 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_literal_singleton(yypool, S_FALSE); }
#line 2009 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 72:
#line 320 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_literal_list(yypool, (yyvsp[0].node_list)); }
#line 2015 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 73:
#line 321 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_value) = ast_alloc_literal_dict(yypool, (yyvsp[0].node_list)); }
#line 2021 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 74:
#line 322 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_list) = ast_alloc_nodelist(yypool); }
#line 2027 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 75:
#line 323 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_list) = (yyvsp[-1].node_list); }
#line 2033 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 76:
#line 324 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_list) = ast_alloc_nodelist(yypool); }
#line 2039 "grammar.y.c" /* yacc.c:1652  */
    break;

  case 77:
#line 325 "grammar.y" /* yacc.c:1652  */
    { (yyval.node_list) = (yyvsp[-1].node_list); }
#line 2045 "grammar.y.c" /* yacc.c:1652  */
    break;


#line 2049 "grammar.y.c" /* yacc.c:1652  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (&yylloc, scanner, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (&yylloc, scanner, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, scanner);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, scanner, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, scanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp, scanner);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 326 "grammar.y" /* yacc.c:1918  */


void yyerror(YYLTYPE *yylloc, yyscan_t scanner, const char* msg) {
    (void) scanner;

    fprintf(stderr, "<stdin>:%d:%d-%d: %s\n",
        yylloc->first_line, yylloc->first_column, yylloc->last_column, msg);
}

#endif /* __clang_analyzer__ */
