#include <conio.h>
#include <math.h>
#include <stdlib.h>
typedef struct
{
	int r,g,b,bgR,bgG,bgB;
}COLOR;

static int Round(const double a)
{
	return (int)(a + 0.5); 
}

static void cleanrect(int x,int y,int w,int h)
{
	for (int j=0;j<h;j++)
	{
		for (int i=0;i<w;i++)
		{
	    	gotoxy((x+i),y+j);
    		printf("\e[0m \n");
		}
	}
}
static void drawrect2(int x,int y,int w,int h,COLOR color)
{
	for (int j=0;j<h;j++)
	{
		for (int i=0;i<w;i++)
		{
	    	gotoxy((x+i),y+j);
    		printf("\e[48;5;%dm\e[38;5;%dm\e[1m \e[0m\n",color.r*36+color.g*6+color.b+16,color.bgR*36+color.bgG*6+color.bgB+16);
		}
	}
}
static int DrawPoint(const char *s,int x, int y,COLOR color)
{
	if (x<0||y<0)
	return 0;
    gotoxy(x, y);
    printf("\e[48;5;%dm\e[38;5;%dm%s\e[0m\n",color.r*36+color.g*6+color.b+16,color.bgR*36+color.bgG*6+color.bgB+16,s);
    return 1;
}
static int DrawPoint2(const char *s,int x, int y,COLOR color)
{
	if (x<0||y<0)
	return 0;
    gotoxy(x, y);
    printf("\e[48;5;%dm\e[38;5;%dm\e[1m%s\e[0m\n",color.r*36+color.g*6+color.b+16,color.bgR*36+color.bgG*6+color.bgB+16,s);
    return 1;
}
static int drawchar(char ch,int *arr,int x,int y,int w,int h,COLOR color)
{
	if (arr==NULL||x<0||y<0)
		return 0;
	for (int k=0,i=0;i<h;i++)
	{
		gotoxy(2*x,y++);
		for (int j=0;j<w;j++)
		{
			if (arr[k++])
				printf("\e[48;5;%dm\e[38;5;%dm%c%c\e[0m",color.r*36+color.g*6+color.b+16,color.bgR*36+color.bgG*6+color.bgB+16,ch,ch);
			else 
				printf("  ");
		}
		printf("\n");
	}
	return 1;
}

static int drawtext(const char str[],int x,int y,COLOR color)
{
	char s[1024];
	strcpy(s,str);
    char delim[] = "\n";
    char *p;
    DrawPoint(strtok(s, delim),x*2,y,color);
    while((p = strtok(NULL, delim)))
    	DrawPoint(p,x*2,++y,color);
}

static int drawbox1(int x,int y,int w,int h,COLOR color)
{
	DrawPoint2("┏ ",x*2,y,color);
	DrawPoint2("┓ ",(x+w)*2,y,color);
	DrawPoint2("┗ ",x*2,y+h,color);
	DrawPoint2("┛ ",(x+w)*2,y+h,color);
	for (int i=1;i<w;i++)
	{
		DrawPoint("━━ ",(x+i)*2-1,y,color);
		DrawPoint("━━ ",(x+i)*2-1,y+h,color);
	}
	for (int i=1;i<h;i++)
	{
	DrawPoint("┃ ",x*2,y+i,color);
	DrawPoint("┃ ",(x+w)*2,y+i,color);
	}
}

static int drawbox(int x,int y,int w,int h,COLOR color)
{
	drawbox1(x,y,w,h,color);
	COLOR color2={color.r,color.g,color.b,color.r,color.g,color.b};
	for (int i=1;i<w;i++)
	{
		for (int j=1;j<h;j++)
		{
			DrawPoint(" 度",(x+i)*2-1,y+j,color2);
		}
	}
}

static void drawline(const char *s, int x0, int y0, int x1, int y1,COLOR color) 
{
	int dx, dy, epsl, k;
    double x, y, xIncre, yIncre;
    dx = x1 - x0;
    dy = y1 - y0;
    x = x0;
    y = y0;
    epsl=abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    xIncre = (double)(dx) / epsl;
    yIncre = (double)(dy) / epsl;
    for (k = 0; k <= epsl; k++)
    {
    	int tx = Round(x);
        int ty = Round(y );
        DrawPoint(s,tx, ty,color);
        x += xIncre;
        y += yIncre;
    }
}
static void ellipsePlotPoints(const char *s,int xCenter, int yCenter, int x,int y,int mode,COLOR color)
{
 	if (mode)
 	{
 		drawline(s,xCenter + x, yCenter + y,xCenter - x, yCenter + y,color);
     	drawline(s,xCenter + x, yCenter - y,xCenter - x, yCenter - y,color);
 	}
 	else
 	{
 		DrawPoint(s,xCenter + x, yCenter + y,color);
         DrawPoint(s,xCenter - x, yCenter + y,color);
         DrawPoint(s,xCenter + x, yCenter - y,color);
         DrawPoint(s,xCenter - x, yCenter - y,color);
 	}
 }
 
static void DrawCircle(const char *s,int xCenter, int yCenter, int Rx, int Ry,COLOR color,int mode)
{
    int Rx2 = Rx * Rx, Ry2 = Ry * Ry;
    int twoRx2 = 2 * Rx2, twoRy2 = 2 * Ry2;
    int p, x = 0,y = Ry, px = 0,py = twoRx2 * y;
    // Plot the initial point in each quadrant
    ellipsePlotPoints(s,xCenter, yCenter, x, y,mode,color);
    /* Region 1 */
    p = Round(Ry2 - (Rx2 * Ry) + (0.25 * Rx2));
    while (px < py) 
    {
        x++;
        px += twoRy2;
        if (p < 0) 
            p += Ry2 + px;
        else 
        {
            y--;
            py -= twoRx2;
            p += Ry2 + px - py;
        }
        ellipsePlotPoints(s,xCenter, yCenter, x, y,mode,color);
    }
    /* Region 2 */
    p = Round(Ry2 * (x + 0.5) * (x + 0.5) + Rx2 * (y - 1) * (y - 1) - Rx2 * Ry2);
    while (y > 0) 
    {
        y--;
        py -= twoRx2;
        if (p > 0) 
            p += Rx2 - py;
        else 
        {
            x++;
            px += twoRx2;
            p += Rx2 - py + px;
        }
        ellipsePlotPoints(s,xCenter, yCenter, x, y,mode,color);
    }
}