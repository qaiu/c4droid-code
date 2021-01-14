/*c4droid代码手册
消灭星星[v5.3]
亅小夕ve 编写
*/
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include<stdlib.h>
#include<time.h>
#include "butt2.hxx"
int aH, BOX = 90, map[10][10], H = 9, W = 8, wH, wW;
int Kinds = 3, clean, score, sBest, sAdd;

SDL_Window *w = NULL;
SDL_Surface *s = NULL;
SDL_Texture *t[5], *blocks;
SDL_Renderer *r = NULL;
SDL_Event e;
TTF_Font *font = NULL;

SDL_Rect cut[8] = { {500, 0, 1, 1}
,
{0 * 85, 0, 85, 114}
,
{1 * 85, 0, 85, 114}
,
{2 * 85, 0, 85, 114}
,
{3 * 85, 0, 85, 114}
,
{4 * 85, 0, 85, 114}
,
{5 * 85 + 120, 0, 85, 114}
,
{6 * 85 + 120, 0, 85, 114}
,
}
, box = {
0, 0, BOX, BOX};
void SDL_star();
void SDL_end();
int gameStar();
void begin();
void menu();
int inMenu();
void about();
void over();
int scoreGet();
void scoreSave(int a);
void codeOut();
void mapMake();
void mapDraw();
void out(int a, int b, int c);
int cleanAble();
int getClean(int a, int b);
int boxClean(int a, int b, int c);
void combine();
int in(double x, double y, int *a, int *b);
void difficult();
void scoreBest();
void load(double t);
/* ￭￭￭￭￭￭￭￭￭￭￭￭￭￭￭ */
int main(int, char **) {
  SDL_star();
  begin();
  M_Init(w, r);
  menu();
  M_Quit();
  SDL_end();
  return 0;
}
/* ￭￭￭￭￭￭￭￭￭￭￭￭￭￭￭ */
void load(double t) {
  M_Button b(0.6 * wW, 0.7 * wH, 200, 100);
  b.text("loading..", 120, 30, 50);
  SDL_Rect re = { wW * 0.7, wH * 0.8, 100, 100 };
  t = 1000 * t / 6;
  for (int i = 1; i <= 6; i++) {
    SDL_RenderClear(r);
    b.show();
    SDL_RenderCopy(r, blocks, &cut[i], &re);
    SDL_RenderPresent(r);
    SDL_Delay(t);
  }
}
int inMenu() {
  M_Button b[4] = {
    M_Button(100, 0.7 * wH, wW - 200, 150),
    M_Button(200, 0.5 * wH, wW - 400, 100),
    M_Button(200, 0.3 * wH, wW - 400, 100),
    M_Button(200, 0.1 * wH, wW - 400, 100)
  };
  int Quit = 0, i;
  b[0].text("开 始", 100, 150, 150);
  b[1].text("关 于", 100, 150, 150);
  b[2].text("最高分", 100, 150, 150);
  b[3].text("退 出", 100, 150, 150);
  while (!Quit) {
    for (i = 0; i <= 3; i++)
      if (b[i].touch()) {
        Quit = 1;
        break;
      }
    SDL_RenderCopy(r, t[0], NULL, NULL);
    b[0].show();
    b[1].show();
    b[2].show();
    b[3].show();
    SDL_RenderPresent(r);
    SDL_RenderClear(r);
  }
  return i;
}


