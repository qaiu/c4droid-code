/**
 * 安卓kk内存编辑器(memedit00)V1.5.3
 * Author: QAIU
 * Create: 19.07.10
 * Update: 19.8.22
 * --------------
 * QQ: 736226400
 * blog: qaiu.top
**/


#ifndef MEMEDIT00
#define MEMEDIT00

#include <stdio.h>
enum
{
	REGION_C_BSS = 1, //  1. Cb
	REGION_C_HEAP,    //  2. Ch
	REGION_C_ALLOC,   //  3. Ca
	REGION_C_DATA,	//  4. Cd
	REGION_STACK,	 //  5. S
	REGION_BAD,	   //  6. B (V内存)
	REGION_JAVA_HEAP, //  7. Jh (慢)
	REGION_ANONY,	 //  8. A
	REGION_CODE_APP,  //  9. Xa
	REGION_CODE_SYS,  // 10. Xs
};

enum
{
	// 支持的搜索和打印的类型
	DWORD = 1,		// 4字节整数
	WORD,			 // 2字节整数
	QWORD,			// 8字节长整数
	FLOAT,			// 单精度浮点数
	DOUBLE,	   	// 双精度浮点数
	BYTE,		 	// 单字节数byte
	HEX_L,			// 小端16进制
	HEX_B		 	// 大端16进制
};

enum
{
	// 支持root和免root两种模式
	// 需要在框架中给二进制执行权限
	// 如 chmod +x 二进制文件名
	MODE_ROOT = 1,
	MODE_NO_ROOT
};

//------------------------------------------------------------
/* API调用方法:
 * kk::函数名();
 * 例如: 初始化函数 kk::init(argv,"包名");
 * 输入kk::系统会自动提示补全，如果全局不想输入kk::前缀需要在源码main开头加入
 * using namespace kk;
 * 即可直接调用 init(argv,"包名");
 */
