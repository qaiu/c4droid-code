const int SW=540;
const int SH=960;
const int SBPP=32;

unsigned int rander(int);

struct{
	SDL_Surface *background;
	SDL_Surface *ground;
	SDL_Surface *pipe_top;
	SDL_Surface *pipe_bottom;
}
Map[2]={
	{NULL,NULL,NULL,NULL},
	{NULL,NULL,NULL,NULL}
};

struct{
	SDL_Surface *first;
	SDL_Surface *second;
	SDL_Surface *third;
}
Bird[3]={
	{NULL,NULL,NULL},
	{NULL,NULL,NULL},
	{NULL,NULL,NULL}
};

struct{
	SDL_Surface *caption;
	SDL_Surface *messagebox;
	SDL_Surface *button_start;
	SDL_Surface *text;
}
Ui={NULL,NULL,NULL,NULL};

struct{
	Mix_Chunk *point;
	Mix_Chunk *hit;
	Mix_Chunk *wing;
}
Sound={NULL,NULL,NULL};

Mix_Music *music=NULL;
SDL_Surface *screen=NULL;


SDL_Event event;

TTF_Font *textfont=NULL;
SDL_Color textcolor={195,196,125};

unsigned long fps,score=0;
short 
start=0,go=0,height=SH/2,displaymessagebox=-100,
whatday=rander(rand()),whichone=rander(rand()),
move[3]={-200,-400,-600},
length[3]={rander(rand())*10,rander(rand())*10,rander(rand())*10};

char scoredisplay[100];





