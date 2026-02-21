#ifndef __DATA_H__
#define __DATA_H__

#include <stddef.h>

/* =========================================================================
 * Type Definitions & Data Structures
 * ========================================================================= */

/* Tag: Type discriminator for the Data union */
typedef enum { 
    TNAT,   // Natural Number (unsigned int)
    TCHR,   // Character
    TSTR,   // String
    TSYM,   // Symbol (Variable name)
    TNIL,   // Empty List (NIL)
    TDUO,   // Pair (Cons cell)
    TOPR,   // Built-in Operator (Function pointer)
    TVEC    // Vector (Dynamic Array)
} Tag;

typedef unsigned int Nat;
typedef char Chr;
typedef char *Str;

/* Forward declarations */
typedef struct _Pair *Duo;
typedef struct _Vec *Vec;
typedef struct _Data *(*Opr)(struct _Data *args);

/* Main Data Structure (Tagged Union) */
typedef struct _Data {
    Tag tag;
    union {
        Nat nv;     // TNAT
        Chr cv;     // TCHR
        Duo pv;     // TDUO
        Str sv;     // TSTR, TSYM
        Opr fv;     // TOPR
        Vec vv;     // TVEC
    };
} Data;

/* Pair Structure (Cons Cell for Lists) */
typedef struct _Pair {
    Data *d[2];     // [0]: car, [1]: cdr
} Pair;

/* Vector Structure (Dynamic Array) */
typedef struct _Vec {
    Data **items;   // Array of pointers to Data
    size_t len;     // Current number of elements
    size_t cap;     // Current allocated capacity
} VecNode;

/* =========================================================================
 * Function Prototypes
 * ========================================================================= */

/* --- Constructors --- */
Data *mknat(Nat n);
Data *mkchr(Chr c);
Data *mkstr(Str s);
Data *mksym(Str s);
Data *mknil();
Data *mkduo(Data *a, Data *b);
Data *mkop(Opr fn);
Data *mkvec();

/* --- Vector Operations --- */
Data *vec_get(Data *vec, Nat idx);
int   vec_set(Data *vec, Nat idx, Data *val);
int   vec_push(Data *vec, Data *val);
Data *vec_pop(Data *vec);
int   vec_insert(Data *vec, Nat idx, Data *val);
Data *vec_delete(Data *vec, Nat idx);
Nat   vec_len(Data *vec);

/* --- List Accessors --- */
Data *fst(Data *duo);       // car
Data *snd(Data *duo);       // cdr
Data **pfst(Data *duo);     // &car
Data **psnd(Data *duo);     // &cdr

/* --- Type Predicates --- */
int is_atom(Data *d);
int is_list(Data *d);
int is_nil(Data *d);
int is_duo(Data *d);
int is_symbol(Data *d);
int is_keyword(Data *d);
int is_vector(Data *d);

/* --- I/O --- */
int pr_data(Data *d);

#endif
