#include <stdio.h>
#include <stdlib.h>
#include "emulator.h"

int main(int argc, char *argv[]){
	if(argc != 2) {
		printf("Syntax:\n\temulator rom.gb\n");
		exit(-1);
	}

	gb_init();
	gb_load_rom(argv[1]);
	
	int i = 0x100;

	while(1) {
		unsigned char op_code = GB_MEMORY[i];
		unsigned char code_len = OPCODE_LEN[op_code];
		unsigned char *line =  &GB_MEMORY[i];
		short int value = 0;

		printf("\t");
		char str_buff[128];

		switch(code_len) {
			case 1:
				printf("%-30s", OPCODE_STR[op_code]);
				printf("; $%04x:\t$%02x\n", i, op_code);
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
				printf("; $%04x:\t$%02x $%02x\n", i, op_code, (unsigned char)line[1]);

				break;
			case 3:
				value = ((line[2]) << 8) + line[1];
				sprintf(str_buff, OPCODE_STR[op_code], value);
				printf("%-30s", str_buff);
				printf("; $%04x:\t$%02x $%02x $%02x\n", i, op_code, line[1], line[2]);
				break;				
			default:
				printf("ERROR: unsupported instruction\n");
				exit(-1);
		}

		//printf("0x%02hhX %d %d\n", ROM[i], op_code, code_len);
		//printf("%x\n", instr);

		switch(op_code) {
			case 0xC3:
				printf( "; --------------\n");
				i = value;
				break;
			default:
				i+=code_len;
		}
		
	}

	
	return 0;
}