#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./hashset.h"
#define LOAD_LIMIT 0.8

static void resize(struct Hashset *hs);

/* Found it in libcfu-0.03 */
/* Perl's hash function */
static uint32_t hash_func(void *key, size_t length) {
    size_t i = length;
    uint32_t hv = 0; /* could put a seed here instead of zero */
    unsigned char *s = (unsigned char *)key;
    while (i--) {
        hv += *s++;
        hv += (hv << 10);
        hv ^= (hv >> 6);
    }
    hv += (hv << 3);
    hv ^= (hv >> 11);
    hv += (hv << 15);
    
    return hv;
}

uint32_t hash_function_int32(void *a) {
    return hash_func(a, sizeof(int32_t));
}

static void init_info(info *inf) {
    inf->key = V63_NULL;
}

static void init_buckets(info **buckets, size_t hmax) {
    size_t i;
    
    *buckets = malloc(hmax * sizeof(info));
    for (i = 0; i < hmax; i++) {
        init_info(&(*buckets)[i]);
    }
}

Hashset *init_hs(size_t hmax) {
    Hashset *hs;

    hs = malloc(sizeof(*hs));
    DIE(hs == NULL, "hs malloc error");

    hs->hmax = hmax;
    hs->load_limit = LOAD_LIMIT;
    hs->size = 0;

    init_buckets(&hs->buckets, hmax);

    return hs;
}

static info* get_info(Hashset *hs, unsigned int index, V63 key) {
    info *buckets = hs->buckets;
    info *inf = buckets + index;
    size_t hmax = hs->hmax;
    // Mask for checking if the key is put
    V63 k_mask = 1;
    // Use the MSB as an on/off flag
    k_mask <<= sizeof(k_mask) * 8 - 1;

    while (!V63_IS_NULL(inf->key) && !V63_EQUAL(inf->key, key)) {
        index++;
        if (index >= hmax) {
            index = 0;
        }
        inf = buckets + index;
    }

    return inf;
}

static void put_info(Hashset *hs, info *inf) {
    uint32_t index;
    info *inf2;

    if (inf == NULL) {
        return;
    }

    index = hash_function_int32(&inf->key) % hs->hmax;
    inf2 = get_info(hs, index, inf->key);
    *inf2 = *inf;
}

static void resize(Hashset *hs) {
    size_t hmax = hs->hmax, i;
    info *old_buckets;
    info inf;

    old_buckets = hs->buckets;
    hs->hmax = 2 * hmax;
    init_buckets(&hs->buckets, hs->hmax);

    /* Redistribute the keys */
    for (i = 0; i < hmax; i++) {
        inf = old_buckets[i];
        if (!V63_IS_NULL(inf.key)) {
            put_info(hs, &inf);
        }
    }
    free(old_buckets);
}

void hs_put(Hashset *hs, V63 key) {
    unsigned int index = hash_function_int32(&key) % hs->hmax;
    struct info *inf;

    inf = get_info(hs, index, key);

    if (!V63_IS_NULL(inf->key)) {
        return;
    }

    hs->size++;

    inf->key = V63_SET_ON(key);

    if ((double)hs->size / hs->hmax > hs->load_limit) {
        resize(hs);
    }
}

int has_key(struct Hashset *hs, V63 key) {
    unsigned int index = hash_function_int32(&key) % hs->hmax;
    struct info *info;

    info = get_info(hs, index, key);
    if (V63_IS_NULL(info->key)) {
        return 0;
    }

    return 1;
}

void free_hs(struct Hashset *hs) {
    free(hs->buckets);
    free(hs);
}

size_t get_hs_size(struct Hashset *hs) {
    if (hs == NULL) {
        return -1;
    }

    return hs->size;
}

size_t get_hs_hmax(struct Hashset *hs) {
    if (hs == NULL) {
        return -1;
    }

    return hs->hmax;
}
