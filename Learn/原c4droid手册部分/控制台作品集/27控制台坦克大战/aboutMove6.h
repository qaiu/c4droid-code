void turnBack(int u) {
  switch (T[u].c) {
  case (2):
    if (moveAble(u, 1, 0)) {
      tankMake(u, 0);
      T[u].a++;
      tankMake(u, 1);
      mapDraw();
    }
    break;
  case (4):
    if (moveAble(u, 0, 1)) {
      tankMake(u, 0);
      T[u].b++;
      tankMake(u, 1);
      mapDraw();
    }
    break;
  case (6):
    if (moveAble(u, 0, -1)) {
      tankMake(u, 0);
      T[u].b--;
      tankMake(u, 1);
      mapDraw();
    }
    break;
  case (8):
    if (moveAble(u, -1, 0)) {
      tankMake(u, 0);
      T[u].a--;
      tankMake(u, 1);
      mapDraw();
    }
    break;
  }
}

void attack_() {
  /* 炮弹的抵消 */
  int a;
  switch (T[0].Ac) {
  case (2):
    for (a = 1; a <= eSum; a++) {
      if ((T[0].Aa == T[a].Aa || T[0].Aa + 1 == T[a].Aa) && T[0].Ab == T[a].Ab)
        if (T[0].Ad == 2 && T[a].Ad == 2) {
          T[0].Ad = 0;
          T[a].Ad = 0;
          map[T[0].Aa][T[0].Ab] = s[0];
          map[T[a].Aa][T[a].Ab] = s[0];
          mapDraw();
        }
    }
    break;
  case (4):
    for (a = 1; a <= eSum; a++) {
      if (T[0].Aa == T[a].Aa && (T[0].Ab == T[a].Ab || T[0].Ab - 1 == T[a].Ab))
        if (T[0].Ad == 2 && T[a].Ad == 2) {
          T[0].Ad = 0;
          T[a].Ad = 0;
          map[T[0].Aa][T[0].Ab] = s[0];
          map[T[a].Aa][T[a].Ab] = s[0];
          mapDraw();
        }
    }
    break;
  case (6):
    for (a = 1; a <= eSum; a++) {
      if (T[0].Aa == T[a].Aa && (T[0].Ab == T[a].Ab || T[0].Ab + 1 == T[a].Ab))
        if (T[0].Ad == 2 && T[a].Ad == 2) {
          T[0].Ad = 0;
          T[a].Ad = 0;
          map[T[0].Aa][T[0].Ab] = s[0];

          map[T[a].Aa][T[a].Ab] = s[0];
          mapDraw();
        }
    }
    break;
  case (8):
    for (a = 1; a <= eSum; a++) {
      if ((T[0].Aa == T[a].Aa || T[0].Aa - 1 == T[a].Aa) && T[0].Ab == T[a].Ab)
        if (T[0].Ad == 2 && T[a].Ad == 2) {
          T[0].Ad = 0;
          T[a].Ad = 0;
          map[T[0].Aa][T[0].Ab] = s[0];
          map[T[a].Aa][T[a].Ab] = s[0];
          mapDraw();
        }
    }
    break;
  }
}
void getHp() {
  int a, b, c;
  tt[0].F = clock();
  tt[0].D = (double)(tt[0].F - tt[0].S) / CLOCKS_PER_SEC;
  if (tt[0].D >= getHpTime) {
    tt[0].S = clock();
    a = (rand() % (H - 3) + 2);
    b = (rand() % (W - 3) + 2);
    if (map[a][b] == s[0]) {
      map[a][b] = s[2];
    }
  }
}
void enemy() {
  int a;
  enemyMake();
  for (a = 1; a <= eSum; a++) {
    if (T[a].hp > 0) {
      attack(a);
      emove(a);
    }
    if (die(a)) {
      point++;
      kills++;
    }
  }
}
void mineAttack() {
  int u = 0;
  if (Ad == 2) {
    switch (Ac) {
    case (2):
      map[T[u].a - 1][T[u].b - 1] = s[0];
      map[T[u].a - 2][T[u].b - 1] = s[0];
      map[T[u].a - 2][T[u].b] = s[0];
      map[T[u].a - 2][T[u].b + 1] = s[0];
      map[T[u].a - 1][T[u].b + 1] = s[0];
      break;
    case (4):
      map[T[u].a - 1][T[u].b - 1] = s[0];
      map[T[u].a - 1][T[u].b - 2] = s[0];
      map[T[u].a][T[u].b - 2] = s[0];
      map[T[u].a + 1][T[u].b - 2] = s[0];
      map[T[u].a + 1][T[u].b - 1] = s[0];
      break;
    case (6):
      map[T[u].a - 1][T[u].b + 1] = s[0];
      map[T[u].a - 1][T[u].b + 2] = s[0];
      map[T[u].a][T[u].b + 2] = s[0];
      map[T[u].a + 1][T[u].b + 2] = s[0];
      map[T[u].a + 1][T[u].b + 1] = s[0];
      break;
    case (8):
      map[T[u].a + 1][T[u].b - 1] = s[0];
      map[T[u].a + 2][T[u].b - 1] = s[0];
      map[T[u].a + 2][T[u].b] = s[0];
      map[T[u].a + 2][T[u].b + 1] = s[0];
      map[T[u].a + 1][T[u].b + 1] = s[0];
      break;
    }
    Ad = 0;
    mapDraw();
  }
  if (Ad == 1) {
    Ac = T[0].c;
    switch (Ac) {
    case (2):
      if ((map[T[u].a - 1][T[u].b - 1] == s[2] ||
           map[T[u].a - 2][T[u].b - 1] == s[2] ||
           map[T[u].a - 2][T[u].b] == s[2] ||
           map[T[u].a - 2][T[u].b + 1] == s[2] ||
           map[T[u].a - 1][T[u].b + 1] == s[2]) && T[0].hp < myHpMax)
        T[0].hp++;
      if (T[0].a > 2) {
        map[T[u].a - 1][T[u].b - 1] = s[3];
        map[T[u].a - 2][T[u].b - 1] = s[3];
        map[T[u].a - 2][T[u].b] = s[3];
        map[T[u].a - 2][T[u].b + 1] = s[3];
        map[T[u].a - 1][T[u].b + 1] = s[3];
        Ad = 2;
      }
      break;
    case (4):
      if ((map[T[u].a - 1][T[u].b - 1] == s[2] ||
           map[T[u].a - 1][T[u].b - 2] == s[2] ||
           map[T[u].a][T[u].b - 2] == s[2] ||
           map[T[u].a + 1][T[u].b - 2] == s[2] ||
           map[T[u].a + 1][T[u].b - 1] == s[2] && T[0].hp < myHpMax))
        T[0].hp++;
      if (T[0].b > 2) {
        map[T[u].a - 1][T[u].b - 1] = s[3];
        map[T[u].a - 1][T[u].b - 2] = s[3];
        map[T[u].a][T[u].b - 2] = s[3];
        map[T[u].a + 1][T[u].b - 2] = s[3];
        map[T[u].a + 1][T[u].b - 1] = s[3];
        Ad = 2;
      }
      break;
    case (6):
      if ((map[T[u].a - 1][T[u].b + 1] == s[2] ||
           map[T[u].a - 1][T[u].b + 2] == s[2]
           || map[T[u].a][T[u].b + 2] == s[2]
           || map[T[u].a + 1][T[u].b + 2] == s[2]
           || map[T[u].a + 1][T[u].b + 1] == s[2] && T[0].hp < myHpMax))

        T[0].hp++;
      if (T[0].b < W - 2) {
        map[T[u].a - 1][T[u].b + 1] = s[3];
        map[T[u].a - 1][T[u].b + 2] = s[3];
        map[T[u].a][T[u].b + 2] = s[3];
        map[T[u].a + 1][T[u].b + 2] = s[3];
        map[T[u].a + 1][T[u].b + 1] = s[3];
        Ad = 2;
      }
      break;
    case (8):
      if ((map[T[u].a + 1][T[u].b - 1] == s[2] ||
           map[T[u].a + 2][T[u].b - 1] == s[2] ||
           map[T[u].a + 2][T[u].b] == s[2] ||
           map[T[u].a + 2][T[u].b + 1] == s[2] ||
           map[T[u].a + 1][T[u].b + 1] == s[2] && T[0].hp < myHpMax))

        T[0].hp++;
      if (T[0].a < H - 2) {
        map[T[u].a + 1][T[u].b - 1] = s[3];
        map[T[u].a + 2][T[u].b - 1] = s[3];
        map[T[u].a + 2][T[u].b] = s[3];
        map[T[u].a + 2][T[u].b + 1] = s[3];
        map[T[u].a + 1][T[u].b + 1] = s[3];
        Ad = 2;
      }
      break;
    }

    mapDraw();
  }
}

