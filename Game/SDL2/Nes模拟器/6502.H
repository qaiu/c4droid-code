
#ifndef _6502_H_
#define _6502_H_

#include "stdio.h"
#include "PPU.h"

/*CPU中断标志*/
typedef enum {CLR0 = 0, SET1 = !CLR0} CPU_InitFlag;

extern  CPU_InitFlag NMI_Flag;
extern  CPU_InitFlag NMI_Flag;

/*存储器指针*/
extern uint8* prg_rombank0;	/* prg-rom lower bank*/
extern uint8* prg_rombank1;	/* prg-rom upper bank*/

/*指令解释*/
void implied6502(void);
void immediate6502(void);
void abs6502(void);
void relative6502(void);
void indirect6502(void);
void absx6502(void);
void absy6502(void);
void zp6502(void);
void zpx6502(void);
void zpy6502(void);
void indx6502(void);
void indy6502(void);
void indabsx6502(void);
void indzp6502(void);
void adc6502(void);
void and6502(void);
void asl6502(void);
void asla6502(void);
void bcc6502(void);
void bcs6502(void);
void beq6502(void);
void bit6502(void);
void bmi6502(void);
void bne6502(void);
void bpl6502(void);
void brk6502(void);
void bvc6502(void);
void bvs6502(void);
void clc6502(void);
void cld6502(void);
void cli6502(void);
void clv6502(void);
void cmp6502(void);
void cpx6502(void);
void cpy6502(void);
void dec6502(void);
void dex6502(void);
void dey6502(void);
void eor6502(void);
void inc6502(void);
void inx6502(void);
void iny6502(void);
void jmp6502(void);
void jsr6502(void);
void lda6502(void);
void ldx6502(void);
void ldy6502(void);
void lsr6502(void);
void lsra6502(void);
void nop6502(void);
void ora6502(void);
void pha6502(void);
void php6502(void);
void pla6502(void);
void plp6502(void);
void rol6502(void);
void rola6502(void);
void ror6502(void);
void rora6502(void);
void rti6502(void);
void rts6502(void);
void sbc6502(void);
void sec6502(void);
void sed6502(void);
void sei6502(void);
void sta6502(void);
void stx6502(void);
void sty6502(void);
void tax6502(void);
void tay6502(void);
void tsx6502(void);
void txa6502(void);
void txs6502(void);
void tya6502(void);
void bra6502(void);
void dea6502(void);
void ina6502(void);
void phx6502(void);
void plx6502(void);
void phy6502(void);
void ply6502(void);
void stz6502(void);
void tsb6502(void);
void trb6502(void);

/* Non maskerable interrupt */
void nmi6502(void);

/* Maskerable Interrupt */
void irq6502(void);

/* must be called first to initialise all 6502 engines arrays */

void init6502mem(	uint8* exp_romptr,
					uint8* sramptr,			
					uint8* prg_rombank0ptr,		
					uint8  rom_num);

/* sets all of the 6502 registers. The program counter is set from
 * locations $FFFC and $FFFD masked with the above addrmask
 */
void reset6502(void);

/* run the 6502 engine for specified number of clock cycles */
void exec6502(int32 timerTicks);

#endif	/* _6502_H_*/
 