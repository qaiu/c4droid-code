#include "nes_main.h"

// 6502标志位 flags = NVRBDIZC
#define	C_FLAG		0x01		// 1: Carry
#define	Z_FLAG		0x02		// 1: Zero
#define	I_FLAG		0x04		// 1: Irq disabled
#define	D_FLAG		0x08		// 1: Decimal mode flag (NES unused)
#define	B_FLAG		0x10		// 1: Break
#define	R_FLAG		0x20		// 1: Reserved (Always 1)
#define	V_FLAG		0x40		// 1: Overflow
#define	N_FLAG		0x80		// 1: Negative

/* flags = NVRBDIZC */
BYTE a_reg, x_reg, y_reg, flag_reg, s_reg;
WORD pc_reg;

/* 中断标志 */
CPU_InitFlag NMI_Flag;
CPU_InitFlag IRQ_Flag;

/* Macros for convenience */
#define A a_reg
#define X x_reg
#define Y y_reg
#define P flag_reg
#define S s_reg
#define PC pc_reg

/* internal registers */
BYTE opcode;
int32 clockticks6502;

/* help variables */
WORD savepc;
BYTE value;
int32 sum, saveflags;
WORD help;

/* 6502 memory map */
uint8 ram6502[0x800];			/* RAM */
uint8 *exp_rom;					/* expansion rom */
uint8 *sram;					/* sram */
uint8 *prg_rombank0;			/* prg-rom lower bank */
uint8 *prg_rombank1;			/* prg-rom upper bank */

typedef struct
{
	unsigned short int ticks;
	void (*instruction) (void);
	void (*adrmode) (void);
} OPCODE;