int die(int u) {
  int a, r = 0;
  if (u) {
    if (T[u].hp > 0) {
      if (DieAble(u, T[0].p) || DieAble(u, s[3])) {
        T[0].Ad = 0;
        T[u].hp--;
        if (T[u].hp > 0) {
          tankMake(u, 1);
          mapDraw();
        } else {
          r = 1;
          T[u].Ad = 0;
          tankMake(u, 0);
          if (map[T[u].Aa][T[u].Ab] == T[u].p)
            map[T[u].Aa][T[u].Ab] = s[0];
          mapDraw();
        }
      }
    }
    if (T[u].hp > 0) {
      for (a = 1; a <= eSum; a++) {
        if (u == a)
          continue;
        if (DieAble(u, T[a].p)) {
          T[a].Ad = 0;
          tankMake(u, 1);
          mapDraw();
        }
      }
    }
  } else {
    for (a = 1; a <= eSum; a++) {
      if (DieAble(0, T[a].p)) {
        T[a].Ad = 0;
        T[0].hp--;
        tankMake(0, 1);
        mapDraw();
      }
    }
  }
  return (r);
}

void enemyMake() {
  int a = 0, b = 0, c = 1;
  eSum = 0;
  for (a = 1; a <= 10; a++)
    if (T[a].hp > 0)
      eSum++;

  if (eSum < eSumMax) {
    for (a = 1; a <= 10; a++) {
      if (T[a].hp <= 0)
        break;
    }

    T[a].a = (rand() % (H - 3) + 2);
    T[a].b = (rand() % (W - 3) + 2);
    T[a].c = 2;
    for (b = 0; b <= eSum; b++) {
      if (a == b)
        continue;
      if (DieAble(a, T[b].s)) {
        c = 0;
        break;
      }
    }
     if (c) {
      for (b = 1; b <= 5; b++)
        if (DieAble(a, s[b])) {
          c = 0;
          break;
        }
    }
    if (c)
      T[a].hp = enemyHp;
    else
      T[a].hp = 0;
  }
}
int turnAble(int u, char c) {
  char d = s[0];
  int r = 0, a = 0, b = 0;
  tankMake(u, 0);
  switch (c) {
  case (Up):
    r =
      map[a + T[u].a - 1][b + T[u].b] == d &&
      map[a + T[u].a][b + T[u].b] == d &&
      map[a + T[u].a][b + T[u].b - 1] == d &&
      map[a + T[u].a][b + T[u].b + 1] == d &&
      map[a + T[u].a + 1][b + T[u].b - 1] == d
      && map[a + T[u].a + 1][b + T[u].b + 1] == d;
    break;
  case (Left):
    r =
      map[a + T[u].a][b + T[u].b - 1] == d &&
      map[a + T[u].a][b + T[u].b] == d &&
      map[a + T[u].a - 1][b + T[u].b] == d &&
      map[a + T[u].a + 1][b + T[u].b] == d &&
      map[a + T[u].a - 1][b + T[u].b + 1] == d
      && map[a + T[u].a + 1][b + T[u].b + 1] == d;
    break;
  case (Right):
    r =
      map[a + T[u].a][b + T[u].b + 1] == d &&
      map[a + T[u].a][b + T[u].b] == d &&
      map[a + T[u].a - 1][b + T[u].b] == d &&
      map[a + T[u].a + 1][b + T[u].b] == d &&
      map[a + T[u].a - 1][b + T[u].b - 1] == d
      && map[a + T[u].a + 1][b + T[u].b - 1] == d;
    break;
  case (Down):
    r =
      map[a + T[u].a + 1][b + T[u].b] == d &&
      map[a + T[u].a][b + T[u].b] == d &&
      map[a + T[u].a][b + T[u].b - 1] == d &&
      map[a + T[u].a][b + T[u].b + 1] == d &&
      map[a + T[u].a - 1][b + T[u].b - 1] == d
      && map[a + T[u].a - 1][b + T[u].b + 1] == d;
    break;
  }
  tankMake(u, 1);
  return r;
}
void emove(int u) {
  int ein;
  t[u].F = clock();
  t[u].D = (double)(t[u].F - t[u].S) / CLOCKS_PER_SEC;
  if (t[u].D >= 0.5) {
    t[u].S = clock();
    ein = rand() % 4;
    switch (ein) {
    case (0):
      T[u].in = '2';
      break;
    case (1):
      T[u].in = '4';
      break;
    case (2):
      T[u].in = '6';
      break;
    case (3):
      T[u].in = '8';
      break;
    }
    move(u, T[u].in);
    T[u].Ad = 1;
  }
}



