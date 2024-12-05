#include <stdlib.h>
#include <stdio.h>
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

static int read_var(PagePool_t* pool, FlashDecriptor_t fd, uint64_t desired_v){
    uint64_t ra = 0;
    const FetchValueInputArgs_t read_aa_args = {
        .var_id = fd,
        .out_parameter = &ra,
        .size_out_parameter = sizeof(ra),
    };
    int8_t err = flash_memory_fetch_value(pool, &read_aa_args);
    if (err < 0) {
        return err;
    }else{
        if (ra != desired_v) {
            return -7;
        }else{
        }
    }
    return 0;
}

int main(int argc __attribute_maybe_unused__, char *argv[] __attribute_maybe_unused__)
{
    const uint8_t pool_size = 10;
    STACK_PAGEPOOL_T(pool, pool_size, )
    if(init_pool(pool,pool_size)){
        FAILED("failed init the pool");
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

    int8_t err = 0;
    uint16_t bb = 512;
    FlashDecriptor_t fd_bb;
    const StoreNewValueInputArgs_t bb_args = {
        .value = &bb,
        .data_type = UINT16,
        .var_description = "a variable 512"
    };
    if (store_new_var(pool,&fd_bb, &bb_args) < 0) {
        FAILED("failed storing a var");
    }else{
        PASSED("data fetched and are correct");
    }

    err = read_var(pool, fd_a, aa);
    if (err < 0) {
        FAILED("failed reading a var");
        fprintf(stderr, "error code: %d\n", err);
    }else{
        PASSED("data fetched for var_b and are correct");
    }
    
    err = read_var(pool, fd_bb, bb);
    if (err < 0) {
        FAILED("failed reading a var");
        fprintf(stderr, "error code: %d\n", err);
    }else{
        PASSED("data fetched for var_b and are correct");
    }

    err = read_var(pool, 82, aa);
    if (err < 0) {
        if (err == -5) {
            PASSED("variable with wrong id not founded with correct error");
        }else{
            FAILED("variable with wrong id not founded but with wrong error");
            fprintf(stderr, "err: %d\n", err);
        }
    }else{
            FAILED("variable with wrong founded");
    }

    print_status_flash();

    print_SCORE();

    return EXIT_SUCCESS;
}