OPCODE opcodetable[256];
void startopcodetable(void)
{
	opcodetable[0].ticks = 7;
	opcodetable[0].instruction = brk6502;
	opcodetable[0].adrmode = implied6502;
	opcodetable[1].ticks = 6;
	opcodetable[1].instruction = ora6502;
	opcodetable[1].adrmode = indx6502;
	opcodetable[2].ticks = 2;
	opcodetable[2].instruction = nop6502;
	opcodetable[2].adrmode = implied6502;
	opcodetable[3].ticks = 2;
	opcodetable[3].instruction = nop6502;
	opcodetable[3].adrmode = implied6502;
	opcodetable[4].ticks = 3;
	opcodetable[4].instruction = tsb6502;
	opcodetable[4].adrmode = zp6502;
	opcodetable[5].ticks = 3;
	opcodetable[5].instruction = ora6502;
	opcodetable[5].adrmode = zp6502;
	opcodetable[6].ticks = 5;
	opcodetable[6].instruction = asl6502;
	opcodetable[6].adrmode = zp6502;
	opcodetable[7].ticks = 2;
	opcodetable[7].instruction = nop6502;
	opcodetable[7].adrmode = implied6502;
	opcodetable[8].ticks = 3;
	opcodetable[8].instruction = php6502;
	opcodetable[8].adrmode = implied6502;
	opcodetable[9].ticks = 3;
	opcodetable[9].instruction = ora6502;
	opcodetable[9].adrmode = immediate6502;
	opcodetable[10].ticks = 2;
	opcodetable[10].instruction = asla6502;
	opcodetable[10].adrmode = implied6502;
	opcodetable[11].ticks = 2;
	opcodetable[11].instruction = ora6502;
	opcodetable[11].adrmode = implied6502;
	opcodetable[12].ticks = 4;
	opcodetable[12].instruction = tsb6502;
	opcodetable[12].adrmode = abs6502;
	opcodetable[13].ticks = 4;
	opcodetable[13].instruction = ora6502;
	opcodetable[13].adrmode = abs6502;
	opcodetable[14].ticks = 6;
	opcodetable[14].instruction = asl6502;
	opcodetable[14].adrmode = abs6502;
	opcodetable[15].ticks = 2;
	opcodetable[15].instruction = nop6502;
	opcodetable[15].adrmode = implied6502;
	opcodetable[16].ticks = 2;
	opcodetable[16].instruction = bpl6502;
	opcodetable[16].adrmode = relative6502;
	opcodetable[17].ticks = 5;
	opcodetable[17].instruction = ora6502;
	opcodetable[17].adrmode = indy6502;
	opcodetable[18].ticks = 3;
	opcodetable[18].instruction = ora6502;
	opcodetable[18].adrmode = indzp6502;
	opcodetable[19].ticks = 2;
	opcodetable[19].instruction = nop6502;
	opcodetable[19].adrmode = implied6502;
	opcodetable[20].ticks = 3;
	opcodetable[20].instruction = trb6502;
	opcodetable[20].adrmode = zp6502;
	opcodetable[21].ticks = 4;
	opcodetable[21].instruction = ora6502;
	opcodetable[21].adrmode = zpx6502;
	opcodetable[22].ticks = 6;
	opcodetable[22].instruction = asl6502;
	opcodetable[22].adrmode = zpx6502;
	opcodetable[23].ticks = 2;
	opcodetable[23].instruction = nop6502;
	opcodetable[23].adrmode = implied6502;
	opcodetable[24].ticks = 2;
	opcodetable[24].instruction = clc6502;
	opcodetable[24].adrmode = implied6502;
	opcodetable[25].ticks = 4;
	opcodetable[25].instruction = ora6502;
	opcodetable[25].adrmode = absy6502;
	opcodetable[26].ticks = 2;
	opcodetable[26].instruction = ina6502;
	opcodetable[26].adrmode = implied6502;
	opcodetable[27].ticks = 2;
	opcodetable[27].instruction = nop6502;
	opcodetable[27].adrmode = implied6502;
	opcodetable[28].ticks = 4;
	opcodetable[28].instruction = trb6502;
	opcodetable[28].adrmode = abs6502;
	opcodetable[29].ticks = 4;
	opcodetable[29].instruction = ora6502;
	opcodetable[29].adrmode = absx6502;
	opcodetable[30].ticks = 7;
	opcodetable[30].instruction = asl6502;
	opcodetable[30].adrmode = absx6502;
	opcodetable[31].ticks = 2;
	opcodetable[31].instruction = nop6502;
	opcodetable[31].adrmode = implied6502;
	opcodetable[32].ticks = 6;
	opcodetable[32].instruction = jsr6502;
	opcodetable[32].adrmode = abs6502;
	opcodetable[33].ticks = 6;
	opcodetable[33].instruction = and6502;
	opcodetable[33].adrmode = indx6502;
	opcodetable[34].ticks = 2;
	opcodetable[34].instruction = nop6502;
	opcodetable[34].adrmode = implied6502;
	opcodetable[35].ticks = 2;
	opcodetable[35].instruction = nop6502;
	opcodetable[35].adrmode = implied6502;
	opcodetable[36].ticks = 3;
	opcodetable[36].instruction = bit6502;
	opcodetable[36].adrmode = zp6502;
	opcodetable[37].ticks = 3;
	opcodetable[37].instruction = and6502;
	opcodetable[37].adrmode = zp6502;
	opcodetable[38].ticks = 5;
	opcodetable[38].instruction = rol6502;
	opcodetable[38].adrmode = zp6502;
	opcodetable[39].ticks = 2;
	opcodetable[39].instruction = nop6502;
	opcodetable[39].adrmode = implied6502;
	opcodetable[40].ticks = 4;
	opcodetable[40].instruction = plp6502;
	opcodetable[40].adrmode = implied6502;
	opcodetable[41].ticks = 3;
	opcodetable[41].instruction = and6502;
	opcodetable[41].adrmode = immediate6502;
	opcodetable[42].ticks = 2;
	opcodetable[42].instruction = rola6502;
	opcodetable[42].adrmode = implied6502;
	opcodetable[43].ticks = 2;
	opcodetable[43].instruction = nop6502;
	opcodetable[43].adrmode = implied6502;
	opcodetable[44].ticks = 4;
	opcodetable[44].instruction = bit6502;
	opcodetable[44].adrmode = abs6502;
	opcodetable[45].ticks = 4;
	opcodetable[45].instruction = and6502;
	opcodetable[45].adrmode = abs6502;
	opcodetable[46].ticks = 6;
	opcodetable[46].instruction = rol6502;
	opcodetable[46].adrmode = abs6502;
	opcodetable[47].ticks = 2;
	opcodetable[47].instruction = nop6502;
	opcodetable[47].adrmode = implied6502;
	opcodetable[48].ticks = 2;
	opcodetable[48].instruction = bmi6502;
	opcodetable[48].adrmode = relative6502;
	opcodetable[49].ticks = 5;
	opcodetable[49].instruction = and6502;
	opcodetable[49].adrmode = indy6502;
	opcodetable[50].ticks = 3;
	opcodetable[50].instruction = and6502;
	opcodetable[50].adrmode = indzp6502;
	opcodetable[51].ticks = 2;
	opcodetable[51].instruction = nop6502;
	opcodetable[51].adrmode = implied6502;
	opcodetable[52].ticks = 4;
	opcodetable[52].instruction = bit6502;
	opcodetable[52].adrmode = zpx6502;
	opcodetable[53].ticks = 4;
	opcodetable[53].instruction = and6502;
	opcodetable[53].adrmode = zpx6502;
	opcodetable[54].ticks = 6;
	opcodetable[54].instruction = rol6502;
	opcodetable[54].adrmode = zpx6502;
	opcodetable[55].ticks = 2;
	opcodetable[55].instruction = nop6502;
	opcodetable[55].adrmode = implied6502;
	opcodetable[56].ticks = 2;
	opcodetable[56].instruction = sec6502;
	opcodetable[56].adrmode = implied6502;
	opcodetable[57].ticks = 4;
	opcodetable[57].instruction = and6502;
	opcodetable[57].adrmode = absy6502;
	opcodetable[58].ticks = 2;
	opcodetable[58].instruction = dea6502;
	opcodetable[58].adrmode = implied6502;
	opcodetable[59].ticks = 2;
	opcodetable[59].instruction = nop6502;
	opcodetable[59].adrmode = implied6502;
	opcodetable[60].ticks = 4;
	opcodetable[60].instruction = bit6502;
	opcodetable[60].adrmode = absx6502;
	opcodetable[61].ticks = 4;
	opcodetable[61].instruction = and6502;
	opcodetable[61].adrmode = absx6502;
	opcodetable[62].ticks = 7;
	opcodetable[62].instruction = rol6502;
	opcodetable[62].adrmode = absx6502;
	opcodetable[63].ticks = 2;
	opcodetable[63].instruction = nop6502;
	opcodetable[63].adrmode = implied6502;
	opcodetable[64].ticks = 6;
	opcodetable[64].instruction = rti6502;
	opcodetable[64].adrmode = implied6502;
	opcodetable[65].ticks = 6;
	opcodetable[65].instruction = eor6502;
	opcodetable[65].adrmode = indx6502;
	opcodetable[66].ticks = 2;
	opcodetable[66].instruction = nop6502;
	opcodetable[66].adrmode = implied6502;
	opcodetable[67].ticks = 2;
	opcodetable[67].instruction = nop6502;
	opcodetable[67].adrmode = implied6502;
	opcodetable[68].ticks = 2;
	opcodetable[68].instruction = nop6502;
	opcodetable[68].adrmode = implied6502;
	opcodetable[69].ticks = 3;
	opcodetable[69].instruction = eor6502;
	opcodetable[69].adrmode = zp6502;
	opcodetable[70].ticks = 5;
	opcodetable[70].instruction = lsr6502;
	opcodetable[70].adrmode = zp6502;
	opcodetable[71].ticks = 2;
	opcodetable[71].instruction = nop6502;
	opcodetable[71].adrmode = implied6502;
	opcodetable[72].ticks = 3;
	opcodetable[72].instruction = pha6502;
	opcodetable[72].adrmode = implied6502;
	opcodetable[73].ticks = 3;
	opcodetable[73].instruction = eor6502;
	opcodetable[73].adrmode = immediate6502;
	opcodetable[74].ticks = 2;
	opcodetable[74].instruction = lsra6502;
	opcodetable[74].adrmode = implied6502;
	opcodetable[75].ticks = 2;
	opcodetable[75].instruction = nop6502;
	opcodetable[75].adrmode = implied6502;
	opcodetable[76].ticks = 3;
	opcodetable[76].instruction = jmp6502;
	opcodetable[76].adrmode = abs6502;
	opcodetable[77].ticks = 4;
	opcodetable[77].instruction = eor6502;
	opcodetable[77].adrmode = abs6502;
	opcodetable[78].ticks = 6;
	opcodetable[78].instruction = lsr6502;
	opcodetable[78].adrmode = abs6502;
	opcodetable[79].ticks = 2;
	opcodetable[79].instruction = nop6502;
	opcodetable[79].adrmode = implied6502;
	opcodetable[80].ticks = 2;
	opcodetable[80].instruction = bvc6502;
	opcodetable[80].adrmode = relative6502;
	opcodetable[81].ticks = 5;
	opcodetable[81].instruction = eor6502;
	opcodetable[81].adrmode = indy6502;
	opcodetable[82].ticks = 3;
	opcodetable[82].instruction = eor6502;
	opcodetable[82].adrmode = indzp6502;
	opcodetable[83].ticks = 2;
	opcodetable[83].instruction = nop6502;
	opcodetable[83].adrmode = implied6502;
	opcodetable[84].ticks = 2;
	opcodetable[84].instruction = nop6502;
	opcodetable[84].adrmode = implied6502;
	opcodetable[85].ticks = 4;
	opcodetable[85].instruction = eor6502;
	opcodetable[85].adrmode = zpx6502;
	opcodetable[86].ticks = 6;
	opcodetable[86].instruction = lsr6502;
	opcodetable[86].adrmode = zpx6502;
	opcodetable[87].ticks = 2;
	opcodetable[87].instruction = nop6502;
	opcodetable[87].adrmode = implied6502;
	opcodetable[88].ticks = 2;
	opcodetable[88].instruction = cli6502;
	opcodetable[88].adrmode = implied6502;
	opcodetable[89].ticks = 4;
	opcodetable[89].instruction = eor6502;
	opcodetable[89].adrmode = absy6502;
	opcodetable[90].ticks = 3;
	opcodetable[90].instruction = phy6502;
	opcodetable[90].adrmode = implied6502;
	opcodetable[91].ticks = 2;
	opcodetable[91].instruction = nop6502;
	opcodetable[91].adrmode = implied6502;
	opcodetable[92].ticks = 2;
	opcodetable[92].instruction = nop6502;
	opcodetable[92].adrmode = implied6502;
	opcodetable[93].ticks = 4;
	opcodetable[93].instruction = eor6502;
	opcodetable[93].adrmode = absx6502;
	opcodetable[94].ticks = 7;
	opcodetable[94].instruction = lsr6502;
	opcodetable[94].adrmode = absx6502;
	opcodetable[95].ticks = 2;
	opcodetable[95].instruction = nop6502;
	opcodetable[95].adrmode = implied6502;
	opcodetable[96].ticks = 6;
	opcodetable[96].instruction = rts6502;
	opcodetable[96].adrmode = implied6502;
	opcodetable[97].ticks = 6;
	opcodetable[97].instruction = adc6502;
	opcodetable[97].adrmode = indx6502;
	opcodetable[98].ticks = 2;
	opcodetable[98].instruction = nop6502;
	opcodetable[98].adrmode = implied6502;
	opcodetable[99].ticks = 2;
	opcodetable[99].instruction = nop6502;
	opcodetable[99].adrmode = implied6502;
	opcodetable[100].ticks = 3;
	opcodetable[100].instruction = stz6502;
	opcodetable[100].adrmode = zp6502;
	opcodetable[101].ticks = 3;
	opcodetable[101].instruction = adc6502;
	opcodetable[101].adrmode = zp6502;
	opcodetable[102].ticks = 5;
	opcodetable[102].instruction = ror6502;
	opcodetable[102].adrmode = zp6502;
	opcodetable[103].ticks = 2;
	opcodetable[103].instruction = nop6502;
	opcodetable[103].adrmode = implied6502;
	opcodetable[104].ticks = 4;
	opcodetable[104].instruction = pla6502;
	opcodetable[104].adrmode = implied6502;
	opcodetable[105].ticks = 3;
	opcodetable[105].instruction = adc6502;
	opcodetable[105].adrmode = immediate6502;
	opcodetable[106].ticks = 2;
	opcodetable[106].instruction = rora6502;
	opcodetable[106].adrmode = implied6502;
	opcodetable[107].ticks = 2;
	opcodetable[107].instruction = nop6502;
	opcodetable[107].adrmode = implied6502;
	opcodetable[108].ticks = 5;
	opcodetable[108].instruction = jmp6502;
	opcodetable[108].adrmode = indirect6502;
	opcodetable[109].ticks = 4;
	opcodetable[109].instruction = adc6502;
	opcodetable[109].adrmode = abs6502;
	opcodetable[110].ticks = 6;
	opcodetable[110].instruction = ror6502;
	opcodetable[110].adrmode = abs6502;
	opcodetable[111].ticks = 2;
	opcodetable[111].instruction = nop6502;
	opcodetable[111].adrmode = implied6502;
	opcodetable[112].ticks = 2;
	opcodetable[112].instruction = bvs6502;
	opcodetable[112].adrmode = implied6502;
	opcodetable[113].ticks = 5;
	opcodetable[113].instruction = adc6502;
	opcodetable[113].adrmode = indy6502;
	opcodetable[114].ticks = 3;
	opcodetable[114].instruction = adc6502;
	opcodetable[114].adrmode = indzp6502;
	opcodetable[115].ticks = 2;
	opcodetable[115].instruction = nop6502;
	opcodetable[115].adrmode = implied6502;
	opcodetable[116].ticks = 4;
	opcodetable[116].instruction = stz6502;
	opcodetable[116].adrmode = zpx6502;
	opcodetable[117].ticks = 4;
	opcodetable[117].instruction = adc6502;
	opcodetable[117].adrmode = zpx6502;
	opcodetable[118].ticks = 6;
	opcodetable[118].instruction = ror6502;
	opcodetable[118].adrmode = zpx6502;
	opcodetable[119].ticks = 2;
	opcodetable[119].instruction = nop6502;
	opcodetable[119].adrmode = implied6502;
	opcodetable[120].ticks = 2;
	opcodetable[120].instruction = sei6502;
	opcodetable[120].adrmode = implied6502;
	opcodetable[121].ticks = 4;
	opcodetable[121].instruction = adc6502;
	opcodetable[121].adrmode = absy6502;
	opcodetable[122].ticks = 4;
	opcodetable[122].instruction = ply6502;
	opcodetable[122].adrmode = implied6502;
	opcodetable[123].ticks = 2;
	opcodetable[123].instruction = nop6502;
	opcodetable[123].adrmode = implied6502;
	opcodetable[124].ticks = 6;
	opcodetable[124].instruction = jmp6502;
	opcodetable[124].adrmode = indabsx6502;
	opcodetable[125].ticks = 4;
	opcodetable[125].instruction = adc6502;
	opcodetable[125].adrmode = absx6502;
	opcodetable[126].ticks = 7;
	opcodetable[126].instruction = ror6502;
	opcodetable[126].adrmode = absx6502;
	opcodetable[127].ticks = 2;
	opcodetable[127].instruction = nop6502;
	opcodetable[127].adrmode = implied6502;
	opcodetable[128].ticks = 2;
	opcodetable[128].instruction = bra6502;
	opcodetable[128].adrmode = relative6502;
	opcodetable[129].ticks = 6;
	opcodetable[129].instruction = sta6502;
	opcodetable[129].adrmode = indx6502;
	opcodetable[130].ticks = 2;
	opcodetable[130].instruction = nop6502;
	opcodetable[130].adrmode = implied6502;
	opcodetable[131].ticks = 2;
	opcodetable[131].instruction = nop6502;
	opcodetable[131].adrmode = implied6502;
	opcodetable[132].ticks = 2;
	opcodetable[132].instruction = sty6502;
	opcodetable[132].adrmode = zp6502;
	opcodetable[133].ticks = 2;
	opcodetable[133].instruction = sta6502;
	opcodetable[133].adrmode = zp6502;
	opcodetable[134].ticks = 2;
	opcodetable[134].instruction = stx6502;
	opcodetable[134].adrmode = zp6502;
	opcodetable[135].ticks = 2;
	opcodetable[135].instruction = nop6502;
	opcodetable[135].adrmode = implied6502;
	opcodetable[136].ticks = 2;
	opcodetable[136].instruction = dey6502;
	opcodetable[136].adrmode = implied6502;
	opcodetable[137].ticks = 2;
	opcodetable[137].instruction = bit6502;
	opcodetable[137].adrmode = immediate6502;
	opcodetable[138].ticks = 2;
	opcodetable[138].instruction = txa6502;
	opcodetable[138].adrmode = implied6502;
	opcodetable[139].ticks = 2;
	opcodetable[139].instruction = nop6502;
	opcodetable[139].adrmode = implied6502;
	opcodetable[140].ticks = 4;
	opcodetable[140].instruction = sty6502;
	opcodetable[140].adrmode = abs6502;
	opcodetable[141].ticks = 4;
	opcodetable[141].instruction = sta6502;
	opcodetable[141].adrmode = abs6502;
	opcodetable[142].ticks = 4;
	opcodetable[142].instruction = stx6502;
	opcodetable[142].adrmode = abs6502;
	opcodetable[143].ticks = 2;
	opcodetable[143].instruction = nop6502;
	opcodetable[143].adrmode = implied6502;
	opcodetable[144].ticks = 2;
	opcodetable[144].instruction = bcc6502;
	opcodetable[144].adrmode = relative6502;
	opcodetable[145].ticks = 6;
	opcodetable[145].instruction = sta6502;
	opcodetable[145].adrmode = indy6502;
	opcodetable[146].ticks = 3;
	opcodetable[146].instruction = sta6502;
	opcodetable[146].adrmode = indzp6502;
	opcodetable[147].ticks = 2;
	opcodetable[147].instruction = nop6502;
	opcodetable[147].adrmode = implied6502;
	opcodetable[148].ticks = 4;
	opcodetable[148].instruction = sty6502;
	opcodetable[148].adrmode = zpx6502;
	opcodetable[149].ticks = 4;
	opcodetable[149].instruction = sta6502;
	opcodetable[149].adrmode = zpx6502;
	opcodetable[150].ticks = 4;
	opcodetable[150].instruction = stx6502;
	opcodetable[150].adrmode = zpy6502;
	opcodetable[151].ticks = 2;
	opcodetable[151].instruction = nop6502;
	opcodetable[151].adrmode = implied6502;
	opcodetable[152].ticks = 2;
	opcodetable[152].instruction = tya6502;
	opcodetable[152].adrmode = implied6502;
	opcodetable[153].ticks = 5;
	opcodetable[153].instruction = sta6502;
	opcodetable[153].adrmode = absy6502;
	opcodetable[154].ticks = 2;
	opcodetable[154].instruction = txs6502;
	opcodetable[154].adrmode = implied6502;
	opcodetable[155].ticks = 2;
	opcodetable[155].instruction = nop6502;
	opcodetable[155].adrmode = implied6502;
	opcodetable[156].ticks = 4;
	opcodetable[156].instruction = stz6502;
	opcodetable[156].adrmode = abs6502;
	opcodetable[157].ticks = 5;
	opcodetable[157].instruction = sta6502;
	opcodetable[157].adrmode = absx6502;
	opcodetable[158].ticks = 5;
	opcodetable[158].instruction = stz6502;
	opcodetable[158].adrmode = absx6502;
	opcodetable[159].ticks = 2;
	opcodetable[159].instruction = nop6502;
	opcodetable[159].adrmode = implied6502;
	opcodetable[160].ticks = 3;
	opcodetable[160].instruction = ldy6502;
	opcodetable[160].adrmode = immediate6502;
	opcodetable[161].ticks = 6;
	opcodetable[161].instruction = lda6502;
	opcodetable[161].adrmode = indx6502;
	opcodetable[162].ticks = 3;
	opcodetable[162].instruction = ldx6502;
	opcodetable[162].adrmode = immediate6502;
	opcodetable[163].ticks = 2;
	opcodetable[163].instruction = nop6502;
	opcodetable[163].adrmode = implied6502;
	opcodetable[164].ticks = 3;
	opcodetable[164].instruction = ldy6502;
	opcodetable[164].adrmode = zp6502;
	opcodetable[165].ticks = 3;
	opcodetable[165].instruction = lda6502;
	opcodetable[165].adrmode = zp6502;
	opcodetable[166].ticks = 3;
	opcodetable[166].instruction = ldx6502;
	opcodetable[166].adrmode = zp6502;
	opcodetable[167].ticks = 2;
	opcodetable[167].instruction = nop6502;
	opcodetable[167].adrmode = implied6502;
	opcodetable[168].ticks = 2;
	opcodetable[168].instruction = tay6502;
	opcodetable[168].adrmode = implied6502;
	opcodetable[169].ticks = 3;
	opcodetable[169].instruction = lda6502;
	opcodetable[169].adrmode = immediate6502;
	opcodetable[170].ticks = 2;
	opcodetable[170].instruction = tax6502;
	opcodetable[170].adrmode = implied6502;
	opcodetable[171].ticks = 2;
	opcodetable[171].instruction = nop6502;
	opcodetable[171].adrmode = implied6502;
	opcodetable[172].ticks = 4;
	opcodetable[172].instruction = ldy6502;
	opcodetable[172].adrmode = abs6502;
	opcodetable[173].ticks = 4;
	opcodetable[173].instruction = lda6502;
	opcodetable[173].adrmode = abs6502;
	opcodetable[174].ticks = 4;
	opcodetable[174].instruction = ldx6502;
	opcodetable[174].adrmode = abs6502;
	opcodetable[175].ticks = 2;
	opcodetable[175].instruction = nop6502;
	opcodetable[175].adrmode = implied6502;
	opcodetable[176].ticks = 2;
	opcodetable[176].instruction = bcs6502;
	opcodetable[176].adrmode = relative6502;
	opcodetable[177].ticks = 5;
	opcodetable[177].instruction = lda6502;
	opcodetable[177].adrmode = indy6502;
	opcodetable[178].ticks = 3;
	opcodetable[178].instruction = lda6502;
	opcodetable[178].adrmode = indzp6502;
	opcodetable[179].ticks = 2;
	opcodetable[179].instruction = nop6502;
	opcodetable[179].adrmode = implied6502;
	opcodetable[180].ticks = 4;
	opcodetable[180].instruction = ldy6502;
	opcodetable[180].adrmode = zpx6502;
	opcodetable[181].ticks = 4;
	opcodetable[181].instruction = lda6502;
	opcodetable[181].adrmode = zpx6502;
	opcodetable[182].ticks = 4;
	opcodetable[182].instruction = ldx6502;
	opcodetable[182].adrmode = zpy6502;
	opcodetable[183].ticks = 2;
	opcodetable[183].instruction = nop6502;
	opcodetable[183].adrmode = implied6502;
	opcodetable[184].ticks = 2;
	opcodetable[184].instruction = clv6502;
	opcodetable[184].adrmode = implied6502;
	opcodetable[185].ticks = 4;
	opcodetable[185].instruction = lda6502;
	opcodetable[185].adrmode = absy6502;
	opcodetable[186].ticks = 2;
	opcodetable[186].instruction = tsx6502;
	opcodetable[186].adrmode = implied6502;
	opcodetable[187].ticks = 2;
	opcodetable[187].instruction = nop6502;
	opcodetable[187].adrmode = implied6502;
	opcodetable[188].ticks = 4;
	opcodetable[188].instruction = ldy6502;
	opcodetable[188].adrmode = absx6502;
	opcodetable[189].ticks = 4;
	opcodetable[189].instruction = lda6502;
	opcodetable[189].adrmode = absx6502;
	opcodetable[190].ticks = 4;
	opcodetable[190].instruction = ldx6502;
	opcodetable[190].adrmode = absy6502;
	opcodetable[191].ticks = 2;
	opcodetable[191].instruction = nop6502;
	opcodetable[191].adrmode = implied6502;
	opcodetable[192].ticks = 3;
	opcodetable[192].instruction = cpy6502;
	opcodetable[192].adrmode = immediate6502;
	opcodetable[193].ticks = 6;
	opcodetable[193].instruction = cmp6502;
	opcodetable[193].adrmode = indx6502;
	opcodetable[194].ticks = 2;
	opcodetable[194].instruction = nop6502;
	opcodetable[194].adrmode = implied6502;
	opcodetable[195].ticks = 2;
	opcodetable[195].instruction = nop6502;
	opcodetable[195].adrmode = implied6502;
	opcodetable[196].ticks = 3;
	opcodetable[196].instruction = cpy6502;
	opcodetable[196].adrmode = zp6502;
	opcodetable[197].ticks = 3;
	opcodetable[197].instruction = cmp6502;
	opcodetable[197].adrmode = zp6502;
	opcodetable[198].ticks = 5;
	opcodetable[198].instruction = dec6502;
	opcodetable[198].adrmode = zp6502;
	opcodetable[199].ticks = 2;
	opcodetable[199].instruction = nop6502;
	opcodetable[199].adrmode = implied6502;
	opcodetable[200].ticks = 2;
	opcodetable[200].instruction = iny6502;
	opcodetable[200].adrmode = implied6502;
	opcodetable[201].ticks = 3;
	opcodetable[201].instruction = cmp6502;
	opcodetable[201].adrmode = immediate6502;
	opcodetable[202].ticks = 2;
	opcodetable[202].instruction = dex6502;
	opcodetable[202].adrmode = implied6502;
	opcodetable[203].ticks = 2;
	opcodetable[203].instruction = nop6502;
	opcodetable[203].adrmode = implied6502;
	opcodetable[204].ticks = 4;
	opcodetable[204].instruction = cpy6502;
	opcodetable[204].adrmode = abs6502;
	opcodetable[205].ticks = 4;
	opcodetable[205].instruction = cmp6502;
	opcodetable[205].adrmode = abs6502;
	opcodetable[206].ticks = 6;
	opcodetable[206].instruction = dec6502;
	opcodetable[206].adrmode = abs6502;
	opcodetable[207].ticks = 2;
	opcodetable[207].instruction = nop6502;
	opcodetable[207].adrmode = implied6502;
	opcodetable[208].ticks = 2;
	opcodetable[208].instruction = bne6502;
	opcodetable[208].adrmode = relative6502;
	opcodetable[209].ticks = 5;
	opcodetable[209].instruction = cmp6502;
	opcodetable[209].adrmode = indy6502;
	opcodetable[210].ticks = 3;
	opcodetable[210].instruction = cmp6502;
	opcodetable[210].adrmode = indzp6502;
	opcodetable[211].ticks = 2;
	opcodetable[211].instruction = nop6502;
	opcodetable[211].adrmode = implied6502;
	opcodetable[212].ticks = 2;
	opcodetable[212].instruction = nop6502;
	opcodetable[212].adrmode = implied6502;
	opcodetable[213].ticks = 4;
	opcodetable[213].instruction = cmp6502;
	opcodetable[213].adrmode = zpx6502;
	opcodetable[214].ticks = 6;
	opcodetable[214].instruction = dec6502;
	opcodetable[214].adrmode = zpx6502;
	opcodetable[215].ticks = 2;
	opcodetable[215].instruction = nop6502;
	opcodetable[215].adrmode = implied6502;
	opcodetable[216].ticks = 2;
	opcodetable[216].instruction = cld6502;
	opcodetable[216].adrmode = implied6502;
	opcodetable[217].ticks = 4;
	opcodetable[217].instruction = cmp6502;
	opcodetable[217].adrmode = absy6502;
	opcodetable[218].ticks = 3;
	opcodetable[218].instruction = phx6502;
	opcodetable[218].adrmode = implied6502;
	opcodetable[219].ticks = 2;
	opcodetable[219].instruction = nop6502;
	opcodetable[219].adrmode = implied6502;
	opcodetable[220].ticks = 2;
	opcodetable[220].instruction = nop6502;
	opcodetable[220].adrmode = implied6502;
	opcodetable[221].ticks = 4;
	opcodetable[221].instruction = cmp6502;
	opcodetable[221].adrmode = absx6502;
	opcodetable[222].ticks = 7;
	opcodetable[222].instruction = dec6502;
	opcodetable[222].adrmode = absx6502;
	opcodetable[223].ticks = 2;
	opcodetable[223].instruction = nop6502;
	opcodetable[223].adrmode = implied6502;
	opcodetable[224].ticks = 3;
	opcodetable[224].instruction = cpx6502;
	opcodetable[224].adrmode = immediate6502;
	opcodetable[225].ticks = 6;
	opcodetable[225].instruction = sbc6502;
	opcodetable[225].adrmode = indx6502;
	opcodetable[226].ticks = 2;
	opcodetable[226].instruction = nop6502;
	opcodetable[226].adrmode = implied6502;
	opcodetable[227].ticks = 2;
	opcodetable[227].instruction = nop6502;
	opcodetable[227].adrmode = implied6502;
	opcodetable[228].ticks = 3;
	opcodetable[228].instruction = cpx6502;
	opcodetable[228].adrmode = zp6502;
	opcodetable[229].ticks = 3;
	opcodetable[229].instruction = sbc6502;
	opcodetable[229].adrmode = zp6502;
	opcodetable[230].ticks = 5;
	opcodetable[230].instruction = inc6502;
	opcodetable[230].adrmode = zp6502;
	opcodetable[231].ticks = 2;
	opcodetable[231].instruction = nop6502;
	opcodetable[231].adrmode = implied6502;
	opcodetable[232].ticks = 2;
	opcodetable[232].instruction = inx6502;
	opcodetable[232].adrmode = implied6502;
	opcodetable[233].ticks = 3;
	opcodetable[233].instruction = sbc6502;
	opcodetable[233].adrmode = immediate6502;
	opcodetable[234].ticks = 2;
	opcodetable[234].instruction = nop6502;
	opcodetable[234].adrmode = implied6502;
	opcodetable[235].ticks = 2;
	opcodetable[235].instruction = nop6502;
	opcodetable[235].adrmode = implied6502;
	opcodetable[236].ticks = 4;
	opcodetable[236].instruction = cpx6502;
	opcodetable[236].adrmode = abs6502;
	opcodetable[237].ticks = 4;
	opcodetable[237].instruction = sbc6502;
	opcodetable[237].adrmode = abs6502;
	opcodetable[238].ticks = 6;
	opcodetable[238].instruction = inc6502;
	opcodetable[238].adrmode = abs6502;
	opcodetable[239].ticks = 2;
	opcodetable[239].instruction = nop6502;
	opcodetable[239].adrmode = implied6502;
	opcodetable[240].ticks = 2;
	opcodetable[240].instruction = beq6502;
	opcodetable[240].adrmode = relative6502;
	opcodetable[241].ticks = 5;
	opcodetable[241].instruction = sbc6502;
	opcodetable[241].adrmode = indy6502;
	opcodetable[242].ticks = 3;
	opcodetable[242].instruction = sbc6502;
	opcodetable[242].adrmode = indzp6502;
	opcodetable[243].ticks = 2;
	opcodetable[243].instruction = nop6502;
	opcodetable[243].adrmode = implied6502;
	opcodetable[244].ticks = 2;
	opcodetable[244].instruction = nop6502;
	opcodetable[244].adrmode = implied6502;
	opcodetable[245].ticks = 4;
	opcodetable[245].instruction = sbc6502;
	opcodetable[245].adrmode = zpx6502;
	opcodetable[246].ticks = 6;
	opcodetable[246].instruction = inc6502;
	opcodetable[246].adrmode = zpx6502;
	opcodetable[247].ticks = 2;
	opcodetable[247].instruction = nop6502;
	opcodetable[247].adrmode = implied6502;
	opcodetable[248].ticks = 2;
	opcodetable[248].instruction = sed6502;
	opcodetable[248].adrmode = implied6502;
	opcodetable[249].ticks = 4;
	opcodetable[249].instruction = sbc6502;
	opcodetable[249].adrmode = absy6502;
	opcodetable[250].ticks = 4;
	opcodetable[250].instruction = plx6502;
	opcodetable[250].adrmode = implied6502;
	opcodetable[251].ticks = 2;
	opcodetable[251].instruction = nop6502;
	opcodetable[251].adrmode = implied6502;
	opcodetable[252].ticks = 2;
	opcodetable[252].instruction = nop6502;
	opcodetable[252].adrmode = implied6502;
	opcodetable[253].ticks = 4;
	opcodetable[253].instruction = sbc6502;
	opcodetable[253].adrmode = absx6502;
	opcodetable[254].ticks = 7;
	opcodetable[254].instruction = inc6502;
	opcodetable[254].adrmode = absx6502;
	opcodetable[255].ticks = 2;
	opcodetable[255].instruction = nop6502;
	opcodetable[255].adrmode = implied6502;

}

