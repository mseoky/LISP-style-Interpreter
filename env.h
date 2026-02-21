#ifndef __ENV_H__
#define __ENV_H__

#include "data.h"

/* =========================================================================
 * Environment Management
 * Structure: (Parent-Env . Binding-List)
 * Binding-List: ((Key . Value) (Key . Value) ...)
 * ========================================================================= */

/* * Function: mkenv
 * Description: Creates a new environment frame linked to a parent scope.
 */
Data *mkenv(Data *parent);

/* * Function: envget
 * Description: Retrieves the value associated with a symbol.
 * Searches current scope first, then recursively searches parent scopes.
 */
Data *envget(Data *env, Data *key);

/* * Function: envset
 * Description: Binds a symbol to a value in the current scope.
 * If the symbol exists, it shadows the previous definition (adds to front).
 * Returns the updated environment structure.
 */
Data *envset(Data *env, Data *key, Data *value);

#endif
