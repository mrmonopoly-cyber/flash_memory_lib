/**
 * @file flash_memory.h
 * @brief Header file for flash memory management library.
 *
 * This file defines data structures, macros, and function prototypes for managing
 * variables in flash memory, including initialization, storing, fetching, updating,
 * and retrieving metadata for variables in flash memory.
 */

#ifndef __FLASH_MEMORY__
#define __FLASH_MEMORY__

#include <stdint.h>

/** @def BASE_PAGEPOOL_T_SIZE
 *  @brief Base size of the PagePool_t structure in bytes.
 */
#define BASE_PAGEPOOL_T_SIZE 40

/** @def STORED_VAR_SIZE
 *  @brief Size of a single stored variable in bytes.
 */
#define STORED_VAR_SIZE 24

/**
 * @brief Macro to initialize a stack-allocated PagePool_t buffer.
 *
 * @param ptr_name Name of the pointer to the PagePool_t.
 * @param size Number of variables to allocate in the PagePool_t.
 * @param var_attribute_compiler Additional compiler-specific attributes.
 */
#define STACK_PAGEPOOL_T(ptr_name, size, var_attribute_compiler) \
    char raw_buffer_pagepool_##ptr_name[BASE_PAGEPOOL_T_SIZE + size * STORED_VAR_SIZE] = {}; \
    PagePool_t* ptr_name var_attribute_compiler = raw_buffer_pagepool_##ptr_name;

/**
 * @enum DataTypesInFlash
 * @brief Enum for supported data types in flash memory.
 */
enum DataTypesInFlash {
    UINT8,   /**< Unsigned 8-bit integer */
    UINT16,  /**< Unsigned 16-bit integer */
    UINT32,  /**< Unsigned 32-bit integer */
    UINT64,  /**< Unsigned 64-bit integer */

    INT8,    /**< Signed 8-bit integer */
    INT16,   /**< Signed 16-bit integer */
    INT32,   /**< Signed 32-bit integer */
    INT64,   /**< Signed 64-bit integer */

    FLOAT,   /**< Single-precision floating-point */
    DOUBLE,  /**< Double-precision floating-point */
};

/**
 * @typedef FlashDecriptor_t
 * @brief Type representing a descriptor for variables stored in flash memory.
 */
typedef uint32_t FlashDecriptor_t;

/**
 * @typedef PagePool_t
 * @brief Placeholder type for a PagePool_t structure.
 */
typedef void PagePool_t;

/**
 * @typedef hardware_init
 * @brief Function pointer type for hardware initialization.
 *
 * @return Status code (0 for success, non-zero for failure).
 */
typedef int8_t (*hardware_init)(void);

/**
 * @typedef hardware_write
 * @brief Function pointer type for writing data to hardware.
 *
 * @param hw_metadata Metadata about the hardware.
 * @param var_id ID of the variable to write.
 * @param new_value Pointer to the new value to write.
 * @param size_new_value Size of the new value in bytes.
 * @return Status code (0 for success, non-zero for failure).
 */
typedef int8_t (*hardware_write)(void* hw_metadata, const FlashDecriptor_t var_id,
                                const void* new_value, const uint32_t size_new_value);

/**
 * @typedef hardware_read
 * @brief Function pointer type for reading data from hardware.
 *
 * @param hw_metadata Metadata about the hardware.
 * @param var_id ID of the variable to read.
 * @param o_buffer Output buffer to store the read value.
 * @param size_o_buffer Size of the output buffer in bytes.
 * @return Status code (0 for success, non-zero for failure).
 */
typedef int8_t (*hardware_read)(void* hw_metadata, const FlashDecriptor_t var_id,
                                void* o_buffer, const uint32_t size_o_buffer);

/**
 * @typedef create_id_var
 * @brief Function pointer type for creating a new variable ID.
 *
 * @param data_type Data type of the variable.
 * @param out Pointer to store the created variable ID.
 * @param o_hw_metadata Pointer to store hardware metadata.
 * @return Status code (0 for success, non-zero for failure).
 */
typedef int8_t (*create_id_var)(enum DataTypesInFlash, FlashDecriptor_t* out, void** o_hw_metadata);

/**
 * @typedef free_hw_metadata
 * @brief Function pointer type for freeing hardware metadata.
 *
 * @param metadata Pointer to the hardware metadata to free.
 * @return Status code (0 for success, non-zero for failure).
 */