/******************************************************************************
 ******************************************************************************
 *
 *								6502 存储器镜像读写
 *
 ******************************************************************************
 ******************************************************************************
 */

/* 
 * sprite DMA (写地址到$4014, 左移8位，原数据地址)
 */
void SprDMA(BYTE scr_addr)		// scr_addr 为高8位地址
{
	uint8 *scr_addrptr = 0;		// 将传送sprite数据 指针
	int i;

	switch (scr_addr >> 4)
	{							// 选择源地址数据区域
	case 0x0:					// RAM
	case 0x1:
		scr_addrptr = &ram6502[(scr_addr << 8)];
		break;
	case 0x6:					// SRAM
	case 0x7:
		scr_addrptr = sram + ((scr_addr << 8) - 0x6000);
		break;
	case 0x8:					/* 程序存储器 只读 */
	case 0x9:
	case 0xA:
	case 0xB:
		scr_addrptr = &prg_rombank0[(scr_addr << 8) - 0x8000];
		break;
	case 0xC:
	case 0xD:
	case 0xE:
	case 0xF:
		scr_addrptr = &prg_rombank1[(scr_addr << 8) - 0xC000];
		break;
	}

	for (i = 0; i < 256; i++)
	{
		Spr_Mem.spr_ram[i] = scr_addrptr[i];
	}
	// 每次传输，需要512 cycles (about 4.5 scanlines worth)

	clockticks6502 -= 512;		// 花掉512CPU时钟周期
}

