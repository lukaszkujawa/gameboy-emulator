#include <stdio.h>
#include <stdlib.h>
#include "emulator.h"

string OPCODE_STR[0xFF] = {
	"nop", 		"lc bc, %d", 	"ld (bc), a", 	"inc bc", "inc b", 		"dec b", 	"ld b,%d", 		"rlca", 	"ld ($%04x),sp", 	"add hl,bc", 	"ld a,(bc)", 	"dec bc", 	"inc c", 	"dec c", 	"ld c, %d", 		"rrca",
	"stop 0", 	"ld de, %d", 	"ld (de), a", 	"inc de", "inc d", 		"dec d", 	"ld d,%d", 		"rla", 		"jr %d", 			"add hl, de", 	"ld a, (de)", 	"dec de", 	"inc e", 	"dec e", 	"ld e, %d", 	"rra",
	"jr nz, %d","ld hl, %d",	"ld (hl+), a",	"inc hl", "inc h", 		"dec h",	"ld h, %d", 	"daa",		"jr z, %d", 		"add hl, hl",	"ld a, (hl+)",	"dec hl", 	"inc l", 	"dec l", 	"ld l, %d",		"cpl",
	"jr nc, %d","ld sp, %d",	"ld (hl-), a",	"inc sp", "inc (hl)", 	"dec (hl)", "ld (hl), %d", 	"scf", 		"jr c, %d",			"add hl, sp",	"ld a, (hl-)",	"dec sp", 	"inc a", 	"dec a", 	"ld a, %d", 	"ccf",
	"ld b, b", 	"ld b, c",		"ld b, d", 		"ld b, e","ld b, h", 	"ld b, l", 	"ld b, (hl)", 	"ld b, a", 	"ld c, b", 			"ld c, c", 		"ld c, d", 		"ld c, e", 	"ld c, h", 	"ld c, l", 	"ld c, (hl)", 	"ld c, a",
	"ld d, b",	"ld d, c",		"ld d, d",		"ld d, e","ld d, h",	"ld d, l",	"ld d, (hl)",	"ld d, a",	"ld e, b",			"ld e, c",		"ld e, d",		"ld e, e",	"ld e, h",	"ld e, l", 	"ld e, (hl)",	"ld e, a",
	"ld h, b",	"ld h, c",		"ld h, d",		"ld h, e","ld h, h",	"ld h, l",	"ld h, (hl)",	"ld h, a",	"ld l, b",			"ld l, c",		"ld l, d",		"ld l, e",	"ld l, h",	"ld l, l", 	"ld l, (hl)",	"ld l, a",
	"ld (hl), b","ld (hl), c",		"ld (hl), d",		"ld (hl), e","ld (hl), h",	"ld (hl), l",	"halt",	"ld (hl), a",			"ld a, b",		"ld a, c",		"ld a, d",		"ld a, e",	"ld a, h",	"ld a, l", 	"ld a, (hl)",	"ld a, a",
	"add a, b", "add a, c",		"add a, d", "add a, e",	  "add a, h", 	"add a, l", "add a, (hl)", 	"add a, a", "adc a, b", 		"adc a, c", 	"adc a, d", 	"adc a, e", "adc a, h", "adc a, l", "adc a, (hl)", "adc a, a",
	"sub b", "sub c", "sub d", "sub e", "sub h", "sub l", "sub (hl)", "sub a", "sbc a, b", "sbc a, c",  "sbc a, d",  "sbc a, e",  "sbc a, h",  "sbc a, l",  "sbc a, (hl)",  "sbc a, a",
	"and b", "and c", "and d", "and e", "and h", "and l", "and (hl)", "and a", "xor b", "xor c", "xor d", "xor e", "xor h", "xor l", "xor (hl)", "xor a",
	"or b", "or c", "or d", "or e", "or h", "or l", "or (hl)", "or a", "cp b", "cp c", "cp d", "cp e", "cp h", "cp l", "cp (hl)", "cp a",
	"ret nz", "pop bc", "jp nz, $%04x", "jp $%04x", "call, nz, $%04x", "push bc", "add a, %d", "rst 00h", "ret z", "ret", "jp z, $%04x", "prefix cb", "call z, $%04x", "call $%04x", "adc a, %d", "rst 08h",
	"ret nc", "pop de", "jp nc, $%04x", "???", "call nc, $%04x", "push de", "sub %d", "rst 10h", "ret c", "reti", "jp c, $%04x", "???", "call c, $%04x", "???", "sbc a, %d", "rst 18h",
	"ldh ($%02x), a", "pop hl", "ld (c), a", "???", "???", "push hl", "and %d", "rst 20h", "add sp, $%02x", "jp (hl)", "ld ($%04x), a", "???", "???", "???", "xor %d", "rst 28h",
	"ldh a, ($%02x)", "pop af", "ld a, (c)", "di", "???", "push af", "or %d", "rst 30h", "ld hl, sp+$%02x", "ld sp, hl", "ld a, ($%04x)", "ei", "???", "???", "cp %d", "rst 38h"
};

char OPCODE_LEN[0xFF] = {
	1, 3, 1, 1, 1, 1, 2, 1, 3, 1, 1, 1, 1, 1, 2, 1,
	2, 3, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1,
	2, 3, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1,
	2, 3, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 3, 3, 3, 1, 2, 1, 1, 1, 3, 2, 3, 3, 2, 1,
	1, 1, 3, 0, 3, 1, 2, 1, 1, 1, 3, 0, 3, 0 ,2, 1,
	2, 1, 2, 0, 0, 1, 2, 1, 2, 1, 3, 0, 0 ,0, 2, 1,
	2, 1, 2, 1, 0, 1, 2, 1, 2, 1, 3, 1, 0, 0, 2, 1
};

void gb_init() {
	GB_MEMORY = malloc(0xFFFF);
}

void gb_load_rom(char * game_path) {
	long ROM_SIZE=0;

	printf("Loading: %s\n", game_path);

	FILE *f = fopen(game_path, "rb");
	if(f == 0) {
		printf("ERROR: Couldn't open file");
		exit(-1);
	}

	fseek(f, 0, SEEK_END);
	ROM_SIZE = ftell(f);
	printf("File size: %lu\n", ROM_SIZE);
	fseek(f, 0, SEEK_SET);
	fread(GB_MEMORY, ROM_SIZE, 1, f);
	fclose(f);
}

