#include <stdio.h>
#include <stdlib.h>
#include "./flash_memory/flash_memory.h"
#include "./test_flash/hw_flash.h"
#include "./test_score_lib/test_lib.h"
#include "./hardware_spec/hardware_spec.h"


static int init_pool(PagePool_t* pool, uint8_t pool_size){
    const InitInputArgs_t init_args ={
        .hw_init = hardware_init_imp,
        .hw_read = hardware_read_imp,
        .hw_write = hardware_write_imp,
        .hw_id_var = create_id_var_imp,
        .free_metadata = free_hw_metadata_imp,
        .out_page_pool_ptr = pool,
        .max_number_of_vars = pool_size,
    };

    if (flash_memory_init(&init_args)){
        FAILED("init failed");
        return -1;
    }
    return 0;
}

static int store_new_var(PagePool_t* pool, FlashDecriptor_t* o_var_id){
    const char aa = 'a';
    const StoreNewValueInputArgs_t args = {
        .data_type = UINT8,
        .var_description = "a char of value \"a\"",
        .value = &aa,
    };
    int8_t err = flash_memory_store_new_value(pool, &args, o_var_id);
    if(err < 0){
        FAILED("storing new value failed with error code: ");
        fprintf(stderr, "%d\n", err);
        return -1;
    }

    return 0;

}

static int invalid_store_NULL_var(PagePool_t* pool, FlashDecriptor_t* o_var_id){
    const StoreNewValueInputArgs_t args = {
        .data_type = UINT8,
        .var_description = "an invalid store",
        .value = NULL,
    };
    int8_t err = flash_memory_store_new_value(pool, &args, o_var_id);
    if(err < 0){
        PASSED("invalid store blocked with value: ");
        fprintf(stderr, "%d\n", err);
    }else{
        FAILED("NULL variable stored");
        return -1;
    }

    return 0;

}

static int invalid_store_wrong_data_type(PagePool_t* pool, FlashDecriptor_t* o_var_id){
    const StoreNewValueInputArgs_t args = {
        .data_type = UINT8,
        .var_description = "an invalid store",
        .value = NULL,
    };
    int8_t err = flash_memory_store_new_value(pool, &args, o_var_id);
    if(err < 0){
        PASSED("invalid store blocked with value: ");
        fprintf(stderr, "%d\n", err);
    }else{
        FAILED("NULL variable stored");
        return -1;
    }

    return 0;

}

int main(int argc __attribute_maybe_unused__, char *argv[] __attribute_maybe_unused__)
{
    const uint8_t pool_size = 10;
    STACK_PAGEPOOL_T(pool, pool_size, );
    if(init_pool(pool, pool_size)){
        return -1;
    }

    FlashDecriptor_t aa_var_id = 0;
    if (store_new_var(pool, &aa_var_id) < 0) {
        FAILED("failed to store var aa");
    }else{
        PASSED("var aa stored");
    }
    

    FlashDecriptor_t invalid_store_id_null= 0;
    if (invalid_store_NULL_var(pool, &invalid_store_id_null) < 0) {
        FAILED("invalid store null ptr not blocked");
    }else{
        PASSED("invalid store null ptr blocked");
    }

    FlashDecriptor_t fd_wrong_data_type_var= 0;
    if (invalid_store_wrong_data_type(pool, &fd_wrong_data_type_var) < 0) {
        FAILED("invalid store wrong data type not blocked");
    }else{
        PASSED("invalid store wrong data type blocked");
    }


    print_SCORE();

    return EXIT_SUCCESS;
}
