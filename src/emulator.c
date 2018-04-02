#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#include "cpu.h"
#include "gpu.h"



int main(int argc, char *argv[]) {	
	gpu_init();

	if(argc != 2) {
		printf("Syntax:\n\temulator rom.gb\n");
		exit(-1);
	}

	gb_init();
	gb_load_rom(argv[1]);
	gb_run(1);

	gpu_quit();

	return 0;
}