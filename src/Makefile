CC = gcc
C_FLAGS = -Wall -Wextra
C_EXFLAGS = 

flash_memory_src_path := $(or $(FLASH_MEMORY_ROOT), .)
flash_memory.c := $(flash_memory_src_path)/flash_memory.c

all: release

DEBUG_FLAGS = -O0 -fsanitize=address -g
RELEASE_FLAGS = -O2

debug: C_FLAGS += $(DEBUG_FLAGS)
debug: clean compile

release: C_FLAGS += $(RELEASE_FLAGS)
release: clean compile

compile: flash_memory.o

flash_memory.o: $(flash_memory.c) 
	$(CC) $(C_FLAGS) $(C_EXFLAGS) $(DEBUG) $(flash_memory.c) -c

flash_memory_clean:
ifeq ($(wildcard flash_memory.o), flash_memory.o)
	rm flash_memory.o
endif

clean: flash_memory_clean
