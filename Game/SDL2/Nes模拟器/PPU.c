#include "nes_main.h"


/* 变量声明
   ------------------------------------------------------------------ */
/* 存储器相关 */
uint8 NameTable[1024 * 2];

PPU_RegType PPU_Reg;
PPU_MemType PPU_Mem;

Spr_MemType Spr_Mem;
SpriteType *sprite;				// 指向第一个sprite 0 的位置

/* 显示相关 */
uint8 SpriteHitFlag, PPU_Latch_Flag;	// sprite #0
										// 显示碰撞点所在扫描行号,
										// 背景位移￥2005写入标志 
int PPU_scanline;				// 当前扫描行
uint16 Buffer_scanline[8 + 256 + 8];	// 行显示缓存,上下标越界最大为7，显示区 
										// 
										// 
										// 
										// 
										// 7 ~ 263 0~7 263~270
										// 为防止溢出区
uint8 PPU_BG_VScrlOrg, PPU_BG_HScrlOrg;
extern uint16 *LCDBUF;
extern SDLglue_Surface *chuang;

/********************************************************************************
 **NES 调色板 颜色表（RGB565） 64
 */
const uint16 NES_Color_Palette[64] = {
	0x73AE, 0x20D1, 0x0015, 0x4013, 0x880E, 0xA802, 0xA000, 0x7840,
	0x4160, 0x0220, 0x0280, 0x01E2, 0x19EB, 0x0000, 0x0000, 0x0000,
	0xBDF7, 0x039D, 0x21DD, 0x801E, 0xB817, 0xE00B, 0xD940, 0xCA61,
	0x8B80, 0x04A0, 0x0540, 0x0487, 0x0411, 0x0000, 0x0000, 0x0000,
	0xFFFF, 0x3DFF, 0x5CBF, 0xA45F, 0xF3DF, 0xFBB6, 0xFBAC, 0xFCC7,
	0xF5E7, 0x8682, 0x4EE9, 0x5FD3, 0x075B, 0x0000, 0x0000, 0x0000,
	0xFFFF, 0xAF3F, 0xC6BF, 0xD65F, 0xFE3F, 0xFE3B, 0xFDF6, 0xFED5,
	0xFF34, 0xE7F4, 0xAF97, 0xB7F9, 0x9FFE, 0x0000, 0x0000, 0x0000
};

/* 
 **
 *********************************************************************************/


/* 函数定义
   ---------------------------------------------------------------------- */

/* 
 * PPU 初始化
 */
void PPU_Init(uint8 * patterntableptr,	/* Pattern table 地址 */
			  uint8 ScreenMirrorType	/* 屏幕镜像类型 */
	)
{
	memset(&PPU_Mem, 0, sizeof(PPU_Mem));	// 清零存储器
	memset(&Spr_Mem, 0, sizeof(Spr_Mem));
	memset(&PPU_Reg, 0, sizeof(PPU_Reg));
	sprite = (SpriteType *) & Spr_Mem.spr_ram[0];
	PPU_Mem.patterntable0 = patterntableptr;
	PPU_Mem.patterntable1 = patterntableptr + 0x1000;

	if (ScreenMirrorType == 0)
	{							// 水平镜像
		PPU_Mem.name_table[0] = &NameTable[0];
		PPU_Mem.name_table[1] = &NameTable[0];
		PPU_Mem.name_table[2] = &NameTable[1024];
		PPU_Mem.name_table[3] = &NameTable[1024];
	}
	else
	{							// 垂直镜像
		PPU_Mem.name_table[0] = &NameTable[0];
		PPU_Mem.name_table[1] = &NameTable[1024];
		PPU_Mem.name_table[2] = &NameTable[0];
		PPU_Mem.name_table[3] = &NameTable[1024];
	}

	SpriteHitFlag = PPU_Latch_Flag = FALSE;
	PPU_BG_VScrlOrg = 0;
	PPU_BG_HScrlOrg = 0;
	PPU_scanline = 0;

}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
   PPU 存储器与寄存器函数组 */

/* 
 * 读PPU name table 数据
 */
