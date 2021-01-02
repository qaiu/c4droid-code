void *gettouch (void *data)
{
float dx,dy;
float tx,ty;
float xx=w/8,yy=h-w/8;//触碰点坐标
tons *p;
float movex,movey;
float tempx,tempy;
SDL_Event event;

while (1)
  {
  while (SDL_PollEvent(&event))
    {
    movex=warp.x+warp.move*
                  cos (l_angle/180*PI);
    movey=warp.y+warp.move*
                  sin (l_angle/180*PI);
    if (other==false&&rush==false
    &&rushing==false )
      {
      for 
      (p=head;p->next!=NULL;)
        {
        p=p->next;
        if (LENGTH
        (p->x-movex,p->y-movey)
        <(w/16))
          {
          warp.toton=p;
          totton=true;
          break;
          }
        else if 
        (p->next==NULL)
          tottton=false;
        }
      }
    else if (other==true )
      {
      //置换桶检测
      for 
      (p=head;p->next!=NULL;)
        {
        p=p->next;
        if (LENGTH
        (p->x-warp.x,p->y-warp.y)
        <(w/16))
          {
          warp.toton=p;
          totton=true;
          break;
          }
        else if 
        (p->next==NULL)
          tottton=false;
        }
      //超出范围检测
      if (LENGTH(warp.x-warp.x2,
        warp.y-warp.y2)>w/3)
        {
        button_m=true;
        button_o=true;
        button_r=true;
        warp.x=warp.x2;
        warp.y=warp.y2;
        other=false;
        }
      }
    xx=w/8;
    yy=h/6*5;
//事件—————————————
//———————————————
    if (event.type==
    SDL_FINGERDOWN)
      {
      //羊菇头
      if (LENGTH
      (event.tfinger.x*w-xx,
      event.tfinger.y*h-yy)
      <(w/15))
        l=true;
      else if 
      (LENGTH
      (event.tfinger.x*w-xx,
      event.tfinger.y*h-yy)
      <(w/9))
        {
        if (other==false
        &&intton==false )
          button_r=true;
        lr=true;
        }
        //瞬移键
      if (LENGTH
      (event.tfinger.x*w-(w/5*4),
      event.tfinger.y*h-(h/6*5))
      <(w/16)&&button_m==true )
        {
        if (totton==true )
          {
          button_o=false;
          button_r=false;
          warp.x=warp.toton->x;
          warp.y=warp.toton->y;
          intton=true;
          }
        else
          {
          m=true;
          warp.x=movex;
          warp.y=movey;
          if (intton==true )
            {
            button_o=true;
            if (lm==true||lr==true )
              button_r=true;
            intton=false;
            }
          }
        }
        //分身
      if (LENGTH
      (event.tfinger.x*w-(w/8*7),
      event.tfinger.y*h-(h/7*5))
      <(w/32)&&button_o==true )
        {
        button_m=false;
        button_o=false;
        button_r=false;
        warp.x2=warp.x;
        warp.y2=warp.y;
        warp.x=movex;
        warp.y=movey;
        otr=10;
        other=true;
        }
      //分身时
      if (other==true )
        {
        //返回
        if (LENGTH
        (event.tfinger.x*w-(w/8*7),
        event.tfinger.y*h-(h/6*5))
        <(w/32))
          {
          button_m=true;
          button_o=true;
          button_r=true;
          warp.x=warp.x2;
          warp.y=warp.y2;
          other=false;
          }
        //换位
        else if (totton==true&&LENGTH
        (event.tfinger.x*w-w/8*6,
        event.tfinger.y*h-h/6*5)
        <(w/32))
          {
          button_m=true;
          button_o=true;
          button_r=true;
          tempx=warp.x2;
          tempy=warp.y2;
          warp.x=warp.toton->x;
          warp.y=warp.toton->y;
          warp.toton->x=tempx;
          warp.toton->y=tempy;
          other=false;
          othermove=true;
          }
        }
        //冲撞
      if (rushing==false )
        {
        if (LENGTH
        (event.tfinger.x*w-(w/12*11),
        event.tfinger.y*h-(h/6*5))
        <(w/32)&&button_r==true )
          {
          for 
          (p=head;p->next!=NULL;)
            {
            p=p->next;
            if (LENGTH
            (p->x-warp.x,p->y-warp.y)
            <warp.move)
              {
              button_m=false;
              button_o=false;
              button_r=false;
              if (totton==true )
                tottton=false;
              warp.rushton=p;
              times=0;
              speed=warp.speed;
              rush=true;
              break;
              }
            }
          }
        }
      else
        if (LENGTH
        (event.tfinger.x*w-(w/12*11),
        event.tfinger.y*h-(h/6*5))
        <(w/16)&&button_r==true )
          {
          for 
          (p=head;p->next!=NULL;)
            {
            p=p->next;
            if (LENGTH
            (p->x-warp.x,p->y-warp.y)
            <warp.move)
              {
              warp.rushton=p;
              times=0;
              speed=warp.speed;
              rushing=false;
              rush=true;
              break;
              }
            }
          }
      }
    if (event.type==
    SDL_FINGERMOTION)
      {
      tx=event.tfinger.x*w;
      ty=event.tfinger.y*h;
      //滑出羊菇头
      if (LENGTH(tx-xx,ty-yy)>w/8&&
      LENGTH(tx-xx,ty-yy)<(w/8+15))
        {
        button_r=false;
        l=false;
        lr=false;
        lm=false;
        if (rush==true )
          rush=false;
        warp.rushton=NULL;
        }
      //羊菇头开始运作(至少3)
      if (lm==false&&l==true&&
      LENGTH
      (event.tfinger.dx*w,
      event.tfinger.dy*h)>3)
        { 
        if (other==false
        &&intton==false )
          button_r=true;
        lm=true;
        }
      //羊菇头角度更新
      if ((lm==true||lr==true )&&
      LENGTH
      (event.tfinger.x*w-xx,
      event.tfinger.y*h-yy)<(w/8))
        l_angle=atan
        ((ty-yy)/(tx-xx))*180/PI+
        180*(tx<xx);
      }
    if (event.type==
    SDL_FINGERUP)
      {
      //放开羊菇头
      if (LENGTH
      (event.tfinger.x*w-xx,
      event.tfinger.y*h-yy)<(w/8))
        {
        button_r=false;
        l=false;
        lr=false;
        lm=false;
        if (rush==true )
          {
          rushing=true;
          rush=false;
          }
        }
      }
    }
  }
}