CC = gcc
C_FLAGS = -Wall -Wextra -std=c11
C_EXFLAGS = 

flash_memory_src_path := $(or $(TEST_SCORE_LIB_ROOT), .)
test_lib.c := $(flash_memory_src_path)/test_lib.c

all: release

DEBUG_FLAGS = -O0 -fsanitize=address -DDEBUG
RELEASE_FLAGS = -O2

debug: C_FLAGS += $(DEBUG_FLAGS)
debug: clean compile

release: C_FLAGS += $(RELEASE_FLAGS)
release: clean compile

compile: test_lib.o

test_lib.o: $(test_lib.c) 
	$(CC) $(C_FLAGS) $(C_EXFLAGS) $(DEBUG) $(test_lib.c) -c

test_score_clean:
ifeq ($(wildcard test_lib.o), test_lib.o)
	rm test_lib.o
endif

clean: test_score_clean
