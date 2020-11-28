/*c4droid代码手册
 *控制台窗口
 *佚名编写
 *TTHHR收集整理
  *使用G++编译器
 *转载请说明出处
*/
#include <stdio.h>
#include <ncurses.h>
int main()
{
initscr();
move(2,12);
addstr("C4droid");
printw(" Hello\n");
move(4,12);
addstr("C and C++");
WINDOW *win=subwin(stdscr,4,8,2,2);
box(win,0,0);
WINDOW *win3=subwin(stdscr,4,28,8,2);
WINDOW *win4=subwin(stdscr,7,28,9,8);
box(win4,0,0);
WINDOW *win5=subwin(win4,2,14,13,20);
box(win5,0,0);
WINDOW *win2=subwin(win4,2,14,10,20);
box(win2,0,0);
move(7,5);
hline('~',25);
move(6,3);
whline(stdscr,'_',30);
start_color();
init_pair(1,COLOR_BLACK,COLOR_BLUE);
wbkgd(win,COLOR_PAIR(1));
init_pair(3,COLOR_RED,COLOR_GREEN);
wbkgd(win3,COLOR_PAIR(3));
init_pair(4,COLOR_WHITE,COLOR_CYAN);
wbkgd(win4,COLOR_PAIR(4));
init_pair(5,COLOR_WHITE,COLOR_WHITE);
wbkgd(win5,COLOR_PAIR(5));
wbkgd(win2,COLOR_PAIR(5));
mvwaddstr(win4,2,2,"Password");
mvwaddstr(win4,4,2,"UserName");
refresh();
getch();
endwin();
return 0;
}