#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "cpu.h"
#include "time.h"
#include "debug.h"
#include "memory.h"

string OPCODE_STR[0x100] = {
	"nop", 		"ld bc, %d", 	"ld (bc), a", 	"inc bc", "inc b", 		"dec b", 	"ld b,%d", 		"rlca", 	"ld ($%04x),sp", 	"add hl,bc", 	"ld a,(bc)", 	"dec bc", 	"inc c", 	"dec c", 	"ld c, %d", 		"rrca",
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
	"ret nz", "pop bc", "jp nz, $%04x", "jp $%04x", "call nz, $%04x", "push bc", "add a, %d", "rst 00h", "ret z", "ret", "jp z, $%04x", "prefix cb", "call z, $%04x", "call $%04x", "adc a, %d", "rst 08h",
	"ret nc", "pop de", "jp nc, $%04x", "???", "call nc, $%04x", "push de", "sub %d", "rst 10h", "ret c", "reti", "jp c, $%04x", "???", "call c, $%04x", "???", "sbc a, %d", "rst 18h",
	"ldh ($%02x), a", "pop hl", "ld (c), a", "???", "???", "push hl", "and %d", "rst 20h", "add sp, $%02x", "jp (hl)", "ld ($%04x), a", "???", "???", "???", "xor %d", "rst 28h",
	"ldh a, ($%02x)", "pop af", "ld a, (c)", "di", "???", "push af", "or %d", "rst 30h", "ld hl, sp+$%02x", "ld sp, hl", "ld a, ($%04x)", "ei", "???", "???", "cp %d", "rst 38h"
};

char OPCODE_LEN[0x100] = {
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

gb_rom_header *ROM_HEADER;

cpu CPU;

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

	ROM_HEADER = (gb_rom_header*) &GB_MEMORY[0x134];
}

void gb_reset() {
	memset(&CPU, 0, sizeof(cpu));
}



void gb_run(int debug) {
	if(debug == 1) {
		debug_breaks[debug_break_i] = 0x0100;
		debug_break_i++;
		debug_pause = 1;
	}

	while(1) {
		CPU.opcode = GB_MEMORY[CPU.r.pc];
		if(debug == 1) {
			debug_debug();
		}
		
		GB_OPCODE_INSTR[CPU.opcode](); // run instruction
		
		CPU.cycles_clock += CPU.clock_t;
		

	}
}

short int gb_instr_value(unsigned short int addr) {
	unsigned char op_code = GB_MEMORY[addr];
	unsigned char code_len = OPCODE_LEN[op_code];
	unsigned char *line =  &GB_MEMORY[addr];
	short int value = 0;

	switch(code_len) {
		case 1:
			break;
		case 2:
			if(op_code == 0x18 || op_code == 0x20 || op_code == 0x28 ||  op_code == 0x30 || op_code == 0x38 || op_code == 0xE8 || op_code == 0xF8 ) {
				value = (char) line[1];
			}
			else {
				value = line[1];
			}
			break;
		case 3:
			value = ((line[2]) << 8) + line[1];
			break;				
		default:
			printf("ERROR: unsupported instruction\n");
			exit(-1);
	}

	return value;
}


