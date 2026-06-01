CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2

all: libkintsugi.a test_kintsugi

libkintsugi.a: src/kintsugi.o
	ar rcs $@ $^

src/kintsugi.o: src/kintsugi.c src/kintsugi.h
	$(CC) $(CFLAGS) -c -o $@ $<

test_kintsugi: tests/test_kintsugi.c libkintsugi.a
	$(CC) $(CFLAGS) -o $@ $< -L. -lkintsugi -lm

test: test_kintsugi
	./test_kintsugi

clean:
	rm -f src/*.o libkintsugi.a test_kintsugi