/* 
 * 读取6502存储器
 */
int get6502memory(WORD addr)	// 没0x2000递增
{
	switch (addr & 0xE000)
	{
	case 0x0000:				// $0000 ~ $0FFF

		return (ram6502[addr & 0x7FF]);
	case 0x2000:				// $2000 ~ $2FFF

		return PPU_RegRead(addr & 0x7);	/* PPU IO接口 */
	case 0x4000:				// $4000 ~ $4FFF //暂不处理
		/* APU 声音处理 IO接口 扩展ROM */
		if (addr < 0x4013)
		{
		}
		if (addr == 0x4014)
		{
		}
		if (addr == 0x4015)
		{
		}
		if (addr == 0x4016)
		{
			return NES_GetJoyPadVlaue(JOYPAD_0);
			// 分为3段8bit数据，第一段8bit为控制器1,第二段控制器3，第三段ID 
			// code
		}
		if (addr == 0x4017)
		{
return NES_GetJoyPadVlaue(JOYPAD_1);				// 分为3段8bit数据，第一段8bit为控制器2,第二段控制器4，第三段ID 
								// code
		}
		break;
	case 0x6000:				/* 卡带SRAM，一般用来存储游戏进度 */

		break;
	case 0x8000:				/* 程序存储器 只读 */
	case 0xA000:

		return prg_rombank0[addr - 0x8000];
	case 0xC000:
	case 0xE000:
		return prg_rombank1[addr - 0xC000];
	}

	return (addr >> 8);			/* 通常不会执行到此处,返回地址高八位
								   APU和SRAM未完成，可能执行到此处 */
}

