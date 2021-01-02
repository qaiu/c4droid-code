int M_run(void *ptr);
int M_W, M_H;
int M_quit = 1;
SDL_Texture *M_t[5];
SDL_Renderer *M_r = NULL;
SDL_Thread *M_thread = NULL;
TTF_Font *M_F = NULL;
SDL_Event M_e;
void M_Init(SDL_Window * uw, SDL_Renderer * ur) {
  TTF_Init();
  SDL_GetWindowSize(uw, &M_W, &M_H);
  M_r = ur;
  M_quit = 0;
  M_thread = SDL_CreateThread(M_run, "T", (void *)NULL);
 /**********************/
  SDL_Surface *s = NULL;
  /**********************/
  M_F = TTF_OpenFont("../../../Resources/Font/D2-coding.ttf", 100);
  s = IMG_Load("mine/bblue.png");
  M_t[3] = SDL_CreateTextureFromSurface(M_r, s);
  s = IMG_Load("mine/bred.png");
  M_t[4] = SDL_CreateTextureFromSurface(M_r, s);
  SDL_FreeSurface(s);
}
/* ❐❐❐❐❐❐❐❐❐❐❐❐❐❐❐❐❐ */
void M_Quit() {
  int a;
  M_quit = 1;
  for (a = 0; a < 5; a++)
    SDL_DestroyTexture(M_t[a]);
  TTF_CloseFont(M_F);
}
int M_run(void *ptr) {
  while (!M_quit) {
    SDL_PollEvent(&M_e);
    SDL_Delay(10);
  }
}
SDL_Event getEve() {
  return M_e;
}
/* ❐❐❐❐❐❐❐❐❐❐❐❐❐❐❐❐❐ */
class M_Button {
private:
  int condition = 0;
  SDL_Rect body;
  SDL_Texture *T = NULL, *bg = NULL;
public:
    M_Button(int x, int y, int w, int h, char *ch = "0") {
    body.x = x;
    body.y = y;
    body.w = w;
    body.h = h;
    bg = M_t[3];
    text(ch);
  } 
  int touch() {
    int x = M_W * M_e.tfinger.x, y = M_H * M_e.tfinger.y;
    //condition =0;
    if (M_e.type == SDL_FINGERDOWN||M_e.type == SDL_FINGERMOTION) {
      if (x > body.x && x < body.x + body.w && y > body.y
          && y < body.y + body.h)
        condition = 1;
    }
 
    if (condition == 1&&M_e.type == SDL_FINGERUP) {
      if (x > body.x && x < body.x + body.w && y > body.y
          && y < body.y + body.h)
        condition = 2;
    }
    if (condition==2) {
      bg = M_t[4];
      return 1;
    } else {
      bg = M_t[3];
      return 0;
    }
  }
  void text(char *ch, int r = 5, int g = 5, int b = 5, int a = 5) {
    SDL_Color color = { r, g, b, a };
    SDL_Surface *s = TTF_RenderUTF8_Blended(M_F, ch, color);
    T = SDL_CreateTextureFromSurface(M_r, s);
    SDL_FreeSurface(s);
  }
  void show() {
    SDL_RenderCopy(M_r, bg, NULL, &body);
    SDL_RenderCopy(M_r, T, NULL, &body);
  }
  ~M_Button() {
    SDL_DestroyTexture(T);

}};
  /* ❐❐❐❐❐❐❐❐❐❐❐❐❐❐❐ */