void (*GB_OPCODE_INSTR[])(void) = {
	op_nop_00, op_ld_bc_d_01, op_ld_bc_a_02, op_inc_bc_03, op_inc_b_04, op_dec_b_05, op_ld_b_d_06, op_rlca_07, op_ld_x_sp_08, op_add_hl_bc_09, op_ld_a_bc_0a, op_dec_bc_0b, op_inc_c_0c, op_dec_c_0d, op_ld_c_x_0e, op_rrca_0f, 
	op_stop_10, op_ld_de_x_11, op_ld_de_a_12, op_inc_de_13, op_inc_d_14, op_dec_d_15, op_ld_d_x_16, op_rla_17, op_jr_x_18, op_add_hl_de_19, op_ld_a_de_1a, op_dec_de_1b, op_inc_e_1c, op_dec_e_1d, op_ld_e_x_1e, op_rra_1f, 
	op_jr_nz_x_20, op_ld_hl_x_21, op_ld_hl_a_22, op_inc_hl_23, op_inc_h_24, op_dec_h_25, op_ld_h_x_26, op_daa_27, op_jr_z_x_28, op_add_hl_hl_29, op_ld_a_hl_2a, op_dec_hl_2b, op_inc_l_2c, op_dec_l_2d, op_ld_l_x_2e, op_cpl_2f, 
	op_jr_nc_30, op_ld_sp_x_31, op_ld_hl_a_32, op_inc_sp_33, op_inc_hl_34, op_dec_hl_35, op_ld_hl_x_36, op_scf_37, op_jr_c_x_38, op_add_hl_sp_39, op_ld_a_hl_3a, op_dec_sp_3b, op_inc_a_3c, op_dec_a_3d, op_ld_a_x_3e, op_ccf_3f, 
	op_ld_b_b_40, op_ld_b_c_41, op_ld_b_d_42, op_ld_b_e_43, op_ld_b_h_44, op_ld_b_l_45, op_ld_b_hl_46, op_ld_b_a_47, op_ld_c_b_48, op_ld_c_c_49, op_ld_c_d_4a, op_ld_c_e_4b, op_ld_c_h_4c, op_ld_c_l_4d, op_ld_c_hl_4e, op_ld_c_a_4f, 
	op_ld_d_b_50, op_ld_d_c_51, op_ld_d_d_52, op_ld_d_e_53, op_ld_b_h_54, op_ld_d_l_55, op_ld_d_hl_56, op_ld_d_a_57, op_ld_e_b_58, op_ld_e_c_59, op_ld_e_d_5a, op_ld_e_e_5b, op_ld_e_h_5c, op_ld_e_l_5d, op_ld_e_hl_5e, op_ld_e_a_5f, 
	op_ld_h_b_60, op_ld_h_c_61, op_ld_h_d_62, op_ld_h_e_63, op_ld_h_h_64, op_ld_h_l_65, op_ld_h_hl_66, op_ld_h_a_67, op_ld_l_b_68, op_ld_l_c_69, op_ld_l_d_6a, op_ld_l_e_6b, op_ld_l_h_6c, op_ld_l_l_6d, op_ld_l_hl_6e, op_ld_l_a_6f, 
	op_ld_hl_b_70, op_ld_hl_c_71, op_ld_hl_d_72, op_ld_hl_e_73, op_ld_hl_h_74, op_ld_hl_l_75, op_ld_hl_hl_76, op_ld_hl_a_77, op_ld_a_b_78, op_ld_a_c_79, op_ld_a_d_7a, op_ld_a_e_7b, op_ld_a_h_7c, op_ld_a_l_7d, op_ld_a_hl_7e, op_ld_a_a_7f, 
	op_add_a_b_80, op_add_a_c_81, op_add_a_d_82, op_add_a_e_83, op_add_a_h_84, op_add_a_l_85, op_add_a_hl_86, op_add_a_a_87, op_adc_a_b_88, op_adc_a_c_89, op_adc_a_d_8a, op_adc_a_e_8b, op_adc_a_h_8c, op_adc_a_l_8d, op_adc_a_hl_8e, op_adc_a_a_8f, 
	op_sub_b_90, op_sub_c_91, op_sub_d_92, op_sub_e_93, op_sub_h_94, op_sub_l_95, op_sub_hl_96, op_sub_a_97, op_sbc_a_b_98, op_sbc_a_c_99, op_sbc_a_d_9a, op_sbc_a_e_9b, op_sbc_a_h_9c, op_sbc_a_l_9d, op_sbc_a_hl_9e, op_sbc_a_a_9f, 
	op_and_b_a0, op_and_c_a1, op_and_d_a2, op_and_e_a3, op_and_h_a4, op_and_l_a5, op_and_hl_a6, op_and_a_a7, op_xor_b_a8, op_xor_c_a9, op_xor_d_aa, op_xor_e_ab, op_xor_h_ac, op_xor_l_ad, op_xor_hl_ae, op_xor_a_af, 
	op_or_b_b0, op_or_c_b1, op_or_d_b2, op_or_e_b3, op_or_h_b4, op_or_l_b5, op_or_hl_b6, op_or_a_b7, op_cp_b_b8, op_cp_c_b9, op_cp_d_ba, op_cp_e_bb, op_cp_h_bc, op_cp_l_bd, op_cp_hl_be, op_cp_a_bf, 
	op_ret_nz_c0, op_pop_bc_c1, op_jp_nx_x_c2, op_jp_x_c3, op_call_nz_x_c4, op_push_bc_c5, op_add_a_x_c6, op_rst_00h_c7, op_ret_z_c8, op_ret_c9, op_jp_z_x_ca, op_cb_cb, op_call_z_x_cc, op_call_x_cd, op_adc_a_x_ce, op_rst_08h_cf, 
	op_ret_nc_d0, op_pop_de_d1, op_jp_nc_x_d2, op_unknown_d3, op_call_nc_x_d4, op_push_de_d5, op_sub_x_d6, op_rst_10h_d7, op_ret_c_d8, op_reti_d9, op_jp_c_x_da, op_unknown_db, op_call_c_x_dc, op_unknown_dd, op_sbc_a_x_de, op_rst_18h_df, 
	op_ldh_x_a_e0, op_pop_hl_e1, op_ld_c_a_e2, op_unknown_e3, op_unknown_e4, op_push_hl_e5, op_and_x_e6, op_rst_20h_e7, op_add_sp_x_e8, op_jp_hl_e9, op_ld_x_a_ea, op_unknown_eb, op_unknown_ec, op_unknown_ed, op_xor_x_ee, op_rst_28h_ef, 
	op_ldh_a_x_f0, op_pop_af_f1, op_ld_a_c_f2, op_di_f3, op_unknown_f4, op_push_hf_f5, op_or_x_f6, op_rst_30h_f7, op_ld_hl_sp_x_f8, op_ld_sp_hl_f9, op_ld_a_x_fa, op_ei_fb, op_unknown_fc, op_unknown_fd, op_cp_x_fe, op_rst_38h_ff
};

