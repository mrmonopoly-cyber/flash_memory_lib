#include "./hw_flash.h"
#include "hardware_spec/hardware_spec.h"

#include <stdint.h>
#include <string.h>

//private
#define INPUT_PTR_CHECK(ptr) if (!ptr) {goto err_input_ptr;}

static struct{
    FlashDecriptor_t next_free;
}flash_metadata;

//public
void print_status_flash(void)
{
    for (uint32_t i =0; i<flash_metadata.next_free; i++) {
        print_page(i);
    }
}

int8_t hardware_init_imp(void)
{
    return 0;
}

int8_t 
hardware_write_imp(void* hw_metadata __attribute_maybe_unused__,const FlashDecriptor_t var_id, const void* new_value, 
        const uint32_t size_new_value)
{
    int8_t err = 0;
    if (!size_new_value) {
        goto new_var_size_too_small;
    }
    
    const uint8_t* value_buffer = new_value;
    uint32_t word1=0,word2=0;

    if (size_new_value > 4) {
        memcpy(&word1, value_buffer, 4);
        memcpy(&word2, value_buffer + 4, size_new_value - 4);
    }else{
        memcpy(&word1, new_value, size_new_value);
    }

    if(write(var_id, word1, word2) < 0){
        goto hw_write_err;
    }

    return 0;


hw_write_err:
    err--;
new_var_size_too_small:
    err--;

    return err;
}

int8_t 
hardware_read_imp(void* hw_metadata __attribute_maybe_unused__,const FlashDecriptor_t var_id,
        void* o_buffer, const uint32_t size_o_buffer)
{
    int8_t err = 0;
    if (size_o_buffer < 8) {
        goto buffer_size_too_small;
    }

    uint32_t* value_buffer = o_buffer;
    if(read(var_id, value_buffer , value_buffer + 1) < 0){
        goto hw_imp_error;
    }


    return 0;


hw_imp_error:
    err--;
buffer_size_too_small:
    err--;

    return err;
}

int8_t 
create_id_var_imp(enum DataTypesInFlash data_type, FlashDecriptor_t* out, void** o_hw_metadata)
{
    *o_hw_metadata = &flash_metadata;
    switch (data_type) {
        case UINT8:
        case UINT16:
        case UINT32:
        case UINT64:
        case INT8:
        case INT16:
        case INT32:
        case INT64:
        case FLOAT:
        case DOUBLE:
            *out = flash_metadata.next_free;
            break;
    }
    flash_metadata.next_free++;

    return 0;
}

int8_t 
free_hw_metadata_imp (void* metadata __attribute_maybe_unused__)
{
    return 0;
}
