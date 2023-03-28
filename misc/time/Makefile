.SILENT:
.PHONY: clean

CFLAGS:=-Wall -W -O2 -g -Wextra

bin/get_resolution: get_resolution.c
	cc $(CFLAGS) -lrt -o $@ $<

bin/get_time: get_time.c
	cc $(CFLAGS) -lrt -o $@ $<

bin/sleep: sleep.c
	cc $(CFLAGS) -lrt -o $@ $<

clean:
	rm bin/*
