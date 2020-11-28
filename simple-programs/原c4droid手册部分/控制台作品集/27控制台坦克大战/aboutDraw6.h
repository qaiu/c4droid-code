void inTank() {
  int u, a;

  for (a = 0; a < 90; a++){
    s[a] = a + 35;
  if (a <= 10)
     t[a].S = tt[a].S = clock();
  }

  T[0].a = 2;
  T[0].b = 12;
  T[0].c = 6;
  T[0].hp = 10;
  T[0].h = s[10];
  T[0].s = s[10];
  T[0].ss = "\E[40;32m✙✙\E[40;37m";
  T[0].p = s[11];
  T[0].pp = "\E[40;34m❖❖\E[40;37m";

  u = 1;
  T[u].a = 3;
  T[u].b = 4;
  T[u].c = 6;
  T[u].s = s[u * 5 + 15];
  T[u].ss = "\E[41;33m▓▓\E[40;37m";
  T[u].p = s[u * 5 + 16];
  T[u].pp = "\E[45;36m▒▒\E[40;37m";
  T[u].hp = 0;
  T[u].h = s[u * 5 + 17];

  u = 2;
  T[u].a = 7;
  T[u].b = 4;
  T[u].c = 6;
  T[u].s = s[u * 5 + 15];
  T[u].ss = "\E[41;33m▓▓\E[40;37m";
  T[u].p = s[u * 5 + 16];
  T[u].pp = "\E[45;36m▒▒\E[40;37m";
  T[u].hp = 0;
  T[u].h = s[u * 5 + 17];
  u = 3;
  T[u].a = 7;
  T[u].b = 4;
  T[u].c = 6;
  T[u].s = s[u * 5 + 15];
  T[u].ss = "\E[41;33m▓▓\E[40;37m";
  T[u].p = s[u * 5 + 16];
  T[u].pp = "\E[45;36m▒▒\E[40;37m";
  T[u].hp = 0;
  T[u].h = s[u * 5 + 17];
  u = 4;
  T[u].a = 7;
  T[u].b = 4;
  T[u].c = 6;
  T[u].s = s[u * 5 + 15];
  T[u].ss = "\E[41;33m▓▓\E[40;37m";
  T[u].p = s[u * 5 + 16];
  T[u].pp = "\E[45;36m▒▒\E[40;37m";
  T[u].hp = 0;
  T[u].h = s[u * 5 + 17];

}



void mapMake() {
  int a, b;
  for (a = 0; a <= H; a++) {
    for (b = 0; b <= W; b++) {
      if (a == 0 || a == H || b == 0 || b == W)
        map[a][b] = s[1];
      else
        map[a][b] = s[0];
    }
  }
}
void mapDraw() {
  int a, b, i;
  for (a = 0; a <= 10; a++) {
    gotoxy(2 * W + 3, a + 1);
    printf(" ");
  }
  for (a = 0; a <= T[0].hp; a++) {
    gotoxy(2 * W + 3, a + 1);
    printf("\E[42;33m┋\E[40;37m");
  }

  for (a = 0; a <= H; a++) {
    for (b = 0; b <= W; b++) {

      if (a == 0 && (b == W || b == W - 1 || b == W - 7 || b == W - 8))
        continue;
      if (level == 2 && a == 0 && (b == 10 || b == 11))
        continue;
      /* 防止分数闪烁 */

      if (map[a][b] == s[0])
        out(b, a, "  ");
      if (map[a][b] == s[1])
        out(b, a, "\E[41;36m██\E[40;37m");
      /* 蓝墙 */
      if (map[a][b] == s[2])
        out(b, a, "\33[42;31m❤ \33[40;37m");
      /* 补血 */

      if (map[a][b] == s[4])
        out(b, a, "▓▓");
      /* 白墙 */
      if (map[a][b] == s[5])
        out(b, a, "\E[41;36m₪₪\E[40;37m");
      /* 红墙 */

      if (map[a][b] == T[0].s)
        out(b, a, T[0].ss);
      /* 我方坦克 */
      if (map[a][b] == T[0].p || map[a][b] == s[3])
        out(b, a, T[0].pp);
      /* 我方炮弹 */

      for (i = 1; i <= eSum; i++) {
        if (map[a][b] == T[i].s)
          out(b, a, T[i].ss);
        /* 敌方坦克 */
        if (map[a][b] == T[i].p)
          out(b, a, T[i].pp);
        /* 敌方炮弹 */
        if (map[a][b] == T[i].h)
          out(b, a, T[i].hp);
        /* 敌方血量显示 */
      }
    }
  }

  if (level == 2) {
    gotoxy(21, 1);
    printf("\E[46;31m时%2d\E[40;37m", gameTime);
  }

  gotoxy(2 * W - 1, 1);
  printf("\E[46;31m分%2d\E[40;37m", point);
  gotoxy(2 * W - 15, 1);
  printf("\E[46;31m击%2d\E[40;37m", kills);
  gotoxy(1, H + 3);

}
void out(int a, int b, char *c) {
  gotoxy(2 * a + 1, b + 1);
  printf("%s", c);
  gotoxy(1, H + 3);
} void out(int a, int b, int c) {
  gotoxy(2 * a + 1, b + 1);
  printf("\E[42;31m%2d\E[40;37m", c);
  gotoxy(1, H + 3);
}