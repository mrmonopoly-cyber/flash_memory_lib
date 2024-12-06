CC = gcc
C_FLAGS = -Wall -Wextra -stc=c11
C_EXFLAGS = 


test_flash_src_path := $(or $(TEST_FLASH_ROOT), .)

ifndef $(HARDWARE_SPEC_ROOT)
HARDWARE_SPEC_ROOT := $(test_flash_src_path)/hardware_spec
include $(HARDWARE_SPEC_ROOT)/Makefile
endif

hw_flash.c := $(test_flash_src_path)/hw_flash.c
all: release

DEBUG_FLAGS = -O0 -fsanitize=address -DDEBUG
RELEASE_FLAGS = -O2

debug: C_FLAGS += $(DEBUG_FLAGS)
debug: clean compile

release: C_FLAGS += $(RELEASE_FLAGS)
release: clean compile

compile: hw_flash.o

hw_flash.o: $(hw_flash.c) 
	$(CC) $(C_FLAGS) $(C_EXFLAGS) $(DEBUG) $(hw_flash.c) -c

hw_flash_clean:
ifeq ($(wildcard hw_flash.o), hw_flash.o)
	rm hw_flash.o
endif

clean: hw_flash_clean
