// 来源：C4droid吧，id: 361719319
// https://tieba.baidu.com/p/4407788680
// 日期：2016-03-12
// 整理&bug修复：QAIU

/********************************/
/*  小白一个 不喜勿喷           */
/*  就那几个函数来回用          */
/* 控制台画画(其实一点用都没有) */
/*  作者 小鞠爱猫猫             */
/*  ...主要算法就是读取字符     */
/*  来显示画面                  */
/*  写的太乱c和c++混搭 献丑了   */
/*  用G++编译                   */
/********************************/
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
//#include <draw.hh>
#include <time.h>
using namespace std;
int xxx = 40;
int yyy = 30;
int j = 0, i = 0;
char ssss,sss;//默认笔和默认背景色
int x = 0, y = 0;
char w = '%';
char xiaoju = '[';//毫无用意
char a, b;
char map[100][100] = {};
int control(char ch);
void Draw(char w[2048]);//把字符变成颜色的函数
void delet();//清除画卷
void print();//输出画卷函数
void write();//保存画卷函数
int re();//读取已保存的画卷
void about();//关于
void help();//帮助函数
void options();//设置光标样式的函数
void forbreak() {
  char t;
  if (kbhit()) {
    // xiaoju = t;
    while (kbhit())
      xiaoju = getch();
  }
}
void tiao() {
  printf("\033[47;30m                                            \033[0m\n");
}
void put() {
  printf("\033[5;30m\033[41;m r "
         "\033[42;m t \033[43;m y "
         "\033[44;m u \033[45;37m i "
         "\033[46;30m o \033[40;37m j \033[47;m p " "\033[0m   ");
}




int main() {
 system("mkdir /storage/sdcard0/C_painting");
  ifstream find("/storage/sdcard0/C_pinting/设置.txt");//读取光标
  if (find.is_open()) {
    find >> map[128] >> a >> b>>ssss>>sss;
    find.close();
  } else {//如果没有设置则默认为##
    a = '#';
    b = '#';
  }
  if(ssss==0)
  ssss='%';//%代表黑色
  if(sss==0)
  sss='.';//.代表白色
  // /////////////
  int i, gb = 1, k = 0, count = 7;
 
  printf("\033[?25l");
  while (1) {
    clrscr();
    printf
      ("\033[47;30m" "　请把键盘切换到数字键盘　" "\033[;5;34m"
       // "👑　　　　　"
       "剪不断，理还乱。%c%c"
       "\033[0m"
       "\033[47;30m\n"
       "　使用２和８移动光标　５选择　　　　　　　　"
       "\033[0m\n", a, b);
    printf("\n");
    for (i = 1; i <= count; i++) {
      printf("　　　　");
      // printf("%2d ", ii);
      if (i == gb)
        printf("\033[;36m>>\033[0m");
      else
        printf("　　");

      if (i == 1 && gb == 1)
        printf("\033[;34m\033[5m创建新的画卷\033[0m");
      else if (i == 1 && gb != 1)
        printf("创建新的画卷");
      if (i == 2 && gb == 2)
        printf("\033[;34m\033[5m载入之前的画卷\033[0m");
      else if (i == 2 && gb != 2)
        printf("载入之前的画卷");
      if (i == 3 && gb == 3)
        printf("\033[;34m\033[5m?\033[0m");
      else if (i == 3 && gb != 3)
        printf("?");
      if (i == 4 && gb == 4)
        printf("\033[;34m\033[5m光标样式设置\033[0m");
      else if (i == 4 && gb != 4)
        printf("光标样式设置");
      if (i == 5 && gb == 5)
        printf("\033[;34m\033[5m帮助\033[0m");
      else if (i == 5 && gb != 5)
        printf("帮助");
      if (i == 6 && gb == 6)
        printf("\033[;34m\033[5m关于\033[0m");
      else if (i == 6 && gb != 6)
        printf("关于");
      if (i == 7 && gb == 7)
        printf("\033[;34m\033[5m退出\033[0m");
      else if (i == 7 && gb != 7)
        printf("退出");


      printf("\n");
    }
    printf("\n");
    tiao();
    char g = getch();
    switch (g) {
    case '2':
      gb--;
      if (gb <= 0)
        gb = count;
      break;
    case '8':
      gb++;
      if (gb > count)
        gb = 1;
      break;
      break;
    case '5':



      if (gb == 1) {
        int done = 0;
        char ch;
        clrscr();printf("最好小于40，要想画大的就得把字体调小点。\n");
        printf("请输入画的宽，并以回车键结束。\n:");
        scanf("%d", &xxx);
        printf("请输入画的高，并以回车键结束。\n:");
        scanf("%d", &yyy);
        delet();
        print();
        while (1) {
          ch = getch();
          if (ch == '0')
            break;
          done = control(ch);
          if (done == 1) {
            print();
          }
        }
      }


      else if (gb == 2) {
        re();
      } else if (gb == 3)
      {printf("无用！\n");getch();}
      else if (gb == 4) {
        options();
      } else if (gb == 5) {
        help();
      } else if (gb == 6) {
        about();
      } else if (gb == 7) {
        system("input keyevent 4");
        system("input keyevent 4");
        system("input keyevent 4");
        system("input keyevent 4");
      }
      // system("su -c 'KEyyyCODE_HOME=3'");

      break;

    default:
      break;
    }
  }
}


