#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bop.h"

/* =========================================================================
 * List Operations
 * ========================================================================= */

Data *bop_car(Data *args) {
    /* args is a list of arguments: ( (1 . 2) ) -> car returns 1 */
    return fst(fst(args));
}

Data *bop_cdr(Data *args) {
    /* args is a list of arguments: ( (1 . 2) ) -> cdr returns 2 */
    return snd(fst(args));
}

Data *bop_cons(Data *args) {
    /* args: (head tail) -> cons returns (head . tail) */
    return mkduo(fst(args), fst(snd(args)));
}

/* =========================================================================
 * Polymorphic Arithmetic Operations
 * ========================================================================= */

/* * Function: bop_add
 * Description: Handles Addition (+) for Numbers, Strings, and Vectors.
 * Priority: Vector > String > Number
 */
Data *bop_add(Data *args) {
    int is_str_mode = 0;
    int is_vec_mode = 0;
    Data *cursor = args;

    /* Pass 1: Type Checking & Mode Detection */
    while (!is_nil(cursor)) {
        Data *val = fst(cursor);
        if (!val) return mknil();
        
        if (val->tag == TVEC) {
            is_vec_mode = 1;
        } else if (val->tag == TSTR) {
            is_str_mode = 1;
        } else if (val->tag != TNAT && val->tag != TCHR) {
            /* Invalid type for addition */
            return mknil();
        }
        
        cursor = snd(cursor);
    }

    /* Case 1: Vector Concatenation */
    if (is_vec_mode) {
        /* Python-style: If one is a vector, all must be vectors (or throw error) */
        /* Currently strict: Mixed types with vectors result in NIL */
        if (is_str_mode) return mknil(); 

        Data *new_vec = mkvec();
        if (!new_vec) return mknil();

        cursor = args;
        while (!is_nil(cursor)) {
            Data *val = fst(cursor);
            
            if (val->tag != TVEC) return mknil(); // Type mismatch

            /* Append all elements of the current vector to new_vec */
            Nat len = vec_len(val);
            for (Nat i = 0; i < len; ++i) {
                vec_push(new_vec, vec_get(val, i));
            }
            cursor = snd(cursor);
        }
        return new_vec;
    }

    /* Case 2: Numeric Addition */
    if (!is_str_mode) {
        unsigned int sum = 0;
        cursor = args;
        while (!is_nil(cursor)) {
            Data *val = fst(cursor);
            sum += (val->tag == TNAT) ? val->nv : (unsigned int)val->cv;
            cursor = snd(cursor);
        }
        return mknat(sum);
    }

    /* Case 3: String Concatenation */
    /* Calculate required buffer size first */
    size_t total_len = 0;
    cursor = args;
    while (!is_nil(cursor)) {
        Data *val = fst(cursor);
        if (val->tag == TSTR) total_len += strlen(val->sv);
        else if (val->tag == TNAT) {
            char tmp[32];
            total_len += snprintf(tmp, 32, "%u", val->nv);
        } else if (val->tag == TCHR) {
            total_len += 1;
        }
        cursor = snd(cursor);
    }

    char *buf = (char*)malloc(total_len + 1);
    if (!buf) return mknil();
    
    /* Construct the result string */
    char *out = buf;
    cursor = args;
    while (!is_nil(cursor)) {
        Data *val = fst(cursor);
        if (val->tag == TSTR) {
            size_t len = strlen(val->sv);
            memcpy(out, val->sv, len);
            out += len;
        } else if (val->tag == TNAT) {
            out += sprintf(out, "%u", val->nv);
        } else if (val->tag == TCHR) {
            *out++ = val->cv;
        }
        cursor = snd(cursor);
    }
    *out = '\0';

    Data *res = mkstr(buf);
    free(buf);
    return res;
}

/* * Function: bop_mul
 * Description: Handles Multiplication (*) for Numbers and Repetition for Strings/Vectors.
 * Priority: Vector > Number > String
 */
Data *bop_mul(Data *args) {
    unsigned int prod = 1;      // For numeric product
    unsigned int repeat = 1;    // For repetition count
    int str_count = 0;
    int vec_count = 0;
    
    Data *target_str = NULL;
    Data *target_vec = NULL;
    Data *cursor = args;

    /* Pass 1: Analyze Arguments */
    while (!is_nil(cursor)) {
        Data *val = fst(cursor);
        if (!val) return mknil();

        if (val->tag == TNAT) {
            prod *= val->nv;
            repeat *= val->nv;
        } else if (val->tag == TCHR) {
            prod *= (unsigned int)val->cv;
            repeat *= (unsigned int)val->cv;
        } else if (val->tag == TSTR) {
            target_str = val;
            str_count++;
        } else if (val->tag == TVEC) {
            target_vec = val;
            vec_count++;
        } else {
            return mknil(); // Invalid type
        }
        cursor = snd(cursor);
    }

    /* Case 1: Vector Repetition (* [1 2] 3) */
    if (vec_count > 0) {
        /* Only allow 1 vector and numbers. No strings mixed. */
        if (vec_count != 1 || str_count > 0) return mknil();
        
        Data *new_vec = mkvec();
        if (!new_vec) return mknil();
        
        Nat len = vec_len(target_vec);
        for (unsigned int k = 0; k < repeat; ++k) {
            for (Nat i = 0; i < len; ++i) {
                vec_push(new_vec, vec_get(target_vec, i));
            }
        }
        return new_vec;
    }

    /* Case 2: Numeric Multiplication (* 2 3) */
    if (str_count == 0) return mknat(prod);

    /* Case 3: String Repetition (* "a" 3) */
    if (str_count != 1) return mknil(); // Only 1 string allowed
    if (repeat == 0) return mkstr("");

    size_t len = strlen(target_str->sv);
    size_t total_len = len * repeat;
    
    char *buf = (char*)malloc(total_len + 1);
    if (!buf) return mknil();

    char *out = buf;
    for (unsigned int i = 0; i < repeat; i++) {
        memcpy(out, target_str->sv, len);
        out += len;
    }
    *out = '\0';

    Data *res = mkstr(buf);
    free(buf);
    return res;
}
