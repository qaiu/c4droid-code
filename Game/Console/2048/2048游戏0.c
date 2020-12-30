#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#define Key_ARROW '0'
#define Key_Up '2'
#define Key_Down '8'
#define Key_Left '4'
#define Key_Right '6'

void Print(int [5][5]);
void reloading(int [5][5]);
void Move_Up(void);
void Move_Down(void);
void Move_Left(void);
void Move_Right(void);
void WinOrNot(void);

int block[5][5];

void Move(int key)
{
	WinOrNot();
	switch(key)
	{
		case Key_Up:Move_Up();break;
		case Key_Down:Move_Down();break;
		case Key_Left:Move_Left();break;
		case Key_Right:Move_Right();
	}
}

int main(void)
{
	int arrow; 
	int key;
	srand((unsigned)time(NULL));
	memset(block,0,100);
	reloading(block);
	reloading(block);
	while(1)
	{
		WinOrNot();
		Print(block);
		arrow=getch();
		key=getch();
		if(arrow=Key_ARROW)
		{
			WinOrNot();
			Move(key);
		}
	}
	return 0;
}

void Print(int a[5][5])
{	
	int i,j;
	clrscr();
	for (i=0;i<5;++i)
	{
		for (j=0;j<5;++j)
		{
			if(a [i][j]!=0)
				printf("%4d|",a[i][j]);
			else
				printf("    |");
		}
		printf("\n");
	}
}

void reloading(int a[5][5])
{
	int num1,num2,seed;
	num1=rand()%5;
	num2=rand()%5;
	seed=rand()%2;
	if(seed==0 && a[num1][num2]==0)
	{
		a[num1][num2]=2;
	}
	else if(seed==1 && a[num1][num2]==0)
	{
		a[num1][num2]=4;
	}
	else
	{
		WinOrNot();
		reloading(block);
	}
}

void Move_Up(void)
{
	int i,j;
	for(i=0;i<5;++i)
	{
		for(j=0;j<5;++j)
		{
			if(block[i][j]==0)
			{
				int temp;
				for(temp=i+1;temp<5;++temp)
				{
					if(block[temp][j]!=0)
					{
						block[i][j]=block[temp][j];
						block[temp][j]=0;
						break;
					}
				}
			}
		}
	}
	for(i=0;i<5;++i)
	{
		for(j=0;j<5;++j)
		{
			if(block[i][j]==block[i+1][j])
			{
				block[i][j]+=block[i][j];
				block[i+1][j]=0;
			}
		}
	}
	WinOrNot();
	reloading(block);
}
void Move_Down(void)
{
	int i,j;
	for(i=4;i>=0;--i)
	{
		for(j=4;j>=0;--j)
		{
			if(block[i][j]==0)
			{
				int temp;
				for(temp=i-1;temp>=0;--temp)
				{
					if(block[temp][j]!=0)
					{
						block[i][j]=block[temp][j];
						block[temp][j]=0;
						break;
					}
				}
			}
		}
	}
	for(i=4;i>=0;--i)
	{
		for(j=4;j>=0;--j)
		{
			if(block[i][j]==block[i-1][j])
			{
				block[i][j]+=block[i][j];
				block[i-1][j]=0;
			}
		}
	}
	WinOrNot();
	reloading(block);
}
void Move_Left(void)
{
	int i,j;
	for(j=0;j<5;++j)
	{
		for(i=0;i<5;++i)
		{
			if(block[i][j]==0)
			{
				int temp;
				for(temp=j+1;temp<5;++temp)
				{
					if(block[i][temp]!=0)
					{
						block[i][j]=block[i][temp];
						block[i][temp]=0;
						break;
					}
				}
			}
		}
	}
	for(j=0;j<5;++j)
	{
		for(i=0;i<5;++i)
		{
			if(block[i][j]==block[i][j+1])
			{
				block[i][j]+=block[i][j];
				block[i][j+1]=0;
			}
		}
	}
	WinOrNot();
	reloading(block);
}
void Move_Right(void)
{
	int i,j;
	for(j=4;j>=0;--j)
	{
		for(i=4;i>=0;--i)
		{
			if(block[i][j]==0)
			{
				int temp;
				for(temp=j-1;temp>=0;--temp)
				{
					if(block[i][temp]!=0)
					{
						block[i][j]=block[i][temp];
						block[i][temp]=0;
						break;
					}
				}
			}
		}
	}
	for(j=4;j>=0;--j)
	{
		for(i=4;i>=0;--i)
		{
			if(block[i][j]==block[i][j-1])
			{
				block[i][j]+=block[i][j];
				block[i][j-1]=0;
			}
		}
	}
	WinOrNot();
	reloading(block);
}

void WinOrNot(void)
{
	int i,j,temp=0;
	for (i=0;i<5;++i)
	{
		for(j=0;j<5;++j)
		{
			if(block[i][j]!=0)
			{
				temp+=1;
			}
			if(block[i][j]==2048)
			{
				printf("You win!\n");
				system("pause");
				exit(0);
			}
		}
	}
	if(temp>=25)
	{
		printf("You lose!\n");
		system("pause");
		exit(0);
	}
}