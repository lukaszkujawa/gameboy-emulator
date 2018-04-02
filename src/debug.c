#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "cpu.h"

uint16_t debug_breaks[128];
uint8_t debug_break_i = 0;
uint8_t debug_pause = 0;

void debug_dump_cpu() {
	printf("  A=$%02x  AF=$%04x\n  F=$%02x \n  B=$%02x  BC=$%04x\n  C=$%02x \n  D=$%02x  DE=$%04x\n  E=$%02x \n  H=$%02x  HL=$%04x\n  L=$%02x \n  SP=$%04x \n  PC=$%04x \n",
		CPU.r.a,
		((_cpu_registers16b*)(&CPU.r))->af, 
		CPU.r.f, 
		CPU.r.b, 
		((_cpu_registers16b*)(&CPU.r))->bc,
		CPU.r.c, 
		CPU.r.d,
		((_cpu_registers16b*)(&CPU.r))->de, 
		CPU.r.e, 
		CPU.r.h, 
		((_cpu_registers16b*)(&CPU.r))->hl,
		CPU.r.l, 
		CPU.r.sp, 
		CPU.r.pc       
	);

	printf("  z=%d \n  n=%d \n  h=%d \n  c=%d\n",
		CPU.flags.z, CPU.flags.n, CPU.flags.h, CPU.flags.c );
}

void debug_debug() {
	char *str = NULL;
	size_t len;
	uint16_t addr = 0;
	int val = 4;

	for(int i = 0 ; i < debug_break_i ; i++) {
		if(debug_breaks[i] == CPU.r.pc) {
			printf("at break $%04x\n", debug_breaks[i]);
			debug_pause = 1;
			break;
		}
	}

	debug_disassm_instr(CPU.r.pc);

	if(debug_pause == 0) {
		return;
	}
		
	while(1) {
		printf("(debug) ");
		getline(&str, &len, stdin);
		
		if(str[0] == 'q') {
			exit(1);
		}

		if(str[0] == 'c') {
			debug_pause = 0;
			break;
		}

		if(str[0] == 'd') {
			debug_disassm_instr_n(CPU.r.pc, (uint16_t)7);
		}

		if(strcmp(str, "reg\n") == 0) {
			debug_dump_cpu();
		}

		if(sscanf(str, "x $%4hx/%d\n", &addr, &val)) {
			for(int i = 0 ; i < val ; i++) {
				printf("$%02x ", GB_MEMORY[addr+i]);
				if(i % 8 == 7) printf("\n");
			}
			printf("\n");
			val = 4;
		}

		if(sscanf(str, "b $%4hx\n", &addr)) {
			printf("setting break at $%04x\n", addr);
			debug_breaks[debug_break_i] = addr;
			debug_break_i++;
		}

		if(str[0] == 'n') {
			break;
		}
	}
}


void debug_disassm_instr_n(uint16_t addr, uint16_t n) {
	for(; n > 0 ;n--){
		uint8_t op_code = GB_MEMORY[addr];
		uint8_t code_len = OPCODE_LEN[op_code];

		debug_disassm_instr(addr);
		addr += code_len;
	}
}


void debug_disassm_instr(uint16_t addr) {
	unsigned char op_code = GB_MEMORY[addr];
	unsigned char code_len = OPCODE_LEN[op_code];
	unsigned char *line =  &GB_MEMORY[addr];
	short int value = 0;

	printf("\t");
	char str_buff[128];

	switch(code_len) {
		case 1:
			printf("%-30s", OPCODE_STR[op_code]);
			printf("; $%04x:\t$%02x\n", addr, op_code);
			break;
		case 2:
			if(op_code == 0x18 || op_code == 0x20 || op_code == 0x28 ||  op_code == 0x30 || op_code == 0x38 || op_code == 0xE8 || op_code == 0xF8 ) {
				value = (char) line[1];
			}
			else {
				value = line[1];
			}
			
			sprintf(str_buff, OPCODE_STR[op_code], value);
			printf("%-30s", str_buff);
			printf("; $%04x:\t$%02x $%02x\n", addr, op_code, (unsigned char)line[1]);

			break;
		case 3:
			value = ((line[2]) << 8) + line[1];
			sprintf(str_buff, OPCODE_STR[op_code], value);
			printf("%-30s", str_buff);
			printf("; $%04x:\t$%02x $%02x $%02x\n", addr, op_code, line[1], line[2]);
			break;				
		default:
			printf("ERROR: unsupported instruction\n");
			exit(-1);
	}

}

