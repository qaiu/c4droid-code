/*c4droid代码手册
//彩色星星
//a876565 编写
TTHHR收集整理
*转载请说明出处
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <termios.h>
#include <ncurses.h>
#define MAX 8

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct starflag
{
	int locus;
	int x, y;
	int ax, ay;
	unsigned timeout;
};

int terset(int flag)
{
	struct termios t;
	tcgetattr(0, &t);
	t.c_lflag &= flag;
	return tcsetattr(0, TCSANOW, &t);
}

int getwz(int *x, int *y)
{
	struct winsize ws;
	if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) != 0)
	{
		perror("TIOCGWINSZ");
		return -1;
	}
	*y = ws.ws_row;
	*x = ws.ws_col;
	return 0;
}

char cbuf[64];
void *star(void *flag)
{
	struct starflag *sf = (struct starflag *)flag;
	int maxx, maxy;
	int x = sf->x, y = sf->y;
	int ax = sf->ax, ay = sf->ay;
	int locus = sf->locus;
	int timeout = sf->timeout;
	while (1)
	{
		pthread_mutex_lock(&mutex);
		getmaxyx(stdscr, maxx, maxy);
		if (!locus)
		{
			mvprintw(x, y, "\x20");
			refresh();
		}
		x += ax;
		y += ay;
		if (x >= maxx || x <= 0)
		{
			x=(x>=maxx)?maxx:0;
			ax = -ax;
		}
		if (y >= maxy || y <=0)
		{
			y=(y>=maxy)?maxy:0;
			ay = -ay;
		}
		attron(COLOR_PAIR((time(NULL) + x + y) % 7));
		mvprintw(x, y, "*");
		refresh();
		pthread_mutex_unlock(&mutex);
		usleep(timeout);
	}
	return 0;
}

void *control(void *data)
{
	// terset(~(ECHO | ICANON));
	char ch;
	while ((ch = getchar()) != 'q')
	{
		if (ch == 'c')
			erase();
	}
	endwin();
	printf("\e[2J\e[0m\e[1;1H\n");
	printf("exit.\n");
	exit(0);
}

void clean()
{
	if (errno)
	{
		endwin();
		perror("E");
	}
}

void loading(int time, int wight)
{
	char*cbuf=new char[wight];
	WINDOW *win;
	win = subwin(stdscr, 3, wight + 2, 0, 3);
	mvwprintw(win,1,0,"*");
	mvwprintw(win,1,wight+1,"*");
	for (int i=0;i<wight;i++)
	cbuf[i]=0x20;
	memcpy(cbuf+wight/2-5,"loading...",10);	mvwprintw(win,1,wight/2-5,"loading...");
	init_pair(1, 7,1);
	wattron(win,COLOR_PAIR(1));
	for (int i = 1; i <= wight; i++)
	{
		mvwprintw(win,1,i,"%c",cbuf[i]);
		wrefresh(win);
		mvprintw(4,wight/2-2,"%d%%",i*100/wight);
		wrefresh(stdscr);
		usleep(time);
	}
	erase();
}

int main()
{
	struct starflag sf;
	pthread_t pth_con;
	pthread_t pth[MAX + 1];
	char buf[128];
	atexit(clean);
	printf(" \e[?25l");
	pthread_create(&pth_con, NULL, control, NULL);

	initscr();
	start_color();
	attron(A_BOLD);
	loading(1000 * 50, 30);
	for (int i = 0; i < 8; i++)
	{
		init_pair(i, i, 0);
	}
	sf.locus = 0;
	sf.timeout = 1000 * 100;
	for (int i = 1; i <= MAX; i++)
	{
		sf.x = 1;
		sf.y = 1;
		sf.ax = i / 2 + 1;
		sf.ay = i / 3 + 1;
		pthread_create(pth + i, NULL, star, &sf);
		sleep(1);
	}

	pthread_join(pth_con, (void **)buf);
	endwin();
	return 0;
}