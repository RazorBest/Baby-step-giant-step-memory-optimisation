#ifndef __HASHTABLE_H
#define __HASHTABLE_H

#include <inttypes.h>
#include "../list/LinkedList.h"
#include "../utils/utils.h"

typedef struct info {
    void *key;
    void *value;
} info;

typedef struct Hashtable {
    info *buckets; /* Array de liste simplu-inlantuite. */
    int size; /* Nr. total de noduri existente curent in toate bucket-urile. */
    int hmax; /* Nr. de bucket-uri. */
    double load_limit; /* Load factorul la care se face resize*/
    /* (Pointer la) Functie pentru a calcula valoarea hash asociata cheilor. */
    unsigned int (*hash_function)(void*);
    /* (Pointer la) Functie pentru a compara doua chei. */
    int (*compare_function)(void*, void*);
} Hashtable;

Hashtable *init_ht(int hmax, uint32_t (*hash_function)(void*), 
             int (*compare_function)(void*, void*));

void put(Hashtable *ht, void *key, int key_size_bytes, void *value);

void* get(Hashtable *ht, void *key);

int has_key(Hashtable *ht, void *key);

int get_ht_size(Hashtable *ht);

int get_ht_hmax(Hashtable *ht);

void free_ht(Hashtable *ht);

/*
 * Functii de comparare a cheilor:
 */
int compare_function_int32(void *a, void *b);

int compare_function_strings(void *a, void *b);

/*
 * Functii de hashing:
 */
unsigned int hash_function_int32(void *a);

unsigned int hash_function_string(void *a);

#endif
