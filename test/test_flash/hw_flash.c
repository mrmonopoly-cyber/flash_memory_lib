#include "./hw_flash.h"
#include "hardware_spec/hardware_spec.h"

#include <stdint.h>
#include <string.h>

//private
#define INPUT_PTR_CHECK(ptr) if (ptr) {goto err_input_ptr;}
#define SET_HIGHER_ERROR_VALUE(curr_err,new_err) if (*curr_err < new_err) *curr_err = new_err

static struct{
    FlashDecriptor_t next_free;
}flash_metadata;

//public
int8_t hardware_init_imp(void){
    return 0;
}

int8_t 
hardware_write_imp(void* hw_metadata __attribute_maybe_unused__,const FlashDecriptor_t var_id, const void* new_value, 
        const uint32_t size_new_value)
{
    if (!size_new_value) {
        goto new_var_size_too_small;
    }

    uint32_t word1=0,word2=0;

    if (size_new_value > 4) {
        memcpy(&word1, new_value, 4);
        memcpy(&word2, new_value + 4, size_new_value - 4);
    }else{
        memcpy(&word1, new_value, size_new_value);
    }

    if(write(var_id, word1, word2) < 0){
        goto hw_write_err;
    }

    return 0;

    int8_t err = 0;

hw_write_err:
    SET_HIGHER_ERROR_VALUE(&err, -2);
new_var_size_too_small:
    SET_HIGHER_ERROR_VALUE(&err, -1);

    return err;
}

int8_t 
hardware_read_imp(void* hw_metadata __attribute_maybe_unused__,const FlashDecriptor_t var_id,void* o_buffer,
        const uint32_t size_o_buffer)
{
    if (size_o_buffer < 8) {
        goto buffer_size_too_small;
    }

    if(read(var_id, o_buffer, o_buffer + 4) < 0){
        goto hw_imp_error;
    }


    return 0;

    int8_t err = 0;

hw_imp_error:
    SET_HIGHER_ERROR_VALUE(&err, -2);
buffer_size_too_small:
    SET_HIGHER_ERROR_VALUE(&err, -1);

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
