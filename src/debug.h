#ifndef DEBUG_H

#define DEBUG_H

void debug_dump_cpu();
void debug_debug();

void debug_disassm_instr(uint16_t addr);
void debug_disassm_instr_n(uint16_t addr, uint16_t n);


extern uint16_t debug_breaks[128];
extern uint8_t debug_break_i;
extern uint8_t debug_pause;

#endif