void op_nop_00(){
	CPU.clock_t = 4;
    CPU.r.pc += 1;
}

// -----------

void op_ld_bc_d_01(){

    CPU.r.b = memory_read_u8(CPU.r.pc+2);
    CPU.r.c = memory_read_u8(CPU.r.pc+1);
    CPU.clock_t = 12;
    CPU.r.pc += 3;
}
void op_ld_de_x_11(){
    CPU.r.d = memory_read_u8(CPU.r.pc+2);
    CPU.r.e = memory_read_u8(CPU.r.pc+1);
    CPU.clock_t = 12;
    CPU.r.pc += 3;
}
void op_ld_hl_x_21(){
    CPU.r.h = memory_read_u8(CPU.r.pc+2);
    CPU.r.l = memory_read_u8(CPU.r.pc+1);
    CPU.clock_t = 12;
    CPU.r.pc += 3;
}
void op_ld_sp_x_31(){
    CPU.r.sp = memory_read_u8(CPU.r.pc+1);
    CPU.r.sp |= memory_read_u8(CPU.r.pc+2) << 8;
    CPU.clock_t = 12;
    CPU.r.pc += 3;
}

// -----------

void op_ld_b_d_06(){
	CPU.r.b = memory_read_u8(CPU.r.pc+1);
    CPU.clock_t = 8;
    CPU.r.pc += 2;
}
void op_ld_c_x_0e(){
	CPU.r.c = memory_read_u8(CPU.r.pc+1);
    CPU.clock_t = 8;
    CPU.r.pc += 2;
}
void op_ld_d_x_16(){
	CPU.r.d = memory_read_u8(CPU.r.pc+1);
    CPU.clock_t = 8;
    CPU.r.pc += 2;
}
void op_ld_e_x_1e(){
	CPU.r.e = memory_read_u8(CPU.r.pc+1);
    CPU.clock_t = 8;
    CPU.r.pc += 2;
}
void op_ld_h_x_26(){
	CPU.r.h = memory_read_u8(CPU.r.pc+1);
    CPU.clock_t = 8;
    CPU.r.pc += 2;
}
void op_ld_l_x_2e(){
	CPU.r.l = memory_read_u8(CPU.r.pc+1);
    CPU.clock_t = 8;
    CPU.r.pc += 2;
}

// MOV A, nn -----------

void op_ld_a_a_7f(){}
void op_ld_a_x_3e(){}
void op_ld_a_b_78(){}
void op_ld_a_c_79(){}
void op_ld_a_d_7a(){}
void op_ld_a_e_7b(){}
void op_ld_a_h_7c(){}
void op_ld_a_l_7d(){}
void op_ld_a_hl_7e(){}
void op_ld_a_bc_0a(){}
void op_ld_a_de_1a(){}
void op_ld_a_x_fa(){}