void Draw(char w[2048]) {
  int i = 0;
  for (i = 0; i <= 2048; i++) {

    if (w[i] == ' ')
      printf("\033[47;30m  \033[0m");
    else if (w[i] == '0')
      printf("\033[47;30m  \033[0m");
    else if (w[i] == '1')
      printf("\033[41;30m  \033[0m");
    else if (w[i] == '2')
      printf("\033[42;30m  \033[0m");
    else if (w[i] == '3')
      printf("\033[43;30m  \033[0m");
    else if (w[i] == '4')
      printf("\033[44;30m  \033[0m");
    else if (w[i] == '5')
      printf("\033[45;30m  \033[0m");
    else if (w[i] == '6')
      printf("\033[46;30m  \033[0m");
    else if (w[i] == '%')
      printf("\033[40;30m  \033[0m");
    else if (w[i] == '\n')
      printf("\n");
    else if (w[i] == '#')
      break;
    else
      printf("\033[47;30m%c\033[0m", w[i]);

  }
}

void options() {
  // strcpy(name, "/storage/sdcard0/0/");
  // scanf("%s", &str);
  // strcat(name, str);
  // strcat(name, ".txt");
  ofstream out("/storage/sdcard0/C_painting/设置.txt", ios::out);
  if (out.is_open()) {
    printf("请输入两个字母或数字或半角的符号。\n:");
    a = getch();
    putchar(a);
    b = getch();
    putchar(b);
    getch();
    out << "笔的样式= " << a << " " << b;
    out.close();
  }

}

void write() {
  int i;
  char name[64], str[20];



  printf("请输入要保存的画卷的名字，并以回车键结束。\n:");
  strcpy(name, "/storage/sdcard0/C_painting/");
  scanf("%s", &str);
  strcat(name, str);
  strcat(name, ".lily");
  ofstream out(name, ios::out);
  if (out.is_open()) {
    out << xxx << " " << yyy << " " << str << "\n";
    // for (i = 0; i<yyy; i++)
    // out << map[i];
    // map[i][xxx-1]='\n';}
    for (i = 0; i < yyy; i++) {
      for (j = 0; j < xxx; j++) {
        out << map[i][j];
      }
    }
    out.close();
  }
  printf("画卷保存在 %s", name);
  getch();
  getch();
}

int re() {
  int done = 0;
  char ch;
  char xx[10], yy[10];
  // fin >> words[0] >> means[0];
  // nn=atoi(words[0]);
  char name[64], str[20];
  printf("请输入要读取的画卷的名字，以回车键结束。\n:");
  strcpy(name, "/storage/sdcard0/C_painting/");
  scanf("%s", &str);
  strcat(name, str);
  strcat(name, ".lily");
  ifstream find(name);
  if (find.is_open()) {
    find >> xx >> yy >> str;
    sscanf(xx, "%d", &xxx);
    sscanf(yy, "%d", &yyy);
    for (i = 0; i <= yyy; i++)
      // for (j = 0; j <=xxx; j++) 
    {
      find >> map[i];
      map[i][xxx - 1] = '\n';
    }
    print();
    find.close();
    while (1) {
      ch = getch();
      if (ch == '0')
        break;
      done = control(ch);
      if (done == 1) {
        print();
      }
    }
  }
  // finn >> map[0];
  else {
    printf
      ("\033[;31m您并没有这个画卷！打开失败！\n\033[0m");
    getch();
    getch();
  }




  /* ifstream fin(FF); // fin >> ww[0] >> w[0]; for (i = 1; i < n; i++) { fin
     >> ww[i] >> w[i]; } fin.close(); */
}

void help() {
  clrscr();
  tiao();
  printf("说明:\n"
         "　　\033[5;34m%c%c\033[0m　是当前的光标\n"
         "　　按\033[5;34mwasd\033[0m来移动光标\n"
         "　　按\033[5;34mrtyuiopj\033[0m在光标的位置添充颜色\n "
         "按键:\n "
         "　　方向键都是:w,s,a,d,\n "
         "　　填充颜色键是:r,t,y,u,i,o,j,p\n "
         "　　退出键是:0\n " "　　保存键是:7\n "
         "　　清除图画是:m\n ", a, b);
         
  printf("rtyuiojp分别对应下面的颜色:\n⠜");
  put();
  printf("\n"
         "总得来说"
         "　就是光标在哪就画哪\n"
         "看完这些就可以去创作属于自己的像素画了\n");
  tiao();
  getch();
}