uint8 PPU_NameTablesRead(void)
{
	uint16 addrtemp = PPU_Mem.PPU_addrcnt & 0xFFF;

	if (addrtemp > 0xC00)
	{
		return PPU_Mem.name_table[3][addrtemp - 0xC00];	// nametable3
	}
	if (addrtemp > 0x800)
	{
		return PPU_Mem.name_table[2][addrtemp - 0x800];	// nametable2
	}
	if (addrtemp > 0x400)
	{
		return PPU_Mem.name_table[1][addrtemp - 0x400];	// nametable1
	}
	else
	{
		return PPU_Mem.name_table[0][addrtemp];	// nametable0
	}
}

/* 
 * 写PPU name table 数据
 */
void PPU_NameTablesWrite(uint8 value)
{
	uint16 addrtemp = PPU_Mem.PPU_addrcnt & 0xFFF;

	if (addrtemp > 0xC00)
	{
		PPU_Mem.name_table[3][addrtemp - 0xC00] = value;	// nametable3
		return;
	}
	if (addrtemp > 0x800)
	{
		PPU_Mem.name_table[2][addrtemp - 0x800] = value;	// nametable2
		return;
	}
	if (addrtemp > 0x400)
	{
		PPU_Mem.name_table[1][addrtemp - 0x400] = value;	// nametable1
		return;
	}
	else
	{
		PPU_Mem.name_table[0][addrtemp] = value;	// nametable0
		return;
	}
}

/* 
 * 写PPU存储器
 */
void PPU_MemWrite(uint8 value)
{
	switch (PPU_Mem.PPU_addrcnt & 0xF000)
	{
	case 0x0000:				// $0000 ~ $0FFF 只读 - 与卡带有关
		// PPU_Mem.patterntable0[PPU_Mem.PPU_addrcnt] = value;
		break;
	case 0x1000:				// $1000 ~ $1FFF 只读 - 与卡带有关
		// PPU_Mem.patterntable1[PPU_Mem.PPU_addrcnt & 0x0FFF] = value;
		break;
	case 0x2000:				// $2000 ~ $2FFF
		PPU_NameTablesWrite(value);
		break;
	case 0x3000:
		// $3000 ~ $3EFF -- $2000 ~ $2EFF的镜像
		// $3F00 ~ $3F0F image palette
		// $3F10 ~ $3F1F sprite palette
		if ((PPU_Mem.PPU_addrcnt & 0x1F) > 0x0F)
		{
			PPU_Mem.sprite_palette[(PPU_Mem.PPU_addrcnt & 0xF)] = value;	// 精灵颜色索引值表
			if ((PPU_Mem.PPU_addrcnt & 3) == 0)
			{					// 对应位置为透明色的景象
				PPU_Mem.sprite_palette[0] = PPU_Mem.image_palette[0] = value;
				PPU_Mem.sprite_palette[4] = PPU_Mem.image_palette[4] = value;
				PPU_Mem.sprite_palette[8] = PPU_Mem.image_palette[8] = value;
				PPU_Mem.sprite_palette[12] = PPU_Mem.image_palette[12] = value;
			}
		}
		else
		{
			PPU_Mem.image_palette[(PPU_Mem.PPU_addrcnt & 0xF)] = value;	// 背景颜色索引值表
		}
		break;
	}
	// 读写后，地址计数器增加，根据$2002 [bit2] 0：+1 1：
	// +32。
	PPU_Reg.R0 & PPU_ADDRINCR ? PPU_Mem.PPU_addrcnt +=
		32 : PPU_Mem.PPU_addrcnt++;
}

/* 
 * 读PPU存储器
 */
