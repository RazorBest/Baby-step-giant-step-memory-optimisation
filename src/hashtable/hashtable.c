#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./hashtable.h"
#define LOAD_LIMIT 0.8

static void resize(struct Hashtable *ht);

int compare_function_int32(void *a, void *b) {
    int32_t int_a = *((int32_t *)a);
    int32_t int_b = *((int32_t *)b);

    if (int_a == int_b) {
        return 0;
    } else if (int_a < int_b) {
        return -1;
    } else {
        return 1;
    }
}

int compare_function_strings(void *a, void *b) {
    char *str_a = (char *)a;
    char *str_b = (char *)b;

    return strcmp(str_a, str_b);
}

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

unsigned int hash_function_string(void *a) {
    /*
     * Credits: http://www.cse.yorku.ca/~oz/hash.html
     */
    unsigned char *puchar_a = (unsigned char*) a;
    unsigned long hash = 5381;
    int c;

    while ((c = *puchar_a++))
        hash = ((hash << 5u) + hash) + c; /* hash * 33 + c */

    return hash;
}

static void init_info(info *inf) {
    inf->key = NULL;
    inf->value = NULL;
}

static void init_buckets(info **buckets, int hmax) {
    int i;
    
    *buckets = malloc(hmax * sizeof(info));
    for (i = 0; i < hmax; i++) {
        init_info(&(*buckets)[i]);
    }
}

Hashtable *init_ht(int hmax, 
             unsigned int (*hash_function)(void*), 
             int (*compare_function)(void*, void*)) {
    Hashtable *ht;

    ht = malloc(sizeof(Hashtable));
    DIE(ht == NULL, "ht malloc error");

    ht->hmax = hmax;
    ht->load_limit = LOAD_LIMIT;
    ht->size = 0;
    ht->hash_function = hash_function;
    ht->compare_function = compare_function;

    init_buckets(&ht->buckets, hmax);

    return ht;
}

static info* get_info(Hashtable *ht, unsigned int index, void *key) {
    info *buckets = ht->buckets;
    info *inf = buckets + index;
    int hmax = ht->hmax;

    while (inf->key != NULL && (*ht->compare_function)(inf->key, key) != 0) {
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

    index = ht->hash_function(inf->key) % ht->hmax;
    inf2 = get_info(ht, index, inf->key);
    *inf2 = *inf;
}

void put_no_copy_key(Hashtable *ht, void *key, void *value) {
    /* TODO */
}

static void resize(Hashtable *ht) {
    int hmax = ht->hmax, i;
    info *old_buckets;
    info inf;

    old_buckets = ht->buckets;
    ht->hmax = 2 * hmax;
    init_buckets(&ht->buckets, ht->hmax);

    /* Redistribute the keys */
    for (i = 0; i < hmax; i++) {
        inf = old_buckets[i];
        if (inf.key != NULL) {
            put_info(ht, &inf);
        }
    }
    free(old_buckets);
}

void put(Hashtable *ht, void *key, int key_size_bytes, void *value) {
    unsigned int index = (*ht->hash_function)(key) % ht->hmax;
    struct info *inf;

    inf = get_info(ht, index, key);

    if (inf == NULL) {
        perror("info is NULL");
        return;
    }

    if (inf->key != NULL) {
        inf->value = value;
        return;
    }

    ht->size++;

    inf->key = malloc(key_size_bytes);
    memcpy(inf->key, key, key_size_bytes);
    inf->value = value;

    if ((double)ht->size / ht->hmax > ht->load_limit) {
        resize(ht);
    }
}

void* get(Hashtable *ht, void *key) {
    unsigned int index = (*ht->hash_function)(key) % ht->hmax;
    struct info *info;

    info = get_info(ht, index, key);
    if (info == NULL) {
        return NULL;
    }

    return info->value;
}

int has_key(struct Hashtable *ht, void *key) {
    unsigned int index = (*ht->hash_function)(key) % ht->hmax;
    struct info *info;

    info = get_info(ht, index, key);
    if (info == NULL) {
        return 0;
    }

    return 1;
}

void free_ht(struct Hashtable *ht) {
    int i;

    for (i = 0; i < ht->hmax; i++) {
        if (ht->buckets[i].key != NULL) {
            free(ht->buckets[i].key);
        }
    }
    free(ht->buckets);
    free(ht);
}

int get_ht_size(struct Hashtable *ht) {
    if (ht == NULL) {
        return -1;
    }

    return ht->size;
}

int get_ht_hmax(struct Hashtable *ht) {
    if (ht == NULL) {
        return -1;
    }

    return ht->hmax;
}
