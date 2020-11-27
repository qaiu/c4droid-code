/*
例程207. 商人过河游戏
整理优化by:千百度QAIU
QQ:736226400
编译环境:gcc/tcc
2017/10/24
*/


#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include "viGL.h"
void dwframe()
{
	COLOR col1={1,1,0,5,4,5};
	COLOR col2={0,3,4};
	
	printf("\e[41m");
	clrscr();
	drawbox(1,1,28,11,col1);
	drawrect2(2,13,58,10,col2);
	drawtext("\t^_^欢迎使用商人过河游戏！^_^\n \n游戏说明:\n三个商人带着三个随从和货物过河，船每次最多只能载两个人\n要求在过河期间，任一岸上商人的人数要大于或等于随从的人\n数，否则随从会杀掉商人抢走货物。\n \n	游戏操作：（1）依次输入过河的商人和随从人数；\n	          （2）非法输入按键将重新开始。",2,2,{1,1,0,5,4,5});
}

char a[6];
char b[6];
void printcase(char a[],char b[])
{
	int i,j,xa,xb,x0,ya,yb,y0;
 	xa=xb=x0=ya=yb=y0=0;
 	gotoxy(11,14);
 	printf("\e[42;34m此岸                彼岸\n");
  	for(i=0;i<6;i++) 
  	{
	   	if(a[i]=='A')
	    		xa++;
	   	else if (a[i]=='B')
	    		xb++;
	   	else if(a[i]=='0')
	    		x0++;
  	}
  	gotoxy(11,15);
  	for(i=1;i<=xa;i++) 
  		printf("\e[46;30m商人  \e[0m");
 	gotoxy(11,16);
  	for(i=1;i<=xb;i++)
  		printf("\e[46;35m随从  \e[0m");
 	for(j=0;j<6;j++)
 	{
	  	if(b[j]=='A')
	   		ya++;
	  	else if(b[j]=='B')
	   		yb++;
	 	else if(b[j]=='0')
	   		y0++;
 	}
	gotoxy(31,15);
	for(j=1;j<=ya;j++)
	  	printf("\e[46;30m商人 \e[0m");
	gotoxy(31,16);
	for(j=1;j<=yb;j++)
	  	printf("\e[46;35m随从 \e[0m");
	if(xa==0 && xb==0 && ya==3 && yb==3)
	{
	  	gotoxy(15,5);
	  	printf("\e[42;30m恭喜你！你已成功帮助商人安全过河！按任意键退出...");
	  	getch();
	  	printf("\e[41m");
  		clrscr();
  		printf("谢谢使用(*°∀°)=3");
	  	exit(0);
	}
}

int main()
{
	int i,x,y,key,ca,cb,j,aA,aB,bA,bB;
	char ch;
Start:	
	dwframe();
	gotoxy(15,16);
	printf("\e[42;30m请按任意键开始游戏（按Q键退出）...");
	ch=getch();
	if(ch=='Q'||ch=='q')
	{
		printf("\e[41m");
		clrscr();
		printf("谢谢使用(*°∀°)=3");
		exit(0);
	}
	dwframe();
	 
	for(i=0;i<3;i++)
		a[i]='A';
	for(i=3;i<6;i++)
		a[i]='B';
	for(i=0;i<6;i++)
		b[i]='0';
	printcase(a,b);
	while(1)
	{
		gotoxy(20,18);
		printf("\e[42;34m输入去彼岸商人的人数： ");
		scanf("%d%d",&x,&y);
		for(ca=0,cb=0,i=0;i<6;i++) 
		{
			if(a[i]=='A')
	    			ca++;
			else if(a[i]=='B')
	    			cb++;
		}
		if(x<0 || x>ca || y<0 || y>cb || x+y<1 ||x+y>2) 
		{
			gotoxy(15,21);
			printf("\e[41;37m输入有误！！按任意键重新开始...\e[0m");
			setbuf(stdin,NULL);
			getch();
			goto Start;
		}
		for(i=1;i<=x;i++) 
		{
	  		for(j=0;j<6;j++)
	   			if(a[j]=='A') 
	   			{
	    				a[j]='0';
	    				break;
	   			}
		}
		for(i=1;i<=x;i++) 
		{
			for(j=0;j<6;j++)
	   			if(b[j]=='0') 
	   			{
	    				b[j]='A';
	       				break;
	   			}
		}
		for(i=1;i<=y;i++) 
		{
	  		for(j=0;j<6;j++)
	   			if(a[j]=='B') 
	   			{
	    				a[j]='0';
	    				break;
	   			}
		}
		for(i=1;i<=y;i++) 
		{
	  		for(j=0;j<6;j++)
	   			if(b[j]=='0') 
	   			{
	    				b[j]='B';
	    				break;
				}
		}
		for(aA=0,aB=0,i=0;i<6;i++) 
		{
	  		if(a[i]=='A')
	   			aA++;
	  		else if(a[i]=='B')
	   			aB++;
		}
		for(bA=0,bB=0,i=0;i<6;i++) 
		{
	  		if(b[i]=='A')
	   			bA++;
	  		else if(b[i]=='B')
	   			bB++;
		}   
	 
		if((aA==3) ||(bA==3) || (aA==aB) || (bA==bB))  
		{
	      		dwframe();
	      		printcase(a,b);
	         }
		else
		{
			gotoxy(15,21);
			printf("\e[41;37m商人被杀，按任意键重新开始...\e[0m");
			setbuf(stdin,NULL);
			getch();
			goto Start;
		}
		gotoxy(20,18);
		printf("\e[47;31m输入回此岸商人的人数： ");
		scanf("%d%d",&x,&y);
		for(ca=0,cb=0,i=0;i<6;i++)
		{
	  		if(b[i]=='A')
	   			ca++;
	  		else if(b[i]=='B')
	   			cb++;
		}
		if(x<0 || x>ca || y<0 || y>cb || x+y<1 || x+y>2) 
		{
			gotoxy(15,21);
	   		printf("输入有误！！按任意键重新开始...");
	   		getch();
	  		goto Start;
		}
		for(i=1;i<=x;i++) 
		{
	  		for(j=0;j<6;j++)
	   			if(b[j]=='A') 
	   			{
	    				b[j]='0';
	    				break;
	   			}
		}
		for(i=1;i<=x;i++) 
		{
	  		for(j=0;j<6;j++)
	   			if(a[j]=='0') 
	   			{
	    				a[j]='A';
	    				break;
				}
		}
		for(i=1;i<=y;i++) 
		{
	  		for(j=0;j<6;j++)
	   		if(b[j]=='B') 
	   		{
	    			b[j]='0';
	    			break;
	   		}
		}
		for(i=1;i<=y;i++) 
		{
	  		for(j=0;j<6;j++)
	   		if(a[j]=='0') 
	   		{
	    			a[j]='B';
				break;
	   		}
		}
		for(aA=0,aB=0,i=0;i<6;i++) 
		{
	  		if(a[i]=='A')
	   			aA++;
	  		else if(a[i]=='B')
	   			aB++;
		}
		for(bA=0,bB=0,i=0;i<6;i++) 
		{
	  		if(b[i]=='A')
	   			bA++;
	  		else if(b[i]=='B')
	   			bB++;
		}
	 	if((aA==3) ||(bA==3) || (aA==aB) || (bA==bB))
	 	{
	  		dwframe();
	  		printcase(a,b);
		}
		else 
		{
 	  		gotoxy(15,21);
	   		printf("商人被杀，按任意键重新开始...");
	   		setbuf(stdin,NULL);
	        	getch();
	        	goto Start;
	  	}
	}
}