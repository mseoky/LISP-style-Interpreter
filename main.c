#include <stdio.h>
#include "data.h"
#include "env.h"
#include "eval.h"
#include "bop.h"
#include "vop.h"

/* Global Parser Interface */
extern int yyparse(void);
Data *Env;

int main() {
    /* Initialize global environment */
    Env = mkenv(mknil());

    /* =========================================================================
     * Built-in Operator Registration
     * ========================================================================= */

    /* --- List Operations --- */
    Env = envset(Env, mksym("car"),  mkop(bop_car));
    Env = envset(Env, mksym("cdr"),  mkop(bop_cdr));
    Env = envset(Env, mksym("cons"), mkop(bop_cons));

    /* --- Arithmetic & String Operations --- */
    Env = envset(Env, mksym("+"), mkop(bop_add));
    Env = envset(Env, mksym("*"), mkop(bop_mul));

    /* --- Vector Operations (Constructors & Accessors) --- */
    Env = envset(Env, mksym("vec"),   mkop(vop_vec));
    Env = envset(Env, mksym("len"),   mkop(vop_len));
    Env = envset(Env, mksym("idx"),   mkop(vop_idx));
    Env = envset(Env, mksym("slice"), mkop(vop_slice));
    Env = envset(Env, mksym("find"),  mkop(vop_find));

    /* --- Vector Operations (Modifiers) --- */
    Env = envset(Env, mksym("set"),  mkop(vop_set));
    Env = envset(Env, mksym("ins"),  mkop(vop_ins));
    Env = envset(Env, mksym("del"),  mkop(vop_del));
    Env = envset(Env, mksym("push"), mkop(vop_push));
    Env = envset(Env, mksym("pop"),  mkop(vop_pop));
    Env = envset(Env, mksym("sort"), mkop(vop_sort));

    /* --- Constant Definitions --- */
    /* Register sort options as self-evaluating symbols */
    Env = envset(Env, mksym("asc"),  mksym("asc"));
    Env = envset(Env, mksym("desc"), mksym("desc"));

    yyparse();

    return 0;
}
