/* 画小窗口———————————
   没什么说的,点左上角的键就会出
   来,唯一的不同点就是执行它的时候
   SDL将在这个函数内循环 */
void openwi1()
{
	int i;						// 循环体
	while (tool == 0)
	{
		SDL_SetRenderDrawColor(render, 50, 50, 50, 255);
		for (i = 0; i < h / 2; i++)
			SDL_RenderDrawLine(render, i, 0, i, h / 2);
		SDL_SetRenderDrawColor(render, 190, 190, 190, 255);
		SDL_RenderDrawLine(render, 0, h / 4, h / 2 - 1, h / 4);
		SDL_RenderDrawLine(render, 0, 0, 0, h / 2);
		SDL_RenderDrawLine(render, 0, h / 2, h / 2 - 1, h / 2);
		 SDL_RenderDrawLine(render, 0, 0, h / 2 - 1, 0);
		SDL_RenderDrawLine(render, h / 2 - 1, 0, h / 2 - 1, h / 2);
		SDL_RenderPresent(render);
	}
}

void openwi2()
{
	node *ob;					// 对象指针
	ob = allob;					// 指向头结点
	float angle = 0;			// 角度,用于三角函数
	float r;
	float as, co1, co2, si1, si2;
	short i;					// 循环体

	// —————矩形刷屏——————
	SDL_SetRenderDrawColor(render, screen_r, screen_g, screen_b, 255);
	for (i = 0; i < h; i++)
		SDL_RenderDrawLine(render, 0, i, w, i);
	// —————绘制对象——————
	for (; ob->next != NULL;)
	{
		ob = ob->next;
		SDL_SetRenderDrawColor(render, (int)ob->color.r, (int)ob->color.g, (int)ob->color.b, 255);
		switch (ob->id)
		{
			/* 矩形——————————— */
		case 0:
			r = sqrt(ob->size.a * ob->size.a + ob->size.b * ob->size.b);
			as = asin(ob->size.b / r) * 180 / PI;
			co1 = r * cos((ob->h + as) / 180 * PI);
			co2 = r * cos((ob->h - as) / 180 * PI);
			si1 = r * sin((ob->h + as) / 180 * PI);
			si2 = r * sin((ob->h - as) / 180 * PI);

			SDL_RenderDrawLine(render,
							   co1 * 5 / 4 + (ob->x - w / 2) * 5 / 4 + w / 2,
							   -si1 * 5 / 4 + (ob->y - h / 2) * 5 / 4 + h / 2,
							   -co2 * 5 / 4 + (ob->x - w / 2) * 5 / 4 + w / 2,
							   si2 * 5 / 4 + (ob->y - h / 2) * 5 / 4 + h / 2);

			SDL_RenderDrawLine(render,
							   -co1 * 5 / 4 + (ob->x - w / 2) * 5 / 4 + w / 2,
							   si1 * 5 / 4 + (ob->y - h / 2) * 5 / 4 + h / 2,
							   -co2 * 5 / 4 + (ob->x - w / 2) * 5 / 4 + w / 2,
							   si2 * 5 / 4 + (ob->y - h / 2) * 5 / 4 + h / 2);

			SDL_RenderDrawLine(render,
							   co1 * 5 / 4 + (ob->x - w / 2) * 5 / 4 + w / 2,
							   -si1 * 5 / 4 + (ob->y - h / 2) * 5 / 4 + h / 2,
							   co2 * 5 / 4 + (ob->x - w / 2) * 5 / 4 + w / 2,
							   -si2 * 5 / 4 + (ob->y - h / 2) * 5 / 4 + h / 2);

			SDL_RenderDrawLine(render,
							   -co1 * 5 / 4 + (ob->x - w / 2) * 5 / 4 + w / 2,
							   si1 * 5 / 4 + (ob->y - h / 2) * 5 / 4 + h / 2,
							   co2 * 5 / 4 + (ob->x - w / 2) * 5 / 4 + w / 2,
							   -si2 * 5 / 4 + (ob->y - h / 2) * 5 / 4 + h / 2);
			break;
			/* 圆形——————————— */
		case 1:
			for (i = 0; i < ob->size.b; i++)
			{
				SDL_RenderDrawLine(render,
								   (ob->size.a) * cos
								   ((angle - ob->h) / 180 * PI) * 5 / 4
								   + (ob->x - w / 2) * 5 / 4 + w / 2,
								   (ob->size.a) * sin
								   ((angle - ob->h) / 180 * PI) * 5 / 4
								   + (ob->y - h / 2) * 5 / 4 + h / 2,
								   (ob->size.a) * cos
								   ((angle - ob->h + 360.0 / (int)ob->size.b) / 180 * PI) * 5 / 4
								   + (ob->x - w / 2) * 5 / 4 + w / 2,
								   (ob->size.a) * sin
								   ((angle - ob->h + 360.0 / (int)ob->size.b) / 180 * PI) * 5 / 4
								   + (ob->y - h / 2) * 5 / 4 + h / 2);
				angle += 360.0 / (int)ob->size.b;
			}
		}
	}
	// ——————输出———————
	SDL_RenderPresent(render);
	while (tool == 8)
	{
	}
}