#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"



int main(int argc, char *argv[]) {	
	if(argc != 2) {
		printf("Syntax:\n\temulator rom.gb\n");
		exit(-1);
	}

	gb_init();
	gb_load_rom(argv[1]);

	gb_run();

	return 0;
}