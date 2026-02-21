#ifndef __VOP_H__
#define __VOP_H__

#include "data.h"

/* =========================================================================
 * Vector Constructors & Accessors (Read-Only)
 * ========================================================================= */

/* * Function: vop_vec
 * Usage: (vec e1 e2 ...) or [e1 e2 ...]
 * Description: Creates a new vector from the given arguments.
 */
Data *vop_vec(Data *args);

/* * Function: vop_len
 * Usage: (len v)
 * Description: Returns the number of elements in vector v.
 */
Data *vop_len(Data *args);

/* * Function: vop_idx
 * Usage: (idx v i)
 * Description: Returns the element at index i. Returns NIL if out of bounds.
 */
Data *vop_idx(Data *args);

/* * Function: vop_slice
 * Usage: (slice v start end)
 * Description: Returns a new vector (shallow copy) containing elements from v[start] to v[end-1].
 */
Data *vop_slice(Data *args);

/* * Function: vop_find
 * Usage: (find v val)
 * Description: Linear search. Returns the first index of val, or NIL if not found.
 */
Data *vop_find(Data *args);

/* =========================================================================
 * Vector Modifiers (Side Effects)
 * ========================================================================= */

/* * Function: vop_set
 * Usage: (set v i val)
 * Description: Overwrites the element at index i with val.
 */
Data *vop_set(Data *args);

/* * Function: vop_ins
 * Usage: (ins v i val)
 * Description: Inserts val at index i, shifting subsequent elements to the right.
 */
Data *vop_ins(Data *args);

/* * Function: vop_del
 * Usage: (del v i)
 * Description: Removes the element at index i, shifting subsequent elements to the left.
 */
Data *vop_del(Data *args);

/* * Function: vop_push
 * Usage: (push v val)
 * Description: Appends val to the end of the vector.
 */
Data *vop_push(Data *args);

/* * Function: vop_pop
 * Usage: (pop v)
 * Description: Removes and returns the last element of the vector.
 */
Data *vop_pop(Data *args);

/* * Function: vop_sort
 * Usage: (sort v [mode])
 * Description: Sorts the vector in-place.
 * Optional mode: 'asc (ascending, default) or 'desc (descending).
 */
Data *vop_sort(Data *args);

#endif