/* 
 * 写6502存储器
 */
void put6502memory(WORD addr, BYTE value)
{
	switch (addr & 0xE000)
	{
	case 0x0000:
		ram6502[addr & 0x7FF] = value;
		break;
	case 0x2000:				/* PPU IO接口 */
		PPU_RegWrite(addr & 0x7, value);
		break;
	case 0x4000:				/* APU 声音处理 IO接口 扩展ROM */
		if (addr == 0x4014)
		{
			SprDMA(value);		// value
								// 为将要传送的数据地址的高八位
		}
		if (addr == 0x4016)
		{
			/* bit0 JoyPad 控制 */
			if (value & 1)
			{					// 先写bit0:1复位按键状态，在写bit0:0禁止
				NES_JoyPadReset();
			}
				else
				{
					 NES_JoyPadDisable();
					// 先写1再写0，复位控制器
				}
				/* 其他位暂未使用 */
			}
			// 其余寄存器暂未处理
			break;
	case 0x6000:				/* 卡带SRAM，一般用来存储游戏进度 */
			break;
	case 0x8000:				/* 程序存储器
								   只读(写入时与存储器切换(mapper)有关，暂略) */
	case 0xA000:
	case 0xC000:
	case 0xE000:
			break;
		}
	}

/******************************************************************************
 ******************************************************************************
 *
 *								寻址模式
 *
 ******************************************************************************
 ******************************************************************************
 */

	/* Adressing modes */
	/* Implied */
	void implied6502(void)
	{
	}

	/* #Immediate */
	void immediate6502(void)
	{
		savepc = PC++;
	}

	/* ABS */
	void abs6502(void)
	{
		savepc = get6502memory(PC);
		savepc += (get6502memory(PC + 1) << 8);

		PC++;
		PC++;
	}

	/* Branch */
	void relative6502(void)
	{

		savepc = get6502memory(PC);
		PC++;

		if (savepc & 0x80)
			savepc -= 0x100;
		if ((savepc >> 8) != (PC >> 8))
			clockticks6502++;
	}

	/* (ABS) */
	void indirect6502(void)
	{
		help = get6502memory(PC);
		help += (get6502memory(PC + 1) << 8);
		savepc = get6502memory(help);
		savepc += (get6502memory(help + 1) << 8);

		PC++;
		PC++;
	}

	/* ABS,X */
	void absx6502(void)
	{

		savepc = get6502memory(PC);
		savepc += (get6502memory(PC + 1) << 8);

		PC++;
		PC++;
		if (opcodetable[opcode].ticks == 4)
			if ((savepc >> 8) != ((savepc + X) >> 8))
				clockticks6502++;
		savepc += X;
	}

	/* ABS,Y */
	void absy6502(void)
	{

		savepc = get6502memory(PC);
		savepc += (get6502memory(PC + 1) << 8);

		PC++;
		PC++;

		if (opcodetable[opcode].ticks == 4)
			if ((savepc >> 8) != ((savepc + Y) >> 8))
				clockticks6502++;
		savepc += Y;
	}

	/* ZP */
	void zp6502(void)
	{
		savepc = get6502memory(PC);
		PC++;
	}

	/* ZP,X */
	void zpx6502(void)
	{
		savepc = get6502memory(PC) + X;
		PC++;

		savepc &= 0x00ff;
	}

	/* ZP,Y */
	void zpy6502(void)
	{
		// savepc=gameImage[PC++]+Y;
		savepc = get6502memory(PC) + Y;
		PC++;

		savepc &= 0x00ff;
	}

	/* (ZP,X) */
	void indx6502(void)
	{

		value = get6502memory(PC) + Y;
		PC++;
		savepc = get6502memory(value);
		savepc += get6502memory(value + 1) << 8;
	}

	/* (ZP),Y */
	void indy6502(void)
	{

		value = get6502memory(PC);
		PC++;
		savepc = get6502memory(value);
		savepc += get6502memory(value + 1) << 8;

		if (opcodetable[opcode].ticks == 5)
			if ((savepc >> 8) != ((savepc + Y) >> 8))
				clockticks6502++;
		savepc += Y;
	}

	/* (ABS,X) */
	void indabsx6502(void)
	{
		help = get6502memory(PC);
		help = (get6502memory(PC + 1) << 8) + X;
		savepc = get6502memory(help);
		savepc += get6502memory(help + 1) << 8;
	}

	/* (ZP) */
	void indzp6502(void)
	{
		value = get6502memory(PC);
		PC++;
		savepc = get6502memory(value);
		savepc += get6502memory(value + 1) << 8;
	}