void about() {
  int Quit = 0;
  M_Button b(wW - 200, wH - 100, 200, 100);
  b.text("返回", 100, 150, 150);
  while (!Quit) {
    if (b.touch()) {
      Quit = 1;
    }
    SDL_RenderCopy(r, t[0], NULL, NULL);
    b.show();
    SDL_RenderPresent(r);
    SDL_RenderClear(r);
  }
}
void scoreBest() {
  char *ch;
  int Quit = 0;
  M_Button
    b(wW - 200, wH - 100, 200, 100),
    bClean(200, wH - 200, wW - 400, 100), bView(100, 100, wW - 200, 100);

  b.text("返回", 100, 150, 150);
  bClean.text("清除记录", 200, 20, 100);
  sprintf(ch, "最高分：%d", scoreGet());
  bView.text(ch, 120, 130, 140);
  while (!Quit) {
    if (b.touch())
      Quit = 1;
    if (bClean.touch()) {
      scoreSave(0);
      bClean.text("清除完成", 200, 50, 100);
      sprintf(ch, "最高分：%d", scoreGet());
      bView.text(ch, 120, 130, 140);
    }
    SDL_RenderCopy(r, t[0], NULL, NULL);
    b.show();
    bClean.show();
    bView.show();
    SDL_RenderPresent(r);
    SDL_RenderClear(r);
  }
}
void menu() {
  int Quit = 0;
  while (!Quit) {
    switch (inMenu()) {
    case (0):
      load(0.2);
      difficult();
      load(0.3);
      gameStar();
      SDL_Delay(100);
      break;
    case (1):
      about();
      SDL_Delay(100);
      break;
    case (2):
      scoreBest();
      SDL_Delay(100);
      break;
    case (3):
      SDL_Delay(100);
      Quit = 1;
      break;
    }
  }
}
/* ￭￭￭￭￭￭￭￭￭￭￭￭￭￭￭ */
int gameStar() {
  int ea, eb, Quit = 0;
  char ch[10];
  M_Button bQ(wW - 200, 0, 200, 160), bScore(0, 0, wW - 210, 160),
    bView(0, 200, wW, 200);

  bQ.text("退出", 100, 150, 150);
  sprintf(ch, "分数：%d", score);
  bScore.text(ch, 120, 150, 150);
  score = 0;
  mapMake();

  SDL_RenderCopy(r, t[1], NULL, NULL);
  mapDraw();
  bScore.show();
  bQ.show();
  SDL_RenderPresent(r);

  while (!Quit) {
  	 if (bQ.touch())
        return 0;
    e = getEve();
    if (e.type == SDL_FINGERDOWN || e.type == SDL_FINGERMOTION) {
     
      if (in(e.tfinger.x, e.tfinger.y, &ea, &eb)) {
        getClean(ea, eb);
        combine();
      }
      SDL_RenderCopy(r, t[1], NULL, NULL);
      mapDraw();
      Quit = !cleanAble();
      if (clean > 2) {
        sprintf(ch, "%d连击+%d分", clean, sAdd * clean);
        bView.text(ch, 100, 150, 150);
        bView.show();
      }
      sprintf(ch, "分数：%d", score);
      bScore.text(ch, 100, 150, 150);
      bQ.show();
      bScore.show();
      SDL_RenderPresent(r);
      SDL_RenderClear(r);
      SDL_Delay(100);
    }
  }
  if (!cleanAble())
    over();
  SDL_Delay(1000);
}
void difficult() {
  int i, Quit = 0;
  M_Button b[3] = {
    M_Button(200, 0.7 * wH, wW - 400, 100),
    M_Button(200, 0.5 * wH, wW - 400, 100),
    M_Button(200, 0.3 * wH, wW - 400, 100)
  };
  b[0].text("简单", 150, 100, 150);
  b[1].text("一般", 150, 100, 150);
  b[2].text("困难", 150, 100, 150);
  while (!Quit) {
    for (i = 0; i <= 2; i++)
      if (b[i].touch()) {
        Kinds = 3 + i;
        Quit = 1;
        break;
      }
    SDL_RenderCopy(r, t[0], NULL, NULL);
    b[0].show();
    b[1].show();
    b[2].show();
    SDL_RenderPresent(r);
    SDL_RenderClear(r);
  }
}
/* ￭￭￭￭￭￭￭￭￭￭￭￭￭￭￭ */
void mapMake() {
  int a, b;
  for (a = 0; a <= H; a++)
    for (b = 0; b <= W; b++)
      map[a][b] = rand() % Kinds + 1;
}
void mapDraw() {
  int a, b;
  for (a = 0; a <= H; a++)
    for (b = 0; b <= W; b++)
      out(a, b, map[a][b]);
}
int in(double x, double y, int *a, int *b) {
  int i, ix = x * wW, iy = y * wH - aH;
  *a = -1;
  *b = -1;
  for (i = 0; i <= H; i++)
    if (iy > i * BOX && iy < (i + 1) * BOX)
      *a = i;
  for (i = 0; i <= W; i++)
    if (ix > i * BOX && ix < (i + 1) * BOX)
      *b = i;
  return (*a != -1 && *b != -1);
}
void out(int a, int b, int c) {
  box.x = b * BOX;
  box.y = a * BOX + aH;
  SDL_RenderCopy(r, blocks, &cut[c], &box);
}
int boxClean(int a, int b, int c) {
  if (c == 0)
    return -1;
  map[a][b] = 0;
  clean++;
  if (a > 0 && map[a - 1][b] == c)
    boxClean(a - 1, b, c);
  if (a < H && map[a + 1][b] == c)
    boxClean(a + 1, b, c);
  if (b > 0 && map[a][b - 1] == c)
    boxClean(a, b - 1, c);
  if (b < W && map[a][b + 1] == c)
    boxClean(a, b + 1, c);
}
int getClean(int a, int b) {
  int c = map[a][b];
  clean = 0;
  if ((a > 0 && map[a - 1][b] == c) ||
      (a < H && map[a + 1][b] == c) ||
      (b > 0 && map[a][b - 1] == c) || (b < W && map[a][b + 1] == c)) {
    boxClean(a, b, c);
    sAdd = (Kinds - 2) * clean;
    if (clean > 2)
      score += sAdd * (clean);
    else
      score += sAdd;
  }
  return clean;
}
void combine() {
  int a, b, i;
  /* 下落 */
  for (a = 0; a <= H; a++)
    for (b = 0; b <= W; b++)
      if (map[a][b] == 0) {
        for (i = a; i > 0; i--)
          map[i][b] = map[i - 1][b];
        map[i][b] = 0;
      }
  /* 左移 */
  for (b = W - 1; b >= 0; b--)
    if (map[H][b] == 0) {
      for (i = b; i <= W; i++) {
        for (a = 0; a <= H; a++) {
          if (i == W)
            map[a][i] = 0;
          else
            map[a][i] = map[a][i + 1];
        }
      }
    }
}