uint8 PPU_MemRead(void)
{
	// 由于硬件原因，NES
	// PPU每次读取返回的是缓冲值，为时机读取地址减1；
	uint8 temp;

	temp = PPU_Mem.PPU_readtemp;	// 保存缓冲值，作为返回值

	switch (PPU_Mem.PPU_addrcnt & 0xF000)
	{
	case 0x0000:				// $0000 ~ $0FFF
		PPU_Mem.PPU_readtemp = PPU_Mem.patterntable0[PPU_Mem.PPU_addrcnt];	// 读取地址指定值到缓冲
		break;
	case 0x1000:				// $1000 ~ $1FFF
		PPU_Mem.PPU_readtemp = PPU_Mem.patterntable1[PPU_Mem.PPU_addrcnt & 0x0FFF];	// 读取地址指定值到缓冲
		break;
	case 0x2000:				// $2000 ~ $2FFF
		PPU_Mem.PPU_readtemp = PPU_NameTablesRead();	// 读取地址指定值到缓冲
		break;
	case 0x3000:
		if (PPU_Mem.PPU_addrcnt >= 0x3F10)
		{
			temp = PPU_Mem.sprite_palette[(PPU_Mem.PPU_addrcnt & 0xF)];	// PPU 
																		// 
			// 
			// 
			// 
			// 读取缓冲不适用 
			// palette 
			// 调色板,直接返回
			break;
		}
		if (PPU_Mem.PPU_addrcnt >= 0x3F00)
		{
			temp = PPU_Mem.image_palette[(PPU_Mem.PPU_addrcnt & 0xF)];	// PPU 
																		// 
			// 
			// 
			// 
			// 读取缓冲不适用 
			// palette 
			// 调色板,直接返回
			break;
		}
		break;
	default:
		temp = 0;
	}
	// 读写后，地址计数器增加，根据$2002 [bit2] 0：+1 1：
	// +32。
	PPU_Reg.R0 & PPU_ADDRINCR ? PPU_Mem.PPU_addrcnt +=
		32 : PPU_Mem.PPU_addrcnt++;
	return temp;
}

/* 
 * 写PPU寄存器
 */
void PPU_RegWrite(uint16 RX, uint8 value)
{

	switch (RX)
	{
		/* $2000 */
	case 0:
		PPU_Reg.R0 = value;
		/* $2001 */ break;
	case 1:
		PPU_Reg.R1 = value;
		/* $2003 */ break;
	case 3:					// Sprite Memory Address，
		// 8位地址计数器
		Spr_Mem.spr_addrcnt = value;
		/* $2004 */ break;
	case 4:					// Sprite Memory Data ,每次存取 sprite ram 
								// 
		// 
		// 
		// 
		// 地址计数器spr_addrcnt自动加1
		Spr_Mem.spr_ram[Spr_Mem.spr_addrcnt++] = value;
		/* $2005 */ break;
	case 5:					// PPU_Reg.R5 = value;
		if (PPU_Latch_Flag)
		{						// 真1：垂直scroll数据
			PPU_BG_VScrlOrg = (value > 239) ? 0 : value;

		}
		else
		{						// 假0：水平scroll数据
			PPU_BG_HScrlOrg = value;
		}
		PPU_Latch_Flag ^= 1;
		/* $2006 */ break;
	case 6:

		PPU_Mem.PPU_addrcnt = (PPU_Mem.PPU_addrcnt << 8) + value;	// PPU
		// 存储器地址计数器，先写高8位，后写低8位
		PPU_Latch_Flag ^= 1;
		/* $2007 */ break;
	case 7:					/* 写 PPU Memory Data */
		PPU_MemWrite(value);
		break;
	}
}

/* 
 * 读PPU寄存器
 */
