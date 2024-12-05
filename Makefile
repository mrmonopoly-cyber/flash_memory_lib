flash_memory_general_src_path = $(or $(FLASH_MEMORY_ROOT_GENERAL), .)

ifndef $(FLASH_MEMORY_ROOT)
FLASH_MEMORY_ROOT := $(flash_memory_general_src_path)/src
include $(FLASH_MEMORY_ROOT)/Makefile
endif

clean:
