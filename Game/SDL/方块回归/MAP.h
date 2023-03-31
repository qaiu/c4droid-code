				/* 此文件为游戏每关的布局 */
#include <head.h>
void level_1()
{
	SDL_Rect b1;
	int dir1 = 1;
	b1.x = 0;
	b1.y = 250;
	b1.h = 30;
	b1.w = 150;
	while (l == 1 && q_r == 0)
	{
		game_start();
		apply(b1.x, 250, box_rect1, screen, NULL);
		if (check(b1, boxa) || check(b1, boxb))
			life = 0;
		if (check(boxa, boxb))
			life = 0;
		if (life == 0)
			over();
		if (dir1 == 1)
			b1.x += 3;
		if (dir1 == 2)
			b1.x -= 3;
		if (b1.x >= 810)
			dir1 = 2;
		if (b1.x <= 0)
			dir1 = 1;
		SDL_Flip(screen);
		Fps();
	}
	q_r = 0;
}

void level_2()
{
	SDL_Rect b2_1, b2_2, b2_3;
	b2_1.x = 200;
	b2_1.y = 200;
	b2_1.h = 30;
	b2_1.w = 150;
	b2_2.x = 600;
	b2_2.y = 200;
	b2_2.h = 150;
	b2_2.w = 30;
	while (l == 2 && q_r == 0)
	{
		game_start();
		apply(200, 200, box_rect1, screen, NULL);
		apply(600, 200, box_rect2, screen, NULL);
		if (check(b2_1, boxa) || check(b2_1, boxb))
			life = 0;
		if (check(b2_2, boxa) || check(b2_2, boxb))
			life = 0;
		if (check(boxa, boxb))
			life = 0;
		if (life == 0)
			over();
		SDL_Flip(screen);
		Fps();
	}
	q_r = 0;
}

void level_4()
{
	SDL_Rect b4_1, b4_2, b4_3, b4_4, b4_5, b4_6;
	b4_1.x = 650;
	b4_1.y = 80;
	b4_1.h = 30;
	b4_1.w = 150;
	b4_2.x = 460;
	b4_2.y = 210;
	b4_2.h = 30;
	b4_2.w = 150;
	b4_3.x = 650;
	b4_3.y = 400;
	b4_3.h = 30;
	b4_3.w = 150;
	b4_4.x = 100;
	b4_4.y = 0;
	b4_4.h = 150;
	b4_4.w = 30;
	b4_5.x = 250;
	b4_5.y = 400;
	b4_5.h = 30;
	b4_5.w = 150;
	b4_6.x = 200;
	b4_6.y = 150;
	b4_6.h = 150;
	b4_6.w = 30;
	while (l == 4 && q_r == 0)
	{
		game_start();
		apply(650, 80, box_rect1, screen, NULL);
		apply(460, 210, box_rect1, screen, NULL);
		apply(650, 400, box_rect1, screen, NULL);
		apply(100, 0, box_rect4, screen, NULL);
		apply(250, 400, box_rect5, screen, NULL);
		apply(200, 150, box_rect6, screen, NULL);
		if (check(b4_1, boxa) || check(b4_1, boxb))
			life = 0;
		if (check(b4_2, boxa) || check(b4_2, boxb))
			life = 0;
		if (check(b4_3, boxa) || check(b4_3, boxb))
			life = 0;
		if (check(b4_4, boxa) || check(b4_4, boxb))
			life = 0;
		if (check(b4_5, boxa) || check(b4_5, boxb))
			life = 0;
		if (check(b4_6, boxa) || check(b4_6, boxb))
			life = 0;
		if (check(boxa, boxb))
			life = 0;
		if (life == 0)
			over();
		SDL_Flip(screen);
		Fps();
	}
	q_r = 0;
}


void level_3()
{
	SDL_Rect b3_1, b3_2, b3_3, b3_4, b3_5;
	b3_1.x = 250;
	b3_1.y = 200;
	b3_1.h = 30;
	b3_1.w = 150;
	b3_2.x = 710;
	b3_2.y = 160;
	b3_2.h = 150;
	b3_2.w = 30;
	b3_3.x = 810;
	b3_3.y = 430;
	b3_3.h = 30;
	b3_3.w = 150;
	b3_4.x = 0;
	b3_4.y = 80;
	b3_4.h = 30;
	b3_4.w = 150;
	b3_5.x = 500;
	b3_5.y = 300;
	b3_5.h = 150;
	b3_5.w = 30;
	while (l == 3 && q_r == 0)
	{
		game_start();
		apply(250, 200, box_rect1, screen, NULL);
		apply(710, 160, box_rect2, screen, NULL);
		apply(810, 430, box_rect3, screen, NULL);
		apply(0, 80, box_rect3, screen, NULL);
		apply(500, 300, box_rect4, screen, NULL);
		if (check(b3_1, boxa) || check(b3_1, boxb))
			life = 0;
		if (check(b3_2, boxa) || check(b3_2, boxb))
			life = 0;
		if (check(b3_3, boxa) || check(b3_3, boxb))
			life = 0;
		if (check(b3_4, boxa) || check(b3_4, boxb))
			life = 0;
		if (check(b3_5, boxa) || check(b3_5, boxb))
			life = 0;
		if (check(boxa, boxb))
			life = 0;
		if (life == 0)
			over();
		SDL_Flip(screen);
		Fps();
	}
	q_r = 0;
}

void level_5()
{
	SDL_Rect b5_1, b5_2, b5_3, b5_4, b5_5;
	b5_1.x = 250;
	b5_1.y = 80;
	b5_1.h = 150;
	b5_1.w = 30;
	b5_2.x = 710;
	b5_2.y = 150;
	b5_2.h = 30;
	b5_2.w = 150;
	b5_3.x = 460;
	b5_3.y = 250;
	b5_3.h = 30;
	b5_3.w = 150;
	b5_4.x = 150;
	b5_4.y = 380;
	b5_4.h = 150;
	b5_4.w = 30;
	b5_5.x = 500;
	b5_5.y = 390;
	b5_5.h = 150;
	b5_5.w = 30;
	while (l == 5 && q_r == 0)
	{
		game_start();
		apply(250, 80, box_rect4, screen, NULL);
		apply(710, 150, box_rect3, screen, NULL);
		apply(460, 250, box_rect3, screen, NULL);
		apply(150, 380, box_rect4, screen, NULL);
		apply(500, 390, box_rect4, screen, NULL);
		if (check(b5_1, boxa) || check(b5_1, boxb))
			life = 0;
		if (check(b5_2, boxa) || check(b5_2, boxb))
			life = 0;
		if (check(b5_3, boxa) || check(b5_3, boxb))
			life = 0;
		if (check(b5_4, boxa) || check(b5_4, boxb))
			life = 0;
		if (check(b5_5, boxa) || check(b5_5, boxb))
			life = 0;
		if (check(boxa, boxb))
			life = 0;
		if (life == 0)
			over();
		SDL_Flip(screen);
		Fps();
	}
	q_r = 0;
}

void level_6()
{
	int dir = 1;
	SDL_Rect b6_1, b6_2, b6_3, b6_4, b6_5;
	b6_1.x = 250;
	b6_1.y = 150;
	b6_1.h = 30;
	b6_1.w = 150;
	b6_2.x = 750;
	b6_2.y = 250;
	b6_2.h = 150;
	b6_2.w = 30;
	b6_3.x = 810;
	b6_3.y = 430;
	b6_3.h = 30;
	b6_3.w = 150;
	b6_4.x = 0;
	b6_4.y = 80;
	b6_4.h = 30;
	b6_4.w = 150;
	b6_5.x = 500;
	b6_5.y = 350;
	b6_5.h = 30;
	b6_5.w = 150;
	while (l == 6 && q_r == 0)
	{
		game_start();
		apply(b6_1.x, 150, box_rect1, screen, NULL);
		apply(750, 250, box_rect2, screen, NULL);
		apply(810, 430, box_rect3, screen, NULL);
		apply(0, 80, box_rect3, screen, NULL);
		apply(b6_5.x, 350, box_rect1, screen, NULL);
		if (check(b6_1, boxa) || check(b6_1, boxb))
			life = 0;
		if (check(b6_2, boxa) || check(b6_2, boxb))
			life = 0;
		if (check(b6_3, boxa) || check(b6_3, boxb))
			life = 0;
		if (check(b6_4, boxa) || check(b6_4, boxb))
			life = 0;
		if (check(b6_5, boxa) || check(b6_5, boxb))
			life = 0;
		if (check(boxa, boxb))
			life = 0;
		if (life == 0)
			over();
		SDL_Flip(screen);
		if (dir == 1)
		{
			b6_1.x += 2;
			b6_5.x -= 2;
		}
		if (dir == 2)
		{
			b6_5.x += 2;
			b6_1.x -= 2;
		}
		if (b6_5.x < 200)
			dir = 2;
		if (b6_5.x > 450)
			dir = 1;
		Fps();
	}
	q_r = 0;
}

void level_7()
{
	int dir7 = 1;
	SDL_Rect b7_1, b7_2, b7_3, b7_4, b7_5;
	b7_1.x = 0;
	b7_1.y = 100;
	b7_1.h = 30;
	b7_1.w = 150;
	b7_2.x = 810;
	b7_2.y = 100;
	b7_2.h = 30;
	b7_2.w = 150;
	b7_3.x = 100;
	b7_3.y = 230;
	b7_3.h = 30;
	b7_3.w = 150;
	b7_4.x = 0;
	b7_4.y = 350;
	b7_4.h = 30;
	b7_4.w = 150;
	b7_5.x = 810;
	b7_5.y = 350;
	b7_5.h = 30;
	b7_5.w = 150;
	while (l == 7 && q_r == 0)
	{
		game_start();
		apply(b7_1.x, 100, box_rect1, screen, NULL);
		apply(b7_2.x, 100, box_rect3, screen, NULL);
		apply(b7_3.x, 230, box_rect5, screen, NULL);
		apply(b7_4.x, 350, box_rect3, screen, NULL);
		apply(b7_5.x, 350, box_rect1, screen, NULL);
		if (check(b7_1, boxa) || check(b7_1, boxb))
			life = 0;
		if (check(b7_2, boxa) || check(b7_2, boxb))
			life = 0;
		if (check(b7_3, boxa) || check(b7_3, boxb))
			life = 0;
		if (check(b7_4, boxa) || check(b7_4, boxb))
			life = 0;
		if (check(b7_5, boxa) || check(b7_5, boxb))
			life = 0;
		if (check(boxa, boxb))
			life = 0;
		if (life == 0)
			over();
		SDL_Flip(screen);
		if (dir7 == 1)
		{
			b7_1.x += 2;
			b7_2.x -= 2;
			b7_3.x += 4;
			b7_4.x += 2;
			b7_5.x -= 2;
		}
		if (b7_1.x > 300)
			dir7 = 2;
		if (dir7 == 2)
		{
			b7_1.x -= 2;
			b7_2.x += 2;
			b7_3.x -= 4;
			b7_4.x -= 2;
			b7_5.x += 2;
		}
		if (b7_1.x < 0)
			dir7 = 1;
		Fps();
	}
	q_r = 0;
}

void level_8()
{
	int dir8 = 1;
	SDL_Rect b8_1, b8_2, b8_3, b8_4, b8_5;
	b8_1.x = 100;
	b8_1.y = 0;
	b8_1.h = 150;
	b8_1.w = 30;
	b8_2.x = 270;
	b8_2.y = 0;
	b8_2.h = 150;
	b8_2.w = 30;
	b8_3.x = 450;
	b8_3.y = 200;
	b8_3.h = 150;
	b8_3.w = 30;
	b8_4.x = 730;
	b8_4.y = 390;
	b8_4.h = 150;
	b8_4.w = 30;
	b8_5.x = 810;
	b8_5.y = 100;
	b8_5.h = 30;
	b8_5.w = 150;
	while (l == 8 && q_r == 0)
	{
		game_start();
		apply(100, 0, box_rect2, screen, NULL);
		apply(270, b8_2.y, box_rect4, screen, NULL);
		apply(450, 200, box_rect4, screen, NULL);
		apply(730, b8_4.y, box_rect4, screen, NULL);
		apply(810, 100, box_rect5, screen, NULL);
		if (check(b8_1, boxa) || check(b8_1, boxb))
			life = 0;
		if (check(b8_2, boxa) || check(b8_2, boxb))
			life = 0;
		if (check(b8_3, boxa) || check(b8_3, boxb))
			life = 0;
		if (check(b8_4, boxa) || check(b8_4, boxb))
			life = 0;
		if (check(b8_5, boxa) || check(b8_5, boxb))
			life = 0;
		if (check(boxa, boxb))
			life = 0;
		if (life == 0)
			over();
		if (dir8 == 1)
		{
			b8_2.y += 3;
			b8_4.y -= 3;
		}
		if (dir8 == 2)
		{
			b8_2.y -= 3;
			b8_4.y += 3;
		}
		if (b8_2.y > 390)
			dir8 = 2;
		if (b8_2.y < 0)
			dir8 = 1;
		SDL_Flip(screen);
		Fps();
	}
	q_r = 0;
}