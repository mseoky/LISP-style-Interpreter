%{
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "y.tab.h"
%}

    /* =========================================================================
     * Regular Expression Definitions
     * ========================================================================= */
digit   [0-9]
alpha   [a-zA-Z]
symbol  [-+*/%=]
ch      \'[^\']\'
/* String pattern that allows escaped quotes (e.g., "foo \"bar\"") */
str     \"(\\.|[^"\\])*\"

%%

    /* =========================================================================
     * Token Rules
     * ========================================================================= */

{ch} {
    /* Extract character literal */
    yylval.cval = yytext[1];
    return CHAR;
}

{str} {
    /* Process string literal with Escape Sequence Handling */
    /* Allocate max possible length (yyleng - 2 quotes + null terminator) */
    char *s = (char*)malloc(yyleng - 1);
    char *p = s;
    int i = 1; /* Skip opening quote */

    /* Iterate until closing quote */
    while (i < yyleng - 1) {
        if (yytext[i] == '\\') {
            /* Handle Escape Sequences */
            i++; /* Move past the backslash */
            switch (yytext[i]) {
                case 'n': *p++ = '\n'; break;
                case 't': *p++ = '\t'; break;
                case 'r': *p++ = '\r'; break;
                case '\\':*p++ = '\\'; break;
                case '"': *p++ = '"';  break; /* \" becomes " */
                default:  *p++ = yytext[i]; break;
            }
        } else {
            /* Regular character */
            *p++ = yytext[i];
        }
        i++;
    }
    *p = '\0'; /* Null-terminate */
    
    yylval.sval = s;
    return STRING;
}

{digit}+ {
    /* Parse integer */
    yylval.ival = atoi(yytext);
    return NUM;
}

{alpha}({alpha}|{digit})* {
    /* Parse variable/identifier */
    yylval.sval = strdup(yytext);
    return VAR;
}

{symbol} {
    /* Parse operator symbol */
    yylval.sval = strdup(yytext);
    return SYM;
}

    /* Delimiters */
"("   { return '('; }
")"   { return ')'; }
"["   { return '['; }
"]"   { return ']'; }

    /* Whitespace & Newline */
[ \t\r] { }
\n      { return '\n'; }

    /* Error Fallback */
. {
    fprintf(stderr, "%c illegal\n", yytext[0]);
    exit(-1);
}

%%

int yywrap() { return 1; }
