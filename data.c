#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"

/* =========================================================================
 * Internal Helpers
 * ========================================================================= */

/* * Helper: vec_ensure_capacity
 * Description: Ensures the vector has enough capacity for new elements.
 * Strategy: Doubles capacity when full (Amortized O(1)).
 */
static int vec_ensure_capacity(Vec v, size_t need) {
    if (!v) return 0;
    if (v->cap >= need) return 1;

    size_t newcap = v->cap ? v->cap : 4;
    while (newcap < need) newcap *= 2;

    Data **newitems = (Data**)realloc(v->items, newcap * sizeof(Data*));
    if (!newitems) return 0;

    v->items = newitems;
    v->cap = newcap;
    return 1;
}

/* * Helper: mkdata
 * Description: Allocates memory for a generic Data node and sets the tag.
 */
static Data *mkdata(Tag tag) {
    Data *d = (Data*)malloc(sizeof(Data));
    if (!d) return NULL;
    d->tag = tag;
    return d;
}

/* =========================================================================
 * Constructors
 * ========================================================================= */

Data *mknat(Nat n) {
    Data *d = mkdata(TNAT);
    if (d) d->nv = n;
    return d;
}

Data *mkchr(Chr c) {
    Data *d = mkdata(TCHR);
    if (d) d->cv = c;
    return d;
}

Data *mkstr(Str s) {
    Data *d = mkdata(TSTR);
    if (d) d->sv = strdup(s);
    return d;
}

Data *mksym(Str s) {
    Data *d = mkdata(TSYM);
    if (d) d->sv = strdup(s);
    return d;
}

Data *mknil() {
    return mkdata(TNIL);
}

Data *mkduo(Data *a, Data *b) {
    Data *d = mkdata(TDUO);
    if (!d) return NULL;
    
    Duo p = (Duo)malloc(sizeof(Pair));
    if (!p) {
        free(d);
        return NULL;
    }
    p->d[0] = a;
    p->d[1] = b;
    d->pv = p;
    return d;
}

Data *mkop(Opr fn) {
    Data *d = mkdata(TOPR);
    if (d) d->fv = fn;
    return d;
}

Data *mkvec() {
    Data *d = mkdata(TVEC);
    if (!d) return NULL;
    
    Vec v = (Vec)malloc(sizeof(VecNode));
    if (!v) {
        free(d);
        return NULL;
    }
    v->items = NULL;
    v->len = 0;
    v->cap = 0;
    d->vv = v;
    return d;
}

/* =========================================================================
 * Vector Operations
 * ========================================================================= */

Nat vec_len(Data *vec) {
    if (!vec || vec->tag != TVEC || !vec->vv) return 0;
    return (Nat)vec->vv->len;
}

Data *vec_get(Data *vec, Nat idx) {
    if (!vec || vec->tag != TVEC || !vec->vv) return NULL;
    if (idx >= vec->vv->len) return NULL;
    return vec->vv->items[idx];
}

int vec_set(Data *vec, Nat idx, Data *val) {
    if (!vec || vec->tag != TVEC || !vec->vv) return 0;
    if (idx >= vec->vv->len) return 0;
    
    vec->vv->items[idx] = val;
    return 1;
}

int vec_push(Data *vec, Data *val) {
    if (!vec || vec->tag != TVEC || !vec->vv) return 0;
    if (!vec_ensure_capacity(vec->vv, vec->vv->len + 1)) return 0;
    
    vec->vv->items[vec->vv->len++] = val;
    return 1;
}

Data *vec_pop(Data *vec) {
    if (!vec || vec->tag != TVEC || !vec->vv) return NULL;
    if (vec->vv->len == 0) return NULL;
    
    return vec->vv->items[--vec->vv->len];
}

int vec_insert(Data *vec, Nat idx, Data *val) {
    if (!vec || vec->tag != TVEC || !vec->vv) return 0;
    if (idx > vec->vv->len) return 0; // Allowing insert at end (== len)
    if (!vec_ensure_capacity(vec->vv, vec->vv->len + 1)) return 0;
    
    /* Shift elements to the right */
    for (size_t i = vec->vv->len; i > idx; --i) {
        vec->vv->items[i] = vec->vv->items[i - 1];
    }
    vec->vv->items[idx] = val;
    vec->vv->len++;
    return 1;
}

Data *vec_delete(Data *vec, Nat idx) {
    if (!vec || vec->tag != TVEC || !vec->vv) return NULL;
    if (idx >= vec->vv->len) return NULL;
    
    Data *removed = vec->vv->items[idx];
    
    /* Shift elements to the left */
    for (size_t i = idx + 1; i < vec->vv->len; ++i) {
        vec->vv->items[i - 1] = vec->vv->items[i];
    }
    vec->vv->len--;
    return removed;
}

/* =========================================================================
 * Accessors & Predicates
 * ========================================================================= */

Data *fst(Data *d) { return d->pv->d[0]; }
Data *snd(Data *d) { return d->pv->d[1]; }
Data **pfst(Data *d) { return &d->pv->d[0]; }
Data **psnd(Data *d) { return &d->pv->d[1]; }

int is_atom(Data *d) { return d && d->tag != TDUO; }
int is_list(Data *d) { return d && (d->tag == TNIL || d->tag == TDUO); }
int is_nil(Data *d) { return d && d->tag == TNIL; }
int is_duo(Data *d) { return d && d->tag == TDUO; }
int is_symbol(Data *d) { return d && d->tag == TSYM; }
int is_keyword(Data *d) { return is_symbol(d) && d->sv && d->sv[0] == ':'; }
int is_vector(Data *d) { return d && d->tag == TVEC; }

/* =========================================================================
 * Printer
 * ========================================================================= */

int pr_data(Data *d) {
    if (!d) {
        printf("()");
        return 0;
    }
    switch(d->tag) {
        case TNAT: printf("%u", d->nv); break;
        case TCHR: printf("%c", d->cv); break;
        case TSTR: printf("%s", d->sv); break;
        case TSYM: printf("%s", d->sv); break;
        case TNIL: printf("()"); break;
        case TDUO:
            printf("(");
            pr_data(fst(d));
            printf(" . ");
            pr_data(snd(d));
            printf(")");
            break;
        case TVEC: {
            printf("[");
            if (d->vv) {
                size_t len = d->vv->len;
                for (size_t i = 0; i < len; ++i) {
                    pr_data(d->vv->items[i]);
                    if (i + 1 < len) printf(" ");
                }
            }
            printf("]");
            break;
        }
        case TOPR:
            printf("<opr>");
            break;
    }
    return 0;
}
