.SILENT:
.PHONY: clean run build
.DEFAULT_GOAL:=run

CFLAGS:=-O -Wextra -Wall -Wpedantic

parent: parent.c
	cc $(CFLAGS) -o $@ $<

child: child.c
	cc $(CFLAGS) -o $@ $<

run: parent child
	./parent --deadline 1 --min 7 --max 8 -n 2

clean:
	rm parent child
