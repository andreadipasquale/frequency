TARGET = main

SOURCESDIR = src
FILESDIR = files
TESTDIR = test

SOURCES = $(wildcard $(SOURCESDIR)/*.c)
OBJECTS = $(patsubst $(SOURCESDIR)/%.c,$(SOURCESDIR)/%.o, $(SOURCES))
FILES = $(wildcard $(FILESDIR)/*.txt)
SCRIPT = $(wildcard $(TESTDIR)/*.sh)
EXECUTABLE = ./$(TARGET)

CC = gcc
CFLAGS = -O2 -Wall -Wextra -Werror
RM = rm -f

all: CFLAGS += -DNDEBUG
all: $(TARGET) $(FILES)

debug: CFLAGS += -g
debug: $(TARGET) $(FILES)

$(TARGET): $(OBJECTS)
	@echo
	@echo "Building executable $@..."
	$(CC) $(CFLAGS) $^ -o $@
	@echo

$(SOURCESDIR)/%.o: $(SOURCESDIR)/%.c
	@echo "Building object $@..."
	$(CC) $(CFLAGS) -c $< -o $@

$(FILESDIR)/%.txt: $(TARGET)
	@echo "Testing executable $^..."
	@test "$(shell $(SCRIPT) $@)" = "$(shell $(EXECUTABLE) -f $@)" \
	|| { echo 'Test FAILED [file $@]'; exit 1; } \
	&& { echo 'Test PASSED [file $@]'; }

clean: $(OBJECTS)
	$(RM) $(TARGET)
	$(RM) $^

.PHONY: debug clean
