#ifndef CPU_H

#define CPU_H

#include <stdint.h>

typedef char* string;

extern char OPCODE_LEN[0x100];

extern string OPCODE_STR[0x100];

uint8_t *GB_MEMORY;

void gb_dump_cpu();

void gb_run();

void gb_init();

void gb_load_rom(char * game_path);

void gb_disassm_instr(uint16_t addr);
void gb_disassm_instr_n(uint16_t addr, uint16_t n);

short int gb_instr_value(unsigned short int addr);

typedef struct  
{
	unsigned char title[14];
	uint8_t color;
	uint16_t license;
	uint8_t device;
	uint8_t catridge_type;
	uint8_t rom_size;
	uint8_t ram_size;
	uint8_t dest;
	uint8_t license_code;
	uint8_t rom_ver;

} gb_rom_header;

typedef struct
{
	uint8_t a;
	uint8_t f;
	uint8_t b;
	uint8_t c;
	uint8_t d;
	uint8_t e;
	uint8_t h;
	uint8_t l;
	uint16_t sp;
	uint16_t pc;
} _cpu_registers;

typedef struct
{
	uint16_t af;
	uint16_t bc;
	uint16_t de;
	uint16_t hl;
	uint16_t sp;
	uint16_t pc;
} _cpu_registers16b;

typedef struct
{
	/* flags 1bit */
	uint8_t z; // zero
	uint8_t n; // substract
	uint8_t h; // half carry
	uint8_t c; // carry
} _cpu_flags;

typedef struct
{
	_cpu_registers r;
	_cpu_flags flags;

	uint8_t opcode;

	uint8_t clock_m; // machine cycles
	uint8_t clock_t; // clock cycles

    unsigned long cycles_machine;
    unsigned long cycles_clock;
} cpu;

extern cpu CPU;

void op_nop_00();
void op_ld_bc_d_01();
void op_ld_bc_a_02();
void op_inc_bc_03();
void op_inc_b_04();
void op_dec_b_05();
void op_ld_b_d_06();
void op_rlca_07();
void op_ld_x_sp_08();
void op_add_hl_bc_09();
void op_ld_a_bc_0a();
void op_dec_bc_0b();
void op_inc_c_0c();
void op_dec_c_0d();
void op_ld_c_x_0e();
void op_rrca_0f();

void op_stop_10();
void op_ld_de_x_11();
void op_ld_de_a_12();
void op_inc_de_13();
void op_inc_d_14();
void op_dec_d_15();
void op_ld_d_x_16();
void op_rla_17();
void op_jr_x_18();
void op_add_hl_de_19();
void op_ld_a_de_1a();
void op_dec_de_1b();
void op_inc_e_1c();
void op_dec_e_1d();
void op_ld_e_x_1e();
void op_rra_1f();

void op_jr_nz_x_20();
void op_ld_hl_x_21();
void op_ld_hl_a_22();
void op_inc_hl_23();
void op_inc_h_24();
void op_dec_h_25();
void op_ld_h_x_26();
void op_daa_27();
void op_jr_z_x_28();
void op_add_hl_hl_29();
void op_ld_a_hl_2a();
void op_dec_hl_2b();
void op_inc_l_2c();
void op_dec_l_2d();
void op_ld_l_x_2e();
void op_cpl_2f();

void op_jr_nc_30();
void op_ld_sp_x_31();
void op_ld_hl_a_32();
void op_inc_sp_33();
void op_inc_hl_34();
void op_dec_hl_35();
void op_ld_hl_x_36();
void op_scf_37();
void op_jr_c_x_38();
void op_add_hl_sp_39();
void op_ld_a_hl_3a();
void op_dec_sp_3b();
void op_inc_a_3c();
void op_dec_a_3d();
void op_ld_a_x_3e();
void op_ccf_3f();

void op_ld_b_b_40();
void op_ld_b_c_41();
void op_ld_b_d_42();
void op_ld_b_e_43();
void op_ld_b_h_44();
void op_ld_b_l_45();
void op_ld_b_hl_46();
void op_ld_b_a_47();
void op_ld_c_b_48();
void op_ld_c_c_49();
void op_ld_c_d_4a();
void op_ld_c_e_4b();
void op_ld_c_h_4c();
void op_ld_c_l_4d();
void op_ld_c_hl_4e();
void op_ld_c_a_4f();

void op_ld_d_b_50();
void op_ld_d_c_51();
void op_ld_d_d_52();
void op_ld_d_e_53();
void op_ld_b_h_54();
void op_ld_d_l_55();
void op_ld_d_hl_56();
void op_ld_d_a_57();
void op_ld_e_b_58();
void op_ld_e_c_59();
void op_ld_e_d_5a();
void op_ld_e_e_5b();
void op_ld_e_h_5c();
void op_ld_e_l_5d();
void op_ld_e_hl_5e();
void op_ld_e_a_5f();

