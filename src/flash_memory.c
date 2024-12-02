#include "./flash_memory.h"
#include <stdint.h>
#include <string.h>


typedef struct MemoryPage{
    void* ptr_data;
    enum DataTypesInFlash data_type;
}MemoryPage;

struct PagePool{
    MemoryPage pages[MAX_NUMBER_OF_PAGES];
    void* start_address_flash;
    uint32_t flash_size;
    uint8_t next;
};

#define CHECK_INIT(page_pool,ret_err) if (!page_pool.start_address_flash) {return ret_err;}
#define TPAGEPOOL_INTO_PAGEPOOL(ptr_var_name, ptr_t_page_pool)\
    struct PagePool* ptr_var_name = ptr_t_page_pool;

#define INPUT_PTR_CHECK(ptr) if (ptr) {goto err_input_ptr;}
#define THROW_ERROR_IF_HAPPEN(err_out,expr) if (err_out < 0) {expr;}

//private

static int input_check_init_input(const InitInputArgs* args)
{
    INPUT_PTR_CHECK(args);
    INPUT_PTR_CHECK(args->flash_start_ptr);
    INPUT_PTR_CHECK(args->out_page_pool_ptr);
    if (!args->flash_size) goto err_input_ptr;

    return 0;

err_input_ptr:
    return -1;
}

static int input_check_store_new_value_input(const StoreNewValueInputArgs* args)
{
    INPUT_PTR_CHECK(args);
    INPUT_PTR_CHECK(args->value);
    if (args->data_type > DOUBLE) goto err_input_ptr;

    return 0;

err_input_ptr:
    return -1;
}

static int input_check_fetch_value_input(const FetchValueInputArgs* args)
{
    INPUT_PTR_CHECK(args);
    INPUT_PTR_CHECK(args->out_parameter);

    return 0;

err_input_ptr:
    return -1;
}

static int input_check_update_value_input(const UpdateValueInputArgs* args)
{
    INPUT_PTR_CHECK(args);
    INPUT_PTR_CHECK(args->new_value);
    if (!args->size_new_value) goto err_input_ptr;

    return 0;

err_input_ptr:
    return -1;
}

static int input_check_get_var_metadata_input(const MetadataStoreVariableInFlash* args){
    INPUT_PTR_CHECK(args);
    INPUT_PTR_CHECK(args->var_description);
    if (args->data_type > DOUBLE) goto err_input_ptr;

    return 0;

err_input_ptr:
    return -1;
}


//public
int 
flash_memory_init(const InitInputArgs*const args)
{
    THROW_ERROR_IF_HAPPEN(input_check_init_input(args), {goto err_input_ptr;});

    TPAGEPOOL_INTO_PAGEPOOL(pool, args->out_page_pool_ptr);
    if (!pool->start_address_flash) {
        goto already_init_pool;
    }

    pool->start_address_flash = args->flash_start_ptr;
    pool->flash_size = args->flash_size;
    memset(pool->pages, 0, sizeof(pool->pages));

    return 0;

err_input_ptr:
    return -1;

already_init_pool:
    return -2;
}

int 
flash_memory_store_new_value(const TPagePool* self, const StoreNewValueInputArgs* const args)
{
    THROW_ERROR_IF_HAPPEN(input_check_store_new_value_input(args), {goto err_input_ptr;});

err_input_ptr:
    return -1;
}

int 
flash_memory_fetch_value(const TPagePool* const self, const FetchValueInputArgs* const args)
{
    INPUT_PTR_CHECK(self);
    THROW_ERROR_IF_HAPPEN(input_check_fetch_value_input(args), {goto err_input_ptr;})


err_input_ptr:
    return -1;
}

int 
flash_memory_update_value(const TPagePool* const self,const UpdateValueInputArgs* const args)
{
    INPUT_PTR_CHECK(self);
    THROW_ERROR_IF_HAPPEN(input_check_update_value_input(args), {goto err_input_ptr;})


err_input_ptr:
    return -1;
}

int 
flash_memory_get_var_metadata(const TPagePool* const self,const MetadataStoreVariableInFlash* args)
{

    INPUT_PTR_CHECK(self);
    THROW_ERROR_IF_HAPPEN(input_check_get_var_metadata_input(args), {goto err_input_ptr;});


err_input_ptr:
    return -1;
}
