%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "env.h"
#include "eval.h"

/* =========================================================================
 * Parser Declarations & Helpers
 * ========================================================================= */

/* Lexer Interface */
int yylex(void);
void yyerror(const char *s);

/* Helper: Process the AST after parsing a line */
int eval_print(Data *exp, Data *env);

/* Helper: Convert a list of expressions (from '[ ... ]') into a Vector */
static Data *exprs_to_vec(Data *list);

/* Global Root & Environment references */
Data *Root;
extern Data *Env;
%}

/* =========================================================================
 * Yacc Definitions
 * ========================================================================= */

%union {
     int ival;
     char cval;
     char *sval;
     Data *pval;
}

/* Tokens */
%token <ival> NUM
%token <sval> VAR SYM
%token <cval> CHAR
%token <sval> STRING

/* Non-terminals (return pointers to Data) */
%type <pval> Line Expr Atom List Vec Exprs

%%

/* =========================================================================
 * Grammar Rules
 * ========================================================================= */

Prog    : Lines
        ;

Lines   : Line { if ($1) eval_print($1, Env); } Lines
        | /* empty */
        ;

Line    : Expr '\n' { $$ = $1; }
        | '\n'      { $$ = NULL; }
        ;

Expr    : Atom { $$ = $1; }
        | List { $$ = $1; }
        | Vec  { $$ = $1; }
        ;

Atom    : NUM     { $$ = mknat($1); }
        | VAR     { $$ = mksym($1); }
        | SYM     { $$ = mksym($1); }
        | CHAR    { $$ = mkchr($1); }
        | STRING  { $$ = mkstr($1); }
        ;

/* List Syntax: (e1 e2 ...) */
List    : '(' Exprs ')' { $$ = $2; }
        ;

/* Vector Syntax: [e1 e2 ...] */
Vec     : '[' Exprs ']' { $$ = exprs_to_vec($2); }
        ;

/* Sequence of Expressions (recursive) */
Exprs   : /* empty */ { $$ = mknil(); }
        | Expr Exprs  { $$ = mkduo($1, $2); }
        ;

%%

/* =========================================================================
 * Subroutines & REPL Logic
 * ========================================================================= */

/* * Function: eval_print
 * Description: Evaluates the parsed expression and prints the result.
 * Features:
 * 1. Checks for undefined symbols first (prints "UNDEFINED").
 * 2. Suppresses printing for assignment statements ('=').
 */
int eval_print(Data *exp, Data *env) {
    /* 1. Check for Undefined Variable Reference */
    /* If the user types a symbol that isn't bound, warn them. */
    if (!exp) return 0;
    if (is_symbol(exp)) {
        Data *val = envget(env, exp);
        if (is_nil(val)) {
            puts("UNDEFINED");
            return 0;
        }
    }

    /* 2. Check for Assignment (Side-effect only) */
    /* We generally don't print the result of an assignment to keep output clean. */
    int is_assignment = 0;
    if (is_duo(exp)) {
        Data *op = fst(exp);
        if (is_symbol(op) && strcmp(op->sv, "=") == 0) {
            is_assignment = 1;
        }
    }

    /* 3. Evaluate */
    Data *res = eval(exp, env);
    
    /* 4. Print Result (unless assignment) */
    if (!is_assignment) {
        pr_data(res);
        puts("");
    }
    
    return 0;
}

/* Standard Yacc error handler */
void yyerror(const char *s) {
     fprintf(stderr, "Syntax Error: %s\n", s);
}

/* * Function: exprs_to_vec
 * Description: transform a linked list of expressions into a Vector Data node.
 * Usage: Called when parsing '[ ... ]'.
 */
static Data *exprs_to_vec(Data *list) {
    Data *vec = mkvec();
    if (!vec) return mknil();

    Data *cursor = list;
    while (!is_nil(cursor)) {
        Data *val = fst(cursor);
        vec_push(vec, val);
        cursor = snd(cursor);
    }
    return vec;
}