void print() {
  clrscr();
  put();
  printf("宽:%d 高:%d\n", xxx, yyy);
  for (i = 0; i < yyy; i++) {
    for (j = 0; j < xxx; j++) {
      // if (i == y && j == x){printf("MM");continue;}
      if (map[i][j] == '%' && i == y && j == x) {
        printf("\033[40;34;5m%c%c\033[0m", a, b);
        continue;
      } else if (map[i][j] == '1' && i == y && j == x) {
        printf("\033[41;34;5m%c%c\033[0m", a, b);
        continue;
      } else if (map[i][j] == '2' && i == y && j == x) {
        printf("\033[42;34;5m%c%c\033[0m", a, b);
        continue;
      } else if (map[i][j] == '3' && i == y && j == x) {
        printf("\033[43;34;5m%c%c\033[0m", a, b);
        continue;
      } else if (map[i][j] == '4' && i == y && j == x) {
        printf("\033[44;34;5m%c%c\033[0m", a, b);
        continue;
      } else if (map[i][j] == '5' && i == y && j == x) {
        printf("\033[45;34;5m%c%c\033[0m", a, b);
        continue;
      } else if (map[i][j] == '6' && i == y && j == x) {
        printf("\033[46;34;5m%c%c\033[0m", a, b);
        continue;
      } else if (map[i][j] == '.' && i == y && j == x) {
        printf("\033[47;34;5m%c%c\033[0m", a, b);
        continue;
      } else if (i == y && j == x) {
        printf("\033[47;34;5m%c%c\033[0m", a, b);
        continue;
      }

      if (map[i][j] == '%')
        printf("\033[40;30m  \033[0m");
      if (map[i][j] == '1')
        printf("\033[41;30m  \033[0m");
      if (map[i][j] == '2')
        printf("\033[42;30m  \033[0m");
      if (map[i][j] == '3')
        printf("\033[43;30m  \033[0m");
      if (map[i][j] == '4')
        printf("\033[44;30m  \033[0m");
      if (map[i][j] == '5')
        printf("\033[45;30m  \033[0m");
      if (map[i][j] == '6')
        printf("\033[46;30m  \033[0m");
      if (map[i][j] == '.')
        printf("\033[47;30m  \033[0m");
      if (map[i][j] == '\n')
        printf("\n");
      
        /* if (map[i][j] == '%') printf("\033[40;30;5m%c \033[0m", map[i][j]);
           if (map[i][j] == '1') printf("\033[41;30;5m%c \033[0m", map[i][j]);
           if (map[i][j] == '2') printf("\033[42;30;5m%c \033[0m", map[i][j]);
           if (map[i][j] == '3') printf("\033[43;30;5m%c \033[0m", map[i][j]);
           if (map[i][j] == '4') printf("\033[44;30;5m%c \033[0m", map[i][j]);
           if (map[i][j] == '5') printf("\033[45;30;5m%c \033[0m", map[i][j]);
           if (map[i][j] == '6') printf("\033[46;30;5m%c \033[0m", map[i][j]);
           if (map[i][j] == '.') printf("\033[47;30;5m%c \033[0m", map[i][j]);
           if (map[i][j] == '\n') printf("\n"); */
    }
  }
}
void delet() {
  int i, j;
  for (i = 0; i <= yyy; i++) {
    for (j = 0; j <= xxx; j++) {
      if (j == xxx - 1)
        map[i][j] = '\n';
      else
        map[i][j] = sss;
    }
  }
}

