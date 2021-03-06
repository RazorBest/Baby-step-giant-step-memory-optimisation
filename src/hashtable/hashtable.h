#ifndef __HASHTABLE_H
#define __HASHTABLE_H

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
    uint64_t value;
} info;

typedef struct Hashtable {
    info *buckets;
    size_t size;
    size_t hmax;
    double load_limit;
    unsigned int (*hash_function)(void*);
    int (*compare_function)(void*, void*);
} Hashtable;

Hashtable *init_ht(size_t hmax);

void put(Hashtable *ht, V63 key, V63 value);

int get(Hashtable *ht, V63 key, V63 *val);

int has_key(Hashtable *ht, V63 key);

size_t get_ht_size(Hashtable *ht);

size_t get_ht_hmax(Hashtable *ht);

void free_ht(Hashtable *ht);

unsigned int hash_function_int32(void *a);


#endif
