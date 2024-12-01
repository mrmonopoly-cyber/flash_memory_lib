#ifndef __FLASH_MEMORY__
#define __FLASH_MEMORY__

#include <stdint.h>

#define MAX_NUMBER_OF_PAGES 10

typedef void TPagePool;
typedef void (*hardware_specific_function)(void);

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
    void* const flash_start_ptr;
    uint32_t flash_size;
    TPagePool* out_page_pool_ptr;
    const hardware_specific_function post_init;

    const hardware_specific_function pre_read;
    const hardware_specific_function post_read;

    const hardware_specific_function pre_write;
    const hardware_specific_function post_write;
}InitInputArgs;

typedef struct {
    enum DataTypesInFlash data_type;
    void* value;
    char var_description[]; //string must be NULL terminating
}StoreNewValueInputArgs;

typedef struct {
    uint32_t var_id;
    const void* out_parameter;
    const uint32_t size_out_parameter; //in bytes
}FetchValueInputArgs;

typedef struct {
    uint32_t var_id;
    const void* new_value; 
    const uint32_t size_new_value; //in bytes
}UpdateValueInputArgs;

typedef struct{
    enum DataTypesInFlash data_type;
    const char* const var_description;
}MetadataStoreVariableInFlash;

int 
flash_memory_init(const InitInputArgs*const args);

int 
flash_memory_store_new_value(
        const TPagePool* self, 
        const StoreNewValueInputArgs* const args);

int 
flash_memory_fetch_value(
        const TPagePool* const self, 
        const FetchValueInputArgs* const args);

int 
flash_memory_update_value(
        const TPagePool* const self,
        const UpdateValueInputArgs* const args);

int 
flash_memory_get_var_metadata(
        const TPagePool* const self,
        const uint32_t var_id,
        const MetadataStoreVariableInFlash* args);

#endif // !__FLASH_MEMORY__
