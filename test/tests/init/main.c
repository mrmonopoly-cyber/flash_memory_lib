#include <stdio.h>
#include <stdlib.h>
#include <sys/cdefs.h>
#include "./flash_memory/flash_memory.h"
#include "./test_flash/hw_flash.h"
#include "./test_score_lib/test_lib.h"

#define SET_HIGHER_ERROR_VALUE(curr_err,new_err) if (*curr_err < new_err) *curr_err = new_err
#define MAX_VARS (10)

static int test_wrong_input_init_hw_init(){
    STACK_PAGEPOOL_T(page_pool, MAX_VARS, __attribute_maybe_unused__);

    const InitInputArgs_t init_args = {
        .hw_init = NULL,
        .hw_read = hardware_read_imp,
        .hw_write = hardware_write_imp,
        .hw_id_var = create_id_var_imp,
        .free_metadata = free_hw_metadata_imp,
        .out_page_pool_ptr = page_pool,
        .max_number_of_vars = MAX_VARS,
    };
    int err_init = flash_memory_init(&init_args);
    if (err_init < 0) {
        PASSED("NULL hw_init found");
        return 0;
    }else{
        FAILED("NULL hw_init found found");
        return -1;
    }
}

static int test_wrong_input_init_hw_read(){
    STACK_PAGEPOOL_T(page_pool, MAX_VARS, __attribute_maybe_unused__);

    const InitInputArgs_t init_args = {
        .hw_init = hardware_init_imp,
        .hw_read = NULL,
        .hw_write = hardware_write_imp,
        .hw_id_var = create_id_var_imp,
        .free_metadata = free_hw_metadata_imp,
        .out_page_pool_ptr = page_pool,
        .max_number_of_vars = MAX_VARS,
    };
    int err_init = flash_memory_init(&init_args);
    if (err_init < 0) {
        PASSED("NULL hw_read found");
        return 0;
    }else{
        FAILED("NULL hw_read not found");
        return -1;
    }
}
static int test_wrong_input_init_hw_write(){
    STACK_PAGEPOOL_T(page_pool, MAX_VARS, __attribute_maybe_unused__);

    const InitInputArgs_t init_args = {
        .hw_init = hardware_init_imp,
        .hw_read = hardware_read_imp,
        .hw_write = NULL,
        .hw_id_var = create_id_var_imp,
        .free_metadata = free_hw_metadata_imp,
        .out_page_pool_ptr = page_pool,
        .max_number_of_vars = MAX_VARS,
    };
    int err_init = flash_memory_init(&init_args);
    if (err_init < 0) {
        PASSED("NULL hw_write found");
        return 0;
    }else{
        FAILED("NULL hw_write not found");
        return -1;
    }
}

static int test_wrong_input_init_hw_create_id(){
    STACK_PAGEPOOL_T(page_pool, MAX_VARS, __attribute_maybe_unused__);

    const InitInputArgs_t init_args = {
        .hw_init = hardware_init_imp,
        .hw_read = hardware_read_imp,
        .hw_write = hardware_write_imp,
        .hw_id_var = NULL,
        .free_metadata = free_hw_metadata_imp,
        .out_page_pool_ptr = page_pool,
        .max_number_of_vars = MAX_VARS,
    };
    int err_init = flash_memory_init(&init_args);
    if (err_init < 0) {
        PASSED("NULL hw_create_id found");
        return 0;
    }else{
        FAILED("NULL hw_cerate_id not found");
        return -1;
    }
}

static int test_wrong_input_init_hw_free_metadata(){
    STACK_PAGEPOOL_T(page_pool, MAX_VARS, __attribute_maybe_unused__);

    const InitInputArgs_t init_args = {
        .hw_init = hardware_init_imp,
        .hw_read = hardware_read_imp,
        .hw_write = hardware_write_imp,
        .hw_id_var = create_id_var_imp,
        .free_metadata = NULL,
        .out_page_pool_ptr = page_pool,
        .max_number_of_vars = MAX_VARS,
    };
    int err_init = flash_memory_init(&init_args);
    if (err_init < 0) {
        PASSED("NULL hw_free_metadata found");
        return 0;
    }else{
        FAILED("NULL hw_free_metadata not found");
        return -1;
    }
}

static int test_wrong_input_init_out_page_pool_ptr(){
    STACK_PAGEPOOL_T(page_pool, MAX_VARS, __attribute_maybe_unused__);

    const InitInputArgs_t init_args = {
        .hw_init = hardware_init_imp,
        .hw_read = hardware_read_imp,
        .hw_write = hardware_write_imp,
        .hw_id_var = create_id_var_imp,
        .free_metadata = free_hw_metadata_imp,
        .out_page_pool_ptr = NULL,
        .max_number_of_vars = MAX_VARS,
    };
    int err_init = flash_memory_init(&init_args);
    if (err_init < 0) {
        PASSED("NULL out_page_pool_ptr found");
        return 0;
    }else{
        FAILED("NULL out_page_pool_ptr not found");
        return -1;
    }
}

static int test_wrong_input_init_max_number_of_vars(){
    STACK_PAGEPOOL_T(page_pool, MAX_VARS, __attribute_maybe_unused__);

    const InitInputArgs_t init_args = {
        .hw_init = hardware_init_imp,
        .hw_read = hardware_read_imp,
        .hw_write = hardware_write_imp,
        .hw_id_var = create_id_var_imp,
        .free_metadata = free_hw_metadata_imp,
        .out_page_pool_ptr = page_pool,
        .max_number_of_vars = 0,
    };
    int err_init = flash_memory_init(&init_args);
    if (err_init < 0) {
        PASSED("0 max_number_of_vars found");
        return 0;
    }else{
        FAILED("0 max_number_of_vars not found");
        return -1;
    }
}

static int test_correct_init_with_double_init(){
    STACK_PAGEPOOL_T(page_pool, MAX_VARS, __attribute_maybe_unused__);

    const InitInputArgs_t init_args = {
        .hw_init = hardware_init_imp,
        .hw_read = hardware_read_imp,
        .hw_write = hardware_write_imp,
        .hw_id_var = create_id_var_imp,
        .free_metadata = free_hw_metadata_imp,
        .out_page_pool_ptr = page_pool,
        .max_number_of_vars = MAX_VARS,
    };
    int err_init = flash_memory_init(&init_args);
    if ((err_init < 0) ) {
        FAILED("init failed");
        return -1;
    }else{
#ifdef DEBUG 
        if(!integrity_check_PagePool_t(page_pool)){
            FAILED("init passed but consistency failed");
            return -2;
        }
#endif /* ifndef DEBUG */
        PASSED("init ok");
    }

    err_init = flash_memory_init(&init_args);
    if (err_init == -2) {
        PASSED("found double init");
        return -2;
    }else if (err_init < 0){
        FAILED("found error: ");
        fprintf(stderr, "%d\n", err_init);
        return err_init;
    }else{
        FAILED("double init not found");
    }

    return 0;
}

int main(int argc __attribute_maybe_unused__, char *argv[] __attribute_maybe_unused__)
{
    test_wrong_input_init_hw_init();
    test_wrong_input_init_hw_read();
    test_wrong_input_init_hw_write();
    test_wrong_input_init_hw_create_id();
    test_wrong_input_init_hw_free_metadata();
    test_wrong_input_init_out_page_pool_ptr();
    test_wrong_input_init_max_number_of_vars();
    test_correct_init_with_double_init();

    print_SCORE();
    return EXIT_SUCCESS;

}