int DieAble(int u, char d) {
  int ret;
  switch (T[u].c) {
  case (2):
    ret =
      map[T[u].a - 1][T[u].b] == d ||
      map[T[u].a][T[u].b] == d ||
      map[T[u].a][T[u].b - 1] == d ||
      map[T[u].a][T[u].b + 1] == d || map[T[u].a + 1][T[u].b - 1] == d
      || map[T[u].a + 1][T[u].b + 1] == d;
    break;
  case (4):
    ret =
      map[T[u].a][T[u].b - 1] == d ||
      map[T[u].a][T[u].b] == d ||
      map[T[u].a - 1][T[u].b] == d ||
      map[T[u].a + 1][T[u].b] == d || map[T[u].a - 1][T[u].b + 1] == d
      || map[T[u].a + 1][T[u].b + 1] == d;
    break;
  case (6):
    ret =
      map[T[u].a][T[u].b + 1] == d ||
      map[T[u].a][T[u].b] == d ||
      map[T[u].a - 1][T[u].b] == d ||
      map[T[u].a + 1][T[u].b] == d || map[T[u].a - 1][T[u].b - 1] == d
      || map[T[u].a + 1][T[u].b - 1] == d;
    break;
  case (8):
    ret =
      map[T[u].a + 1][T[u].b] == d ||
      map[T[u].a][T[u].b] == d ||
      map[T[u].a][T[u].b - 1] == d ||
      map[T[u].a][T[u].b + 1] == d || map[T[u].a - 1][T[u].b - 1] == d
      || map[T[u].a - 1][T[u].b + 1] == d;
    break;
  }
  return ret;
}


