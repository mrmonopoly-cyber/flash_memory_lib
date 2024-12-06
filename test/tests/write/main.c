#include <stdio.h>
#include <stdlib.h>
#include "./flash_memory/flash_memory.h"
#include "./test_flash/hw_flash.h"
#include "./test_score_lib/test_lib.h"

static int init_pool(PagePool_t* pool,uint8_t pool_size){
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

static int store_new_var(PagePool_t* pool,FlashDecriptor_t* o_var_id, const StoreNewValueInputArgs_t* args){
    int8_t err = flash_memory_store_new_value(pool, args, o_var_id);
    if(err < 0){
        FAILED("storing new value failed with error code: ");
        fprintf(stderr, "%d\n", err);
        return -1;
    }

    return 0;

}

int main(int argc __attribute_maybe_unused__, char *argv[] __attribute_maybe_unused__)
{
    const uint8_t pool_size = 2;
    STACK_PAGEPOOL_T(pool, pool_size, );

    if (init_pool(pool, pool_size) < 0) {
        FAILED("init failed");
        return -1;
    }

    char aa = 'a';
    FlashDecriptor_t fd_a;
    const StoreNewValueInputArgs_t aa_args = {
        .value = &aa,
        .data_type = UINT8,
        .var_description = "a variable containing a character \'a\'"
    };
    if (store_new_var(pool,&fd_a, &aa_args) < 0) {
        FAILED("failed storing a var");
        return -2;
    }


    print_SCORE();

    return EXIT_SUCCESS;
}
