#include <string.h>
#include <stdio.h>
#include "env.h"

/* =========================================================================
 * Implementation
 * ========================================================================= */

/* Create a new environment: (Parent . NIL) */
Data *mkenv(Data *parent) {
    return mkduo(parent, mknil());
}

/* Lookup a symbol's value */
Data *envget(Data *env, Data *key) {
    if (is_nil(env)) return mknil();

    /* Structure decomposition */
    Data *parent = fst(env);
    Data *bindings = snd(env);
    Data *cursor = bindings;

    /* 1. Search in the current scope */
    while (is_duo(cursor)) {
        Data *pair = fst(cursor);       // (Key . Value)
        Data *current_key = fst(pair);
        
        if (is_symbol(current_key) && strcmp(current_key->sv, key->sv) == 0) {
            return snd(pair); // Found
        }
        cursor = snd(cursor);
    }

    /* 2. Recursive search in parent scope (Lexical Scoping) */
    if (!is_nil(parent)) {
        return envget(parent, key);
    }
    
    /* Not found */
    return mknil();
}

/* Define/Update a variable */
Data *envset(Data *env, Data *key, Data *value) {
    Data *parent = fst(env);
    Data *bindings = snd(env);

    /* Create new binding pair: (Key . Value) */
    Data *new_binding = mkduo(key, value);

    /* Prepend to the binding list (Shadowing existing keys) */
    Data *new_bindings = mkduo(new_binding, bindings);

    /* Return updated environment head */
    return mkduo(parent, new_bindings); 
}
