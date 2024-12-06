# Flash Memory Management Library

This library provides a modular and extensible interface for managing variables in flash memory. Designed for embedded systems, it facilitates initialization, storage, retrieval, updating, and metadata management for variables stored in flash memory.

## Features
- Supports various data types, including integers, floating-point numbers, and more.
- Abstracted hardware interface using function pointers for flexibility.
- Lightweight and optimized for embedded environments.
- Includes optional debugging tools for integrity checks.

## Getting Started

### Prerequisites
- A C compiler (e.g., `gcc`, `clang`).
- Make utility (for using the provided Makefile).

### Building the Library
1. Clone the repository:
   ```bash
   git clone https://github.com/mrmonopoly-cyber/flash_memory_lib.git
   cd flash_memory_lib
   ```

2. Build the library using the Makefile:
   - Debug mode:
     ```bash
     make debug
     ```
   - Release mode:
     ```bash
     make release
     ```

### Integration
Include the `flash_memory.h` header in your project and link the compiled library. Implement the hardware abstraction functions (e.g., `hardware_read`, `hardware_write`) specific to your hardware.

## API Overview

### Core Functions
- **Initialization**:
  ```c
  int8_t flash_memory_init(const InitInputArgs_t* const args);
  ```
  Initializes the flash memory system.

- **Storing a New Value**:
  ```c
  int8_t flash_memory_store_new_value(PagePool_t* self, const StoreNewValueInputArgs_t* const args, FlashDecriptor_t* o_fd);
  ```
  Stores a new variable in flash memory and provides a descriptor.

- **Fetching a Value**:
  ```c
  int8_t flash_memory_fetch_value(const PagePool_t* const self, const FetchValueInputArgs_t* const args);
  ```
  Retrieves a stored variable by its descriptor.

- **Updating a Value**:
  ```c
  int8_t flash_memory_update_value(PagePool_t* const self, const UpdateValueInputArgs_t* const args);
  ```
  Updates an existing variable in flash memory.

- **Retrieving Metadata**:
  ```c
  int8_t flash_memory_get_var_metadata(const PagePool_t* const self, const FlashDecriptor_t var_id, MetadataStoreVariableInFlash_t* out);
  ```
  Gets metadata for a stored variable.

### Debugging (Optional)
- **Integrity Check**:
  ```c
  uint8_t integrity_check_PagePool_t(const PagePool_t* self);
  ```
  Ensures the integrity of the `PagePool_t` instance (available in debug builds).

## Hardware Abstraction
The library uses function pointers to interface with hardware, making it adaptable to different platforms. You need to implement the following:

- **Hardware Initialization**:
  ```c
  typedef int8_t (*hardware_init)(void);
  ```

- **Read from Flash**:
  ```c
  typedef int8_t (*hardware_read)(void* hw_metadata, const FlashDecriptor_t var_id, void* o_buffer, const uint32_t size_o_buffer);
  ```

- **Write to Flash**:
  ```c
  typedef int8_t (*hardware_write)(void* hw_metadata, const FlashDecriptor_t var_id, const void* new_value, const uint32_t size_new_value);
  ```

- **Create Variable ID**:
  ```c
  typedef int8_t (*create_id_var)(enum DataTypesInFlash, FlashDecriptor_t* out, void** o_hw_metadata);
  ```

- **Free Metadata**:
  ```c
  typedef int8_t (*free_hw_metadata)(void* metadata);
  ```

## Testing
In the repo the are also a few test. If you want to see check [test](./test/Readme.md)

## License
This project is licensed under the MIT License. See the `LICENSE` file for details.

## Contributions
Contributions, bug reports, and feature requests are welcome! Please create an issue or submit a pull request.

## Author
[Mr. Monopoly Cyber](https://github.com/mrmonopoly-cyber)

