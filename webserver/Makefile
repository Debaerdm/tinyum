CC=gcc
LD=gcc
CFLAGS=-Wall -Wextra -W -Werror -Wwrite-strings -pedantic -Wshadow -Wstrict-overflow -fno-strict-aliasing -Wno-missing-field-initializers -O2 -g
LDFLAGS=
EXE=tinyum
HEADERS=$(wildcard *.h)
CFILES=$(wildcard *.c)
OBJS=$(CFILES:.c=.o)

.PHONY:all clean mrproper

all: $(EXE)

$(EXE): $(OBJS)
	$(LD) $^ $(LDFLAGS) -o $@

makefile.dep: $(CFILES) $(HEADERS)
	$(CC) -MM $(CFILES) > $@

clean:
	$(RM) $(OBJS) makefile.dep

mrproper: clean
	$(RM) $(EXE)

include makefile.dep
