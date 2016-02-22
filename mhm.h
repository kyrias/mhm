#ifndef mhm_h
#define mhm_h

#include <stdbool.h>

#define HM_OK (0)
#define HM_FULL (-1)
#define HM_MISSING (-2)
#define HM_OOM (-3)


typedef struct hm_element {
	bool in_use;
	char * key;
	void * value;
} hm_element;

typedef struct hm_map {
	size_t size;
	size_t used;
	hm_element * data;
} hm_map;


hm_map * hm_new(void);
int hm_double_size(hm_map * map);

int hm_insert(hm_map * map, char * key, void * value);
int hm_find(hm_map * map, char * key, void ** value);

int hm_hash(hm_map * map, char * key_string);
unsigned int hm_hash_string(hm_map * map, char * key_string);

#endif
