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
	hm_map * map = hm_new();

	char * foo = strnew("foo");
	char * bar = strnew("bar");
	char * baz = strnew("baz");

	char * one = strnew("one");
	char * two = strnew("two");

	hm_insert(map, foo, foo);
	hm_insert(map, foo, bar);
	hm_insert(map, foo, baz);


	hm_insert(map, one, two);
	hm_insert(map, one, one);

	hm_insert(map, two, two);


	void ** value = (void **)malloc(sizeof(char *));

	hm_find(map, foo, value);
	printf("%s\n", *value);

	hm_find(map, one, value);
	printf("%s\n", *value);

	hm_find(map, two, value);
	printf("%s\n", *value);

	free(foo);
	free(bar);
	free(baz);
	free(one);
	free(two);
	free(value);

	free(map->data);
	free(map);
}
