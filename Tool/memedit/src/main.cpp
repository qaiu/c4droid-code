#include "memedit00.h"
/*
1 Cb	REGION_C_BSS=1 ,
2 Ch	REGION_C_HEAP ,
3 Ca	REGION_C_ALLOC,
4 Cd	REGION_C_DATA,
5 S 	REGION_STACK,
6 B 	REGION_BAD,
7 JH	REGION_JAVA_HEAP, (慢)
8 A 	REGION_ANONY,
9 Xa	REGION_CODE_APP,
10Xs    REGION_CODE_SYS,
*/
int main(int argc, char **argv)
{
	kk::init(argv, "com.tencent.mobileqq");
	kk::set_ranges(8,0);
	
	int sum;
	
	sum = kk::search_number("123",DWORD,0);
	kk::print_addr_value(DWORD,WORD,FLOAT,DOUBLE,HEX_L,0);//打印地址的值
		
	printf("搜索完成，找到%d个\n",sum);
	sum= kk::search_number_range("1409349037",DWORD,0x4);
	kk::print_addr_value(DWORD,WORD,FLOAT,DOUBLE,HEX_L,0);//打印地址的值	
	printf("搜索完成，找到%d个\n",sum);
	
//	kk::clear_results();
//	kk::edit_all("66668",DWORD,0,0,1);
//	while(1) usleep(100000000);
//	kk::write_addr("888",DWORD,0xB9178C44);
	
	int k=0;
	kk::read_addr(&k,DWORD,0xB9178C44);
	printf("%d\n",k);
	getchar();
}