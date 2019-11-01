/* A Bison parser, made by GNU Bison 3.3.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

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
#line 13 "grammar.y" /* yacc.c:1921  */

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

#line 105 "grammar.y.h" /* yacc.c:1921  */

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
#line 185 "grammar.y" /* yacc.c:1921  */

    Node *node_value;
    NodeList *node_list;

    const char *string_value;
    long int int_value;
    double float_value;

#line 175 "grammar.y.h" /* yacc.c:1921  */
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
#line 10 "grammar.y" /* yacc.c:1921  */

    void yyerror(YYLTYPE *yylloc, yyscan_t scanner, const char* msg);

#line 205 "grammar.y.h" /* yacc.c:1921  */

#endif /* !YY_YY_GRAMMAR_Y_H_INCLUDED  */
