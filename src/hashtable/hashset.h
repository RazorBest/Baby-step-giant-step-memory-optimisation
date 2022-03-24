#ifndef __HASHSET_H
#define __HASHSET_H

#include <inttypes.h>
#include "../utils/utils.h"

typedef uint64_t V63;
#define V63_NULL ((V63)0)
#define V63_ON_MASK ((V63)(((V63)1) << (sizeof(V63)*8-1)))
#define V63_IS_NULL(x) ((~x) & V63_ON_MASK)
#define V63_SET_ON(x) ((x) | V63_ON_MASK)
#define V63_SET_OFF(x) ((x) & (~V63_ON_MASK))
#define V63_EQUAL(x, y) (((x) & (~V63_ON_MASK)) == (y))

typedef struct info {
    V63 key;
} info;

typedef struct Hashset {
    info *buckets;
    size_t size;
    size_t hmax;
    double load_limit;
    unsigned int (*hash_function)(void*);
    int (*compare_function)(void*, void*);
} Hashset;

Hashset *init_hs(size_t hmax);

void hs_put(Hashset *hs, V63 key);

int has_key(Hashset *hs, V63 key);

size_t get_hs_size(Hashset *hs);

size_t get_hs_hmax(Hashset *hs);

void free_hs(Hashset *hs);

unsigned int hash_function_int32(void *a);

#endif
