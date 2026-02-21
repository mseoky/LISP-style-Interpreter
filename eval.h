#ifndef __EVAL_H__
#define __EVAL_H__

#include "data.h"

/* =========================================================================
 * Evaluator Interface
 * ========================================================================= */

/* * Function: eval_list
 * Description: Evaluates a list of expressions recursively.
 * Used for processing function arguments.
 */
Data *eval_list(Data *args, Data *env);

/* * Function: eval
 * Description: The core evaluation routine.
 * Handles symbols, vectors, special forms (quote, =), and function applications.
 */
Data *eval(Data *expr, Data *env);

/* * Function: eval_kw
 * Description: Handles special keywords/forms that require environment modification.
 * Currently supports: Assignment (=)
 */
Data *eval_kw(Data *keyword, Data *args, Data **env_ptr);

#endif
