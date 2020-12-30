/*
例程33. 扑克牌结构表示
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
2017/10/21
*/
#include <conio.h>
enum suits{
	CLUBS= 0
,DIAMONDS,HEARTS,SPADES};
struct card
{
	int suit; //花色
	char value[3]; //牌面数值
};
struct card deck[52];
char cardval[][3]={"A","2","3","4","5","6","7","8","9","10","J","Q","K"};
char suitsname[][9]={"CLUBS","DIAMONDS","HEARTS","SPADES"};

int main()
{
	int i,j;
	int s;
	clrscr();
	for(i=0;i<=12;i++)
		for(s=0;s<4;s++)
		{
			j=i*4+s;
			deck[j].suit=s;
			strcpy(deck[j].value,cardval[i]);
		}
	for(j=0;j<52;j++)
		printf("(%s%3s)%c",suitsname[deck[j].suit],deck[j].value,j%4==3?'\n':'\t');
	puts("\nPress any key to quit...");
	getch();
	return 0;
}