#ifndef MEMORY_H

#define MEMORY_H

uint8_t memory_read_u8(uint16_t addr);
int8_t memory_read_s8(uint16_t addr);
void memory_write_u8(uint16_t addr, uint8_t val);

#endif