void attack(int u) {

  if (T[u].Ad == 1) {
    T[u].Ac = T[u].c;
    switch (T[u].Ac) {
    case (2):
      T[u].Aa = T[u].a - 1;
      T[u].Ab = T[u].b;
      break;
    case (4):
      T[u].Aa = T[u].a;
      T[u].Ab = T[u].b - 1;
      break;
    case (6):
      T[u].Aa = T[u].a;
      T[u].Ab = T[u].b + 1;
      break;
    case (8):
      T[u].Aa = T[u].a + 1;
      T[u].Ab = T[u].b;
      break;
    }
    T[u].Ad = 2;
  }

  t[u].F = clock();
  t[u].D = (double)(t[u].F - t[u].S) / CLOCKS_PER_SEC;
  if (T[u].Ad == 2 && t[u].D >= 0.05) {
    t[u].S = clock();
    switch (T[u].Ac) {
    case (2):
      if (map[T[u].Aa][T[u].Ab] == T[u].p)
        map[T[u].Aa][T[u].Ab] = s[0];
      T[u].Aa--;
      if (map[T[u].Aa][T[u].Ab] == s[5]) {
        T[u].Ad = 0;
      } else {
        if (map[T[u].Aa][T[u].Ab] == s[4] || map[T[u].Aa][T[u].Ab] == s[2]) {
          map[T[u].Aa][T[u].Ab] = s[0];
          T[u].Ad = 0;
        } else {
          if (T[u].Aa > 0)
            map[T[u].Aa][T[u].Ab] = T[u].p;
          else
            T[u].Ad = 0;
        }
      }
      mapDraw();
      break;
    case (4):
      if (map[T[u].Aa][T[u].Ab] == T[u].p)
        map[T[u].Aa][T[u].Ab] = s[0];
      T[u].Ab--;
      if (map[T[u].Aa][T[u].Ab] == s[5]) {
        T[u].Ad = 0;
      } else {
        if (map[T[u].Aa][T[u].Ab] == s[4] || map[T[u].Aa][T[u].Ab] == s[2]) {
          map[T[u].Aa][T[u].Ab] = s[0];
          T[u].Ad = 0;
        } else {
          if (T[u].Ab > 0)
            map[T[u].Aa][T[u].Ab] = T[u].p;
          else
            T[u].Ad = 0;
        }
      }
      mapDraw();
      break;
    case (6):
      if (map[T[u].Aa][T[u].Ab] == T[u].p)
        map[T[u].Aa][T[u].Ab] = s[0];
      T[u].Ab++;
      if (map[T[u].Aa][T[u].Ab] == s[5]) {
        T[u].Ad = 0;
      } else {
        if (map[T[u].Aa][T[u].Ab] == s[4] || map[T[u].Aa][T[u].Ab] == s[2]) {
          map[T[u].Aa][T[u].Ab] = s[0];
          T[u].Ad = 0;
        } else {
          if (T[u].Ab < W)
            map[T[u].Aa][T[u].Ab] = T[u].p;
          else
            T[u].Ad = 0;
        }
      }
      mapDraw();
      break;
    case (8):
      if (map[T[u].Aa][T[u].Ab] == T[u].p)
        map[T[u].Aa][T[u].Ab] = s[0];
      T[u].Aa++;
      if (map[T[u].Aa][T[u].Ab] == s[5]) {
        T[u].Ad = 0;
      } else {
        if (map[T[u].Aa][T[u].Ab] == s[4] || map[T[u].Aa][T[u].Ab] == s[2]) {
          map[T[u].Aa][T[u].Ab] = s[0];
          T[u].Ad = 0;
        } else {
          if (T[u].Aa < H)
            map[T[u].Aa][T[u].Ab] = T[u].p;
          else
            T[u].Ad = 0;
        }
      }
      mapDraw();
      break;
    }
  }
}

