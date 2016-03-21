CC=gcc
LD=gcc -o
OBJDIR=bin
SRCDIR=src
INCDIR=include
EXE=tinyum
CFILES:=$(wildcard $(SRCDIR)/*.c)
INCLUDES:=$(wildcard $(INCDIR)/*.h)
OBJECTS:=$(CFILES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
CFLAGS=-Wall -Wextra -W -Werror -pedantic -Wwrite-strings -Wshadow -Wstrict-overflow -fno-strict-aliasing -Wno-missing-field-initializers -O2 -g
LDFLAGS=-lpthread
MKDIR_P=mkdir -p

.PHONY:all clean mrproper directories

all: directories $(EXE)

$(EXE): $(OBJECTS)
	$(LD) $@ $(LDFLAGS) $(OBJECTS)
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o  $@
	@echo "Compiled "$<" successfully!"

directories: $(OBJDIR)

$(OBJDIR):
	$(MKDIR_P) $(OBJDIR)
	@echo "Create "$(OBJDIR)" folder!"
clean:
	$(rm) $(OBJECTS)
	@echo "Cleanup complete!"

mrproper: clean
	$(rm) $(EXE)
	@echo "Executable removed!"
