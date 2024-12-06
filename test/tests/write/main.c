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
    const uint8_t pool_size = 20;
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

    char bb = 'b';
    FlashDecriptor_t fd_b;
    const StoreNewValueInputArgs_t bb_args = {
        .value = &bb,
        .data_type = UINT8,
        .var_description = "a variable containing a character \'a\'"
    };
    if (store_new_var(pool,&fd_b, &bb_args) < 0) {
        FAILED("failed storing a var");
        return -2;
    }


    char aa_new = 126;
    const UpdateValueInputArgs_t update_aa = {
        .var_id = fd_a,
        .new_value = &aa_new,
        .size_new_value = sizeof(aa_new),
    };
    int err = flash_memory_update_value(pool, &update_aa);
    if (err < 0) {
        FAILED("failed updating var a with error:");
        fprintf(stderr, "%d\n",err);
    }else{
        PASSED("var a updated");
    }

    char bb_new = 127;
    const UpdateValueInputArgs_t update_bb = {
        .var_id = fd_b,
        .new_value = &bb_new,
        .size_new_value = sizeof(bb_new),
    };
    err = flash_memory_update_value(pool, &update_bb);
    if (err < 0) {
        FAILED("failed updating var b with error:");
        fprintf(stderr, "%d\n",err);
    }else{
        PASSED("var b updated");
    }

    const UpdateValueInputArgs_t update_invalid_id = {
        .var_id = fd_b + 12,
        .new_value = &bb_new,
        .size_new_value = sizeof(bb_new),
    };
    err = flash_memory_update_value(pool, &update_invalid_id);
    if (err < 0) {
        PASSED("blocked updating invalid id var with error:");
        fprintf(stderr, "%d\n",err);
    }else{
        FAILED("invalid id var updated");
    }

    const UpdateValueInputArgs_t update_invalid_null_new_value = {
        .var_id = fd_b + 12,
        .new_value = NULL,
        .size_new_value = sizeof(bb_new),
    };
    err = flash_memory_update_value(pool, &update_invalid_null_new_value);
    if (err < 0) {
        PASSED("blocked updating invalid new_value null var with error:");
        fprintf(stderr, "%d\n",err);
    }else{
        FAILED("invalid var updated with new value NULL");
    }

    const UpdateValueInputArgs_t update_invalid_zero_size_new_value = {
        .var_id = fd_b + 12,
        .new_value = &bb_new,
        .size_new_value = 0,
    };
    err = flash_memory_update_value(pool, &update_invalid_zero_size_new_value);
    if (err < 0) {
        PASSED("blocked updating invalid zero size new_value var with error:");
        fprintf(stderr, "%d\n",err);
    }else{
        FAILED("invalid var updated with zero size new_value");
    }

    print_status_flash();

    print_SCORE();

    return EXIT_SUCCESS;
}
