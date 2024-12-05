#ifndef __HARDWARE_SPEC__
#define __HARDWARE_SPEC__

#include <stdint.h>

int read(const uint8_t page_number, uint32_t* o_word1, uint32_t* o_word2);
int write(const uint8_t page_number, const uint32_t word1, const uint32_t word2);


#endif // !__HARDWARE_SPEC__