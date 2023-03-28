.SILENT:
.PHONY: clean run build
.DEFAULT_GOAL:=run

CFLAGS:=-O -Wextra -Wall -Wpedantic

parent: parent.c
	cc $(CFLAGS) -o $@ $<

child: child.c
	cc $(CFLAGS) -o $@ $<

run: parent child
	./parent --deadline 2.5 --min 1 --max 4  -n 4

clean:
	rm parent child