// -----------

void op_ld_hl_a_32(){
	
	memory_write_u8(((_cpu_registers16b*)(&CPU.r))->hl, CPU.r.a);

	((_cpu_registers16b*)(&CPU.r))->hl--;
	CPU.clock_t = 8;
	CPU.r.pc += 1;
}

// -----------

void op_dec_a_3d(){
	CPU.r.a = (CPU.r.a - 1) & 0xff;
	CPU.flags.h  = ((CPU.r.a & 0x0f) == 0x0f);
	CPU.flags.z  = CPU.r.a == 0;
    CPU.flags.n  = 1;
	CPU.clock_t = 4;
	CPU.r.pc += 1;
}
void op_dec_b_05(){
	CPU.r.b = (CPU.r.b - 1) & 0xff;
	CPU.flags.h  = ((CPU.r.b & 0x0f) == 0x0f);
	CPU.flags.z  = CPU.r.b == 0;
    CPU.flags.n  = 1;
	CPU.clock_t = 4;
	CPU.r.pc += 1;
}
void op_dec_c_0d(){
	CPU.r.c = (CPU.r.c - 1) & 0xff;
	CPU.flags.h  = ((CPU.r.c & 0x0f) == 0x0f);
	CPU.flags.z  = CPU.r.c == 0;
    CPU.flags.n  = 1;
	CPU.clock_t = 4;
	CPU.r.pc += 1;
}
void op_dec_d_15(){
	CPU.r.d = (CPU.r.d - 1) & 0xff;
	CPU.flags.h  = ((CPU.r.d & 0x0f) == 0x0f);
	CPU.flags.z  = CPU.r.d == 0;
    CPU.flags.n  = 1;
	CPU.clock_t = 4;
	CPU.r.pc += 1;
}
void op_dec_e_1d(){
	CPU.r.e = (CPU.r.e - 1) & 0xff;
	CPU.flags.h  = ((CPU.r.e & 0x0f) == 0x0f);
	CPU.flags.z  = CPU.r.e == 0;
    CPU.flags.n  = 1;
	CPU.clock_t = 4;
	CPU.r.pc += 1;
}
void op_dec_h_25(){
	CPU.r.h = (CPU.r.h - 1) & 0xff;
	CPU.flags.h  = ((CPU.r.h & 0x0f) == 0x0f);
	CPU.flags.z  = CPU.r.h == 0;
    CPU.flags.n  = 1;
	CPU.clock_t = 4;
	CPU.r.pc += 1;
}
void op_dec_hl_35(){
	uint16_t addr = ((_cpu_registers16b*)(&CPU.r))->hl;
	uint8_t cell = memory_read_u8(addr);

	cell = (cell - 1) & 0xff;
	memory_write_u8(addr, cell);
	
	CPU.flags.h  = ((cell & 0x0f) == 0x0f);
	CPU.flags.z  = cell == 0;
    CPU.flags.n  = 1;
	CPU.clock_t = 4;
	CPU.r.pc += 1;
}

// -----------
void op_dec_bc_0b(){}
void op_dec_de_1b(){}
void op_dec_hl_2b(){}
void op_dec_sp_3b(){}
// -----------


void op_jr_nz_x_20(){
	if(CPU.flags.z == 0) {
		CPU.r.pc +=  2 + memory_read_s8(CPU.r.pc+1);
	}
	else {
		CPU.r.pc += 2;
	}

	CPU.clock_t = 8;
}
void op_jr_z_x_28(){
	if(CPU.flags.z == 1) {
		CPU.r.pc += 2 + memory_read_u8(CPU.r.pc+1);
	}
	else {
		CPU.r.pc += 2;
	}
	
	CPU.clock_t = 8;
}
void op_jr_nc_30(){
	if(CPU.flags.c == 0) {
		CPU.r.pc += 2+ memory_read_u8(CPU.r.pc+1);
	}
	else {
		CPU.r.pc += 2;
	}
	
	CPU.clock_t = 8;
}
void op_jr_c_x_38(){
	if(CPU.flags.c == 1) {
		CPU.r.pc += 2 + memory_read_u8(CPU.r.pc+1);
	}
	else {
		CPU.r.pc += 2;
	}
	
	CPU.clock_t = 8;
}

