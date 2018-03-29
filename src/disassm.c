#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

int main(int argc, char *argv[]){
	if(argc != 2) {
		printf("Syntax:\n\temulator rom.gb\n");
		exit(-1);
	}

	gb_init();
	gb_load_rom(argv[1]);

	printf("---------------\n");
	printf("Title: %s\n", ROM_HEADER->title);
	printf("Color: $%02x (%s)\n", ROM_HEADER->color, ROM_HEADER->color == 0x80 ? "Yes" : "No");
	printf("Device: $%02x (%s)\n", ROM_HEADER->device, ROM_HEADER->device == 0x00 ? "GameBoy": "Super GameBoy");
	printf("ROM size: $%02x (%d KByte)\n", ROM_HEADER->rom_size, 32 << ROM_HEADER->rom_size);
	printf("RAM size: $%02x (%d KByte)\n", ROM_HEADER->ram_size, 2 << ( 2 * ROM_HEADER->ram_size));
	printf("---------------\n");

	short int *jumps = malloc(1024 * sizeof(short int));
	short int jumps_index = 0;
	
	int i = 0;

	while(1) {
		unsigned char op_code = GB_MEMORY[i];
		unsigned char code_len = OPCODE_LEN[op_code];
		gb_disassm_instr(i);
		short int value = gb_instr_value(i);

		switch(op_code) {
			// JP
			case 0xC3:
				for(int i = 0 ; i < jumps_index ; i++) {
					if(jumps[i] == value) {
						printf(" ; going back to the same address $%02x\n", jumps[i] );
						exit(0);
					}
				}

				printf( "; --------------\n");
				jumps[jumps_index] = value;
				jumps_index++;
				i = value;
				break;
			// CALL
			case 0xCD:

			default:
				i+=code_len;
		}
		
	}

	
	return 0;
}