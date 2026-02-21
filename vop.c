#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "vop.h"

/* =========================================================================
 * Argument Parsing Helpers
 * ========================================================================= */

/* Parse args: (Vector, Index) -> used by idx, del */
static int parse_vec_idx(Data *args, Data **vec_out, Nat *idx_out) {
    Data *vec_arg = fst(args);
    Data *idx_arg = fst(snd(args));

    if (!vec_arg || vec_arg->tag != TVEC) return 0;
    if (!idx_arg || idx_arg->tag != TNAT) return 0;

    *vec_out = vec_arg;
    *idx_out = idx_arg->nv;
    return 1;
}

/* Parse args: (Vector, Index, Value) -> used by set, ins */
static int parse_vec_idx_val(Data *args, Data **vec_out, Nat *idx_out, Data **val_out) {
    Data *vec_arg = fst(args);
    Data *idx_arg = fst(snd(args));
    Data *val_arg = fst(snd(snd(args)));

    if (!vec_arg || vec_arg->tag != TVEC) return 0;
    if (!idx_arg || idx_arg->tag != TNAT) return 0;
    if (!val_arg) return 0;

    *vec_out = vec_arg;
    *idx_out = idx_arg->nv;
    *val_out = val_arg;
    return 1;
}

/* Parse args: (Vector, Start, End) -> used by slice */
static int parse_vec_idx_idx(Data *args, Data **vec_out, Nat *start_out, Nat *end_out) {
    Data *vec_arg = fst(args);
    Data *start_arg = fst(snd(args));
    Data *end_arg = fst(snd(snd(args)));

    if (!vec_arg || vec_arg->tag != TVEC) return 0;
    if (!start_arg || start_arg->tag != TNAT) return 0;
    if (!end_arg || end_arg->tag != TNAT) return 0;

    *vec_out = vec_arg;
    *start_out = start_arg->nv;
    *end_out = end_arg->nv;
    return 1;
}

/* Parse args: (Vector, Value) -> used by find, push */
static int parse_vec_val(Data *args, Data **vec_out, Data **val_out) {
    Data *vec_arg = fst(args);
    Data *val_arg = fst(snd(args));

    if (!vec_arg || vec_arg->tag != TVEC) return 0;
    if (!val_arg) return 0;

    *vec_out = vec_arg;
    *val_out = val_arg;
    return 1;
}

/* =========================================================================
 * Comparison Logic (for Sorting & Searching)
 * ========================================================================= */

/* * Compare two Data items.
 * Order: Nat < Chr < Str < ... < Vec
 * Returns: 1 (>), -1 (<), 0 (=)
 */
static int compare_data(Data *d1, Data *d2) {
    if (!d1 && !d2) return 0;
    if (!d1) return 1;
    if (!d2) return -1;

    // 1. Check type hierarchy
    if (d1->tag != d2->tag) {
        return d1->tag - d2->tag;
    }

    // 2. Compare values within the same type
    switch (d1->tag) {
        case TNAT:
            if (d1->nv > d2->nv) return 1;
            if (d1->nv < d2->nv) return -1;
            return 0;
        case TCHR:
            if (d1->cv > d2->cv) return 1;
            if (d1->cv < d2->cv) return -1;
            return 0;
        case TSTR:
            return strcmp(d1->sv, d2->sv);
        case TVEC: {
            // Lexicographical comparison for vectors
            Nat len1 = vec_len(d1);
            Nat len2 = vec_len(d2);
            Nat min_len = (len1 < len2) ? len1 : len2;

            for (Nat i = 0; i < min_len; ++i) {
                Data *e1 = vec_get(d1, i);
                Data *e2 = vec_get(d2, i);
                
                int cmp = compare_data(e1, e2); // Recursive comparison
                if (cmp != 0) return cmp;
            }

            if (len1 < len2) return -1;
            if (len1 > len2) return 1;
            return 0;
        }
        default:
            return 0;
    }
}

static int comp_asc(const void *a, const void *b) {
    return compare_data(*(Data**)a, *(Data**)b);
}

static int comp_desc(const void *a, const void *b) {
    return compare_data(*(Data**)b, *(Data**)a);
}

/* =========================================================================
 * Vector Operations Implementation
 * ========================================================================= */

