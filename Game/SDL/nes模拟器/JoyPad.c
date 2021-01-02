 #include "nes_main.h"

JoyPadType JoyPad[2];
 extern int8 KEY[8];

void keyscan(void);

uint8 NES_GetJoyPadVlaue(int JoyPadNum)
{
	uint8 retval = 0;
	retval =(uint8) ((JoyPad[JoyPadNum].value >> JoyPad[JoyPadNum].index) & 0x01);
	JoyPad[JoyPadNum].index++;	
	return retval;

}
void NES_JoyPadReset(void)
{
	JoyPad[0].state = JOYPADENABLE;
    JoyPad[0].index = 0;

	JoyPad[1].state = JOYPADENABLE;
	JoyPad[1].index = 0;
}

void NES_JoyPadInit(void)
{
	JoyPad[0].state = JOYPADDISABLE;
    JoyPad[0].index = 0;
	JoyPad[0].value = 1<<20;

	JoyPad[1].state = JOYPADDISABLE;
    JoyPad[1].index = 0;
	JoyPad[1].value = 1<<19;
}

void NES_JoyPadDisable(void)
{    
	 keyscan();
     JoyPad[0].value=(KEY[0]<<7)|(KEY[1]<<6)|(KEY[2]<<5)|(KEY[3]<<4)|(KEY[4]<<3)|(KEY[5]<<2)|(KEY[6]<<1)|KEY[7];
}