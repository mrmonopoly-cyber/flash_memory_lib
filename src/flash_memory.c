#include "./flash_memory.h"
#include <stdint.h>
#include <string.h>

typedef struct StoredVar{
    enum DataTypesInFlash data_type;
    void* extra_metadata;
    uint8_t max_length_description_string;
    char var_description[];
}StoredVar;

struct PagePool{
    void* start_address_flash;
    uint32_t flash_size;
    uint8_t next;
    uint8_t max_number_of_vars;
    hardware_read hw_read;
    hardware_write hw_write;
    StoredVar vars[];
};

#define CHECK_INIT(page_pool,ret_err) if (!page_pool.start_address_flash) {return ret_err;}
#define PAGEPOOL_T_INTO_PAGEPOOL(ptr_var_name, ptr_t_page_pool)\
    struct PagePool* ptr_var_name = ptr_t_page_pool;

#define INPUT_PTR_CHECK(ptr) if (ptr) {goto err_input_ptr;}
#define THROW_ERROR_IF_HAPPEN(err_out,expr) if (err_out < 0) {expr;}

//private

static int input_check_init_input(const InitInputArgs_t* args)
{
    INPUT_PTR_CHECK(args);
    INPUT_PTR_CHECK(args->flash_start_ptr);
    INPUT_PTR_CHECK(args->out_page_pool_ptr);
    if (!args->flash_size) goto err_input_ptr;

    return 0;

err_input_ptr:
    return -1;
}

static int input_check_store_new_value_input(const StoreNewValueInputArgs_t* args)
{
    INPUT_PTR_CHECK(args);
    INPUT_PTR_CHECK(args->value);
    if (args->data_type > DOUBLE) goto err_input_ptr;

    return 0;

err_input_ptr:
    return -1;
}

static int input_check_fetch_value_input(const FetchValueInputArgs_t* args)
{
    INPUT_PTR_CHECK(args);
    INPUT_PTR_CHECK(args->out_parameter);

    return 0;

err_input_ptr:
    return -1;
}

static int input_check_update_value_input(const UpdateValueInputArgs_t* args)
{
    INPUT_PTR_CHECK(args);
    INPUT_PTR_CHECK(args->new_value);
    if (!args->size_new_value) goto err_input_ptr;

    return 0;

err_input_ptr:
    return -1;
}

static int input_check_get_var_metadata_input(const MetadataStoreVariableInFlash_t* args){
    INPUT_PTR_CHECK(args);
    INPUT_PTR_CHECK(args->var_description);
    if (args->data_type > DOUBLE) goto err_input_ptr;

    return 0;

err_input_ptr:
    return -1;
}


//public
uint8_t
flash_memory_init(const InitInputArgs_t*const args)
{
    THROW_ERROR_IF_HAPPEN(input_check_init_input(args), {goto err_input_ptr;});
    PAGEPOOL_T_INTO_PAGEPOOL(pool, args->out_page_pool_ptr);
    if (!pool->start_address_flash) goto already_init_pool;

    uint32_t var_array_size = pool->max_number_of_vars * sizeof(pool->vars[0]);

    pool->start_address_flash = args->flash_start_ptr;
    pool->flash_size = args->flash_size;
    pool->hw_read = args->hw_read;
    pool->hw_write = args->hw_write;
    memset(pool->vars, 0, var_array_size);

    if(args->hw_init() < 0){
        goto error_init_hw;
    }

    return 0;

err_input_ptr:
    return -1;

already_init_pool:
    return -2;

error_init_hw:
    memset(args->out_page_pool_ptr, 0, sizeof(*args->out_page_pool_ptr));
    return -3;
}

uint8_t
flash_memory_store_new_value(const PagePool_t* self, const StoreNewValueInputArgs_t* const args)
{
    THROW_ERROR_IF_HAPPEN(input_check_store_new_value_input(args), {goto err_input_ptr;});

    return 0;

err_input_ptr:
    return -1;
}

uint8_t
flash_memory_fetch_value(const PagePool_t* const self, const FetchValueInputArgs_t* const args)
{
    INPUT_PTR_CHECK(self);
    THROW_ERROR_IF_HAPPEN(input_check_fetch_value_input(args), {goto err_input_ptr;})

    return 0;

err_input_ptr:
    return -1;
}

uint8_t
flash_memory_update_value(const PagePool_t* const self,const UpdateValueInputArgs_t* const args)
{
    INPUT_PTR_CHECK(self);
    THROW_ERROR_IF_HAPPEN(input_check_update_value_input(args), {goto err_input_ptr;})

    return 0;

err_input_ptr:
    return -1;
}

uint8_t
flash_memory_get_var_metadata(const PagePool_t* const self,const MetadataStoreVariableInFlash_t* args)
{

    INPUT_PTR_CHECK(self);
    THROW_ERROR_IF_HAPPEN(input_check_get_var_metadata_input(args), {goto err_input_ptr;});

    return 0;

err_input_ptr:
    return -1;
}