typedef int8_t (*free_hw_metadata)(void* metadata);

/**
 * @struct InitInputArgs_t
 * @brief Input arguments for initializing the flash memory system.
 */
typedef struct {
    PagePool_t* out_page_pool_ptr;       /**< Pointer to the PagePool_t instance. */
    uint8_t max_number_of_vars;         /**< Maximum number of variables. */
    const hardware_init hw_init;        /**< Hardware initialization function. */
    const hardware_read hw_read;        /**< Hardware read function. */
    const hardware_write hw_write;      /**< Hardware write function. */
    const create_id_var hw_id_var;      /**< Function to create variable IDs. */
    const free_hw_metadata free_metadata; /**< Function to free hardware metadata. */
} InitInputArgs_t;

/**
 * @struct StoreNewValueInputArgs_t
 * @brief Input arguments for storing a new value in flash memory.
 */
typedef struct {
    enum DataTypesInFlash data_type; /**< Data type of the variable. */
    const void* value;               /**< Pointer to the value to store. */
    char* var_description;           /**< Description of the variable. */
} StoreNewValueInputArgs_t;

/**
 * @struct FetchValueInputArgs_t
 * @brief Input arguments for fetching a value from flash memory.
 */
typedef struct {
    const FlashDecriptor_t var_id; /**< ID of the variable to fetch. */
    void* out_parameter;          /**< Pointer to store the fetched value. */
    const uint8_t size_out_parameter; /**< Size of the output parameter in bytes. */
} FetchValueInputArgs_t;

/**
 * @struct UpdateValueInputArgs_t
 * @brief Input arguments for updating a value in flash memory.
 */
typedef struct {
    FlashDecriptor_t var_id;     /**< ID of the variable to update. */
    const void* new_value;       /**< Pointer to the new value. */
    const uint8_t size_new_value; /**< Size of the new value in bytes. */
} UpdateValueInputArgs_t;

/**
 * @struct MetadataStoreVariableInFlash_t
 * @brief Metadata for a stored variable in flash memory.
 */
typedef struct {
    FlashDecriptor_t var_id;        /**< ID of the variable. */
    enum DataTypesInFlash data_type; /**< Data type of the variable. */
    const char* var_description;   /**< Description of the variable. */
} MetadataStoreVariableInFlash_t;

/**
 * @brief Initialize the flash memory system.
 *
 * @param args Pointer to the initialization arguments.
 * @return Status code (0 for success, non-zero for failure).
 */
int8_t flash_memory_init(const InitInputArgs_t* const args);

/**
 * @brief Store a new value in flash memory.
 *
 * @param self Pointer to the PagePool_t instance.
 * @param args Pointer to the input arguments for storing a new value.
 * @param o_fd Pointer to store the resulting FlashDescriptor_t.
 * @return Status code (0 for success, non-zero for failure).
 */
int8_t flash_memory_store_new_value(PagePool_t* self, const StoreNewValueInputArgs_t* const args,
        FlashDecriptor_t* o_fd);

/**
 * @brief Fetch a value from flash memory.
 *
 * @param self Pointer to the PagePool_t instance.
 * @param args Pointer to the input arguments for fetching a value.
 * @return Status code (0 for success, non-zero for failure).
 */
int8_t flash_memory_fetch_value(const PagePool_t* const self, const FetchValueInputArgs_t* const args);

/**
 * @brief Update a value in flash memory.
 *
 * @param self Pointer to the PagePool_t instance.
 * @param args Pointer to the input arguments for updating a value.
 * @return Status code (0 for success, non-zero for failure).
 */
int8_t flash_memory_update_value(PagePool_t* const self, const UpdateValueInputArgs_t* const args);

/**
 * @brief Get metadata for a variable stored in flash memory.
 *
 * @param self Pointer to the PagePool_t instance.
 * @param args Pointer to the MetadataStoreVariableInFlash_t structure.
 * @return Status code (0 for success, non-zero for failure).
 */
 int8_t flash_memory_get_var_metadata(const PagePool_t* const self, MetadataStoreVariableInFlash_t* args);
#ifdef DEBUG
 /**
  * @brief Perform an integrity check on the PagePool_t instance.
  * @param self Pointer to the PagePool_t instance.
  * @return 1 if the integrity check passes, 0 otherwise.
  */
uint8_t integrity_check_PagePool_t(const PagePool_t* self);
#endif // DEBUG
#endif // !FLASH_MEMORY"}
