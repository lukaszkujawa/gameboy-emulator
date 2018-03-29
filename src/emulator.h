#ifndef EMULATOR_HEADER

#define EMULATOR_HEADER

typedef char* string;

extern char OPCODE_LEN[0x100];

extern string OPCODE_STR[0x100];

unsigned char *GB_MEMORY;

void gb_init();

void gb_load_rom(char * game_path);


#endif