void about() {
  int sleep = 8;
  xiaoju = '[';
  while (xiaoju == '[') {
    clrscr();
    Draw("                            \n"
         "      %%%%%                 \n"
         "    %%%%%%%%%               \n"
         "   %%%%%%%% %%              \n"
         "   %%%%%%% %%%%  This is me..    \n"
         "  %%%% %%%%%%%%             \n"
         "  %%%   % %%%%%             \n"
         "  %% 444 444%%%             \n"
         "   %  4   4  %              \n"
         "   %  4   4  %              \n"
         "    %   1   %               \n"
         "   %44663664 %              \n"
         "  %  4463644 %    1         \n"
         "  %  4463644%    131        \n"
         "   %%%444444%     1 2       \n"
         "     %%%4222%   2 22        \n"
         "        %%%%      2         \n"
         "                            \n#");
    printf("\033[47;30m"
           "　　　　　　　　　　　　　　"
           "感谢所有支持和不支持我的人　""\n"
           "　作者　小鞠爱猫猫　　　　　"
           "　　　　　　　　2016.03.12　"
           "\033[0m\n");
    usleep(100000 * sleep);
    clrscr();
    Draw("                            \n"
         "      %%%%%                 \n"
         "    %%%%%%%%%               \n"
         "   %%%%%%%% %%              \n"
         "   %%%%%%% %%%%  This is me..    \n"
         "  %%%% %%%%%%%%             \n"
         "  %%%   % %%%%%             \n"
         "  %% 444 444%%%             \n"
         "   %  4   4  %              \n"
         "   %  4   4  %              \n"
         "    %   1   %               \n"
         "   % 46636644%              \n"
         "   % 4463644  %   1         \n"
         "    %4463644  %  131        \n"
         "    %444444%%%  2 1         \n"
         "    %2224%%%      2 2       \n"
         "     %%%%         2         \n"
         "                            \n#");
    printf("\033[47;30m"
           "　　　　　　　　　　　　　　"
           "感谢所有支持和不支持我的人　""\n"
           "　作者　小鞠爱猫猫　　　　　"
           "　　　　　　　　2016.03.12　"
           "\033[0m\n");
    usleep(100000 * sleep);
    forbreak();
  }

  if (xiaoju == ' ') {
    clrscr();
    Draw("                            \n"
         "    %%%%%44       %%%%%     \n"
         "   %%%%%%%%44   %%%%%%%%%   \n"
         "   %%%%%% % %  %%%%%%%% %%  \n"
         "  %%%%%  %%%% %%%%%%%% %%%% \n"
         "  %%%%%%%%%%% %%%% %%%%%%%% \n"
         "  %%%%%%% %%% %%%   % %%%%% \n"
         "  %%%4%% 4 %% %% 444 444%%% \n"
         "  %% 4   4 %%  %  4   4  %  \n"
         "  %% 4   4 %%  %  4   4  %  \n"
         "  %%%  1  %%%   %   1   %   \n"
         "  %% 53335 %%  %44663664 %  \n"
         "  %  53535  % %  4463644 %  \n"
         "   %5555555  %%  4463644%   \n"
         "   %5555555%%  %44444444%   \n"
         "  %153525654%   %222%222%   \n"
         "   %%%%%%%%%     %%% %%%    \n" "                            \n#");
    printf("\033[47;30m"
           "  你的一个举动，足以使我毁灭"
           "或重生。                J&Q " "\033[0m");

    getch();
  }
}



int control(char ch) {
  switch (ch) {
  case 'a':
    {
      if (x - 1 >= 0) {
        x--;
        w = map[y][x];
        return 1;
      } else
        return 0;
    }

  case 'd':
    {
      if (x + 1 < xxx - 1) {
        x++;
        w = map[y][x];
        return 1;
      } else
        return 0;

    }

  case 's':
    {
      if (y + 1 < yyy) {
        y++;
        w = map[y][x];
        return 1;

      } else
        return 0;

    }
  case 'w':
    {
      if (y - 1 >= 0) {
        y = y - 1;
        w = map[y][x];
        return 1;
      } else
        return 0;

    }
  case '4':
    {
      if (x - 1 >= 0) {
        x--;
        w = map[y][x];
        return 1;
      } else
        return 0;
    }

  case '6':
    {
      if (x + 1 < xxx - 1) {
        x++;
        w = map[y][x];
        return 1;
      } else
        return 0;

    }
  case '8':
    {
      if (y + 1 < yyy) {
        y++;
        w = map[y][x];
        return 1;

      } else
        return 0;
    }
  case '2':
    {
      if (y - 1 >= 0) {
        y = y - 1;
        w = map[y][x];
        return 1;
      } else
        return 0;
    }
  case '5':
    {
      map[y][x] = ssss;
      return 1;
    }
  case '1':
    {
      map[y][x] = '.';
      return 1;
    }
  case 'j':
    {
      map[y][x] = '%';
      return 1;
    }
  case 'r':
    {
      map[y][x] = '1';
      return 1;
    }
  case 't':
    {
      map[y][x] = '2';
      return 1;
    }
  case 'y':
    {
      map[y][x] = '3';
      return 1;
    }

  case 'u':
    {
      map[y][x] = '4';
      return 1;
    }
  case 'i':
    {
      map[y][x] = '5';
      return 1;
    }
  case 'o':
    {
      map[y][x] = '6';
      return 1;
    }
  case 'p':
    {
      map[y][x] = '.';
      return 1;
    }
  case 'm':
    {
      delet();
      return 1;
    }
  case '7':
    {
      write();
      return 1;
    }


  default:
    return 0;
  }
}