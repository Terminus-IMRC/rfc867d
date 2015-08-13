CC := gcc
CFLAGS := -O2 -pipe -g
RM := rm -f

rfc867d: rfc867d.o

.PHONY: clean
clean:
	$(RM) rfc867d.o rfc867d
