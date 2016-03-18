CC=gcc
LD=gcc -o
OBJDIR=bin
SRCDIR=src
INCDIR=include
EXE=tinyum
CFILES:=$(wildcard $(SRCDIR)/*.c)
INCLUDES:=$(wildcard $(INCDIR)/*.h)
OBJECTS:=$(CFILES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
CFLAGS=-Wall -Wextra -W -Werror -Wwrite-strings -Wshadow -Wstrict-overflow -fno-strict-aliasing -Wno-missing-field-initializers -O2 -g
LDFLAGS=

.PHONY:all clean mrproper

all: $(EXE)

$(EXE): $(OBJECTS)
	$(LD) $@ $(LSFLAGS) $(OBJECTS)
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

clean:
	$(rm) $(OBJECTS)
	@echo "Cleanup complete!"

mrproper: clean
	$(rm) $(EXE)
	@echo "Executable removed!"
