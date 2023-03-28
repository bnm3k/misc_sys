.SILENT:
.PHONY: clean run build
.DEFAULT_GOAL:=run

CFLAGS:=-O -Wextra -Wall -Wpedantic

parent: parent.c
	cc $(CFLAGS) -o $@ $<

child: child.c
	cc $(CFLAGS) -o $@ $<

run: parent child
	./parent

clean:
	rm parent child