void move(int u, char in) {
  switch (in) {
  case (Left):
    if (T[u].c != 4) {
      if (turnAble(u, Left)) {
        tankMake(u, 0);
        T[u].c = 4;
        tankMake(u, 1);
        mapDraw();
      }
    } else {
      if (moveAble(u, 0, -1)) {
        tankMake(u, 0);
        T[u].b--;
        tankMake(u, 1);
        mapDraw();
      }
    }
    break;
  case (Right):
    if (T[u].c != 6) {
      if (turnAble(u, Right)) {
        tankMake(u, 0);
        T[u].c = 6;
        tankMake(u, 1);
        mapDraw();
      }
    } else {
      if (moveAble(u, 0, +1)) {
        tankMake(u, 0);
        T[u].b++;
        tankMake(u, 1);
        mapDraw();
      }
    }
    break;
  case (Up):

    if (T[u].c != 2) {
      if (turnAble(u, Up)) {
        tankMake(u, 0);
        T[u].c = 2;
        tankMake(u, 1);
        mapDraw();
      }
    } else {
      if (moveAble(u, -1, 0)) {
        tankMake(u, 0);
        T[u].a--;
        tankMake(u, 1);
        mapDraw();
      }
    }
    break;
  case (Down):
    if (T[u].c != 8) {
      if (turnAble(u, Down)) {
        tankMake(u, 0);
        T[u].c = 8;
        tankMake(u, 1);
        mapDraw();
      }
    } else {
      if (moveAble(u, +1, 0)) {
        tankMake(u, 0);
        T[u].a++;
        tankMake(u, 1);
        mapDraw();
      }
    }
    break;
  case (inAttack1):
    if (T[u].Ad != 2)
      T[u].Ad = 1;
    break;
  case (inAttack2):
    Ad = 1;
    break;

  case (Back):
    turnBack(0);
    break;
  }
}


int moveAble(int u, int a, int b) {
  char d = s[0];
  int r = 0;
  tankMake(u, 0);
  switch (T[u].c) {
  case (2):
    r =
      map[a + T[u].a - 1][b + T[u].b] == d &&
      map[a + T[u].a][b + T[u].b] == d &&
      map[a + T[u].a][b + T[u].b - 1] == d &&
      map[a + T[u].a][b + T[u].b + 1] == d &&
      map[a + T[u].a + 1][b + T[u].b - 1] == d
      && map[a + T[u].a + 1][b + T[u].b + 1] == d;
    break;
  case (4):
    r =
      map[a + T[u].a][b + T[u].b - 1] == d &&
      map[a + T[u].a][b + T[u].b] == d &&
      map[a + T[u].a - 1][b + T[u].b] == d &&
      map[a + T[u].a + 1][b + T[u].b] == d &&
      map[a + T[u].a - 1][b + T[u].b + 1] == d
      && map[a + T[u].a + 1][b + T[u].b + 1] == d;
    break;
  case (6):
    r =
      map[a + T[u].a][b + T[u].b + 1] == d &&
      map[a + T[u].a][b + T[u].b] == d &&
      map[a + T[u].a - 1][b + T[u].b] == d &&
      map[a + T[u].a + 1][b + T[u].b] == d &&
      map[a + T[u].a - 1][b + T[u].b - 1] == d
      && map[a + T[u].a + 1][b + T[u].b - 1] == d;
    break;
  case (8):
    r =
      map[a + T[u].a + 1][b + T[u].b] == d &&
      map[a + T[u].a][b + T[u].b] == d &&
      map[a + T[u].a][b + T[u].b - 1] == d &&
      map[a + T[u].a][b + T[u].b + 1] == d &&
      map[a + T[u].a - 1][b + T[u].b - 1] == d
      && map[a + T[u].a - 1][b + T[u].b + 1] == d;
    break;
  }
  tankMake(u, 1);
  return r;
}