//------------------------------------------------------------
namespace kk
{
	
/* 
 * 函数：init
 * 功能：初始化，获取root，屏蔽监视过保护(防闪退)
 * 参数 
		char**argv：必须是main函数第二个参数用于获取root
		char*str： 应用或游戏包名用于获取pid
 * 注意：应用或游戏未运行时会初始化失败，程序将自动退出
 */
void init(char **argv, const char *str);
//------------------------------------------------------------


/*
 * 函数：set_ranges
 * 功能：设置内存范围
 * 参数：参数范围枚举，或者对应数值(至少设置1个参数，必须以0作为最后一个参数)
 * 返回：空
 * 示例1 kk::set_rangers(1,2,3,0);
 * 示例2 kk::set_rangers(REGION_C_BSS,REGION_C_HEAP,REGION_C_ALLOC,0); 
 */
void set_ranges(int arg, ...);
//------------------------------------------------------------

/* 
 * 函数：search_number
 * 功能：搜索内存数据
 * 参数：
        1.value_str： 要搜索的数值字符串"1234"、"3.112"，如果搜索指定范围的数值请使用~表示范围比如"12~15"
        2.type: 搜索的数据类型DWORD FLOAT DOUBLE等等，具体参考头文件指定的类型
        3.offset: 偏移量,非首次搜索时有效
 * 返回：搜索到的数据个数
 * 示例1： kk::search_number("123",DWORD,0);搜索DWORD(int)类型数123
 * 示例2： kk::search_number("2~5",FLOAT,0);首次搜索：范围搜索浮点数(2～5)
 * 示例3： kk::search_number("3~4",FLOAT,0);非首次搜索：搜索浮点范围(3～4) 可以看做是缩小范围
 * 示例4： kk::search_number("123",DWORD,0x10);非首次搜索：偏移搜索DWORD类型123，偏移量为0x10
 * 示例5： kk::search_number("123~125",DWORD,0x10);非首次搜索：偏移搜索DWORD范围(123～125)，偏移量为0x10
 */
int search_number(const char *value_str, int type, int offset);
//------------------------------------------------------------

/* 
 * 函数：edit_all
 * 功能：写入数据到搜索到的地址里
 * 参数：
		1.value_str写入数据的字符串
		2.写入的数据类型
		3.int sum 写入的数据个数范围(为0表示全部写入)，即写入前sum个数据到内存
		4.int offset偏移写入的偏移量(为0表示不偏移)
		5.bool is_freeze 是否冻结(0表示不冻结,1表示冻结)
 * 返回：空
 * 示例1.kk::edit_all("123", DWORD,0,0,0);//写入DWORD值"123", DWORD到搜索到的全部地址，不冻结
 * 示例2.kk::edit_all("123", DWORD,0,0x10,0);//偏移写入DWORD值"123", DWORD到搜索到的全部地址，偏移量为0x10，不冻结
 * 示例3.kk::edit_all("123", DWORD,10,0,0);//写入DWORD值"123", DWORD到搜索到的前10个地址，不冻结
 * 示例4.kk::edit_all("123", DWORD,10,0x8,1);//偏移写入DWORD值"123", DWORD到搜索到的前10个地址，偏移量为0，写入后冻结
 */
void edit_all(const char *value_str, int type, int sum, int offset, bool freeze);
//------------------------------------------------------------

/* 
 * 函数：search_number_range
 * 功能：对于非首次搜索，搜索指定内存范围的数据
 * 参数：
        1.value_str：要搜索的数值，由模板类型指定下同
		2.offset 内存范围的偏移，不宜过大，最好0x800(4096)之内
 * 返回：搜索到的数据个数
 * 示例1：kk::search_number_range("123",DWORD,0x100);//基于上次的搜索结果，在向后偏移256字节的范围内搜索DWORD值123
 * 示例2：kk::search_number_range("2",DOUBLE,0x100);//基于上次的搜索结果，在向后偏移256字节的范围内搜索DOUBLE值2
 */
int search_number_range(const char *value_str, int type, int16_t offset);


/* 
 * 函数：clear_results
 * 功能：清除搜索结果
 * 返回值和参数均为空
 * 说明：如果要进行多次搜索则每次开始新的搜索前都要调用此函数清空上一次搜索的结果
 */
void clear_results();

//设置冻结时间
void set_freeze_time(long tim);
//------------------------------------------------------------

//清空冻结列表(取消所有冻结)
void clear_freeze_results();
//------------------------------------------------------------

/* 
 * 函数：print_addr_value
 * 功能：打印搜索结果的数值可以为多各类型
 * 参数：数据类型枚举列表至少设置1个参数，必须以0作为最后一个参数)
 * 示例1： kk::print_addr_value(DWORD,WORD,FLOAT,DOUBLE,HEX_L,0);//打印地址的值
 * 示例2： kk::print_addr_value(DWORD,0) //参数必须以0结束
 */
void print_addr_value(int type,...);
//------------------------------------------------------------

/*
 * 函数: printf_addr
 * 功能: 打印搜索结果(仅地址)
 */
void print_addr();
//------------------------------------------------------------

/*
 * 函数名: write_addr;
 * 功能: 指定地址写入
 * 参数: 写入数值的字符串，数据类型(DWORD等)，写入地址
 * 示例: kk::write_addr("123756",DWORD,0xF64431EA);
 */
void write_addr(const char *value_str, int type, long addr);
//------------------------------------------------------------

/*
 * 函数名: write_addr;
 * 功能: 指定地址读取
 * 参数: 读取后保存的变量指针，数据类型(DWORD等)，读取地址
 * 示例: kk::write_addr(&buf,DWORD,0xF64431EA);
 */

void read_addr(void* val, int type, long addr);

//间隔写入，参数:写入的值，写入类型，偏移
void edit_interval(const char*value_str, int type, int offset);

//设置间隔时间，参数:间隔写入时间，间隔恢复时间
void set_interval_time(long write_tim,long restore_tim);

//清除间隔写入	
void clear_interval_results();

//输出重定向到文件
//参数: 文件名(绝对路径)
//void print_file(const char *filename);

//输出重定向到终端
//void print_stdout();


}//namespace kk end

#endif