void op_ld_h_b_60();
void op_ld_h_c_61();
void op_ld_h_d_62();
void op_ld_h_e_63();
void op_ld_h_h_64();
void op_ld_h_l_65();
void op_ld_h_hl_66();
void op_ld_h_a_67();
void op_ld_l_b_68();
void op_ld_l_c_69();
void op_ld_l_d_6a();
void op_ld_l_e_6b();
void op_ld_l_h_6c();
void op_ld_l_l_6d();
void op_ld_l_hl_6e();
void op_ld_l_a_6f();

void op_ld_hl_b_70();
void op_ld_hl_c_71();
void op_ld_hl_d_72();
void op_ld_hl_e_73();
void op_ld_hl_h_74();
void op_ld_hl_l_75();
void op_ld_hl_hl_76();
void op_ld_hl_a_77();
void op_ld_a_b_78();
void op_ld_a_c_79();
void op_ld_a_d_7a();
void op_ld_a_e_7b();
void op_ld_a_h_7c();
void op_ld_a_l_7d();
void op_ld_a_hl_7e();
void op_ld_a_a_7f();

void op_add_a_b_80();
void op_add_a_c_81();
void op_add_a_d_82();
void op_add_a_e_83();
void op_add_a_h_84();
void op_add_a_l_85();
void op_add_a_hl_86();
void op_add_a_a_87();
void op_adc_a_b_88();
void op_adc_a_c_89();
void op_adc_a_d_8a();
void op_adc_a_e_8b();
void op_adc_a_h_8c();
void op_adc_a_l_8d();
void op_adc_a_hl_8e();
void op_adc_a_a_8f();

void op_sub_b_90();
void op_sub_c_91();
void op_sub_d_92();
void op_sub_e_93();
void op_sub_h_94();
void op_sub_l_95();
void op_sub_hl_96();
void op_sub_a_97();
void op_sbc_a_b_98();
void op_sbc_a_c_99();
void op_sbc_a_d_9a();
void op_sbc_a_e_9b();
void op_sbc_a_h_9c();
void op_sbc_a_l_9d();
void op_sbc_a_hl_9e();
void op_sbc_a_a_9f();

void op_and_b_a0();
void op_and_c_a1();
void op_and_d_a2();
void op_and_e_a3();
void op_and_h_a4();
void op_and_l_a5();
void op_and_hl_a6();
void op_and_a_a7();
void op_xor_b_a8();
void op_xor_c_a9();
void op_xor_d_aa();
void op_xor_e_ab();
void op_xor_h_ac();
void op_xor_l_ad();
void op_xor_hl_ae();
void op_xor_a_af();

void op_or_b_b0();
void op_or_c_b1();
void op_or_d_b2();
void op_or_e_b3();
void op_or_h_b4();
void op_or_l_b5();
void op_or_hl_b6();
void op_or_a_b7();
void op_cp_b_b8();
void op_cp_c_b9();
void op_cp_d_ba();
void op_cp_e_bb();
void op_cp_h_bc();
void op_cp_l_bd();
void op_cp_hl_be();
void op_cp_a_bf();

void op_ret_nz_c0();
void op_pop_bc_c1();
void op_jp_nx_x_c2();
void op_jp_x_c3();
void op_call_nz_x_c4();
void op_push_bc_c5();
void op_add_a_x_c6();
void op_rst_00h_c7();
void op_ret_z_c8();
void op_ret_c9();
void op_jp_z_x_ca();
void op_cb_cb();
void op_call_z_x_cc();
void op_call_x_cd();
void op_adc_a_x_ce();
void op_rst_08h_cf();

void op_ret_nc_d0();
void op_pop_de_d1();
void op_jp_nc_x_d2();
void op_unknown_d3();
void op_call_nc_x_d4();
void op_push_de_d5();
void op_sub_x_d6();
void op_rst_10h_d7();
void op_ret_c_d8();
void op_reti_d9();
void op_jp_c_x_da();
void op_unknown_db();
void op_call_c_x_dc();
void op_unknown_dd();
void op_sbc_a_x_de();
void op_rst_18h_df();

void op_ldh_x_a_e0();
void op_pop_hl_e1();
void op_ld_c_a_e2();
void op_unknown_e3();
void op_unknown_e4();
void op_push_hl_e5();
void op_and_x_e6();
void op_rst_20h_e7();
void op_add_sp_x_e8();
void op_jp_hl_e9();
void op_ld_x_a_ea();
void op_unknown_eb();
void op_unknown_ec();
void op_unknown_ed();
void op_xor_x_ee();
void op_rst_28h_ef();

void op_ldh_a_x_f0();
void op_pop_af_f1();
void op_ld_a_c_f2();
void op_di_f3();
void op_unknown_f4();
void op_push_hf_f5();
void op_or_x_f6();
void op_rst_30h_f7();
void op_ld_hl_sp_x_f8();
void op_ld_sp_hl_f9();
void op_ld_a_x_fa();
void op_ei_fb();
void op_unknown_fc();
void op_unknown_fd();
void op_cp_x_fe();
void op_rst_38h_ff();


extern void (*GB_OPCODE_INSTR[])(void);

extern gb_rom_header *ROM_HEADER;

#endif
