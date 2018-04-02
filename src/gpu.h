#ifndef GPU_H

#define GPU_H

typedef struct
{
	uint8_t mode;
	uint8_t line;
	uint16_t mode_clock;
} gpu;

extern gpu GPU;

void gpu_init();
void gpu_quit();
void gpu_step();

#endif