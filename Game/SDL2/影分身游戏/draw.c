void draw()
{
float i;
tons *p;

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
if (button_o==true )
  for (i=0;i<360;i+=7)
    SDL_RenderDrawPoint(render,
    (w/32)*cos (i/180*PI)+(w/8*7),
    (w/32)*sin (i/180*PI)+(h/7*5));
if (button_r==true )
  for (i=0;i<360;i+=7)
    SDL_RenderDrawPoint(render,
    (w/32)*cos (i/180*PI)+(w/12*11),
    (w/32)*sin (i/180*PI)+(h/6*5));

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

//锁定桶
if (tottton==false )
  {
  totton=false;
  warp.toton=NULL;
  tottton=true;
  }

if (totton==true )
  {
  SDL_SetRenderDrawColor
  (render,250,210,70,255);
  for (i=0;i<360;i+=10)
    SDL_RenderDrawLine(render,
    (w/53)*cos (i/180*PI)
    +warp.toton->x,
    (w/53)*sin (i/180*PI)
    +warp.toton->y,
    (w/40)*cos (i/180*PI)
    +warp.toton->x,
    (w/40)*sin (i/180*PI)
    +warp.toton->y);
  }

if (intton==true )
  {
  SDL_SetRenderDrawColor
  (render,250,210,70,255);
  for (i=0;i<360;i+=10)
    SDL_RenderDrawLine(render,
    (w/53)*cos (i/180*PI)+warp.x,
    (w/53)*sin (i/180*PI)+warp.y,
    (w/40)*cos (i/180*PI)+warp.x,
    (w/40)*sin (i/180*PI)+warp.y);
  }
//画瞬移
if (m==true )
  {
  SDL_SetRenderDrawColor
  (render,250,210,70,255);
  //画摁键
  for (i=0;i<360;i++)
    SDL_RenderDrawPoint(render,
  (w/16)*cos (i/180*PI)+(w/20*16),
  (w/16)*sin (i/180*PI)+(h/6*5));
  //瞬移点
  for (i=0;i<360;i++)
    SDL_RenderDrawLine(render,
    (w/53)*cos (i/180*PI)+warp.x,
    (w/53)*sin (i/180*PI)+warp.y,
    (w/40)*cos (i/180*PI)+warp.x,
    (w/40)*sin (i/180*PI)+warp.y);
  SDL_RenderPresent(render);
  m=false;
  SDL_Delay(100);
  }
//画摁键
if (button_m==true )
  {
  SDL_SetRenderDrawColor
  (render,150,150,150,255);
  for (i=0;i<360;i+=7)
    SDL_RenderDrawPoint(render,
    (w/16)*cos (i/180*PI)+(w/20*16),
    (w/16)*sin (i/180*PI)+(h/6*5));
  }
//画warp
if (intton==false&&other==false )
  {
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
  SDL_SetRenderDrawColor
  (render,250,210,70,255);
  for
  (i=l_angle-30;i<l_angle+30;i+=2)
    SDL_RenderDrawLine(render,
    (w/53)*cos (i/180*PI)+warp.x,
    (w/53)*sin (i/180*PI)+warp.y,
    (w/36.4)*cos (i/180*PI)+warp.x,
    (w/36.4)*sin (i/180*PI)+warp.y);
  }
//分身
if (other==true )
  {
  //原
  SDL_SetRenderDrawColor
  (render,240,240,240,255);
  for (i=0;i<360;i+=2)
    SDL_RenderDrawLine(render,
    (w/47)*cos (i/180*PI)+warp.x2,
    (w/47)*sin (i/180*PI)+warp.y2,
    (w/40)*cos (i/180*PI)+warp.x2,
    (w/40)*sin (i/180*PI)+warp.y2);
  for (i=0;i<360;i+=2)
    SDL_RenderDrawPoint(render,
    (w/61.5)*cos (i/180*PI)+warp.x2,
    (w/61.5)*sin (i/180*PI)+warp.y2);
  //现
  SDL_SetRenderDrawColor
  (render,250,210,70,255);
  for (i=0;i<360;i+=2)
    SDL_RenderDrawLine(render,
    (w/47)*cos (i/180*PI)+warp.x,
    (w/47)*sin (i/180*PI)+warp.y,
    (w/40)*cos (i/180*PI)+warp.x,
    (w/40)*sin (i/180*PI)+warp.y);
  //区域线
  for (i=0;i<360;i++)
    SDL_RenderDrawPoint(render,
    (w/otr)*cos (i/180*PI)+warp.x2,
    (w/otr)*sin (i/180*PI)+warp.y2);
  if (otr>3)
    otr--;
  }

//光环
SDL_SetRenderDrawColor
(render,250,210,70,255);
for
(i=1;i<91;i+=2)
  {
  SDL_RenderDrawLine(render,
  (w/30.8)*cos ((i+warp.round)
  /180*PI)+warp.x,
  (w/30.8)*sin ((i+warp.round)
  /180*PI)+warp.y,
  (w/26.7)*cos ((i+warp.round)
  /180*PI)+warp.x,
  (w/26.7)*sin ((i+warp.round)
  /180*PI)+warp.y);

  SDL_RenderDrawLine(render,
  (w/30.8)*cos
  ((i+warp.round+180)
  /180*PI)+warp.x,
  (w/30.8)*sin
  ((i+warp.round+180)
  /180*PI)+warp.y,
  (w/26.7)*cos
  ((i+warp.round+180)
  /180*PI)+warp.x,
  (w/26.7)*sin
  ((i+warp.round+180)
  /180*PI)+warp.y);
}
//瞬移位置
if (other==false )
SDL_RenderDrawPoint(render,
warp.move*cos
(l_angle/180*PI)+warp.x,
warp.move*sin
(l_angle/180*PI)+warp.y);

//特殊摁键
if (other==true )
  {
  SDL_SetRenderDrawColor
  (render,150,150,150,255);
  for (i=0;i<360;i+=7)
    SDL_RenderDrawPoint(render,
    (w/32)*cos (i/180*PI)+w/8*7,
    (w/32)*sin (i/180*PI)+h/6*5);
  SDL_SetRenderDrawColor
  (render,250,210,70,255);
  if (totton==true )
    for (i=0;i<360;i+=7)
      SDL_RenderDrawPoint(render,
      (w/32)*cos (i/180*PI)+(w/8*6),
      (w/32)*sin (i/180*PI)+(h/6*5));
  }

//换位残线
if (othermove==true )
  {
  SDL_SetRenderDrawColor
  (render,250,210,70,255);
  SDL_RenderDrawLine(render,
  warp.x,warp.y,warp.x2,warp.y2);
  SDL_RenderDrawLine(render,
  warp.x+5,warp.y,
  warp.x2+5,warp.y2);
  SDL_RenderDrawLine(render,
  warp.x,warp.y+5,
  warp.x2,warp.y2+5);
  othermove=false;
  SDL_RenderPresent(render);
  SDL_Delay(100);
  }
SDL_RenderPresent(render);
SDL_SetRenderDrawColor
(render,30,30,30,255);
SDL_RenderClear(render);
}