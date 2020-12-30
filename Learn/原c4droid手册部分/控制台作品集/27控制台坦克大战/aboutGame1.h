void tips(int t) {
  char *c = NULL;
  switch (t) {
  case (1):
    c = "⒈ 消灭10个敌人";
    break;
  case (2):
    c = "⒉ 坚持80秒";
    break;
  case (3):
    c = "⒊ 消灭10个敌人";
    break;
  case (4):
    c = "⒋ 消灭15个敌人";
    break;
  }
  out(2, 2, c);
  out(12, 13, "按任意键继续");
  while (getch() == '!') {
  }
  usleep(500000);
  clrscr();
}
void youWin() {
  clrscr();
  printf("\E[6;36m ");
  printf("          ▍★∴\n");
  printf("  ．．．▍▍.．█▍★∵../\n");
  printf("◥█▅██▅██▅██◤\n");
  printf("　◥████████◤\n");
  printf("～～◥██████◤～\n");
  printf("    ❝❝通关❞❞");
}
void youDie() {
  clrscr();
  printf("\E[6;33m ");
  printf("                  ...................\n");
  printf("                         ●○●○●○●○\n");
  printf("                        ტტტტტტტტტ\n");
  printf("                       ❝❝你输了❞❞");
}

void game1() {
  int over = 0;
  int a, b = 0;
  point = 0;
  while (level <= 4 && over == 0) {
    tips(level);
    switch (level) {
    case (1):
      mapMake();
      tankMake(0, 1);
      mapDraw();
      enemyHp = 2;
      getHpTime = 10;
      kills = 0;
      gameAble = 1;
      eSumMax = 2;
      while (gameAble) {
        if (kbhit()) {
          T[0].in = getch();
          move(0, T[0].in);
        }
        usleep(50000);
        fflush(stdin);
        fflush(stdout);

        attack(0);
        mineAttack();
        getHp();
        die(0);
        enemy();
        attack_();
        if (kills >= 10) {
          level++;
          break;
        }
        if (T[0].hp <= 0) {
          over = 1;
          break;
        }
      }
      break;
    case (2):
      mapMake();
      tankMake(0, 1);
      mapDraw();
      enemyHp = 2;
      getHpTime = 10;
      kills = 0;
      gameAble = 1;
      eSumMax = 2;
      while (gameAble) {
        if (kbhit()) {
          T[0].in = getch();
          move(0, T[0].in);
        }
        usleep(50000);
        fflush(stdin);
        fflush(stdout);

        attack(0);
        mineAttack();
        getHp();
        die(0);
        enemy();
        attack_();

        tt[1].F = clock();
        tt[1].D = (double)(tt[1].F - tt[1].S) / CLOCKS_PER_SEC;
        if (tt[1].D >= 1) {
          tt[1].S = clock();
          gameTime--;
        }
        if (gameTime <= 0) {
          level++;
          break;
        }
         if (T[0].hp <= 0) {
          over = 1;
          break;
        }
      }
      break;
    case (3):
      mapMake();
      T[0].a = 13;
      T[0].b = 13;
      T[0].c = 2;
      tankMake(0, 1);
      for (a = 4; a <= 8; a++)
        map[a][4] = s[4];
      for (a = 4; a <= 8; a++)
        map[a][7] = s[4];
      for (a = 4; a <= 8; a++)
        map[a][10] = s[5];
      for (a = 4; a <= 8; a++)
        map[a][13] = s[4];
      for (a = 4; a <= 8; a++)
        map[a][16] = s[4];
      for (a = 4; a <= 8; a++)
        map[a][19] = s[5];
      for (a = 4; a <= 8; a++)
        map[a][22] = s[4];
      mapDraw();
      enemyHp = 2;
      getHpTime = 9;
      gameAble = 1;
      eSumMax = 2;
      kills = 0;
      while (gameAble) {
        if (kbhit()) {
          T[0].in = getch();
          move(0, T[0].in);
        }
        usleep(50000);
        fflush(stdin);
        fflush(stdout);
        attack(0);
        mineAttack();
        getHp();
        die(0);
        enemy();
        attack_();
        if (kills == 10) {
          level++;
          break;
        }
        if (T[0].hp <= 0) {
          over = 1;
          break;
        }
      }
      break;
    case (4):
      mapMake();
      T[0].a = 13;
      T[0].b = 13;
      T[0].c = 2;
      tankMake(0, 1);

      for (a = 5; a <= 8; a++)
        map[a][19] = s[5];
      for (a = 5; a <= 8; a++)
        map[a][18] = s[5];
      for (a = 5; a <= 8; a++)
        map[a][17] = s[5];
      for (a = 5; a <= 8; a++)
        map[a][9] = s[5];
      for (a = 5; a <= 8; a++)
        map[a][8] = s[5];
      for (a = 5; a <= 8; a++)
        map[a][7] = s[5];
      mapDraw();

      enemyHp = 6;
      getHpTime = 8;
      gameAble = 1;
      eSumMax = 3;
      kills = 0;

      while (gameAble) {
        if (kbhit()) {
          T[0].in = getch();
          move(0, T[0].in);
        }
        usleep(80000);
        fflush(stdin);
        fflush(stdout);

        attack(0);
        mineAttack();
        getHp();
        die(0);
        enemy();
        attack_();
        if (kills == 15) {
          level = 10;
          break;
        }
        if (T[0].hp <= 0) {
          over = 1;
          break;
        }
      }
      break;
    }
  }

  if (level == 10)
    youWin();
  if (over == 1)
    youDie();
}