/*c4droid代码手册
 *推箱子
 *2002a1116 编写
 *TTHHR收集整理
 *转载请说明出处
*/
#include<iostream>
#include<stdlib.h>
#include<conio.h>
using namespace std;
const int h=12,w=18;
int wo[h][w];
int ph=h/2-1,pw=w/2-1;
int phl,pwl;
int *p=&wo[h/2-1][w/2-1];
int *pl;
int world[h][w];
const int bo=4;
int *box[bo];
int *boxl[bo];
int *boer[bo];
int *boerl[bo];
int foot=0;
bool a=1;
int gett=0;
void out();
void get()
{
	int a,b;
	do
	{
		a=random()%h-1;
	b=random()%w-1;
	}while(wo[a][b]!=0);
	wo[a][b]=7;
}
void worl()
{
	if(a!=1)
	{
	for(int i=0;i<h;++i)
	{
		for(int f=0;f<w;++f)
		{
			wo[i][f]=world[i][f];
		}
	}
	p=pl;
	for(int o=0;o<bo;o++)
	{
		box[o]=boxl[o];
		boer[o]=boerl[o];
	}
	foot--;
	ph=phl;
	pw=pwl;
	}
}
void woo()
{
	for(int i=0;i<h;++i)
	{
		for(int f=0;f<w;++f)
		{
			world[i][f]=wo[i][f];
		}
	}
	pl=p;
	for(int o=0;o<bo;o++)
	{
		boxl[o]=box[o];
		boerl[o]=boer[o];
	}
	phl=ph;
	pwl=pw;
}
bool did()
{
	for(int i=0;i<bo;i++)
	{
	      if(*boer[i]==0)
	      {
	      	*boer[i]=5;
	      }
	      if(*boer[i]==4)
	      *boer[i]=3;
	}
	for(int f=0;f<bo;f++)
	{
	      if(*boer[f]!=3)
	      {
	      	return true;
	      }
	}
	return false;
}
bool pm()
{
	char b;
	b=getch();
	int i=0;
	if(b=='@')
	{
		worl();
		return false;
	}
	woo();
	switch(b)
	{
		case '2':
        ph--;
        for(int i=0;i<bo;i++)
        {
        if(&wo[ph][pw]==box[i])
        {
        	if(wo[ph-1][pw]==4||wo[ph-1][pw]==3||wo[ph-1][pw]==7)
        	{
        		ph++;
        		return false;
        	}
        	if((ph-1)>=0)
        	{
        		*box[i]=0;
        		box[i]=&wo[ph-1][pw];
        		*box[i]=4;
        	}
        	else
        	{
        	ph++;
        		return false;
        	}
        }
        }
        if(ph>=0&&wo[ph][pw]!=7)
        {
        	*p=0;
        	p=&wo[ph][pw];
        	*p=1;
        	foot++;
        	a=0;
        }
        else 
        ph++;
        break;
        case '4':
       pw--;
        for(int i=0;i<bo;i++)
        {
        if(&wo[ph][pw]==box[i])
        {
        	if(wo[ph][pw-1]==4||wo[ph][pw-1]==3||wo[ph][pw-1]==7)
        	{
        		pw++;
        		return false;
        	}
        	if((pw-1)>=0)
        	{
        		*box[i]=0;
        		box[i]=&wo[ph][pw-1];
        		*box[i]=4;
        	}
        	else
        	{
        	pw++;
        		return false;
        	}
        }
        }
        if(pw>=0&&wo[ph][pw]!=7)
        {
        	*p=0;
        	p=&wo[ph][pw];
        	*p=1;
        	foot++;
        	a=0;
        }
        else
        pw++;
        break;
        case '6':
        pw++;
        for(int i=0;i<bo;i++)
        {
        if(&wo[ph][pw]==box[i])
        {
        	if(wo[ph][pw+1]==4||wo[ph][pw+1]==3||wo[ph][pw+1]==7)
        	{
        		pw--;
        		return false;
        	}
        	if((pw+1)<w)
        	{
        		*box[i]=0;
        		box[i]=&wo[ph][pw+1];
        		*box[i]=4;
        	}
        	else
        	{
        	pw--;
        		return false;
        	}
        }
        }
        if(pw<w&&wo[ph][pw]!=7)
        {
        	*p=0;
        	p=&wo[ph][pw];
        	*p=1;
        	++foot;
        	a=0;
        }
        else
        pw--;
        break;
        case '8':
        ph++;
        for(int i=0;i<bo;i++)
        {
        if(&wo[ph][pw]==box[i])
        {
        	if(wo[ph+1][pw]==4||wo[ph+1][pw]==3||wo[ph+1][pw]==7)
        	{
        		ph--;
        		return false;
        	}
        	if((ph+1)<h)
        	{
        		*box[i]=0;
        		box[i]=&wo[ph+1][pw];
        		*box[i]=4;
        	}
        	else
        	{
        	ph--;
        		return false;
        	}
        }
        }
        if(ph<h&&wo[ph][pw]!=7)
        {
        	*p=0;
        	p=&wo[ph][pw];
        	*p=1;
        	++foot;
        	a=0;
        }
	else
	ph--;
	break;
	}
}
void boxer()
{
	int a,b;
	for(int i=0;i<bo;i++)
	{
		do
		{
			a=random()%(h-1);
			b=random()%(w-1);
			boer[i]=&wo[a][b];
		}while(*boer[i]!=0);
		*boer[i]=5;
	}
}
void ran()
{
		for(int i=0;i<bo;i++)
		{
			do
			{
			int a=1+random()%(h-3);
			int b=1+random()%(w-3);
		    box[i]=&wo[a][b];
			}while(*box[i]!=0);
			*box[i]=4;
		}
}
void dowo()
{
	for(int i=0;i<h;i++)
	{
		for(int f=0;f<w;++f)
		{
			wo[i][f]=0;
		}
	}
	*p=1;
}
void out()
{
	clrscr();
	cout<<"\033[?25l"<<flush;
		for(int i=0;i<h;i++)
	{
		for(int f=0;f<w;++f)
		{
		     switch(wo[i][f])
		     {
		     	case 1:
		     	cout<<"\033[;45m 1\033[0m"<<flush;
		     	break;
		     	case 2:
		     	cout<<"\033[;48m 2\033[0m"<<flush;
		     	break;
		     	case 3:
		     	cout<<"\033[;47m 3\033[0m"<<flush;
		     	break;
		     	case 5:
		     	cout<<"\033[;43m 5\033[0m"<<flush;
		     	break;
		     	case 0:
		     	cout<<"  "<<flush;
		     	break;
		     	case 4:
		     	cout<<"\033[;42m 4\033[0m"<<flush;
		     	break;
		     	case 7:
		     	cout<<"\033[;44m 7\033[0m"<<flush;
		     	break;
		     	default:
		     	break;
		     }
		}
		cout<<"$"<<endl;
	}
	for(int o=0;o<=w*2;o++)
	cout<<"="<<flush;
}
int main()
{
	int k=gett;
	do
	{
		gett=0;
		foot=0;
		p=&wo[h/2-1][w/2-1];
		ph=h/2-1;
		pw=w/2-1;
		a=1;
	dowo ();
	ran();
	boxer();
	if(k<15)
	k++;
	for(;gett<k;)
		{
			gett++;
			get();
		}
	bool over;
	do
	{
		over=did();
	out();
	pm();
	}while(over);
	clrscr();
	cout<<"你走了"<<foot<<"步"<<flush;
	getch();
	}while(1);
}