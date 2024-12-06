#ifndef __HW_FLASH__
#define __HW_FLASH__

#include <stdint.h>
#include "./project_root/flash_memory.h"

void 
print_status_flash(void);

int8_t 
hardware_init_imp(void);

int8_t 
hardware_write_imp(void* hw_metadata __attribute_maybe_unused__,const FlashDecriptor_t var_id, const void* new_value, 
        const uint32_t size_new_value)__attribute__((__nonnull__(1,3)));

int8_t 
hardware_read_imp(void* hw_metadata __attribute_maybe_unused__,const FlashDecriptor_t var_id,void* o_buffer,
        const uint32_t size_o_buffer)__attribute__((__nonnull__(1,3)));

int8_t 
create_id_var_imp (enum DataTypesInFlash, FlashDecriptor_t* out, 
        void** o_hw_metadata)__attribute__((__nonnull__(2,3)));

int8_t 
free_hw_metadata_imp (void* metadata __attribute_maybe_unused__);

int8_t 
create_id_var_imp(enum DataTypesInFlash data_type, FlashDecriptor_t* out, void** o_hw_metadata);

#endif // !__HW_FLASH__