/******************************************************************************
 ******************************************************************************
 *
 *								指令
 *
 ******************************************************************************
 ******************************************************************************
 */
	/* Instructions */
	void adc6502(void)
	{
		opcodetable[opcode].adrmode();
		value = get6502memory(savepc);

		saveflags = (P & 0x01);
		sum = ((char)A) + ((char)value) + saveflags;
		if ((sum > 0x7f) || (sum < -0x80))
			P |= 0x40;
		else
			P &= 0xbf;
		sum = A + value + saveflags;
		if (sum > 0xff)
			P |= 0x01;
		else
			P &= 0xfe;
		A = sum;
		if (P & 0x08)
		{
			P &= 0xfe;
			if ((A & 0x0f) > 0x09)
				A += 0x06;
			if ((A & 0xf0) > 0x90)
			{
				A += 0x60;
				P |= 0x01;
			}
		}
		else
		{
			clockticks6502++;
		}
		if (A)
			P &= 0xfd;
		else
			P |= 0x02;
		if (A & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void and6502(void)
	{
		opcodetable[opcode].adrmode();
		value = get6502memory(savepc);

		A &= value;
		if (A)
			P &= 0xfd;
		else
			P |= 0x02;
		if (A & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void asl6502(void)
	{
		opcodetable[opcode].adrmode();
		value = get6502memory(savepc);
		P = (P & 0xfe) | ((value >> 7) & 0x01);
		value = value << 1;
		put6502memory(savepc, value);
		if (value)
			P &= 0xfd;
		else
			P |= 0x02;
		if (value & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void asla6502(void)
	{
		P = (P & 0xfe) | ((A >> 7) & 0x01);
		A = A << 1;
		if (A)
			P &= 0xfd;
		else
			P |= 0x02;
		if (A & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void bcc6502(void)
	{
		if ((P & 0x01) == 0)
		{
			opcodetable[opcode].adrmode();
			PC += savepc;
			clockticks6502++;
		}
		else
		{
			value = get6502memory(PC);
			PC++;
		}
	}

	void bcs6502(void)
	{
		if (P & 0x01)
		{
			opcodetable[opcode].adrmode();
			PC += savepc;
			clockticks6502++;
		}
		else
		{
			value = get6502memory(PC);
			PC++;
		}
	}

	void beq6502(void)
	{
		if (P & 0x02)
		{
			opcodetable[opcode].adrmode();
			PC += savepc;
			clockticks6502++;
		}
		else
		{
			value = get6502memory(PC);
			PC++;
		}
	}

	void bit6502(void)
	{
		opcodetable[opcode].adrmode();
		value = get6502memory(savepc);
		if (value & A)
			P &= 0xfd;
		else
			P |= 0x02;

		/* set negative and overflow flags from value */
		P = (P & 0x3f) | (value & 0xc0);
	}

	void bmi6502(void)
	{
		if (P & 0x80)
		{
			opcodetable[opcode].adrmode();
			PC += savepc;
			clockticks6502++;
		}
		else
		{
			value = get6502memory(PC);
			PC++;
		}
	}

	void bne6502(void)
	{
		if ((P & 0x02) == 0)
		{
			opcodetable[opcode].adrmode();
			PC += savepc;
			clockticks6502++;
		}
		else
		{
			value = get6502memory(PC);
			PC++;
		}
	}

	void bpl6502(void)
	{
		if ((P & 0x80) == 0)
		{
			opcodetable[opcode].adrmode();
			PC += savepc;
			clockticks6502++;
		}
		else
		{
			value = get6502memory(PC);
			PC++;
		}
	}

	void brk6502(void)
	{
		PC++;
		put6502memory(0x0100 + S--, (BYTE) (PC >> 8));
		put6502memory(0x0100 + S--, (BYTE) (PC & 0xff));
		put6502memory(0x0100 + S--, P);
		P |= 0x14;
		PC = get6502memory(0xFFFE) + (get6502memory(0xFFFF) << 8);
	}

	void bvc6502(void)
	{
		if ((P & 0x40) == 0)
		{
			opcodetable[opcode].adrmode();
			PC += savepc;
			clockticks6502++;
		}
		else
		{
			value = get6502memory(PC);
			PC++;
		}
	}

	void bvs6502(void)
	{
		if (P & 0x40)
		{
			opcodetable[opcode].adrmode();
			PC += savepc;
			clockticks6502++;
		}
		else
		{
			value = get6502memory(PC);
			PC++;
		}
	}

	void clc6502(void)
	{
		P &= 0xfe;
	}

	void cld6502(void)
	{
		P &= 0xf7;
	}

	void cli6502(void)
	{
		P &= 0xfb;
	}

	void clv6502(void)
	{
		P &= 0xbf;
	}

	void cmp6502(void)
	{
		opcodetable[opcode].adrmode();
		value = get6502memory(savepc);
		if (A + 0x100 - value > 0xff)
			P |= 0x01;
		else
			P &= 0xfe;
		value = A + 0x100 - value;
		if (value)
			P &= 0xfd;
		else
			P |= 0x02;
		if (value & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void cpx6502(void)
	{
		opcodetable[opcode].adrmode();
		value = get6502memory(savepc);
		if (X + 0x100 - value > 0xff)
			P |= 0x01;
		else
			P &= 0xfe;
		value = X + 0x100 - value;
		if (value)
			P &= 0xfd;
		else
			P |= 0x02;
		if (value & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void cpy6502(void)
	{
		opcodetable[opcode].adrmode();
		value = get6502memory(savepc);
		if (Y + 0x100 - value > 0xff)
			P |= 0x01;
		else
			P &= 0xfe;
		value = Y + 0x100 - value;
		if (value)
			P &= 0xfd;
		else
			P |= 0x02;
		if (value & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void dec6502(void)
	{
		uint8 temp;

		opcodetable[opcode].adrmode();
		temp = get6502memory(savepc);
		temp--;
		put6502memory(savepc, temp);

		value = get6502memory(savepc);
		if (value)
			P &= 0xfd;
		else
			P |= 0x02;
		if (value & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void dex6502(void)
	{
		X--;
		if (X)
			P &= 0xfd;
		else
			P |= 0x02;
		if (X & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void dey6502(void)
	{
		Y--;
		if (Y)
			P &= 0xfd;
		else
			P |= 0x02;
		if (Y & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void eor6502(void)
	{
		opcodetable[opcode].adrmode();
		A ^= get6502memory(savepc);
		if (A)
			P &= 0xfd;
		else
			P |= 0x02;
		if (A & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void inc6502(void)
	{
		uint8 temp;

		opcodetable[opcode].adrmode();
		temp = get6502memory(savepc);
		temp++;
		put6502memory(savepc, temp);

		value = get6502memory(savepc);
		if (value)
			P &= 0xfd;
		else
			P |= 0x02;
		if (value & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void inx6502(void)
	{
		X++;
		if (X)
			P &= 0xfd;
		else
			P |= 0x02;
		if (X & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void iny6502(void)
	{
		Y++;
		if (Y)
			P &= 0xfd;
		else
			P |= 0x02;
		if (Y & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void jmp6502(void)
	{
		opcodetable[opcode].adrmode();
		PC = savepc;
	}

	void jsr6502(void)
	{
		PC++;
		put6502memory(0x0100 + S--, (BYTE) (PC >> 8));
		put6502memory(0x0100 + S--, (BYTE) (PC & 0xff));
		PC--;
		opcodetable[opcode].adrmode();
		PC = savepc;
	}

	void lda6502(void)
	{
		opcodetable[opcode].adrmode();
		A = get6502memory(savepc);
		// set the zero flag
		if (A)
			P &= 0xfd;
		else
			P |= 0x02;
		// set the negative flag
		if (A & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void ldx6502(void)
	{
		opcodetable[opcode].adrmode();
		X = get6502memory(savepc);
		if (X)
			P &= 0xfd;
		else
			P |= 0x02;
		if (X & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void ldy6502(void)
	{
		opcodetable[opcode].adrmode();
		Y = get6502memory(savepc);
		if (Y)
			P &= 0xfd;
		else
			P |= 0x02;
		if (Y & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void lsr6502(void)
	{
		opcodetable[opcode].adrmode();
		value = get6502memory(savepc);

		/* set carry flag if shifting right causes a bit to be lost */
		P = (P & 0xfe) | (value & 0x01);

		value = value >> 1;
		put6502memory(savepc, value);

		/* set zero flag if value is zero */
		if (value != 0)
			P &= 0xfd;
		else
			P |= 0x02;

		/* set negative flag if bit 8 set??? can this happen on an LSR? */
		if ((value & 0x80) == 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void lsra6502(void)
	{
		P = (P & 0xfe) | (A & 0x01);
		A = A >> 1;
		if (A)
			P &= 0xfd;
		else
			P |= 0x02;
		if (A & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void nop6502(void)
	{
	}

	void ora6502(void)
	{
		opcodetable[opcode].adrmode();
		A |= get6502memory(savepc);

		if (A)
			P &= 0xfd;
		else
			P |= 0x02;
		if (A & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void pha6502(void)
	{

		put6502memory(0x100 + S--, A);
	}

	void php6502(void)
	{
		put6502memory(0x100 + S--, P);
	}

	void pla6502(void)
	{

		A = get6502memory(++S + 0x100);
		if (A)
			P &= 0xfd;
		else
			P |= 0x02;
		if (A & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void plp6502(void)
	{
		P = get6502memory(++S + 0x100) | 0x20;
	}

	void rol6502(void)
	{
		saveflags = (P & 0x01);
		opcodetable[opcode].adrmode();
		value = get6502memory(savepc);
		P = (P & 0xfe) | ((value >> 7) & 0x01);
		value = value << 1;
		value |= saveflags;
		put6502memory(savepc, value);
		if (value)
			P &= 0xfd;
		else
			P |= 0x02;
		if (value & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void rola6502(void)
	{
		saveflags = (P & 0x01);
		P = (P & 0xfe) | ((A >> 7) & 0x01);
		A = A << 1;
		A |= saveflags;
		if (A)
			P &= 0xfd;
		else
			P |= 0x02;
		if (A & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void ror6502(void)
	{
		saveflags = (P & 0x01);
		opcodetable[opcode].adrmode();
		value = get6502memory(savepc);

		P = (P & 0xfe) | (value & 0x01);
		value = value >> 1;
		if (saveflags)
			value |= 0x80;
		put6502memory(savepc, value);
		if (value)
			P &= 0xfd;
		else
			P |= 0x02;
		if (value & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void rora6502(void)
	{
		saveflags = (P & 0x01);
		P = (P & 0xfe) | (A & 0x01);
		A = A >> 1;
		if (saveflags)
			A |= 0x80;
		if (A)
			P &= 0xfd;
		else
			P |= 0x02;
		if (A & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void rti6502(void)
	{
		P = get6502memory(++S + 0x100);
		P |= 0x20;
		PC = get6502memory(++S + 0x100);
		PC |= (get6502memory(++S + 0x100) << 8);
	}

	void rts6502(void)
	{

		PC = get6502memory(++S + 0x100);
		PC |= (get6502memory(++S + 0x100) << 8);
		PC++;
	}

	void sbc6502(void)
	{
		opcodetable[opcode].adrmode();
		value = get6502memory(savepc) ^ 0xFF;

		saveflags = (P & 0x01);
		sum = ((char)A) + ((char)value) + (saveflags << 4);
		if ((sum > 0x7f) || (sum < -0x80))
			P |= 0x40;
		else
			P &= 0xbf;
		sum = A + value + saveflags;
		if (sum > 0xff)
			P |= 0x01;
		else
			P &= 0xfe;
		A = sum;
		if (P & 0x08)
		{
			A -= 0x66;
			P &= 0xfe;
			if ((A & 0x0f) > 0x09)
				A += 0x06;
			if ((A & 0xf0) > 0x90)
			{
				A += 0x60;
				P |= 0x01;
			}
		}
		else
		{
			clockticks6502++;
		}
		if (A)
			P &= 0xfd;
		else
			P |= 0x02;
		if (A & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void sec6502(void)
	{
		P |= 0x01;
	}

	void sed6502(void)
	{
		P |= 0x08;
	}

	void sei6502(void)
	{
		P |= 0x04;
	}

	void sta6502(void)
	{
		opcodetable[opcode].adrmode();
		put6502memory(savepc, A);
	}

	void stx6502(void)
	{
		opcodetable[opcode].adrmode();
		put6502memory(savepc, X);
	}

	void sty6502(void)
	{
		opcodetable[opcode].adrmode();
		put6502memory(savepc, Y);
	}

	void tax6502(void)
	{
		X = A;
		if (X)
			P &= 0xfd;
		else
			P |= 0x02;
		if (X & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void tay6502(void)
	{
		Y = A;
		if (Y)
			P &= 0xfd;
		else
			P |= 0x02;
		if (Y & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void tsx6502(void)
	{
		X = S;
		if (X)
			P &= 0xfd;
		else
			P |= 0x02;
		if (X & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void txa6502(void)
	{
		A = X;
		if (A)
			P &= 0xfd;
		else
			P |= 0x02;
		if (A & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void txs6502(void)
	{
		S = X;
	}

	void tya6502(void)
	{
		A = Y;
		if (A)
			P &= 0xfd;
		else
			P |= 0x02;
		if (A & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void bra6502(void)
	{
		opcodetable[opcode].adrmode();
		PC += savepc;
		clockticks6502++;
	}

	void dea6502(void)
	{
		A--;
		if (A)
			P &= 0xfd;
		else
			P |= 0x02;
		if (A & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void ina6502(void)
	{
		A++;
		if (A)
			P &= 0xfd;
		else
			P |= 0x02;
		if (A & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void phx6502(void)
	{
		put6502memory(0x100 + S--, X);
	}

	void plx6502(void)
	{
		X = get6502memory(++S + 0x100);

		if (X)
			P &= 0xfd;
		else
			P |= 0x02;
		if (X & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void phy6502(void)
	{
		put6502memory(0x100 + S--, Y);
	}

	void ply6502(void)
	{

		Y = get6502memory(++S + 0x100);

		if (Y)
			P &= 0xfd;
		else
			P |= 0x02;
		if (Y & 0x80)
			P |= 0x80;
		else
			P &= 0x7f;
	}

	void stz6502(void)
	{
		opcodetable[opcode].adrmode();
		put6502memory(savepc, 0);
	}

	void tsb6502(void)
	{
		uint8 temp;

		opcodetable[opcode].adrmode();
		temp = get6502memory(savepc);
		temp |= A;
		put6502memory(savepc, temp);

		if (get6502memory(savepc))
			P &= 0xfd;
		else
			P |= 0x02;
	}

	void trb6502(void)
	{
		uint8 temp;

		opcodetable[opcode].adrmode();
		temp = get6502memory(savepc);
		temp &= (A ^ 0xFF);
		put6502memory(savepc, temp);

		if (get6502memory(savepc))
			P &= 0xfd;
		else
			P |= 0x02;
	}

/******************************************************************************
 ******************************************************************************
 *
 *								CPU复位与中断处理
 *
 ******************************************************************************
 ******************************************************************************
 */

	/* Reset CPU */
	void reset6502(void)
	{
		A = X = Y = P = 0;
		P |= 0x20;				// Z_FLAG | R_FLAG
		S = 0xff;
		PC = get6502memory(0xfffc);	// 复位地址，从中读取PC
		PC |= get6502memory(0xfffd) << 8;

		NMI_Flag = CLR0;
		IRQ_Flag = CLR0;
	}

	/* Non maskerable interrupt */
	void nmi6502(void)
	{
		put6502memory(0x0100 + S--, (BYTE) (PC >> 8));
		put6502memory(0x0100 + S--, (BYTE) (PC & 0xff));
		put6502memory(0x0100 + S--, P);
		P |= 0x04;
		PC = get6502memory(0xfffa);
		PC |= get6502memory(0xfffb) << 8;
	}

	/* Maskerable Interrupt */
	void irq6502(void)
	{
		put6502memory(0x0100 + S--, (BYTE) (PC >> 8));
		put6502memory(0x0100 + S--, (BYTE) (PC & 0xff));
		put6502memory(0x0100 + S--, P);
		P |= 0x04;
		PC = get6502memory(0xfffe);
		PC |= get6502memory(0xffff) << 8;
	}

/******************************************************************************
 ******************************************************************************
 *
 *								执行指令
 *
 ******************************************************************************
 ******************************************************************************
 */
	/* Execute Instruction */

	void exec6502(int32 timerTicks)
	{

		/* 中断检测 */
		if (NMI_Flag == SET1)
		{
			nmi6502();
			NMI_Flag = CLR0;
		}
		else
		{
			if (IRQ_Flag == SET1 && !(P & I_FLAG))
			{
				irq6502();
				IRQ_Flag = CLR0;
			}
		}

		while (timerTicks > clockticks6502) 
	{
		opcode = get6502memory(PC);
		PC++;
		opcodetable[opcode].instruction();
		clockticks6502 += opcodetable[opcode].ticks;
	}
	clockticks6502 -= timerTicks;

	}

	/* 初始化6502 存储器 */
	void init6502mem(uint8 * exp_romptr,
					 uint8 * sramptr, uint8 * prg_rombank0ptr, uint8 rom_num)
	{
		exp_rom = exp_romptr;	/* expansion rom */
		sram = sramptr;			/* sram */
//		prg_rombank0 = prg_rombank0ptr;	/* prg-rom lower bank */
		prg_rombank1 = prg_rombank0ptr + (0x4000 * (rom_num - 1));
	/* prg-romupperbank */
		prg_rombank0 = prg_rombank0ptr;	/* prg-rom lower bank */ 
//		prg_rombank1 = prg_rombank0ptr +0x8000;	/* prg-romupperbank */ 
		clockticks6502 = 0;
	}