#include <stdlib.h>

#include "./hashtable.h"
#define LOAD_LIMIT 0.8

static void resize(struct Hashtable *ht);

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
    inf->value = 0;
}

static void init_buckets(info **buckets, size_t hmax) {
    size_t i;
    
    *buckets = malloc(hmax * sizeof(info));
    for (i = 0; i < hmax; i++) {
        init_info(&(*buckets)[i]);
    }
}

Hashtable *init_ht(size_t hmax) {
    Hashtable *ht;

    ht = malloc(sizeof(Hashtable));
    DIE(ht == NULL, "ht malloc error");

    ht->hmax = hmax;
    ht->load_limit = LOAD_LIMIT;
    ht->size = 0;

    init_buckets(&ht->buckets, hmax);

    return ht;
}

static info* get_info(Hashtable *ht, unsigned int index, V63 key) {
    info *buckets = ht->buckets;
    info *inf = buckets + index;
    size_t hmax = ht->hmax;

    while (!V63_IS_NULL(inf->key) && !V63_EQUAL(inf->key, key)) {
        index++;
        if (index >= hmax) {
            index = 0;
        }
        inf = buckets + index;
    }

    return inf;
}

static void put_info(Hashtable *ht, info *inf) {
    uint32_t index;
    info *inf2;

    if (inf == NULL) {
        return;
    }

    index = hash_function_int32(&inf->key) % ht->hmax;
    inf2 = get_info(ht, index, inf->key);
    *inf2 = *inf;
}

static void resize(Hashtable *ht) {
    size_t hmax = ht->hmax, i;
    info *old_buckets;
    info inf;

    old_buckets = ht->buckets;
    ht->hmax = 2 * hmax;
    init_buckets(&ht->buckets, ht->hmax);

    /* Redistribute the keys */
    for (i = 0; i < hmax; i++) {
        inf = old_buckets[i];
        if (!V63_IS_NULL(inf.key)) {
            put_info(ht, &inf);
        }
    }
    free(old_buckets);
}

void put(Hashtable *ht, V63 key, V63 value) {
    unsigned int index = hash_function_int32(&key) % ht->hmax;
    struct info *inf;

    inf = get_info(ht, index, key);

    if (!V63_IS_NULL(inf->key)) {
        inf->value = value;
        return;
    }

    ht->size++;

    inf->key = V63_SET_ON(key);
    inf->value = value;

    if ((double)ht->size / ht->hmax > ht->load_limit) {
        resize(ht);
    }
}

int get(Hashtable *ht, V63 key, V63 *val) {
    unsigned int index = hash_function_int32(&key) % ht->hmax;
    struct info *info;

    info = get_info(ht, index, key);
    if (V63_IS_NULL(info->key)) {
        return 0;
    }

    *val = info->value;

    return 1;
}

int has_key(struct Hashtable *ht, V63 key) {
    unsigned int index = hash_function_int32(&key) % ht->hmax;
    struct info *info;

    info = get_info(ht, index, key);
    if (V63_IS_NULL(info->key)) {
        return 0;
    }

    return 1;
}

void free_ht(struct Hashtable *ht) {
    free(ht->buckets);
    free(ht);
}

size_t get_ht_size(struct Hashtable *ht) {
    if (ht == NULL) {
        return -1;
    }

    return ht->size;
}

size_t get_ht_hmax(struct Hashtable *ht) {
    if (ht == NULL) {
        return -1;
    }

    return ht->hmax;
}
