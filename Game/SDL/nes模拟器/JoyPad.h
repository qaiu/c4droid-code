
#ifndef _JOYPAD_H_
#define _JOYPAD_H_
	/* Includes
	   ------------------------------------------------------------------ */
#include "nes_main.h"
	/* define
	   ------------------------------------------------------------------ */
#define JOYPAD_0 	0
#define JOYPAD_1 	1
	/* Private typedef
	   ----------------------------------------------------------- */
	typedef enum
{ JOYPADDISABLE = 0, JOYPADENABLE = !JOYPADDISABLE } JoyPadState;

typedef struct
{
	JoyPadState state;
	uint8 index;				// 当前读取位
	uint32 value;				// JoyPad 当前值 
} JoyPadType;

/* function ------------------------------------------------------------------ */
void NES_JoyPadInit(void);
void NES_JoyPadReset(void);
void NES_JoyPadDisable(void);
uint8 NES_GetJoyPadVlaue(int JoyPadNum);


#endif /* _JOYPAD_H_ */