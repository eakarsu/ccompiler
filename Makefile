# mycc - C11 Compiler
# Build system with debug/release modes, auto-deps, ASan

CC       := cc
CFLAGS   := -std=c11 -Wall -Wextra -Wpedantic -Iinclude
LDFLAGS  :=

# Build mode: debug (default) or release
BUILD    ?= debug

ifeq ($(BUILD),release)
  CFLAGS += -O2 -DNDEBUG
else ifeq ($(BUILD),asan)
  CFLAGS += -g -O0 -fsanitize=address,undefined -fno-omit-frame-pointer -DDEBUG
  LDFLAGS += -fsanitize=address,undefined
else
  CFLAGS += -g -O0 -DDEBUG
endif

# Directories
SRCDIR   := src
BUILDDIR := build/$(BUILD)
DEPDIR   := $(BUILDDIR)/deps

# Source files
SRCS := $(SRCDIR)/driver/main.c \
        $(SRCDIR)/util/arena.c \
        $(SRCDIR)/util/hashtab.c \
        $(SRCDIR)/util/strintern.c \
        $(SRCDIR)/util/diag.c \
        $(SRCDIR)/frontend/lexer.c \
        $(SRCDIR)/frontend/preproc.c \
        $(SRCDIR)/frontend/parser.c \
        $(SRCDIR)/frontend/sema.c \
        $(SRCDIR)/frontend/type.c \
        $(SRCDIR)/frontend/ast.c \
        $(SRCDIR)/ir/irgen.c \
        $(SRCDIR)/ir/ir.c \
        $(SRCDIR)/ir/irutil.c \
        $(SRCDIR)/opt/passmanager.c \
        $(SRCDIR)/opt/mem2reg.c \
        $(SRCDIR)/opt/dce.c \
        $(SRCDIR)/opt/sccp.c \
        $(SRCDIR)/opt/gvn.c \
        $(SRCDIR)/opt/inline.c \
        $(SRCDIR)/opt/licm.c \
        $(SRCDIR)/opt/simplifycfg.c \
        $(SRCDIR)/opt/instcombine.c \
        $(SRCDIR)/backend/regalloc.c \
        $(SRCDIR)/backend/x86_64/x86_64_target.c \
        $(SRCDIR)/backend/x86_64/x86_64_isel.c \
        $(SRCDIR)/backend/x86_64/x86_64_emit.c \
        $(SRCDIR)/backend/arm64/arm64_target.c \
        $(SRCDIR)/backend/arm64/arm64_isel.c \
        $(SRCDIR)/backend/arm64/arm64_emit.c

OBJS := $(SRCS:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
DEPS := $(OBJS:.o=.d)

TARGET := $(BUILDDIR)/mycc

# Unit test sources
TEST_SRCS := $(wildcard tests/unit/*.c)
TEST_BINS := $(TEST_SRCS:tests/unit/%.c=$(BUILDDIR)/tests/%)

.PHONY: all clean test test-boundary test-unit test-integration install

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -MF $(BUILDDIR)/$*.d -c -o $@ $<

# Unit tests: compile each test file with the utility objects
UTIL_OBJS := $(BUILDDIR)/util/arena.o $(BUILDDIR)/util/hashtab.o \
             $(BUILDDIR)/util/strintern.o $(BUILDDIR)/util/diag.o

$(BUILDDIR)/tests/%: tests/unit/%.c $(UTIL_OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test-unit: $(TEST_BINS)
	@echo "=== Running unit tests ==="
	@pass=0; fail=0; \
	for t in $(TEST_BINS); do \
		name=$$(basename $$t); \
		if $$t > /dev/null 2>&1; then \
			echo "  PASS  $$name"; \
			pass=$$((pass+1)); \
		else \
			echo "  FAIL  $$name"; \
			fail=$$((fail+1)); \
		fi; \
	done; \
	echo "=== $$pass passed, $$fail failed ==="; \
	test $$fail -eq 0

test-integration: $(TARGET)
	@echo "=== Running integration tests ==="
	@bash tests/run_tests.sh $(TARGET)

test-boundary:
	python3 scripts/validate_tool_boundary.py

test: test-boundary test-unit test-integration

clean:
	rm -rf build

install: $(TARGET)
	install -m 755 $(TARGET) /usr/local/bin/mycc

# Include auto-generated dependencies
-include $(DEPS)
