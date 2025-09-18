# ═══════════════════════════════════════════════════════════════════════════
# C Code Obfuscator Makefile
# ═══════════════════════════════════════════════════════════════════════════

CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic -O2 -g
LDFLAGS = 
INCLUDES = -Isrc

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin
TESTDIR = tests

# Source files
LEXER_SOURCES = $(SRCDIR)/lexer/lexer.c
PARSER_SOURCES = $(SRCDIR)/parser/parser.c
SYMBOLS_SOURCES = $(SRCDIR)/symbols/symbols.c
OBFUSCATOR_SOURCES = $(SRCDIR)/obfuscator/obfuscator.c
CODEGEN_SOURCES = $(SRCDIR)/codegen/codegen.c
MAIN_SOURCES = $(SRCDIR)/main.c

ALL_SOURCES = $(LEXER_SOURCES) $(PARSER_SOURCES) $(SYMBOLS_SOURCES) \
              $(OBFUSCATOR_SOURCES) $(CODEGEN_SOURCES) $(MAIN_SOURCES)

# Object files
OBJECTS = $(ALL_SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Target executable
TARGET = $(BINDIR)/obfuscator

# Test sources
TEST_SOURCES = $(wildcard $(TESTDIR)/*.c)
TEST_OBJECTS = $(TEST_SOURCES:$(TESTDIR)/%.c=$(OBJDIR)/test_%.o)
TEST_TARGET = $(BINDIR)/test_runner

# Default target
all: $(TARGET)

# Create directories
$(OBJDIR):
	mkdir -p $(OBJDIR)/lexer $(OBJDIR)/parser $(OBJDIR)/symbols $(OBJDIR)/obfuscator $(OBJDIR)/codegen

$(BINDIR):
	mkdir -p $(BINDIR)

# Build main executable
$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Compile source files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Build tests
test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJECTS) $(filter-out $(OBJDIR)/main.o,$(OBJECTS)) | $(BINDIR)
	$(CC) $(TEST_OBJECTS) $(filter-out $(OBJDIR)/main.o,$(OBJECTS)) -o $@ $(LDFLAGS)

$(OBJDIR)/test_%.o: $(TESTDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Install (copy to system path)
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# Uninstall
uninstall:
	rm -f /usr/local/bin/obfuscator

# Run with example
example: $(TARGET)
	./$(TARGET) examples/simple.c -o examples/simple_obf.c -l extreme -a artistic

# Debug build
debug: CFLAGS += -DDEBUG -g3 -O0
debug: $(TARGET)

# Release build
release: CFLAGS += -DNDEBUG -O3 -s
release: $(TARGET)

# Format code
format:
	find src -name "*.c" -o -name "*.h" | xargs clang-format -i

# Static analysis
analyze:
	cppcheck --enable=all --std=c99 src/

# Generate documentation
docs:
	doxygen Doxyfile

# Help
help:
	@echo "Available targets:"
	@echo "  all      - Build the obfuscator (default)"
	@echo "  test     - Build and run tests"
	@echo "  clean    - Remove build artifacts"
	@echo "  install  - Install to system path"
	@echo "  example  - Run with example file"
	@echo "  debug    - Build debug version"
	@echo "  release  - Build optimized release"
	@echo "  format   - Format source code"
	@echo "  analyze  - Run static analysis"
	@echo "  docs     - Generate documentation"

.PHONY: all test clean install uninstall example debug release format analyze docs help