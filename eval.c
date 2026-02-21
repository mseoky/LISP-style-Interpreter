#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "eval.h"
#include "env.h"
#include "data.h"

/* Global Environment Reference */
extern Data *Env;

/* =========================================================================
 * Implementation
 * ========================================================================= */

/* Evaluate a list of arguments (Applicative Order) */
Data *eval_list(Data *args, Data *env) {
    if (is_nil(args)) return mknil();
    
    Data *head = eval(fst(args), env);
    Data *tail = eval_list(snd(args), env);
    return mkduo(head, tail);
}

/* Core Evaluator */
Data *eval(Data *expr, Data *env) {
    /* 1. Variable Reference */
    if (is_symbol(expr)) {
        return envget(env, expr);
    }

    /* 2. Vector Evaluation */
    /* Vectors are eagerly evaluated: [ (+ 1 2) ] -> [ 3 ] */
    if (expr && expr->tag == TVEC) {
        Data *new_vec = mkvec();
        size_t len = vec_len(expr);
        
        for (size_t i = 0; i < len; ++i) {
            Data *elem = vec_get(expr, i);
            Data *evaluated_elem = eval(elem, env);
            vec_push(new_vec, evaluated_elem);
        }
        return new_vec;
    }

    /* 3. Function Application or Special Form */
    if (is_duo(expr)) {
        Data *op = fst(expr);
        Data *args = snd(expr);

        /* Check for Special Forms (quote, assignment) */
        if (is_symbol(op)) {
            if (strcmp(op->sv, "quote") == 0) {
                return fst(args); // Return unevaluated argument
            }
            if (strcmp(op->sv, "=") == 0) {
                return eval_kw(op, args, &Env); // Pass global env pointer
            }
        }

        /* Evaluate Operator */
        Data *proc = eval(op, env);

        /* Apply Built-in Operator (TOPR) */
        if (proc && proc->tag == TOPR) {
            Data *eval_args = eval_list(args, env); // Evaluate arguments first
            Data *result = proc->fv(eval_args);     // Call C function
            
            return is_nil(result) ? mknil() : result;
        }
    }

    /* 4. Self-Evaluating (Numbers, Strings, etc.) */
    return expr;
}

/* Special Keyword Handler (Side-Effects) */
Data *eval_kw(Data *keyword, Data *args, Data **env_ptr) {
    /* Assignment: (= var val) */
    if (strcmp(keyword->sv, "=") == 0) {
        Data *var = fst(args);
        
        // Validation: LHS must be a symbol
        if (!is_symbol(var)) {
            printf("Error: Cannot assign to a non-variable value.\n");
            return mknil();
        }

        Data *rhs_expr = fst(snd(args));
        Data *val = eval(rhs_expr, *env_ptr);
        
        // Update Environment
        *env_ptr = envset(*env_ptr, var, val);
        
        return val; 
    }
    
    return mknil();
}
