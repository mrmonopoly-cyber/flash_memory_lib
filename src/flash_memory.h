#ifndef __FLASH_MEMORY__
#define __FLASH_MEMORY__

#include <stdint.h>

typedef uint32_t FlashDecriptor_t;

typedef void PagePool_t;
typedef int8_t (*hardware_init)(void);
typedef uint8_t (*hardware_write)(void* extra_metadata, FlashDecriptor_t var_id,void* new_value,
                                uint32_t size_new_value);
typedef uint8_t (*hardware_read)(void* extra_metadata, FlashDecriptor_t var_id,void* o_buffer,
                                uint32_t size_o_buffer);

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

typedef struct{
    PagePool_t* out_page_pool_ptr;

    void* const flash_start_ptr;
    uint32_t flash_size;
    uint8_t max_number_of_vars;
    uint8_t max_length_var_description_string; //in bytes
    const hardware_init hw_init;
    const hardware_read hw_read;
    const hardware_write hw_write;
}InitInputArgs_t;

typedef struct {
    enum DataTypesInFlash data_type;
    void* value;
    char var_description[];
}StoreNewValueInputArgs_t;

typedef struct {
    FlashDecriptor_t var_id;
    const void* out_parameter;
    const uint32_t size_out_parameter; //in bytes
}FetchValueInputArgs_t;

typedef struct {
    FlashDecriptor_t var_id;
    const void* new_value; 
    const uint32_t size_new_value; //in bytes
}UpdateValueInputArgs_t;

typedef struct{
    const FlashDecriptor_t var_id;
    enum DataTypesInFlash data_type;
    const char* const var_description;
}MetadataStoreVariableInFlash_t;

uint8_t
flash_memory_init(const InitInputArgs_t*const args);

uint8_t
flash_memory_store_new_value(const PagePool_t* self, const StoreNewValueInputArgs_t* const args);

uint8_t
flash_memory_fetch_value(const PagePool_t* const self, const FetchValueInputArgs_t* const args);

uint8_t
flash_memory_update_value(const PagePool_t* const self,const UpdateValueInputArgs_t* const args);

uint8_t
flash_memory_get_var_metadata(const PagePool_t* const self,const MetadataStoreVariableInFlash_t* args);

#endif // !__FLASH_MEMORY__
