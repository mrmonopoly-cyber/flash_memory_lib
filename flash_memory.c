#include "./flash_memory.h"

#if __STDC_VERSION__ >= 201112L
#include <stdatomic.h>
#endif
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//private
typedef struct StoredVar{
#if __STDC_VERSION__ >= 201112L
    atomic_flag lock;

#else
    uint8_t lock;
#endif
    enum DataTypesInFlash data_type;
    FlashDecriptor_t fd;
    char* var_description;
    void* extra_metadata;
}StoredVar;

struct PagePool{
#if __STDC_VERSION__  >= 201112L | defined (__TASKING__)
    atomic_uchar next_var;
#else
    uint8_t next_var_lock;
    uint8_t next_var;
#endif
    uint8_t max_number_of_vars;
    hardware_read hw_read;
    hardware_write hw_write;
#if __STDC_VERSION__ >= 201112L
    atomic_flag hw_id_lock;
#else
    uint8_t hw_id_lock;
#endif
    create_id_var hw_id_var;
    free_hw_metadata free_metadata;
    StoredVar vars[];
};

#ifdef DEBUG
const uint8_t st_var = sizeof(StoredVar);
const uint8_t page_pool = sizeof(struct PagePool);
uint8_t assert_size_pagepool[(sizeof(struct PagePool) == BASE_PAGEPOOL_T_SIZE ) ? 1 : -1] = {};
uint8_t assert_size_stored_var_1[(sizeof(StoredVar) == STORED_VAR_SIZE) ? 1 : -1] = {};
#endif

#define CHECK_INIT(page_pool,ret_err) if (!page_pool.start_address_flash) {return ret_err;}
#define PAGEPOOL_T_INTO_PAGEPOOL(ptr_var_name, ptr_t_page_pool)\
    struct PagePool* ptr_var_name = ptr_t_page_pool;
#define CONST_PAGEPOOL_T_INTO_PAGEPOOL(ptr_var_name, ptr_t_page_pool)\
    const struct PagePool* ptr_var_name = ptr_t_page_pool;

#define INPUT_PTR_CHECK(ptr) if (!ptr) {goto err_input_ptr;}
#define THROW_ERROR_IF_HAPPEN(err_exp,expr) if (err_exp) {expr;}

#define INIT_CHECK(pool,exp) if (!pool->hw_read) {exp}

#if __STDC_VERSION__ >= 201112L
#define EXTRACT_NEXT_VAR(pool,out_var) out_var = atomic_load(&pool->next_var)
#define INCREASE_NEXT_VAR(pool) atomic_fetch_add(&pool->next_var, 1)
#define WORK_UN_STORED_VAR(new_var,exp)\
    /*wait for lock to be release and then lock*/\
    while (atomic_flag_test_and_set(new_var->lock)) {};\
    exp;\
    /*unlock*/\
    atomic_flag_clear(new_var->lock);
#else 
#define EXTRACT_NEXT_VAR(pool,out_var) \
    while(pool->next_var_lock) {}\
    pool->next_var_lock = 1;\
    out_var = pool->next_var;\
    pool->next_var_lock=0;
#define INCREASE_NEXT_VAR(pool) \
while(pool->next_var_lock) {}\
    pool->next_var_lock = 1;\
    pool->next_var++;\
    pool->next_var_lock=0;
#define WORK_UN_STORED_VAR(new_var,exp) \
        while (new_var->lock)) {};\
        new_var->lock=1;\ 87
        exp;\
        new_var->lock=0;
#endif /* if __STDC__ == 201112L | defined (__TASKING__) */

#define FIND_VAR_AND_EXECUTE_EXPR_ON_IT(pool,var_id,expr)\
    StoredVar* var = NULL;\
    uint16_t max = 0;\
    EXTRACT_NEXT_VAR(pool, max);\
    for (uint8_t i=0; i<max; i++) {\
        var = &pool->vars[i];\
        if (var->fd == var_id) {\
            expr;\
        }\
    }\
    goto variable_not_found;