int cleanAble() {
  int a, b, i = 0;
  for (a = 0; i == 0 && a <= H; a++)
    for (b = 0; b <= W; b++) {
      if (map[a][b] != 0 && a < H && b < W
          && (map[a][b] == map[a][b + 1] || map[a][b] == map[a + 1][b]))
        i = 1;
      if (map[a][b] != 0 && a == H && map[a][b] == map[a][b + 1])
        i = 1;
      if (map[a][b] != 0 && b == W && map[a][b] == map[a + 1][b])
        i = 1;
    }
  return i;
}
void over() {
  char ch[10];
  M_Button b(100, 100, wW - 200, 200);
  if (score > scoreGet()) {
    scoreSave(score);
    sprintf(ch, "刷新记录");
  } else
    sprintf(ch, "分数：%d", score);
  b.text(ch, 100, 150, 150);
  SDL_RenderCopy(r, t[1], NULL, NULL);
  b.show();
  SDL_RenderPresent(r);
}
/* ￭￭￭￭￭￭￭￭￭￭￭￭￭￭￭ */
void scoreSave(int a) {
  FILE *fp = fopen("resources/user/xmxx.score", "w+");
  int b = 0, i;
  long j = 1;
  char sc;
  for (i = 1; i <= 6; b++, i++) {
    j *= 10;
    if (a < j)
      break;
  }

  for (i = 0; i <= b; i++) {
    j /= 10;
    sc = ((a / j) % 10) + 48;
    fputc(sc, fp);
  }
  fputc('a', fp);
  fclose(fp);
}

int scoreGet() {
  int sum = 0, i = 0, a[20];
  long j = 1;
  char c;
  FILE *fp = NULL;

  if ((fp = fopen("resources/user/xmxx.score", "r+")) == NULL) {
     fp = fopen("resources/user/xmxx.score", "w+");
    scoreSave(0);
  }
  while ((c = fgetc(fp)) != 'a') {
    a[i] = c - 48;
    i++;
    if (i > 10)
      return -1;
  }
  while (i > 0) {
    i--;
    sum += j * a[i];
    j *= 10;
  }
  fclose(fp);
  return sum;
}
/* ￭￭￭￭￭￭￭￭￭￭￭￭￭￭￭ */
void begin() {
  s = IMG_Load("resources/image/a0.png");
  blocks = SDL_CreateTextureFromSurface(r, s);
  s = IMG_Load("resources/image/a1.jpg");
  t[0] = SDL_CreateTextureFromSurface(r, s);
  s = IMG_Load("resources/image/a2.jpg");
  t[1] = SDL_CreateTextureFromSurface(r, s);

  SDL_GetWindowSize(w, &wW, &wH);
  BOX = wW / (W + 1);
  box.w = box.h = BOX;
  aH = wH - (H * BOX) - BOX;
}

void SDL_star() {
  TTF_Init();
  IMG_Init(IMG_INIT_PNG);
  srand(time(0));
  w = SDL_CreateWindow("game", 0, 0, 0, 0, 0);
  r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);
  font = TTF_OpenFont("../../../Resources/Font/05.ttf", 50);
}
void SDL_end() {
  SDL_RenderClear(r);
  SDL_FreeSurface(s);
  SDL_DestroyRenderer(r);
  SDL_DestroyTexture(t[0]);
  SDL_DestroyWindow(w);
  TTF_CloseFont(font);
  SDL_Quit();
}