#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "crc_32.h"
#include "mhm.h"

#define INITIAL_SIZE (256)
#define MAX_CHAIN_LENGTH (8)


// Hash a string
uint32_t
hm_hash_string(hm_map * map, char * key_string) {
	uint32_t key = crc32buf(key_string, strlen(key_string));

	// https://web.archive.org/web/20120903003157/http://www.cris.com/~Ttwang/tech/inthash.htm
	key = ~key + (key << 15); // key = (key << 15) - key - 1;
	key = key ^ (key >> 12);
	key = key + (key << 2);
	key = key ^ (key >> 4);
	key = key * 2057; // key = (key + (key << 3)) + (key << 11);
	key = key ^ (key >> 16);

	return key % map->size;
}


// Find closest free hash
int32_t
hm_hash(hm_map * map, char * key_string) {
	if (map->used >= (map->size / 2)) {
		return HM_FULL;
	}

	int curr = (signed int) hm_hash_string(map, key_string);

	for (int i = 0; i < MAX_CHAIN_LENGTH; i++) {
		if (map->data[curr].in_use == false) {
			return curr;
		}

		if (map->data[curr].in_use == true &&
		    strcmp(map->data[curr].key, key_string) == 0) {
			return curr;
		}

		curr = curr + 1 % (int32_t)(map->size);
	}

	return HM_FULL;
}


// Allocate new hashmap
hm_map *
hm_new() {
	hm_map * map = (hm_map *) malloc(sizeof map);
	if (!map) {
		return NULL;
	}

	map->data = (hm_element *) calloc(INITIAL_SIZE, (sizeof map->data));
	if (!map->data) {
		free(map);
		return NULL;
	}

	map->size = INITIAL_SIZE;
	map->used = 0;

	return map;
}


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

		int status = hm_insert(map, curr[i].key, curr[i].value);
		if (status != HM_OK) {
			return status;
		}
	}

	free(curr);
	return HM_OK;
}


int
hm_insert(hm_map * map, char * key, void * value) {
	int index = (int)hm_hash_string(map, key);
	while(index == HM_FULL) {
		if (hm_double_size(map) == HM_OOM) {
			return HM_OOM;
		}
		index = hm_hash(map, key);
	}

	map->data[index].key = key;
	map->data[index].value = value;
	map->data[index].in_use = true;

	return false;
}


int
hm_find(hm_map * map, char * key, void ** value) {
	int index = hm_hash(map, key);
	for (int i = 0; i < MAX_CHAIN_LENGTH; i++) {
		if (map->data[index].in_use == true &&
		    strcmp(map->data[index].key, key) == 0)
		{
			*value = (map->data[index].value);
			return HM_OK;
		}
		index = index + 1 % (int)map->size;
	}

	*value = NULL;
	return HM_MISSING;
}