// -----------

void op_jr_x_18(){}

void op_ld_bc_a_02(){}

void op_ld_x_sp_08(){}


void op_ld_de_a_12(){}



void op_ld_hl_a_22(){}


void op_inc_bc_03(){}
void op_inc_b_04(){}
void op_inc_c_0c(){}
void op_inc_de_13(){}
void op_inc_d_14(){}
void op_inc_e_1c(){}
void op_inc_hl_23(){}
void op_inc_h_24(){}



void op_rlca_07(){}
void op_rrca_0f(){}
void op_rla_17(){}
void op_rra_1f(){}

void op_add_hl_bc_09(){}

void op_stop_10(){}




void op_add_hl_de_19(){}






void op_daa_27(){}


void op_add_hl_hl_29(){}
void op_ld_a_hl_2a(){}

void op_inc_l_2c(){}
void op_dec_l_2d(){}
void op_cpl_2f(){}



void op_inc_sp_33(){}
void op_inc_hl_34(){}

void op_ld_hl_x_36(){}
void op_scf_37(){}

void op_add_hl_sp_39(){}
void op_ld_a_hl_3a(){}
void op_inc_a_3c(){}

void op_ccf_3f(){}

void op_ld_b_b_40(){}
void op_ld_b_c_41(){}
void op_ld_b_d_42(){}
void op_ld_b_e_43(){}
void op_ld_b_h_44(){}
void op_ld_b_l_45(){}
void op_ld_b_hl_46(){}
void op_ld_b_a_47(){}
void op_ld_c_b_48(){}
void op_ld_c_c_49(){}
void op_ld_c_d_4a(){}
void op_ld_c_e_4b(){}
void op_ld_c_h_4c(){}
void op_ld_c_l_4d(){}
void op_ld_c_hl_4e(){}
void op_ld_c_a_4f(){}

void op_ld_d_b_50(){}
void op_ld_d_c_51(){}
void op_ld_d_d_52(){}
void op_ld_d_e_53(){}
void op_ld_b_h_54(){}
void op_ld_d_l_55(){}
void op_ld_d_hl_56(){}
void op_ld_d_a_57(){}
void op_ld_e_b_58(){}
void op_ld_e_c_59(){}
void op_ld_e_d_5a(){}
void op_ld_e_e_5b(){}
void op_ld_e_h_5c(){}
void op_ld_e_l_5d(){}
void op_ld_e_hl_5e(){}
void op_ld_e_a_5f(){}

void op_ld_h_b_60(){}
void op_ld_h_c_61(){}
void op_ld_h_d_62(){}
void op_ld_h_e_63(){}
void op_ld_h_h_64(){}
void op_ld_h_l_65(){}
void op_ld_h_hl_66(){}
void op_ld_h_a_67(){}
void op_ld_l_b_68(){}
void op_ld_l_c_69(){}
void op_ld_l_d_6a(){}
void op_ld_l_e_6b(){}
void op_ld_l_h_6c(){}
void op_ld_l_l_6d(){}
void op_ld_l_hl_6e(){}
void op_ld_l_a_6f(){}

void op_ld_hl_b_70(){}
void op_ld_hl_c_71(){}
void op_ld_hl_d_72(){}
void op_ld_hl_e_73(){}
void op_ld_hl_h_74(){}
void op_ld_hl_l_75(){}
void op_ld_hl_hl_76(){}
void op_ld_hl_a_77(){}


void op_add_a_b_80(){}
void op_add_a_c_81(){}
void op_add_a_d_82(){}
void op_add_a_e_83(){}
void op_add_a_h_84(){}
void op_add_a_l_85(){}
void op_add_a_hl_86(){}
void op_add_a_a_87(){}
void op_adc_a_b_88(){}
void op_adc_a_c_89(){}
void op_adc_a_d_8a(){}
void op_adc_a_e_8b(){}
void op_adc_a_h_8c(){}
void op_adc_a_l_8d(){}
void op_adc_a_hl_8e(){}
void op_adc_a_a_8f(){}

