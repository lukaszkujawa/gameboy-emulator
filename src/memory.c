#include <stdint.h>

#include "memory.h"
#include "cpu.h"

uint8_t *GB_MEMORY;

uint8_t memory_read_u8(uint16_t addr) {
	return GB_MEMORY[addr];
}

int8_t memory_read_s8(uint16_t addr) {
	return (int8_t) GB_MEMORY[addr];
}

void memory_write_u8(uint16_t addr, uint8_t val) {
	GB_MEMORY[addr] = val;
}