#define CONST_FIND_VAR_AND_EXECUTE_EXPR_ON_IT(pool,var_id,expr)\
    const StoredVar* var = NULL;\
    for (uint8_t i=0; i<EXTRACT_NEXT_VAR(pool); i++) {\
        var = &pool->vars[i];\
        if (var->fd == var_id) {\
            expr;\
        }\
    }\
    goto variable_not_found;

static int input_check_init_input(const InitInputArgs_t* args)
{
    INPUT_PTR_CHECK(args);
    INPUT_PTR_CHECK(args->out_page_pool_ptr);
    INPUT_PTR_CHECK(args->hw_init);
    INPUT_PTR_CHECK(args->hw_read);
    INPUT_PTR_CHECK(args->hw_write);
    INPUT_PTR_CHECK(args->hw_id_var);
    INPUT_PTR_CHECK(args->free_metadata);
    THROW_ERROR_IF_HAPPEN(!args->max_number_of_vars, {goto err_input_ptr;})

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
    return 0;

err_input_ptr:
    return -1;
}

static int8_t get_size_from_data_type(const enum DataTypesInFlash type){
    switch (type) {
        case UINT8:
        case INT8:
            return sizeof(uint8_t);
        case UINT16:
        case INT16:
            return sizeof(uint16_t);
        case UINT32:
        case INT32:
            return sizeof(uint32_t);
        case UINT64:
        case INT64:
            break;
        case FLOAT:
            return sizeof(float);
        case DOUBLE:
            return sizeof(double);
        default:
            goto error_invalid_size;
    }
error_invalid_size:
    return -1;
}

//public
int8_t
flash_memory_init(const InitInputArgs_t*const args)
{
    int8_t err=0;
    THROW_ERROR_IF_HAPPEN(input_check_init_input(args) < 0, {goto err_input_ptr;});
    PAGEPOOL_T_INTO_PAGEPOOL(pool, args->out_page_pool_ptr);
    if (pool->hw_read) goto already_init_pool;

    uint32_t var_array_size = pool->max_number_of_vars * sizeof(pool->vars[0]);

    pool->hw_read = args->hw_read;
    pool->hw_write = args->hw_write;
    pool->hw_id_var = args->hw_id_var;
    pool->free_metadata = args->free_metadata;
    pool->max_number_of_vars = args->max_number_of_vars;
    memset(pool->vars, 0, var_array_size);

    if(args->hw_init() < 0){
        goto error_init_hw;
    }

    return 0;


error_init_hw:
    memset(args->out_page_pool_ptr, 0, sizeof(*pool));
    err--;
already_init_pool:
    err--;
err_input_ptr:
    err--;

    return err;
}

int8_t
flash_memory_store_new_value(PagePool_t* self, const StoreNewValueInputArgs_t* const args,
        FlashDecriptor_t* o_fd)
{
    int8_t err = 0;
    THROW_ERROR_IF_HAPPEN(input_check_store_new_value_input(args) < 0, {goto err_input_ptr;});
    PAGEPOOL_T_INTO_PAGEPOOL(pool, self);
    INIT_CHECK(pool, {goto pool_not_initialized;});

    uint8_t next_var = 0;
    EXTRACT_NEXT_VAR(pool,next_var);
    if (next_var >= pool->max_number_of_vars) {
        goto full_pool;
    }
    StoredVar* new_var = &pool->vars[next_var];
    void** extra_metadata = &new_var->extra_metadata;
    *o_fd = 0;
    //while (atomic_flag_test_and_set(&pool->hw_id_lock)) {}
        if(pool->hw_id_var(args->data_type,&new_var->fd, extra_metadata ) < 0){
            goto error_assigning_fd_to_var;
        }
    //atomic_flag_clear(&pool->hw_id_lock);

    int16_t data_size = get_size_from_data_type(args->data_type);
    if (data_size < 0) {
        goto error_computing_size_of_var;
    }
    WORK_UN_STORED_VAR(&new_var,{
        if(pool->hw_write(new_var->extra_metadata,new_var->fd,args->value, data_size) < 0){
            goto error_hw_writing_bytes;
        }
        new_var->var_description = args->var_description;
        new_var->data_type = args->data_type;
    })

    *o_fd = new_var->fd;
    INCREASE_NEXT_VAR(pool);

    return 0;

error_hw_writing_bytes:
    if (pool->free_metadata){
        pool->free_metadata(new_var->extra_metadata);
        new_var->extra_metadata = NULL;
    }

error_computing_size_of_var:
    err--;
error_assigning_fd_to_var:
    new_var->fd=0;
    err--;
full_pool:
    err--;
pool_not_initialized:
    err--;
err_input_ptr:
    err--;

    return err;
}

