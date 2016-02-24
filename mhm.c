#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "crc_32.h"
#include "mhm.h"

#define INITIAL_SIZE (256)
#define MAX_CHAIN_LENGTH (8)


// Allocate new hashmap
hm_map *
hm_new(hash_func_ptr hash_func, cmp_func_ptr cmp_func) {
	hm_map * map = (hm_map *) malloc(sizeof *map);
	if (!map) {
		return NULL;
	}

	map->data = (hm_element *) calloc(INITIAL_SIZE, (sizeof *(map->data)));
	if (!map->data) {
		free(map);
		return NULL;
	}

	map->hash_key = hash_func;
	map->cmp_keys = cmp_func;
	map->size = INITIAL_SIZE;
	map->used = 0;

	return map;
}


// Double the size of the map array
int
hm_double_size(hm_map * map) {
	hm_element * temp = (hm_element *) calloc(map->size * 2, (sizeof (hm_element)));
	if (!temp) {
		return HM_OOM;
	}

	// Use new element array
	hm_element * curr = map->data;
	map->data = temp;

	// Update table size
	size_t old_size = map->size;
	map->size *= 2;
	map->used = 0;

	for (size_t i = 0; i < old_size; i++) {
		if (curr[i].in_use == false) {
			continue;
		}

		int status = hm_insert(map, curr[i].key, curr[i].key_len, curr[i].value);
		if (status != HM_OK) {
			return status;
		}
	}

	free(curr);
	return HM_OK;
}


/**
 * Comparison functions
 */

bool hm_cmp_string(void * a, size_t a_len,
                   void * b, size_t b_len) {
    size_t len = a_len > b_len ? a_len : b_len;
	char * strA = (char *)a;
	char * strB = (char *)b;

	if (strncmp(strA, strB, len) == 0) {
		return true;
	} else {
		return false;
	}
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
bool hm_cmp_int(void * a, size_t a_len,
                void * b, size_t b_len) {
	uint32_t * intA = (uint32_t *)a;
	uint32_t * intB = (uint32_t *)b;

	if (*intA == *intB) {
		return true;
	} else {
		return false;
	}
}
#pragma clang diagnostic pop


/**
 * Hashing functions
 */

uint32_t
hm_hash_string(size_t map_size, void * key_pointer) {
	char * key_string = (char *)key_pointer;
	uint32_t key = crc32buf(key_string, strlen(key_string));

	// https://web.archive.org/web/20120903003157/http://www.cris.com/~Ttwang/tech/inthash.htm
	key = ~key + (key << 15); // key = (key << 15) - key - 1;
	key = key ^ (key >> 12);
	key = key + (key << 2);
	key = key ^ (key >> 4);
	key = key * 2057; // key = (key + (key << 3)) + (key << 11);
	key = key ^ (key >> 16);

	return key % map_size;
}

uint32_t
hm_hash_int(size_t map_size, void * key_pointer) {
	uint32_t * key = (uint32_t *)key_pointer;

	return *key % map_size;
}


// Find closest free hash
int32_t
hm_hash(hm_map * map, void * key, size_t key_len) {
	if (map->used >= (map->size / 2)) {
		return HM_FULL;
	}

	int curr = (int) map->hash_key(map->size, key);

	for (int i = 0; i < MAX_CHAIN_LENGTH; i++) {
		if (map->data[curr].in_use == false) {
			return curr;
		}

		if (map->data[curr].in_use == true &&
		    map->cmp_keys(map->data[curr].key, map->data[curr].key_len, key, key_len)) {
			return curr;
		}

		curr = curr + 1 % (int32_t)(map->size);
	}

	return HM_FULL;
}


int
hm_insert(hm_map * map, void * key, size_t key_len, void * value) {
	int index = (int)map->hash_key(map->size, key);
	while(index == HM_FULL) {
		if (hm_double_size(map) == HM_OOM) {
			return HM_OOM;
		}
		index = (int)map->hash_key(map->size, key);
	}

	map->data[index].key = key;
	map->data[index].key_len = key_len;

	map->data[index].value = value;
	map->data[index].in_use = true;

	return false;
}


int
hm_find(hm_map * map, void * key, size_t key_len, void ** value) {
	if (!value) {
		return HM_ERR;
	}

	int index = hm_hash(map, key, key_len);
	for (int i = 0; i < MAX_CHAIN_LENGTH; i++) {
		if (map->data[index].in_use == true &&
		    map->cmp_keys(map->data[index].key, map->data[index].key_len, key, key_len))
		{
			*value = (map->data[index].value);
			return HM_OK;
		}
		index = index + 1 % (int)map->size;
	}

	*value = NULL;
	return HM_MISSING;
}
