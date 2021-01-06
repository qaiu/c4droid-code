#include "nes_main.h"
#include "stdio.h"
#include"Timer.h"
extern SDLglue_Surface *chuang;
extern int8 KEY[8];
extern void keyinit();
uint8 FrameCnt;
SDLglue_Event event, event2;
Timer time;
void NEStimer(int32 data)
{
	uint32 clocks;				// CPU执行时间

	FrameCnt++;					// 帧计数器
	SpriteHitFlag = FALSE;

	for (PPU_scanline = 0; PPU_scanline < 20; PPU_scanline++)
	{
		exec6502(CLOCKS_PER_SCANLINE);
	}
	exec6502(CLOCKS_PER_SCANLINE);

	PPU_scanline++;				// 20++
	PPU_Reg.R2 &= ~R2_SPR0_HIT;

	for (PPU_scanline = 21; PPU_scanline < SCAN_LINE_DISPALY_END_NUM;
		 PPU_scanline++)
	{
		if ((SpriteHitFlag == TRUE) && ((PPU_Reg.R2 & R2_SPR0_HIT) == 0))
		{
			clocks = sprite[0].x * CLOCKS_PER_SCANLINE / NES_DISP_WIDTH;
			exec6502(clocks);	// 需重点优化
			PPU_Reg.R2 |= R2_SPR0_HIT;
			exec6502(CLOCKS_PER_SCANLINE - clocks);
		}
		else
		{
			exec6502(CLOCKS_PER_SCANLINE);	// 耗时大户
		}
		if (PPU_Reg.R1 & (R1_BG_VISIBLE | R1_SPR_VISIBLE))
		{						// 若为假，关闭显示
			if (SpriteHitFlag == FALSE)
			{
				NES_GetSpr0HitFlag(PPU_scanline - SCAN_LINE_DISPALY_START_NUM);
			}
		}
		// if (FrameCnt % 2 == 0)
		{
			// 每3帧显示一次
			// 耗时大户
			// 调帧
			NES_RenderLine(PPU_scanline - SCAN_LINE_DISPALY_START_NUM);
			FrameCnt = 0;


		}


	}
	exec6502(CLOCKS_PER_SCANLINE);
	PPU_Reg.R2 |= R2_VBlank_Flag;	// 设置VBANK 标志
	if (PPU_Reg.R0 & R0_VB_NMI_EN)
	{
		NMI_Flag = SET1;		// 完成一帧扫描，产生NMI中断


		SDLglue_UpdateRect(chuang, 0, 0, 240, 240);
		if (UsableButton == SDL_TRUE)
		{
			SDLglue_FixButtonRect();
			SDLglue_ShowButton();
		}
		SDLglue_Flush();
	}

}

void event_down()				// 按键
{
	if (event.type == SDLglue_KEYDOWN)
	{
		switch (event.key.keysym.sym)
		{
		case SDLK_UP:
			KEY[3] = 1;
			break;
		case SDLK_DOWN:
			KEY[2] = 1;
			break;
		case SDLK_LEFT:
			KEY[1] = 1;
			break;
		case SDLK_RIGHT:
			KEY[0] = 1;
			break;
		case SDLK_4:
			KEY[7] = 1;
			break;
		case SDLK_3:
			KEY[6] = 1;
			break;
		case SDLK_RETURN:
			KEY[4] = 1;
			break;
		case SDLK_ESCAPE:
			KEY[5] = 1;
			break;
		}

	}
	else
	{
		if (event.type == SDLglue_KEYUP)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_UP:
				KEY[3] = 0;
				break;
			case SDLK_DOWN:
				KEY[2] = 0;
				break;
			case SDLK_LEFT:
				KEY[1] = 0;
				break;
			case SDLK_RIGHT:
				KEY[0] = 0;
				break;
			case SDLK_4:
				KEY[7] = 0;
				break;
			case SDLK_3:
				KEY[6] = 0;
				break;
			case SDLK_RETURN:
				KEY[4] = 0;
				break;
			case SDLK_ESCAPE:
				KEY[5] = 0;
				break;
			}

		}
	}
}

uint16 *LCDBUF;

void NesFrameCycle(void)
{
	FrameCnt = 0;

	LCDBUF = (uint16 *) (chuang->pixels);	// 获取数据流写入初始位置
	while (1)
	{
		time.start();
		while (SDLglue_PollEvent(&event))
		{
			SDLglue_HookButton(&event);
			event_down();
		}
		NEStimer(0);
		if (time.get_ticks() < 1000 / 50)
			SDLglue_Delay(1000 / 50 - time.get_ticks());
	}
}

void nes_main(void)
{

	NesHeader *neshreader = (NesHeader *) rom_file;
	NES_JoyPadInit();
	init6502mem(0, 0, (&rom_file[0x10]), neshreader->romnum);
	/* FILE *fp; fp=fopen("/mnt/sdcard/RX/nes.txt","wb");
	   fprintf(fp,"%d,%d,%s,%d,%d",neshreader->romnum,neshreader->vromnum
	   ,neshreader->filetype,neshreader->romfeature,neshreader->rommappernum
	   ); fclose(fp); */
	reset6502();
	PPU_Init((&rom_file[0x10] + (neshreader->romnum * 0x4000)), (neshreader->romfeature & 0x01));	/* PPU_初始化																									 */
	NesFrameCycle();
}