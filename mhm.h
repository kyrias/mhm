#ifndef mhm_h
#define mhm_h

#include <stdbool.h>
#include <stdint.h>

#define HM_OK (0)
#define HM_FULL (-1)
#define HM_MISSING (-2)
#define HM_OOM (-3)
#define HM_ERR (-4)

typedef uint32_t (*hash_func_ptr)(size_t map_size, void * key_pointer);
typedef bool (*cmp_func_ptr)(void * a, size_t a_len, void * b, size_t b_len);

typedef struct hm_element {
	bool in_use;
	size_t key_len;
	void * key;
	void * value;
} hm_element;

typedef struct hm_map {
	hash_func_ptr hash_key;
	cmp_func_ptr cmp_keys;
	size_t size;
	size_t used;
	hm_element * data;
} hm_map;


hm_map * hm_new(hash_func_ptr hash_func, cmp_func_ptr cmp_func);
int hm_double_size(hm_map * map);

int hm_insert(hm_map * map, void * key, size_t key_len, void * value);
int hm_find(hm_map * map, void * key, size_t key_len, void ** value);

int32_t hm_hash(hm_map * map, void * key, size_t len);
uint32_t hm_hash_string(size_t map_size, void * key_string);
uint32_t hm_hash_int(size_t map_size, void * key_string);

bool hm_cmp_string(void * a, size_t a_len,
                   void * b, size_t b_len);
bool hm_cmp_int(void * a, size_t a_len,
                void * b, size_t b_len);

#endif
