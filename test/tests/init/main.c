#include <stdlib.h>
#include <sys/cdefs.h>
#include "./flash_memory/flash_memory.h"
#include "./test_flash/hw_flash.h"
#include "./test_score_lib/test_lib.h"

#define SET_HIGHER_ERROR_VALUE(curr_err,new_err) if (*curr_err < new_err) *curr_err = new_err
#define MAX_VARS (10)

int main(int argc __attribute_maybe_unused__, char *argv[] __attribute_maybe_unused__)
{
    STACK_PAGEPOOL_T(page_pool, MAX_VARS);

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
    if (err_init < 0) {
        FAILED("init failed");
    }else{
        PASSED("init ok");
    }


    print_SCORE();
    return EXIT_SUCCESS;

}
