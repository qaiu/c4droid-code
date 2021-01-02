void drawrush()
{
float i,j=7,m=w/5;
float rushangle;
int ti_me=0;
tons *p;

while (rush==true||rushing==true )
{
if (rushing==true )
  {
  if (times==0)
    {
    rushangle=l_angle+180;
    warp.x=warp.rushton->x;
    warp.y=warp.rushton->y;
    }

  SDL_SetRenderDrawColor
  (render,150,126,42,255);
  for (i=0;i<360;i+=2)
    SDL_RenderDrawLine(render,
    (w/47)*cos (i/180*PI)+
    warp.x-speed*20*
    cos (rushangle/180*PI),
    (w/47)*sin (i/180*PI)+
    warp.y-speed*20*
    sin (rushangle/180*PI),
    (w/40)*cos (i/180*PI)+
    warp.x-speed*20*
    cos (rushangle/180*PI),
    (w/40)*sin (i/180*PI)+
    warp.y-speed*20*
    sin (rushangle/180*PI));

  SDL_SetRenderDrawColor
  (render,250,210,70,255);
  for (i=0;i<360;i+=2)
    SDL_RenderDrawLine(render,
    (w/47)*cos (i/180*PI)+
    warp.x-speed*10*
    cos (rushangle/180*PI),
    (w/47)*sin (i/180*PI)+
    warp.y-speed*10*
    sin (rushangle/180*PI),
    (w/40)*cos (i/180*PI)+
    warp.x-speed*10*
    cos (rushangle/180*PI),
    (w/40)*sin (i/180*PI)+
    warp.y-speed*10*
    sin (rushangle/180*PI));
if (warp.x>=30&&warp.x<=w-30)
  warp.x+=speed*2*
  cos (rushangle/180*PI);
if (warp.y>=30&&warp.y<=h-30)
  warp.y+=speed*2*
  sin (rushangle/180*PI);
if (warp.rushton->x>=30
  &&warp.rushton->x<=w-30)
  warp.rushton->x+=speed*(-4)*
  cos (rushangle/180*PI);
if (warp.rushton->y>=30
  &&warp.rushton->y<=h-30)
  warp.rushton->y+=speed*(-4)*
  sin (rushangle/180*PI);
  times++;
  if (times<=4)
    speed+=3;
  if (times>4)
    speed-=2;
  }
if (times==10)
  {
  if (warp.rushton->x<30)
    warp.rushton->x=30;
  if (warp.rushton->x>w-30)
    warp.rushton->x=w-30;
  if (warp.rushton->y<30)
    warp.rushton->y=30;
  if (warp.rushton->y>h-30)
    warp.rushton->y=h-30;
  rushing=false;
  }
SDL_Delay(10);

//画羊菇头
SDL_SetRenderDrawColor
(render,150,150,150,255);
for (i=0;i<360;i+=7)
  {
  SDL_RenderDrawPoint(render,
  (w/9)*cos (i/180*PI)+(w/8),
  (w/9)*sin (i/180*PI)+(h/6*5));
  SDL_RenderDrawLine(render,
  (w/16)*cos (i/180*PI)+(w/8)+
  (w/23)*cos (l_angle/180*PI)*lm,
  (w/16)*sin (i/180*PI)+(h/6*5)+
  (w/23)*sin (l_angle/180*PI)*lm,
  (w/15)*cos (i/180*PI)+(w/8)+
  (w/23)*cos (l_angle/180*PI)*lm,
  (w/15)*sin (i/180*PI)+(h/6*5)+
  (w/23)*sin (l_angle/180*PI)*lm);
  }
SDL_RenderDrawLine(render,
(w/8),(h/6*5),
(w/9)*cos(l_angle/180*PI)+(w/8),
(w/9)*sin(l_angle/180*PI)+(h/6*5));

//画小摁键
if (button_r==true )
  for (i=0;i<360;i+=7)
    SDL_RenderDrawPoint(render,
    (w/16)*cos (i/180*PI)+(w/12*11),
    (w/16)*sin (i/180*PI)+(h/6*5));

//画桶
SDL_SetRenderDrawColor
(render,50,50,255,255);
for 
(p=head;p->next!=NULL;)
  {
  p=p->next;
  for (i=0;i<360;i+=10)
    SDL_RenderDrawLine(render,
    (w/53)*cos (i/180*PI)+p->x,
    (w/53)*sin (i/180*PI)+p->y,
    (w/40)*cos (i/180*PI)+p->x,
    (w/40)*sin (i/180*PI)+p->y);
  }
SDL_SetRenderDrawColor
  (render,240,240,240,255);
  for (i=0;i<360;i+=2)
    SDL_RenderDrawLine(render,
    (w/47)*cos (i/180*PI)+warp.x,
    (w/47)*sin (i/180*PI)+warp.y,
    (w/40)*cos (i/180*PI)+warp.x,
    (w/40)*sin (i/180*PI)+warp.y);
  for (i=0;i<360;i+=2)
    SDL_RenderDrawPoint(render,
    (w/61.5)*cos (i/180*PI)+warp.x,
    (w/61.5)*sin (i/180*PI)+warp.y);

if (rush==true )
  {
  SDL_SetRenderDrawColor
  (render,250,210,70,255);
  SDL_RenderDrawLine(render,
  (w/10)*cos(l_angle/180*PI)
  +warp.rushton->x,
  (w/10)*sin(l_angle/180*PI)
  +warp.rushton->y,
  (w/10)*cos((l_angle+180)/180*PI)
  +warp.rushton->x,
  (w/10)*sin((l_angle+180)/180*PI)
  +warp.rushton->y);

  SDL_RenderDrawLine(render,
  (w/10)*cos(l_angle/180*PI)
  +warp.rushton->x,
  (w/10)*sin(l_angle/180*PI)
  +warp.rushton->y,
  (w/20)*cos((l_angle+15)/180*PI)
  +warp.rushton->x,
  (w/20)*sin((l_angle+15)/180*PI)
  +warp.rushton->y);

  SDL_RenderDrawLine(render,
  (w/10)*cos(l_angle/180*PI)
  +warp.rushton->x,
  (w/10)*sin(l_angle/180*PI)
  +warp.rushton->y,
  (w/20)*cos((l_angle-15)/180*PI)
  +warp.rushton->x,
  (w/20)*sin((l_angle-15)/180*PI)
  +warp.rushton->y);

  SDL_SetRenderDrawColor
  (render,250,210,70,255);
  for
  (i=45;i<136;i+=2)
    {
    SDL_RenderDrawLine(render,
    m*cos ((i+l_angle+j)
    /180*PI)+warp.rushton->x,
    m*sin ((i+l_angle+j)
    /180*PI)+warp.rushton->y,
    m*cos ((i+l_angle+j)
    /180*PI)+warp.rushton->x,
    m*sin ((i+l_angle+j)
    /180*PI)+warp.rushton->y);

    SDL_RenderDrawLine(render,
    m*cos((i+l_angle+180+j)
    /180*PI)+warp.rushton->x,
    m*sin((i+l_angle+180+j)
    /180*PI)+warp.rushton->y,
    m*cos((i+l_angle+180+j)
    /180*PI)+warp.rushton->x,
    m*sin((i+l_angle+180+j)
    /180*PI)+warp.rushton->y);
    }
  if (ti_me<13)
    m-=7;
  if ((ti_me/50)%2==1 )
    j+=0.3;
  else
    j-=0.3;
  ti_me++;
  }

SDL_RenderPresent(render);
SDL_SetRenderDrawColor
(render,30,30,30,255);
SDL_RenderClear(render);
}
button_m=true;
button_o=true;
}