void tankMake(int u, int e) {
  if (e) {
    switch (T[u].c) {
    case (2):
      map[T[u].a - 1][T[u].b] = T[u].s;
      map[T[u].a][T[u].b] = T[u].h;
      map[T[u].a][T[u].b - 1] = T[u].s;
      map[T[u].a][T[u].b + 1] = T[u].s;
      map[T[u].a + 1][T[u].b - 1] = T[u].s;
      map[T[u].a + 1][T[u].b + 1] = T[u].s;
      break;
    case (4):
      map[T[u].a][T[u].b - 1] = T[u].s;
      map[T[u].a][T[u].b] = T[u].h;
      map[T[u].a - 1][T[u].b] = T[u].s;
      map[T[u].a + 1][T[u].b] = T[u].s;
      map[T[u].a - 1][T[u].b + 1] = T[u].s;
      map[T[u].a + 1][T[u].b + 1] = T[u].s;
      break;
    case (6):
      map[T[u].a][T[u].b + 1] = T[u].s;
      map[T[u].a][T[u].b] = T[u].h;
      map[T[u].a - 1][T[u].b] = T[u].s;
      map[T[u].a + 1][T[u].b] = T[u].s;
      map[T[u].a - 1][T[u].b - 1] = T[u].s;
      map[T[u].a + 1][T[u].b - 1] = T[u].s;
      break;
    case (8):
      map[T[u].a + 1][T[u].b] = T[u].s;
      map[T[u].a][T[u].b] = T[u].h;
      map[T[u].a][T[u].b - 1] = T[u].s;
      map[T[u].a][T[u].b + 1] = T[u].s;
      map[T[u].a - 1][T[u].b - 1] = T[u].s;
      map[T[u].a - 1][T[u].b + 1] = T[u].s;
      break;
    }
  } else {
    switch (T[u].c) {
    case (2):
      map[T[u].a - 1][T[u].b] = s[0];
      map[T[u].a][T[u].b] = s[0];
      map[T[u].a][T[u].b - 1] = s[0];
      map[T[u].a][T[u].b + 1] = s[0];
      map[T[u].a + 1][T[u].b - 1] = s[0];
      map[T[u].a + 1][T[u].b + 1] = s[0];
      break;
    case (4):
      map[T[u].a][T[u].b - 1] = s[0];
      map[T[u].a][T[u].b] = s[0];
      map[T[u].a - 1][T[u].b] = s[0];
      map[T[u].a + 1][T[u].b] = s[0];
      map[T[u].a - 1][T[u].b + 1] = s[0];
      map[T[u].a + 1][T[u].b + 1] = s[0];
      break;
    case (6):
      map[T[u].a][T[u].b + 1] = s[0];
      map[T[u].a][T[u].b] = s[0];
      map[T[u].a - 1][T[u].b] = s[0];
      map[T[u].a + 1][T[u].b] = s[0];
      map[T[u].a - 1][T[u].b - 1] = s[0];
      map[T[u].a + 1][T[u].b - 1] = s[0];
      break;
    case (8):
      map[T[u].a + 1][T[u].b] = s[0];
      map[T[u].a][T[u].b] = s[0];
      map[T[u].a][T[u].b - 1] = s[0];
      map[T[u].a][T[u].b + 1] = s[0];
      map[T[u].a - 1][T[u].b - 1] = s[0];
      map[T[u].a - 1][T[u].b + 1] = s[0];
      break;
    }
  }
}