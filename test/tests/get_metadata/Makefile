CC = gcc
C_FLAGS = -Wall -Wextra
C_EXFLAGS = 

TEST_ROOT = .
OUT_NAME = main

ifndef $(FLASH_MEMORY_ROOT)
FLASH_MEMORY_ROOT:= $(TEST_ROOT)/project_root
include $(FLASH_MEMORY_ROOT)/Makefile
endif

ifndef $(TEST_FLASH_ROOT)
TEST_FLASH_ROOT:= $(TEST_ROOT)/test_flash
include $(TEST_FLASH_ROOT)/Makefile
endif

ifndef $(TEST_SCORE_LIB_ROOT)
TEST_SCORE_LIB_ROOT:= $(TEST_ROOT)/test_score_lib
include $(TEST_SCORE_LIB_ROOT)/Makefile
endif

main.c := $(TEST_ROOT)/main.c
OBJECT = $(wildcard *.o)

all: release

DEBUG_FLAGS = -O0 -fsanitize=address -DDEBUG -g
RELEASE_FLAGS = -O2

debug: C_FLAGS += $(DEBUG_FLAGS)
debug: clean compile

release: C_FLAGS += $(RELEASE_FLAGS)
release: clean compile

compile: main

main: $(main.c) 
	$(CC) $(C_FLAGS) $(C_EXFLAGS) $(DEBUG) $(OBJECT) $(main.c) -o $(OUT_NAME)

test_clean:
ifeq ($(wildcard main), main)
	rm $(OUT_NAME)
endif

clean: test_clean
