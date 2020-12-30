/*c4droid代码手册
 *跑酷
 *be148TTN编写
 *TTHHR收集整理
 *转载请说明出处
*/
//参考自  啊o额iu鱼前辈  的贪吃蛇

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#define up '.'
#define down '/'
#define left ':'
#define right '@'
#define WALL 1
#define NTH 0
#define P 2
#define S 3
#define SPEED_P 2
#define SPEED_M 4
#define JUMP 3
#define W 20
#define H 16
#define T 100
void playermove();
void mapmove();
void draw();
void Init();
int x,y;
int map[H][W];
int mapp;
int rise;
bool over;
int main()
{
	re:long s = 0;
	Init();
	for(int i = 0;x < 0 || x == W || y < 0 || y == H || over;i++)
	{
		if(i % SPEED_M == 0)
		    mapmove();
		if(i % SPEED_P == 0)
		    playermove();
		draw();
		if(i % T == 0)
		{
			s++;
			i = 0;
		}
	}
	printf("YOUR SROCE:%ld",s);
	getch();
	getch();
	goto re;
	return 0;
}
void Init()
{
	for(int i = 0;i != H;i++)
	    for(int t = 0;t != W; t++)
	        if(i == H - 1)
	            map[i][t] = WALL;
	        else
	            map[i][t] = NTH;
	y = H - 2;
	x = 1;
	map[y][x] = P;
	over = 1;
	mapp = H - 1;
}
void mapmove()
{
	for(int i = 0;i != H;i++)
	    for(int t = 0;t != W;t++)
	        if(t == W - 1)
	            map[i][t] = NTH;
	        else 
	            map[i][t] = map[i][t + 1];
	for(int i = 0,k = 0;i != H;i++)
	{
		if(i == mapp)
		{
			map[i][W - 1] = WALL;
			if(random() % W % 3 == 0)
			    mapp--;
			if(random() % H % 2 == 0)
			    mapp++;
			if(mapp >= H || mapp < 0 || mapp - y > JUMP -1)
			    mapp = H - 1;
			if(time(0) % 3 == 0)
			    k++;
		}
		if(k == 2)
		    break;
	}
	map[y][x - 1] = NTH;
	if(map[y][x] == NTH)
	    map[y][x] = P;
	else 
	    over = 0;
}
void playermove()
{
	int xb = x,yb = y;
	char go = 0;
	if(kbhit())
	    go = getch();
	switch(go)
	{
		case up:
		    if(map[y + 1][x] == WALL)
		        rise = JUMP;
		    break;
		case down:
		    yb++;
		    rise = 0;
		    break;
		case left:
		    xb--;
		    break;
		case right:
		    xb++;
		    break;
	}
	map[y][x] = NTH;
	if(map[yb][xb] == NTH)
	{
		y = yb;
		x = xb;
	}
	if(rise)
	{
		rise--;
		if(map[y - 1][x] == WALL)
		    rise = 0;
		else
		    y--;
	}
	else if(map[y + 1][x] == NTH)
	    y++;
	map[y][x] = P;
	while(kbhit())
	    getch();
}
void draw()
{
	for(int i = 0;i != H;i++)
	{
		for(int t = 0;t != W;t++)
		    switch(map[i][t])
		    {
		    	case WALL:
		    	    printf("\33[47;36m囍\33[40;37m");
		    	    break;
		    	case NTH:
		    	    printf("  ");
		    	    break;
		    	case P:
		    	    printf("\33[41;33m㈱\33[40;37m");
		    	    break;
		    }
		putchar('\n');
	}
	usleep(100000 / S);
	clrscr();
}