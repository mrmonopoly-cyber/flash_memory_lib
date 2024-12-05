#ifndef __FLASH_MEMORY__
#define __FLASH_MEMORY__

#include <stdint.h>

#define BASE_PAGEPOOL_T_SIZE 40
#define STORED_VAR_SIZE 24
#define STACK_PAGEPOOL_T(ptr_name,size, var_attribute_compiler)\
    char raw_buffer_pagepool_##ptr_name[BASE_PAGEPOOL_T_SIZE + size * STORED_VAR_SIZE] = {};\
    PagePool_t* ptr_name var_attribute_compiler = raw_buffer_pagepool_##ptr_name; 

enum DataTypesInFlash {
    UINT8,
    UINT16,
    UINT32,
    UINT64,

    INT8,
    INT16,
    INT32,
    INT64,

    FLOAT,
    DOUBLE,
};

typedef uint32_t FlashDecriptor_t;

typedef void PagePool_t;
typedef int8_t (*hardware_init)(void);
typedef int8_t (*hardware_write)(void* hw_metadata,const FlashDecriptor_t var_id,
                                const void* new_value, const uint32_t size_new_value);
typedef int8_t (*hardware_read)(void* hw_metadata,const FlashDecriptor_t var_id,
                                void* o_buffer,const uint32_t size_o_buffer);
typedef int8_t (*create_id_var) (enum DataTypesInFlash, FlashDecriptor_t* out, void** o_hw_metadata);
typedef int8_t (*free_hw_metadata) (void* metadata);

typedef struct{
    PagePool_t* out_page_pool_ptr;

    uint8_t max_number_of_vars;
    const hardware_init hw_init;
    const hardware_read hw_read;
    const hardware_write hw_write;
    const create_id_var hw_id_var;
    const free_hw_metadata free_metadata;
}InitInputArgs_t;

typedef struct {
    enum DataTypesInFlash data_type;
    const void* value;
    char* var_description;
}StoreNewValueInputArgs_t;

typedef struct {
    const FlashDecriptor_t var_id;
    void* out_parameter;
    const uint8_t size_out_parameter; //in bytes
}FetchValueInputArgs_t;

typedef struct {
    FlashDecriptor_t var_id;
    const void* new_value; 
    const uint8_t size_new_value; //in bytes
}UpdateValueInputArgs_t;

typedef struct{
    FlashDecriptor_t var_id;
    enum DataTypesInFlash data_type;
    const char* var_description;
}MetadataStoreVariableInFlash_t;

int8_t
flash_memory_init(const InitInputArgs_t*const args);

int8_t
flash_memory_store_new_value(PagePool_t* self, const StoreNewValueInputArgs_t* const args,
        FlashDecriptor_t* o_fd);

int8_t
flash_memory_fetch_value(const PagePool_t* const self, const FetchValueInputArgs_t* const args);

int8_t
flash_memory_update_value(PagePool_t* const self,const UpdateValueInputArgs_t* const args);

int8_t
flash_memory_get_var_metadata(const PagePool_t* const self,MetadataStoreVariableInFlash_t* args);

#ifdef DEBUG
uint8_t integrity_check_PagePool_t(const PagePool_t* self);
#endif // DEBUG

#endif // !__FLASH_MEMORY__
