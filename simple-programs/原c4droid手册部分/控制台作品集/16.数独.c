/*c4droid代码手册
*数独
*时间小殁 编写
*转载请说明出处
*/
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include<string.h>
#define H 9
void start();
void read();
void judge();
void change();
void end();
void possible();
void possiblex();
void affect();
void affectx();
void guess();
void decide();
int map[H][H], p[H][H][H+1];
int mapx[H][H], px[H][H][H+1];
int x,y,z,n,n0,m,s,x1,y1,z1,x2,y2,x3,y3,mx,my,ok;
int main()
{
printf("直接一个数字一个数字的输入就行了\n空格用0代表噢\n如果你不幸的输错了那样的话就只能重新打开了\nhaha\n来自时间小殁(1048276065)\n谨献给数独爱好者们\n");
	while (1)
	{
	start();
	}
}
	
void start()
{
	read();       /*读取输入然后存储待解决个数*/
	judge();
	/*创建三位数组，前二维代表位置，最后一维0用来储存可能填入数字个数，1~9不可能填入则负值0，将已知数对数组的影响修改三位数组*/
	A:;
	if (n!=0) guess();
	/*填入可能的数字*/
	if (n!=0&&ok!=n) goto A;
	end();
	}
	
void read()
{
	n=0;
	for (x=0;x!=H;x++)
	for (y=0;y!=H;y++)
	for (z=0;z!=H+1;z++)
	p[x][y][z]=1;
	for (x=0;x!=H;x++)
	for (y=0;y!=H;y++)
	{
    	m = getch();
		map[x][y]=m-48;
		printf("%d ", map[x][y]);
		if (y==8) printf("\n");
		if (map[x][y]==0) n++;
     }
     ok=n+1;
}

void judge()
{
	for (x=0;x!=H;x++)
	for (y=0;y!=H;y++)
	if (map[x][y]!=0) possible();
	for (x=0;x!=H;x++)
	for (y=0;y!=H;y++)
	{
		m=0;
		for (z=1;z!=H+1;z++)
		if (p[x][y][z]==1) m++;
		p[x][y][0]=m;
	 }
}

void possible()
{
	for (x1=0;x1!=H;x1++)
	{
		p[x][x1][map[x][y]]=0;
		p[x1][y][map[x][y]]=0;
	}
	mx=x/3;
	my=y/3;
	my=my*3;
	mx=mx*3;
	for (x2=0;x2!=3;x2++)
	for (y2=0;y2!=3;y2++)
	p[x2+mx][y2+my][map[x][y]]=0;
    for (z=0;z!=H+1;z++) p[x][y][z]=0;
    p[x][y][map[x][y]]=1;
}


/*void change()
{
	while (n!=0 && ok!=n)
	{
		ok = n;
    	for (x=0;x!=H;x++)
		for (y=0;y!=H;y++)
		{
			if (map[x][y]==0&&p[x][y][0]==1)
			{
				for (z=1;z!=H+1;z++)
				if (p[x][y][z]==1)
				map[x][y]=z;
				n--;
				affect();
			}
		}
		for (z=1;z!=H+1;z++)  !!!
	    for (x3=0;x3!=3;x3++)
	    for (y3=0;y3!=3;y3++)
	{
		m=0;
		mx=x3*3;
		my=y3*3;
		for (x2=0;x2!=3;x2++)
		for (y2=0;y2!=3;y2++)
		{
			if (p[x2+mx][y2+my][z]==1)
			{
				m++;
				x=x2+mx;
				y=y2+my;
			}
		}
		if (m==1)
		{
			map[x][y]=z;
			possible();
			n--;
		}
	}
	}
}

void possiblex()
{
	for (x1=0;x1!=H;x1++)
	{
		if (mapx[x][x1]==0) px[x][x1][map[x][y]]=0;
		if (mapx[x1][y]==0) px[x1][y][map[x][y]]=0;
	}
	mx=x/3;
	my=y/3;
	my=my*3;
	mx=mx*3;
	for (x2=0;x2!=3;x2++)
	{
		for (y2=0;y2!=3;y2++)
		{
			if (mapx[x2+mx][y2+my]==0) px[x2+mx][y2+my][map[x][y]]=0;
         }
	}
}
*/

