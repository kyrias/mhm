#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mhm.h"

char * strnew(const char * string);

char *
strnew(const char * string) {
	size_t len = strlen(string);
	char * new = malloc(len + 1);
	strcpy(new, string);
	return new;
}

int
main(void) {
	// Test a string-keyed hashmap
	hm_map * map = hm_new(hm_hash_string, hm_cmp_string);

	char * foo = strnew("foo");
	char * bar = strnew("bar");
	char * baz = strnew("baz");
	char * one = strnew("one");
	char * two = strnew("two");

	hm_insert(map, foo, 4, foo);
	hm_insert(map, foo, 4, bar);
	hm_insert(map, foo, 4, baz);


	hm_insert(map, one, 4, two);
	hm_insert(map, one, 4, one);

	hm_insert(map, two, 2, two);


	void ** value = (void **)malloc(sizeof(char *));
	*value = 0;

	hm_find(map, foo, 4, value);
	printf("%s\n", *value);

	hm_find(map, one, 2, value);
	printf("%s\n", *value);

	hm_find(map, two, 2, value);
	printf("%s\n", *value);


	void ** ivalue = (void **)malloc(sizeof(uint32_t *));
	*ivalue = 0;


	// Test an integer-keyed hashmap
	hm_map * intmap = hm_new(hm_hash_int, hm_cmp_int);

	uint32_t ione = 1;
	uint32_t itwo = 2;

	hm_insert(intmap, &ione, 32, foo);
	hm_insert(intmap, &itwo, 32, bar);

	hm_find(intmap, &ione, 32, ivalue);
	printf("%s\n", *ivalue);

	hm_find(intmap, &itwo, 32, ivalue);
	printf("%s\n", *ivalue);


	free(foo);
	free(bar);
	free(baz);
	free(one);
	free(two);
	free(value);
	free(ivalue);

	free(map->data);
	free(map);
	free(intmap->data);
	free(intmap);
}
