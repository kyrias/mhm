#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mhm.h"

char * strnew(const char * string);

char *
strnew(const char * string) {
	char * new = malloc(strlen(string));
	strcpy(new, string);
	return new;
}

int
main(void) {
	hm_map * map = hm_new();

	char * foo = strnew("foo");
	hm_insert(map, "foo", foo);

	char * bar = strnew("bar");
	hm_insert(map, "foo", bar);

	char * baz = strnew("baz");
	hm_insert(map, "foo", baz);


	char * one = strnew("one");
	hm_insert(map, "1", one);

	char * two = strnew("two");
	hm_insert(map, "2", two);


	void ** value = (void **)malloc(sizeof(char *));

	hm_find(map, "foo", value);
	printf("%s\n", *value);
	free(*value);

	hm_find(map, "1", value);
	printf("%s\n", *value);
	free(*value);

	hm_find(map, "2", value);
	printf("%s\n", *value);
	free(*value);

	free(foo);
	free(bar);
	free(baz);
	free(one);
	free(two);
	free(value);
}