void guess()
{
	ok=n;
	for (n0=1;n0!=H+1;n0++)
	for (x=0;x!=H;x++)
	for (y=0;y!=H;y++)
	if (p[x][y][0]==n0&&map[x][y]==0)
	{
		for (z=1;z!=H+1;z++)
		if (p[x][y][z]==1)
		{
			for (x1=0;x1!=H;x1++)
	        for (y1=0;y1!=H;y1++)
	        {
	        	mapx[x1][y1]=map[x1][y1];
	        	for (z1=0;z1!=H+1;z1++)
	        	px[x1][y1][z1]=p[x1][y1][z1];
	        }
	        mapx[x][y]=z;
	        affectx();
	        decide();
	        goto B;
		}
	}
	B:;
}

void affect()
{
	for (x1=0;x1!=H;x1++)
	{
		if (p[x][x1][map[x][y]]==1&&x1!=y)
		{
			p[x][x1][map[x][y]]=0;
			p[x][x1][0]--;
		}
		if (p[x1][y][map[x][y]]==1&&x1!=x)
		{
			p[x1][y][map[x][y]]=0;
			p[x1][y][0]--;
		}
	}
	mx=x/3;
	my=y/3;
	my=my*3;
	mx=mx*3;
	for (x2=0;x2!=3;x2++)
	for (y2=0;y2!=3;y2++)
	if (p[x2+mx][y2+my][map[x][y]]==1&&x2+mx!=x&&y2+my!=y)
	{
		p[x2+mx][y2+my][map[x][y]]=0;
		p[x2+mx][y2+my][0]--;
	}
}


void affectx()
{
	for (x1=0;x1!=H;x1++)
	{
		if (px[x][x1][mapx[x][y]]==1&&x1!=y)
		{
			px[x][x1][mapx[x][y]]=0;
			px[x][x1][0]--;
		}
		if (px[x1][y][mapx[x][y]]==1&&x1!=x)
		{
			px[x1][y][mapx[x][y]]=0;
			px[x1][y][0]--;
		}
	}
	mx=x/3;
	my=y/3;
	my=my*3;
	mx=mx*3;
	for (x2=0;x2!=3;x2++)
	for (y2=0;y2!=3;y2++)
	if (px[x2+mx][y2+my][mapx[x][y]]==1&&x2+mx!=x&&y2+my!=y)
	{
		px[x2+mx][y2+my][mapx[x][y]]=0;
		px[x2+mx][y2+my][0]--;
	}
}

void decide()
{
	for (z1=1;z1!=H+1;z1++)
	{
		for (x1=0;x1!=H;x1++)
		{
			m=0;
			for (y1=0;y1!=H;y1++)
			{
				if (px[x1][y1][z]==1) m++;
			}
			if (m==0)
			{
				p[x][y][z]=0;
				p[x][y][0]--;
				goto mark;
			}
		}
		for (x1=0;x1!=H;x1++)
		{
			m=0;
			for (y1=0;y1!=H;y1++)
			{
				if (px[y1][x1][z]==1) m++;
			}
			if (m==0)
			{
				p[x][y][z]=0;
				p[x][y][0]--;
				goto mark;
			}
		}
			for (x1=0;x1!=3;x1++)
	        for (y1=0;y1!=3;y1++)
	        {
	        	m=0;
	        	mx=x1*3;
	        	my=y1*3;
	        	for (x2=0;x2!=3;x2++)
	            for (y2=0;y2!=3;y2++)
	            {
	            	if (px[x2+mx][y2+my][z]==1)
	            	m++;
	            }
	            if (m==0) 
	            {
	            	p[x][y][z]=0;
	            	p[x][y][0]--;
	            	goto mark;
	            }
	        }
	        }
	        for (x1=0;x1!=H;x1++)
	        for (y1=0;y1!=H;y1++)
	        for (z1=0;z1!=H+1;z1++)
	        p[x1][y1][z1]=px[x1][y1][z1];
	        map[x][y]=z;
	        affect();
	        n--;
	        mark:;
}

void end()
{
	if (ok==0) printf("No\n");
		printf("~~~~~~~~~~~~~~~\n");
		for (x=0;x!=H;x++)
		for (y=0;y!=H;y++)
		{
			printf("%d ", map[x][y]);
			if (y== H-1) printf("\n");
		}
		printf("~~~~~~~~~~~~~~~\n");
}