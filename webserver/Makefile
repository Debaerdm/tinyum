CC=gcc
LD=gcc
CFLAGS=-Wall -W -Werror -Wwrite-strings -g
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
