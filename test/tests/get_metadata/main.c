#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./flash_memory/flash_memory.h"
#include "./test_flash/hw_flash.h"
#include "./test_score_lib/test_lib.h"

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

int main(int argc __attribute_maybe_unused__, char *argv[] __attribute_maybe_unused__)
{
    const uint8_t pool_size = 3;
    STACK_PAGEPOOL_T(pool, pool_size, );
    if(init_pool(pool, pool_size)){
        FAILED("failed init the pool");
        return -1;
    }

    FlashDecriptor_t aa_var_id = 0;
    if (store_new_var(pool, &aa_var_id) < 0) {
        FAILED("failed to store var aa");
    }

    FlashDecriptor_t bb_var_id = 0;
    if (store_new_var(pool, &bb_var_id) < 0) {
        FAILED("failed to store var bb");
    }

    FlashDecriptor_t cc_var_id = 0;
    if (store_new_var(pool, &cc_var_id) < 0) {
        FAILED("failed to store var cc");
    }

    int err =0;

    {
        MetadataStoreVariableInFlash_t a_met = {0};
        err = flash_memory_get_var_metadata(pool,aa_var_id, &a_met);
        if (err < 0) {
            FAILED("failed getting metadata of var a with error:");
            fprintf(stderr, "%d\n", err);
        }else{
            if (a_met.data_type != UINT8 && !strcmp(a_met.var_description, "a char of value \"a\"") ) {
                FAILED("metadata obtained but are wrong");
                fprintf(stderr, "data type: %d, description: %s\n", 
                        a_met.data_type,a_met.var_description );
            }else{
                PASSED("metadata corrected for variable a");
            }
        }
    }

    {
        MetadataStoreVariableInFlash_t b_met = {0};
        err = flash_memory_get_var_metadata(pool,bb_var_id, &b_met);
        if (err < 0) {
            FAILED("failed getting metadata of var b with error:");
            fprintf(stderr, "%d\n", err);
        }else{
            if (b_met.data_type != UINT8 && !strcmp(b_met.var_description, "a char of value \"a\"") ) {
                FAILED("metadata obtained but are wrong");
                fprintf(stderr, "data type: %d, description: %s\n", 
                        b_met.data_type,b_met.var_description );
            }else{
                PASSED("metadata corrected for variable b");
            }
        }
    }

    {
        MetadataStoreVariableInFlash_t b_met = {0};
        err = flash_memory_get_var_metadata(pool,bb_var_id + 12, &b_met);
        if (err < 0) {
            PASSED("blocked getting metadata of invalid var with error:");
            fprintf(stderr, "%d\n", err);
        }else{
            FAILED("metadata obtained but with invalid var fd");
        }
    }

    {
        err = flash_memory_get_var_metadata(pool,bb_var_id, NULL);
        if (err < 0) {
            PASSED("blocked getting metadata of invalid NULL out param");
        }else{
            FAILED("metadata obtained but with invalid NULL out param");
        }
    }

    {
        MetadataStoreVariableInFlash_t b_met = {0};
        err = flash_memory_get_var_metadata(NULL,bb_var_id, &b_met);
        if (err < 0) {
            PASSED("blocked getting metadata with pool NULL");
        }else{
            FAILED("metadata obtained but pool is NULL");
        }
    }
    


    print_SCORE();

    return EXIT_SUCCESS;
}
