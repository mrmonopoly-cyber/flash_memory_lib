#include "./hardware_spec.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//private
#define INPUT_PTR_CHECK(ptr) if (!ptr) {goto err_input_ptr;}

#define IFXFLASH_DFLASH_PAGE_LENGTH (8)
#define SIZE_FLASH (5 * IFXFLASH_DFLASH_PAGE_LENGTH)

static uint8_t flash_memory[SIZE_FLASH];
#define IFXFLASH_DFLASH_START &flash_memory[0]

#define GET_MEMORY_ADDRESS(page)    (IFXFLASH_DFLASH_START + ((page) * IFXFLASH_DFLASH_PAGE_LENGTH))
#define pMEM(page)                  (uint8_t *)(GET_MEMORY_ADDRESS(page))


//public

void print_page(const uint8_t page_number)
{
    uint32_t* flash_ptr = (uint32_t*) pMEM(page_number);
    printf("page number %d: %d,%d\n",page_number,*flash_ptr, *(flash_ptr + 1));
}

void hw_spec_init_buffer(const void* buffer, const uint32_t buffer_size)
{
    uint32_t data_to_copy = buffer_size < SIZE_FLASH? buffer_size: SIZE_FLASH;
    memcpy(flash_memory, buffer, data_to_copy);
}

int 
read(const uint8_t page_number, uint32_t* o_word1, uint32_t* o_word2)
{
    int8_t err = 0;

    INPUT_PTR_CHECK(o_word1);
    INPUT_PTR_CHECK(o_word2);
    uint32_t* flash_ptr = (uint32_t*) pMEM(page_number);
    *o_word1 = *o_word2 = 0;

    if ( ((uint8_t*) (flash_ptr + 1)) >= &flash_memory[SIZE_FLASH]) {
        goto invalid_page_number;
    }

    *o_word1 = *flash_ptr;
    *o_word2 = *(flash_ptr + 1);

    return 0;


invalid_page_number:
    err--;
err_input_ptr:
    err--;

    return err;
}

int 
write(const uint8_t page_number, const uint32_t word1, const uint32_t word2)
{
    int8_t err = 0;

    uint32_t* flash_ptr = (uint32_t*) pMEM(page_number);
    memset(flash_ptr, 0, sizeof(uint64_t));

    if ( ((uint8_t*) (flash_ptr + 1)) >= &flash_memory[SIZE_FLASH]) {
        goto invalid_page_number;
    }

    *flash_ptr = word1;
    *(flash_ptr + 1)= word2;

    return 0;

invalid_page_number:
    err--;

    return err;
}