/* (vec e1 e2 ...) or [ ... ] : Create new vector */
Data *vop_vec(Data *args) {
    Data *vec = mkvec();
    if (!vec) return mknil();

    Data *cursor = args;
    while (!is_nil(cursor)) {
        if (!vec_push(vec, fst(cursor))) return mknil();
        cursor = snd(cursor);
    }
    return vec;
}

/* (len v) : Return length of vector */
Data *vop_len(Data *args) {
    Data *vec = fst(args);
    if (!vec || vec->tag != TVEC) return mknil();
    return mknat(vec_len(vec));
}

/* (idx v i) : Get element at index i */
Data *vop_idx(Data *args) {
    Data *vec; 
    Nat idx;
    
    if (!parse_vec_idx(args, &vec, &idx)) return mknil();
    
    Data *elem = vec_get(vec, idx);
    return elem ? elem : mknil();
}

/* (slice v start end) : Create shallow copy of subsequence [start, end) */
Data *vop_slice(Data *args) {
    Data *vec;
    Nat start, end;

    if (!parse_vec_idx_idx(args, &vec, &start, &end)) return mknil();

    Nat len = vec_len(vec);
    if (start > end || end > len) return mknil();

    Data *new_vec = mkvec();
    if (!new_vec) return mknil();

    // Perform shallow copy
    for (Nat i = start; i < end; ++i) {
        Data *elem = vec_get(vec, i);
        vec_push(new_vec, elem);
    }
    return new_vec;
}

/* (find v val) : Linear search, returns index or NIL */
Data *vop_find(Data *args) {
    Data *vec;
    Data *target;

    if (!parse_vec_val(args, &vec, &target)) return mknil();

    Nat len = vec_len(vec);
    for (Nat i = 0; i < len; ++i) {
        Data *elem = vec_get(vec, i);
        if (compare_data(elem, target) == 0) {
            return mknat(i);
        }
    }
    return mknil();
}

/* (set v i val) : Update element at index i */
Data *vop_set(Data *args) {
    Data *vec; 
    Nat idx; 
    Data *val;

    if (!parse_vec_idx_val(args, &vec, &idx, &val)) return mknil();

    if (!vec_set(vec, idx, val)) return mknil();
    return vec;
}

/* (ins v i val) : Insert val at index i, shifting right */
Data *vop_ins(Data *args) {
    Data *vec; 
    Nat idx; 
    Data *val;

    if (!parse_vec_idx_val(args, &vec, &idx, &val)) return mknil();

    if (!vec_insert(vec, idx, val)) return mknil();
    return vec;
}

/* (del v i) : Delete element at index i, shifting left */
Data *vop_del(Data *args) {
    Data *vec; 
    Nat idx;

    if (!parse_vec_idx(args, &vec, &idx)) return mknil();

    Data *removed = vec_delete(vec, idx);
    return removed ? removed : mknil();
}

/* (push v val) : Append val to end */
Data *vop_push(Data *args) {
    Data *vec;
    Data *val;

    if (!parse_vec_val(args, &vec, &val)) return mknil();

    if (!vec_push(vec, val)) return mknil();
    return vec;
}

/* (pop v) : Remove last element */
Data *vop_pop(Data *args) {
    Data *vec = fst(args);
    if (!vec || vec->tag != TVEC) return mknil();

    Data *res = vec_pop(vec);
    return res ? res : mknil();
}

/* (sort v [mode]) : In-place sort. Mode: 'asc (default), 'desc */
Data *vop_sort(Data *args) {
    Data *vec = fst(args);
    
    // Check for optional mode argument
    Data *rest = snd(args);
    Data *opt = is_nil(rest) ? NULL : fst(rest);

    if (!vec || vec->tag != TVEC) return mknil();
    if (vec_len(vec) < 2) return vec;

    int (*comparator)(const void*, const void*) = comp_asc;

    if (opt && opt->tag == TSYM) {
        if (strcmp(opt->sv, "desc") == 0) {
            comparator = comp_desc;
        } else if (strcmp(opt->sv, "asc") == 0) {
            comparator = comp_asc;
        }
    }

    qsort(vec->vv->items, vec->vv->len, sizeof(Data*), comparator);
    return vec;
}