int8_t
flash_memory_fetch_value(PagePool_t* const self, const FetchValueInputArgs_t* const args)
{
    int8_t err=0;
    INPUT_PTR_CHECK(self);
    THROW_ERROR_IF_HAPPEN(input_check_fetch_value_input(args) < 0, {goto err_input_ptr;})
    PAGEPOOL_T_INTO_PAGEPOOL(pool, self);
    INIT_CHECK(pool, {goto pool_not_initialized;});


    FIND_VAR_AND_EXECUTE_EXPR_ON_IT(pool,args->var_id,{
        if (args->size_out_parameter < get_size_from_data_type(var->data_type)) {
            goto out_buffer_too_small;
        }
        WORK_UN_STORED_VAR(&var, {
            if (pool->hw_read(var->extra_metadata, var->fd, 
                        args->out_parameter, args->size_out_parameter) < 0) {
            goto hw_read_error;
            }
        })
        return 0;
    });

variable_not_found:
    err--;
hw_read_error:
    err--;
out_buffer_too_small:
    err--;
pool_not_initialized:
    err--;
err_input_ptr:
    err--;

    return err;
}

int8_t
flash_memory_update_value(PagePool_t* const self,const UpdateValueInputArgs_t* const args)
{
    int8_t err=0;
    INPUT_PTR_CHECK(self);
    THROW_ERROR_IF_HAPPEN(input_check_update_value_input(args) < 0, {goto err_input_ptr;})
    PAGEPOOL_T_INTO_PAGEPOOL(pool, self);
    INIT_CHECK(pool, {goto pool_not_initialized;});

    FIND_VAR_AND_EXECUTE_EXPR_ON_IT(pool,args->var_id,{
        if (args->size_new_value > get_size_from_data_type(var->data_type)) {
            goto new_var_too_big;
        }
        WORK_UN_STORED_VAR(&var, {
            if (pool->hw_write(var->extra_metadata,args->var_id, 
                        args->new_value,args->size_new_value) < 0) 
            {
                goto hw_write_error;
            }
        });

        return 0;
    });



variable_not_found:
    err--;
hw_write_error:
    err--;
new_var_too_big:
    err--;
pool_not_initialized:
    err--;
err_input_ptr:
    err--;

    return err;
}

int8_t
flash_memory_get_var_metadata(PagePool_t* const self, const FlashDecriptor_t var_id, MetadataStoreVariableInFlash_t* args)
{
    int8_t err =0;
    INPUT_PTR_CHECK(self);
    THROW_ERROR_IF_HAPPEN(input_check_get_var_metadata_input(args) < 0, {goto err_input_ptr;});
    PAGEPOOL_T_INTO_PAGEPOOL(pool, self);
    INIT_CHECK(pool, {goto pool_not_initialized;});

    FIND_VAR_AND_EXECUTE_EXPR_ON_IT(pool,var_id,{
        WORK_UN_STORED_VAR(&var, {
            const StoredVar* c_var = var;
            args->data_type = c_var->data_type;
            args->var_description = var->var_description;
            return 0;
        });
    });

variable_not_found:
    err--;
err_input_ptr:
    err--;
pool_not_initialized:
    err--;

    return err;
}

#ifdef DEBUG
uint8_t integrity_check_PagePool_t(const PagePool_t* self)
{
    CONST_PAGEPOOL_T_INTO_PAGEPOOL(pool, self);
    return 
        pool->hw_read &&
        pool->hw_write && 
        pool->hw_id_var && 
        pool->free_metadata &&
        pool->max_number_of_vars;

}
#endif // DEBUG