void op_sub_b_90(){}
void op_sub_c_91(){}
void op_sub_d_92(){}
void op_sub_e_93(){}
void op_sub_h_94(){}
void op_sub_l_95(){}
void op_sub_hl_96(){}
void op_sub_a_97(){}
void op_sbc_a_b_98(){}
void op_sbc_a_c_99(){}
void op_sbc_a_d_9a(){}
void op_sbc_a_e_9b(){}
void op_sbc_a_h_9c(){}
void op_sbc_a_l_9d(){}
void op_sbc_a_hl_9e(){}
void op_sbc_a_a_9f(){}

void op_and_b_a0(){}
void op_and_c_a1(){}
void op_and_d_a2(){}
void op_and_e_a3(){}
void op_and_h_a4(){}
void op_and_l_a5(){}
void op_and_hl_a6(){}
void op_and_a_a7(){}
void op_xor_b_a8(){}
void op_xor_c_a9(){}
void op_xor_d_aa(){}
void op_xor_e_ab(){}
void op_xor_h_ac(){}
void op_xor_l_ad(){}
void op_xor_hl_ae(){}
void op_xor_a_af(){
	CPU.clock_t = 4;
	CPU.r.a ^= CPU.r.a;
	
	CPU.flags.c = 0; CPU.flags.h = 0; CPU.flags.n = 0;
    CPU.flags.z = (CPU.r.a == 0);
    CPU.r.pc++;
}

void op_or_b_b0(){}
void op_or_c_b1(){}
void op_or_d_b2(){}
void op_or_e_b3(){}
void op_or_h_b4(){}
void op_or_l_b5(){}
void op_or_hl_b6(){}
void op_or_a_b7(){}
void op_cp_b_b8(){}
void op_cp_c_b9(){}
void op_cp_d_ba(){}
void op_cp_e_bb(){}
void op_cp_h_bc(){}
void op_cp_l_bd(){}
void op_cp_hl_be(){}
void op_cp_a_bf(){}

void op_ret_nz_c0(){}
void op_pop_bc_c1(){}
void op_jp_nx_x_c2(){}
void op_jp_x_c3(){
	uint16_t addr = 0;
    addr  = memory_read_u8(CPU.r.pc+1);
    addr |= memory_read_u8(CPU.r.pc+2) << 8;
    CPU.clock_t = 8;
    CPU.r.pc = addr;
}
void op_call_nz_x_c4(){}
void op_push_bc_c5(){}
void op_add_a_x_c6(){}
void op_rst_00h_c7(){}
void op_ret_z_c8(){}
void op_ret_c9(){}
void op_jp_z_x_ca(){}
void op_cb_cb(){}
void op_call_z_x_cc(){}
void op_call_x_cd(){}
void op_adc_a_x_ce(){}
void op_rst_08h_cf(){}

void op_ret_nc_d0(){}
void op_pop_de_d1(){}
void op_jp_nc_x_d2(){}
void op_unknown_d3(){}
void op_call_nc_x_d4(){}
void op_push_de_d5(){}
void op_sub_x_d6(){}
void op_rst_10h_d7(){}
void op_ret_c_d8(){}
void op_reti_d9(){}
void op_jp_c_x_da(){}
void op_unknown_db(){}
void op_call_c_x_dc(){}
void op_unknown_dd(){}
void op_sbc_a_x_de(){}
void op_rst_18h_df(){}

void op_ldh_x_a_e0(){}
void op_pop_hl_e1(){}
void op_ld_c_a_e2(){}
void op_unknown_e3(){}
void op_unknown_e4(){}
void op_push_hl_e5(){}
void op_and_x_e6(){}
void op_rst_20h_e7(){}
void op_add_sp_x_e8(){}
void op_jp_hl_e9(){}
void op_ld_x_a_ea(){}
void op_unknown_eb(){}
void op_unknown_ec(){}
void op_unknown_ed(){}
void op_xor_x_ee(){}
void op_rst_28h_ef(){}

void op_ldh_a_x_f0(){}
void op_pop_af_f1(){}
void op_ld_a_c_f2(){}
void op_di_f3(){}
void op_unknown_f4(){}
void op_push_hf_f5(){}
void op_or_x_f6(){}
void op_rst_30h_f7(){}
void op_ld_hl_sp_x_f8(){}
void op_ld_sp_hl_f9(){}

void op_ei_fb(){}
void op_unknown_fc(){}
void op_unknown_fd(){}
void op_cp_x_fe(){}
void op_rst_38h_ff(){}




