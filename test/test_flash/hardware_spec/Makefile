CC = gcc
C_FLAGS = -Wall -Wextra
C_EXFLAGS = 

hardware_spec_src_path := $(or $(HARDWARE_SPEC_ROOT), .)
hardware_spec.c := $(hardware_spec_src_path)/hardware_spec.c
all: release

DEBUG_FLAGS = -O0 -fsanitize=address -DDEBUG
RELEASE_FLAGS = -O2

debug: C_FLAGS += $(DEBUG_FLAGS)
debug: clean compile

release: C_FLAGS += $(RELEASE_FLAGS)
release: clean compile

compile: hardware_spec.o

hardware_spec.o: $(hardware_spec.c) 
	$(CC) $(C_FLAGS) $(C_EXFLAGS) $(DEBUG) $(hardware_spec.c) -c

hardware_spec_clean:
ifeq ($(wildcard hardware_spec.o), hardware_spec.o)
	rm hardware_spec.o
endif

clean: hardware_spec_clean
