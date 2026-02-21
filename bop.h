#ifndef __BOP_H__
#define __BOP_H__

#include "data.h"

/* =========================================================================
 * Built-in Operators Interface
 * ========================================================================= */

/* --- List Operations --- */
/* Returns the first element (car) of a list */
Data *bop_car(Data *args);

/* Returns the rest of the list (cdr) */
Data *bop_cdr(Data *args);

/* Constructs a pair from two arguments (cons) */
Data *bop_cons(Data *args);

/* --- Polymorphic Arithmetic Operations --- */

/* * Function: bop_add
 * Usage: (+ ...)
 * Supports:
 * 1. Vector Concatenation: (+ [1] [2]) -> [1 2]
 * 2. String Concatenation: (+ "a" "b") -> "ab"
 * 3. Numeric Addition:     (+ 1 2)     -> 3
 */
Data *bop_add(Data *args);

/* * Function: bop_mul
 * Usage: (* ...)
 * Supports:
 * 1. Vector Repetition: (* [1 2] 2) -> [1 2 1 2]
 * 2. String Repetition: (* "a" 3)   -> "aaa"
 * 3. Numeric Product:   (* 2 3)     -> 6
 */
Data *bop_mul(Data *args);

#endif