uint8 PPU_RegRead(uint16 RX)
{
	uint8 temp;

	switch (RX)
	{
	case 0:
		temp = PPU_Reg.R0;		// $2000 RW
		break;
	case 1:
		temp = PPU_Reg.R1;		// $2001 RW
		break;
	case 2:
		temp = PPU_Reg.R2;
		PPU_Reg.R2 &= ~(R0_VB_NMI_EN);
		PPU_Latch_Flag = 0;
		// Make a Nametable 0 in V-Blank
		if ((PPU_scanline > 20 && PPU_scanline < 262)
			&& !(PPU_Reg.R0 & R0_VB_NMI_EN))
		{
			PPU_Reg.R0 &= ~R0_NAME_TABLE;	// 选择 name table #0
		}
		break;;					// $2002 R
	case 4:					/* 读 Sprite Memory Data */
		temp = Spr_Mem.spr_ram[Spr_Mem.spr_addrcnt++];
		break;
	case 7:					/* 读 PPU Memory Data */
		temp = PPU_MemRead();
		break;
	default:					// printf("\r\nPPU 读取地址错误 %d",
		// RX);
		return (uint8) RX;
	}

	return temp;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
   PPU 显示函数组 */

/* 
 * 查找sprite #0碰撞标志
 */
void NES_GetSpr0HitFlag(int y_axes)
{
	int16 i, y_scroll, dy_axes, dx_axes;
	uint8 y_TitleLine, x_TitleLine;
	uint8 spr_size, Spr0_Data, temp;
	uint8 nNameTable, BG_TitlePatNum;
	uint8 BG_Data0, BG_Data1, BG_Data;
	uint16 title_addr;
	uint8 *BG_Patterntable;
	uint8 *Spr_Patterntable;

	/* 判断sprite #0 显示区域是否在当前行 */
	spr_size = PPU_Reg.R0 & R0_SPR_SIZE ? 0x0F : 0x07;	// spr_size
	// 8：0~7，16: 0~15
	dy_axes = y_axes - (uint8) (sprite[0].y + 1);	// 判断sprite#0
	// 是否在当前行显示范围内,0坐标实际值为FF
	if (dy_axes != (dy_axes & spr_size))
		return;
	nNameTable = PPU_Reg.R0 & R0_NAME_TABLE;
	BG_Patterntable = PPU_Reg.R0 & BG_PATTERN_ADDR ? PPU_Mem.patterntable1 : PPU_Mem.patterntable0;	// 背景pattern首地址
	y_scroll = y_axes + PPU_BG_VScrlOrg;	// Scorll
	// 位移后背景显示行的Y坐标，四屏[00]、[01]、[10]、[11]，每屏显示(0+x_scroll_org,y_scroll_org)
	if (y_scroll > 239)
	{
		y_scroll -= 240;
		nNameTable ^= NAME_TABLE_V_MASK;	// 垂直超出屏幕，切换name 
											// 
		// 
		// 
		// 
		// table表
	}
	y_TitleLine = y_scroll >> 3;	// title 行号 0~29（y轴坐标除8）
	x_TitleLine = (PPU_BG_HScrlOrg + sprite[0].x) >> 3;	// title 列号
	// 0~31（y轴坐标除8） 
	// 
	dy_axes = y_scroll & 0x07;	// title显示y轴偏移像素 除8的余数
	dx_axes = PPU_BG_HScrlOrg & 0x07;	// title显示x轴偏移像素
	// 除8的余数
	if (x_TitleLine > 31)		// x轴跨屏显示
		nNameTable ^= NAME_TABLE_H_MASK;
	BG_TitlePatNum = PPU_Mem.name_table[nNameTable][(y_TitleLine << 5) + x_TitleLine];	// y_TitleLine 
																						// 
	// 
	// 
	// 
	// * 
	// 32 
	// +　x_TitleLine,从name表中取得sprite显示位置的背景的title号的 
	// 
	// 
	// 
	// 
	// 
	BG_Data0 = BG_Patterntable[(BG_TitlePatNum << 4) + dy_axes];	// 背景显示数据0
	BG_Data0 |= BG_Patterntable[(BG_TitlePatNum << 4) + dy_axes + 8];
	if ((x_TitleLine + 1) > 31)	// x轴跨屏显示
		nNameTable ^= NAME_TABLE_H_MASK;
	BG_TitlePatNum = PPU_Mem.name_table[nNameTable][(y_TitleLine << 5) + x_TitleLine + 1];	// 从name表中下一个背景显示的title号的
	BG_Data1 = BG_Patterntable[(BG_TitlePatNum << 4) + dy_axes];	// 背景显示数据1
	BG_Data1 |= BG_Patterntable[(BG_TitlePatNum << 4) + dy_axes + 8];
	BG_Data = (BG_Data0 << dx_axes) | (BG_Data1 >> dx_axes);	// 背景与Sprite 
																// 
	// 
	// 
	// 
	// #0
	// 位置相同的当前显示行的显示数据

	/* 取得sprite #0 显示数据 */
	if (sprite[0].attr & SPR_VFLIP)	// 若垂直翻转
		dy_axes = spr_size - dy_axes;
	if (PPU_Reg.R2 & R0_SPR_SIZE)
	{							// 8*16 //若为真，sprite的大小8*16
		/* 取得所在title Pattern首地址 */
		Spr_Patterntable =
			(sprite[0].t_num & 0x01) ? PPU_Mem.patterntable1 : PPU_Mem.
			patterntable0;
		title_addr = (sprite[0].t_num & 0XFE) << 4;	// *16,原地址已*2
		Spr0_Data = Spr_Patterntable[title_addr + dy_axes];
		Spr0_Data |= Spr_Patterntable[title_addr + dy_axes + 8];
	}
	else
	{							// 8*8
		/* 取得sprite #0 所在title Pattern首地址 */
		Spr_Patterntable =
			(PPU_Reg.R0 & SPR_PATTERN_ADDR) ? PPU_Mem.patterntable1 : PPU_Mem.
			patterntable0;
		title_addr = sprite[0].t_num << 4;	// *16
		Spr0_Data = Spr_Patterntable[title_addr + dy_axes];
		Spr0_Data |= Spr_Patterntable[title_addr + dy_axes + 8];
	}

	if (sprite[0].attr & SPR_HFLIP)
	{							/* 若水平翻转, 翻转高低位数据 */
		temp = 0;
		for (i = 0; i < 8; i++)
		{
			temp |= (Spr0_Data >> i) & 1;
			temp <<= i;
		}
		Spr0_Data = temp;
	}
	if (Spr0_Data & BG_Data)
	{
		SpriteHitFlag = TRUE;
	}
}

/* 
 * 显示一行背景，若与sprite碰撞，设置碰撞标志
 */
void NES_RenderBGLine(int y_axes)
{
	int i, y_scroll, /* x_scroll, */ dy_axes, dx_axes;
	int Buffer_LineCnt, y_TitleLine, x_TitleLine;
	uint8 H_byte, L_byte, BG_color_num, BG_attr_value;
	uint8 nNameTable, BG_TitlePatNum;
	uint8 *BG_Patterntable;
	nNameTable = PPU_Reg.R0 & R0_NAME_TABLE;
	BG_Patterntable = PPU_Reg.R0 & BG_PATTERN_ADDR ? PPU_Mem.patterntable1 : PPU_Mem.patterntable0;	// 背景pattern首地址
	y_scroll = y_axes + PPU_BG_VScrlOrg;	// Scorll
	// 位移后显示行的Y坐标
	if (y_scroll > 239)
	{
		y_scroll -= 240;
		nNameTable ^= NAME_TABLE_V_MASK;	// 垂直超出屏幕，切换name 
											// 
		// 
		// 
		// 
		// table表
	}
	y_TitleLine = y_scroll >> 3;	// title 行号 0~29（y轴坐标除8）
	dy_axes = y_scroll & 0x07;	// 除8的余数
	dx_axes = PPU_BG_HScrlOrg & 0x07;	// x轴偏移像素

	/* 先显示一行的左边部分,从第一个像素开始扫描 */
	Buffer_LineCnt = 8 - dx_axes;	// 缓存写入位置(0~
	// 255)，8是显示起始点
	/* x_TitleLine ~ 31 列像素显示（8bit一列） */
	for (x_TitleLine = PPU_BG_HScrlOrg >> 3; x_TitleLine < 32; x_TitleLine++)
	{							// 从左数第一个显示title单元开始
		BG_TitlePatNum = PPU_Mem.name_table[nNameTable][(y_TitleLine << 5) + x_TitleLine];	// y_TitleLine 
																							// 
		// 
		// 
		// 
		// * 
		// 32,当前显示的title号的 
		// 
		L_byte = BG_Patterntable[(BG_TitlePatNum << 4) + dy_axes];	// BG_TitlePatNum 
																	// 
		// 
		// 
		// 
		// * 16 +
		// dy_xaes
		H_byte = BG_Patterntable[(BG_TitlePatNum << 4) + dy_axes + 8];
		// 属性表 中查找 高两位颜色索引值 除4去余数再乘8
		// 除4 
		BG_attr_value = PPU_Mem.name_table[nNameTable][960 + ((y_TitleLine >> 2) << 3) + (x_TitleLine >> 2)];	// title对应的属性表8bit值
		// （title对应的高两位）（y title bit2 右移
		// 1位）(运算)或 （x title bit2）所得值 [000][010][100][110] 
		// 
		// 
		// 
		// 
		// 0 2 4 6为对应的attr 8bit[0:1][2:3][4:5][6:7]
		// 中的高两位颜色值
		BG_attr_value =
			((BG_attr_value >> (((y_TitleLine & 2) << 1) | (x_TitleLine & 2)))
			 & 3) << 2;
		/* x列每次扫描8像素显示 */
		for (i = 7; i >= 0; i--)
		{						// 先写左边像素的颜色
			// [1:0]低两位颜色索引值
			BG_color_num = BG_attr_value;
			BG_color_num |= (L_byte >> i) & 1;
			BG_color_num |= ((H_byte >> i) & 1) << 1;
			if (BG_color_num & 3)
			{					// 如果低两位为0，则为透明色,不写入
				Buffer_scanline[Buffer_LineCnt] =
					NES_Color_Palette[PPU_Mem.image_palette[BG_color_num]];
				Buffer_LineCnt++;
			}
			else
			{
				Buffer_LineCnt++;
			}
		}
	}
	/* 显示一行右边部分, 切换name table表 */
	nNameTable ^= NAME_TABLE_H_MASK;
	for (x_TitleLine = 0; x_TitleLine <= (PPU_BG_HScrlOrg >> 3); x_TitleLine++)
	{
		BG_TitlePatNum = PPU_Mem.name_table[nNameTable][(y_TitleLine << 5) + x_TitleLine];	// y_TitleLine 
																							// 
		// 
		// 
		// 
		// * 
		// 32,当前显示的title号的 
		// 
		L_byte = BG_Patterntable[(BG_TitlePatNum << 4) + dy_axes];
		H_byte = BG_Patterntable[(BG_TitlePatNum << 4) + dy_axes + 8];
		BG_attr_value = PPU_Mem.name_table[nNameTable][960 + ((y_TitleLine >> 2) << 3) + (x_TitleLine >> 2)];	// title对应的属性表8bit值
		BG_attr_value = ((BG_attr_value >> (((y_TitleLine & 2) << 1) | (x_TitleLine & 2))) & 3) << 2;	// 索引颜色[4:3]
		for (i = 7; i >= 0; i--)
		{
			BG_color_num = BG_attr_value;
			BG_color_num |= (L_byte >> i) & 1;
			BG_color_num |= ((H_byte >> i) & 1) << 1;
			if (BG_color_num & 3)
			{
				Buffer_scanline[Buffer_LineCnt] =
					NES_Color_Palette[PPU_Mem.image_palette[BG_color_num]];
				Buffer_LineCnt++;
			}
			else
			{
				Buffer_LineCnt++;
			}
		}
	}
}

/* 
 * 显示一个sprite的title 88
 */
void NES_RenderSprPattern(SpriteType * sprptr, uint8 * Spr_Patterntable,
						  uint16 title_addr, uint8 dy_axes)
{
	int i, dx_axes;
	uint8 Spr_color_num, H_byte, L_byte;

	if (((PPU_Reg.R1 & R1_SPR_LEFT8) == 0) && sprptr->x < 8)
	{							// 禁止左8列像素显示
		dx_axes = 8 - sprptr->x;
		if (dx_axes == 0)
			return;
	}
	else
	{
		dx_axes = 0;
	}
	if (sprptr->attr & SPR_VFLIP)
	{							// 若垂直翻转
		dy_axes = 7 - dy_axes;	// sprite 8*8显示dy_axes行
	}
	L_byte = Spr_Patterntable[title_addr + dy_axes];
	H_byte = Spr_Patterntable[title_addr + dy_axes + 8];
	if (sprptr->attr & SPR_HFLIP)
	{							// 若水平翻转
		for (i = 7; i >= dx_axes; i--)
		{						// 先写右边 颜色数据
			Spr_color_num = (L_byte >> i) & 1;	// bit0
			Spr_color_num |= ((H_byte >> i) & 1) << 1;	// bit1
			if (Spr_color_num == 0)
				continue;
			Spr_color_num |= (sprptr->attr & 0x03) << 2;	// bit23
			Buffer_scanline[sprptr->x + i + 8] = NES_Color_Palette[PPU_Mem.sprite_palette[Spr_color_num]];	// 偏移8
		}
	}
	else
	{
		for (i = 7; i >= dx_axes; i--)
		{						// 先写右边 颜色数据
			Spr_color_num = (L_byte >> (7 - i)) & 1;	// bit0
			Spr_color_num |= ((H_byte >> (7 - i)) & 1) << 1;	// bit1
			if (Spr_color_num == 0)
				continue;
			Spr_color_num |= (sprptr->attr & 0x03) << 2;	// bit23
			Buffer_scanline[sprptr->x + i + 8] = NES_Color_Palette[PPU_Mem.sprite_palette[Spr_color_num]];	// 写入颜色值到缓存

		}
	}
}

/* 
 * sprite 8*8 显示数据扫描
 */
void NES_RenderSprite88(SpriteType * sprptr, int dy_axes)
{
	uint8 *Spr_Patterntable;
	/* 取得所在title Pattern首地址 */
	Spr_Patterntable =
		(PPU_Reg.R0 & SPR_PATTERN_ADDR) ? PPU_Mem.patterntable1 : PPU_Mem.
		patterntable0;
	NES_RenderSprPattern(sprptr, Spr_Patterntable, sprptr->t_num << 4,
						 (uint8) dy_axes);
}

/* 
 * sprite 8*16 显示数据扫描
 */
void NES_RenderSprite16(SpriteType * sprptr, int dy_axes)
{
	if (sprptr->t_num & 0x01)
	{
		if (dy_axes < 8)		// sprite title 奇数号
			NES_RenderSprPattern(sprptr, PPU_Mem.patterntable1, (sprptr->t_num & 0xFE) << 4, (uint8) dy_axes);	// 上8*8
		else
			NES_RenderSprPattern(sprptr, PPU_Mem.patterntable1, sprptr->t_num << 4, (uint8) dy_axes & 7);	// 下8*8
	}
	else
	{
		if (dy_axes < 8)		// sprite title 偶数号
			NES_RenderSprPattern(sprptr, PPU_Mem.patterntable0, sprptr->t_num << 4, (uint8) dy_axes);	// 上8*8
		else
			NES_RenderSprPattern(sprptr, PPU_Mem.patterntable0, (sprptr->t_num | 1) << 4, (uint8) dy_axes & 7);	// 下8*8
	}
}

/* 
 * 填黑
 */
void clear(void)
{
	memset(Buffer_scanline, 0, sizeof(Buffer_scanline));
}

/* 
 * PPU 显示一行
 */
void NES_RenderLine(int y_axes)
{
	int16 i, render_spr_num, spr_size, dy_axes;


	PPU_Reg.R2 &= ~R2_LOST_SPR;	// 设置PPU状态寄存器R2 SPR
	// LOST的标志位


	if (PPU_Reg.R1 & (R1_BG_VISIBLE | R1_SPR_VISIBLE))
	{							// 若为假，关闭显示，填0黑
		/* 清空显示缓存，在此设置底背景色（待确定） */
		for (i = 7; i < 256; i++)
		{						// 显示区 7 ~ 263 0~7 263~270
			// 为防止溢出区
			Buffer_scanline[i] = NES_Color_Palette[PPU_Mem.image_palette[0]];
		}
		// clear();
		spr_size = PPU_Reg.R0 & R0_SPR_SIZE ? 0x0F : 0x07;	// spr_size
		// 8：0~7，16:
		// 0~15
		/* 扫描背景sprite并转换成显示数据写入到缓存,每一行最多只能显示8个Sprite 
		 */
		if (PPU_Reg.R1 & R1_SPR_VISIBLE)
		{						// 若开启sprite显示
			render_spr_num = 0;	// 清零显示计数器
			for (i = 63; i >= 0; i--)
			{					// 若重叠sprites 0
				// 具有显示最高优先级，其余优先级顺序次之，所以最先显示最低优先级
				/* 判断显示层（非） 背景 */
				if (!(sprite[i].attr & SPR_BG_PRIO))
					continue;	// (0=Sprite In front of BG, 1=Sprite Behind
				// BG)
				/* 判断显示位置 */
				dy_axes = y_axes - (uint8) (sprite[i].y + 1);	// 判断sprite是否在当前行显示范围内,sprite 
																// 
				// 
				// 
				// 
				// y
				// (FF,00,01,...EE)(0~239)
				if (dy_axes != (dy_axes & spr_size))
					continue;	// 若不在则返回继续循环查找下一个
				/* 若存在sprite在当前显示行,则转入下面显示阶段 
				 */
				render_spr_num++;	// 已显示的sprite的数目+1
				if (render_spr_num > 8)
				{				// 一行超过8个spreite，跳出循环
					PPU_Reg.R2 |= R2_LOST_SPR;	// 设置PPU状态寄存器R2的标志位
					break;
				}
				if (PPU_Reg.R0 & R0_SPR_SIZE)
				{				// 若为真，sprite的大小8*16
					NES_RenderSprite16(&sprite[i], dy_axes);
				}
				else
				{				// 若为假，sprite的大小8*8
					NES_RenderSprite88(&sprite[i], dy_axes);
				}
			}
		}

		/* 扫描背景 background */
		if (PPU_Reg.R1 & R1_BG_VISIBLE)
		{
			NES_RenderBGLine(y_axes);	// 扫描并设置Sprite
			// #0碰撞标志
		}

		/* 扫描前景sprite并转换成显示数据写入到缓存,每一行最多只能显示8个Sprite 
		 */
		if (PPU_Reg.R1 & R1_SPR_VISIBLE)
		{						// 若开启sprite显示
			render_spr_num = 0;	// 清零显示计数器
			/* 若重叠sprites 0
			   具有显示最高优先级，其余优先级顺序次之，所以最先显示最低优先级
			   备注：若前景sprites
			   优先级低于背景优先级，重叠的颜色，前景优先级低于背景优先级的话，前景将不会显示(暂未处理) 
			 */
			for (i = 63; i >= 0; i--)
			{
				/* 判断显示层 前景 */
				if (sprite[i].attr & SPR_BG_PRIO)
					continue;	// (0=Sprite In front of BG, 1=Sprite Behind
				// BG)
				/* 判断显示位置 */
				dy_axes = y_axes - ((int)sprite[i].y + 1);	// 判断sprite是否在当前行显示范围内,sprite 
															// 
				// 
				// 
				// 
				// y
				// (FF,00,01,...EE)(0~239)
				if (dy_axes != (dy_axes & spr_size))
					continue;	// 若不在则返回继续循环查找下一个
				/* 若存在sprite在当前显示行,则转入下面显示阶段 
				 */
				render_spr_num++;	// 已显示的sprite的数目+1
				if (render_spr_num > 8)
				{				// 一行超过8个spreite，跳出循环
					PPU_Reg.R2 |= R2_LOST_SPR;	// 设置PPU状态寄存器R2的标志位
					break;
				}
				if (PPU_Reg.R0 & R0_SPR_SIZE)
				{				// 若为真，sprite的大小8*16
					NES_RenderSprite16(&sprite[i], dy_axes);
				}
				else
				{				// 若为假，sprite的大小8*8
					NES_RenderSprite88(&sprite[i], dy_axes);
				}
			}
		}
	}
	else
	{
		clear();
	}
	/* 完成扫描，将行显示缓存写入LCD */

	if (SDLglue_MUSTLOCK(chuang))
	{
		if (SDLglue_LockSurface(chuang) < 0)
			return;
	}
	//NES_LCD_DisplayLine(y_axes, Buffer_scanline);
		 memcpy((char *)(LCDBUF + (y_axes * 240)), (char *)(Buffer_scanline +
	   8), 240 * 2); 
	if (SDLglue_MUSTLOCK(chuang))
		SDLglue_UnlockSurface(chuang);



	return;
}

/* 
 * PPU 将行缓存，写入LCD
 */


/*void NES_LCD_DisplayLine(int y_axes, uint16 * Disaplyline_buffer)
{
	uint16 index;
	uint16 j = 0;

	for (index = 8; index < 248; index++)
	{
		j = j % 257;
		LCDBUF[j + (240 * y_axes)] = (uint16) Buffer_scanline[index];
		j++;
	}

}
*/