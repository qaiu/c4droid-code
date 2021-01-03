/**
  ******************************************************************************
  * @file    
  * @author  
  * @version 
  * @date    
  * @brief  
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */  

#ifndef _NES_MAIN_H_
#define _NES_MAIN_H_
/* Includes ------------------------------------------------------------------*/
#include "6502.h"
#include "PPU.h"
#include "JoyPad.h"
//#include"debug.h"
#include "rom.h"

/* define ------------------------------------------------------------------*/
#undef NULL  
#define NULL 0

#ifndef FALSE
	#define FALSE 0
#endif

#ifndef	TRUE
	#define TRUE !FALSE
#endif



#define _NES_DEBUG_



/* Private typedef -----------------------------------------------------------*/
typedef struct{
			 char filetype[4]; 		/* 字符串“NES^Z”用来识别.NES文件 		  */
	unsigned char romnum;		   	/* 16kB ROM的数目 						  */
	unsigned char vromnum;			/* 8kB VROM的数目						  */
	unsigned char romfeature;		/* D0：1＝垂直镜像，0＝水平镜像 
									   D1：1＝有电池记忆，SRAM地址$6000-$7FFF
									   D2：1＝在$7000-$71FF有一个512字节的trainer 
									   D3：1＝4屏幕VRAM布局 
 									   D4－D7：ROM Mapper的低4位 
									*/
	unsigned char rommappernum;		/* D0－D3：保留，必须是0（准备作为副Mapper号^_^）
									   D4－D7：ROM Mapper的高4位 
									*/
	//unsigned char reserve[8];		/* 保留，必须是0 					      */
	/*ROM段升序排列，如果存在trainer，它的512字节摆在ROM段之前*/
	/*VROM段, 升序排列*/
}NesHeader;

/* function ------------------------------------------------------------------*/
void nes_main(void);
void NesFrameCycle(void);
void NES_ReadJoyPad(uint8 JoyPadNum);

#endif /*  _NES_MAIN_H_ */
  /**
  * @}
  */


/*******************************END OF FILE***********************************/