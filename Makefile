CC = clang

CFLAGS ?= -O0 -ggdb
CFLAGS += -Isrc -std=c11 -ggdb -fstack-protector-all -Wl,-z,relro -Wl,-z,now
CFLAGS += -Weverything -Wno-padded

main: main.c mhm.c crc_32.c
	$(CC) $(CFLAGS) -o $@ main.c mhm.c crc_32.c

clean:
	-rm -f main
