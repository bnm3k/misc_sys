.SILENT:
.PHONY: clean run build
.DEFAULT_GOAL:=run

CFLAGS:=-O -Wextra -Wall -Wpedantic

build: main.c
	cc $(CFLAGS) -o a.out main.c

run: build
	./a.out

clean:
	rm a.out
