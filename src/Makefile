CC=gcc
DEBUG=-g -ggdb -O0 -march=native
CFLAGS=-Wall -Werror -Wextra -pedantic $(DEBUG)

TESTDIR=src/tests
HASHTABLE1=src/classic_ht

build: baby_giant

baby_giant: main.o hashtable/hashtable.o
	$(CC) $(CFLAGS) $^ -o $@ -lm

run:
	./baby_giant

clean:
	rm -f hashtable/*.o *.o
	rm -f baby_giant

