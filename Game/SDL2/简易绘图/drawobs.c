void drawob()
{
	node *ob;					// 对象指针
	ob = allob;					// 指向头结点
	float angle = 0;			// 角度,用于三角函数
	float r;
	float as, co1, co2, si1, si2;
	short i;					// 循环体

	// —————矩形刷屏——————
	SDL_SetRenderDrawColor(render, 90, 90, 90, 255);
	for (i = 0; i < h; i++)
		SDL_RenderDrawLine(render, 0, i, w, i);
	SDL_SetRenderDrawColor(render, screen_r, screen_g, screen_b, 255);
	for (i = h / 10; i < h / 10 * 9; i++)
		SDL_RenderDrawLine(render, w / 10, i, w / 10 * 9, i);
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
			SDL_RenderDrawLine(render, co1 + ob->x, -si1 + ob->y, -co2 + ob->x, si2 + ob->y);

			SDL_RenderDrawLine(render, -co1 + ob->x, si1 + ob->y, -co2 + ob->x, si2 + ob->y);

			SDL_RenderDrawLine(render, co1 + ob->x, -si1 + ob->y, co2 + ob->x, -si2 + ob->y);

			SDL_RenderDrawLine(render, -co1 + ob->x, si1 + ob->y, co2 + ob->x, -si2 + ob->y);
			break;
			/* 圆形——————————— */
		case 1:
			for (i = 0; i < ob->size.b; i++)
			{
				SDL_RenderDrawLine(render,
								   (ob->size.a) * cos
								   ((angle - ob->h) / 180 * PI) + ob->x,
								   (ob->size.a) * sin
								   ((angle - ob->h) / 180 * PI) + ob->y,
								   (ob->size.a) * cos
								   ((angle - ob->h + 360.0 / (int)ob->size.b) / 180 * PI) + ob->x,
								   (ob->size.a) * sin
								   ((angle - ob->h + 360.0 / (int)ob->size.b) / 180 * PI) + ob->y);
				angle += 360.0 / (int)ob->size.b;
			}
		}
	}
	// —————画格子线——————
	SDL_SetRenderDrawColor(render, 170, 170, 170, 255);
	/* 左半边——————————— */
	SDL_RenderDrawLine(render, w / 10, 0, w / 10, h);	// 竖线
	SDL_RenderDrawLine(render, 0, h / 6, w / 10, h / 6);
	SDL_RenderDrawLine(render, 0, h / 3, w / 10, h / 3);
	SDL_RenderDrawLine(render, 0, h / 2, w / 10, h / 2);
	SDL_RenderDrawLine(render, 0, h / 3 * 2, w / 10, h / 3 * 2);
	SDL_RenderDrawLine(render, 0, h / 6 * 5, w / 10, h / 6 * 5);
	/* 右半边——————————— */
	SDL_RenderDrawLine(render, w - w / 10, 0, w - w / 10, h);	// 竖线
	SDL_RenderDrawLine(render, w - w / 10, h / 6, w, h / 6);
	SDL_RenderDrawLine(render, w - w / 10, h / 3, w, h / 3);
	SDL_RenderDrawLine(render, w - w / 10, h / 2, w, h / 2);
	SDL_RenderDrawLine(render, w - w / 10, h / 3 * 2, w, h / 3 * 2);
	SDL_RenderDrawLine(render, w - w / 10, h / 6 * 5, w, h / 6 * 5);
	/* 上下 */
	SDL_RenderDrawLine(render, w / 10, h / 10, w / 10 * 9, h / 10);
	SDL_RenderDrawLine(render, w / 10 * 3, 0, w / 10 * 3, h / 10);
	SDL_RenderDrawLine(render, w / 10 * 5, 0, w / 10 * 5, h / 10);
	SDL_RenderDrawLine(render, w / 10 * 7, 0, w / 10 * 7, h / 10);
	SDL_RenderDrawLine(render, w / 10, h / 10 * 9, w / 10 * 9, h / 10 * 9);
	// —————工具选中——————
	SDL_SetRenderDrawColor(render, 155, 70, 20, 255);
	if (tool <= 3)
		for (i = 0; i < h / 6 - 3; i++)
			SDL_RenderDrawLine(render, 0, h / 6 * tool + 2 + i, w / 10 - 2, h / 6 * tool + 2 + i);
	else
		for (i = 0; i < w / 5 - 3; i++)
			SDL_RenderDrawLine(render,
							   w / 5 * (tool - 4) + 1 + w / 10 + i + 1, 1,
							   w / 5 * (tool - 4) + 1 + w / 10 + i + 1, h / 10 - 2);
	// —————对象列表——————
	/* 对象不为空且在该页时 */
	if (nowob != NULL && (nowob->mark - 1) / 3 + 1 == page)
		for (i = 0; i < h / 6 - 3; i++)
			SDL_RenderDrawLine(render, w,
							   h / 3 + h / 6 * (nowob->mark - 1 -
												(nowob->mark - 1) / 3 * 3) + 2 + i,
							   w - w / 10 + 2,
							   h / 3 + h / 6 * (nowob->mark - 1 -
												(nowob->mark - 1) / 3 * 3) + 2 + i);
	/* 利用整数相除返回值的特点 */
	// —————方向锁定——————
	/* x轴方向 */
	if (box == true)
		for (i = 0; i < h / 6 - 3; i++)
			SDL_RenderDrawLine(render, 0, h / 3 * 2 + 2 + i, w / 10 - 2, h / 3 * 2 + 2 + i);
	/* y轴方向 */
	if (boy == true)
		for (i = 0; i < h / 6 - 3; i++)
			SDL_RenderDrawLine(render, 0, h / 6 * 5 + 2 + i, w / 10 - 2, h / 6 * 5 + 2 + i);
	// ——选中对象的可移动方块———
	if (theob != 0)
		for (i = -h / 12; i < h / 12; i++)
			SDL_RenderDrawLine(render, tx - h / 12, ty + i, tx + h / 12, ty + i);
	// —————坐标轴线——————
	if (nowob != NULL)
	{
		/* y轴 */
		if (boy != 0 && nowob != NULL)
		{
			SDL_SetRenderDrawColor(render, 0, 0, 255, 255);
			SDL_RenderDrawLine(render, nowob->x, nowob->y, nowob->x, -100 + nowob->y);
		}
		/* x轴 */
		if (box != 0 && nowob != NULL)
		{
			SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
			SDL_RenderDrawLine(render, nowob->x, nowob->y, 100 + nowob->x, nowob->y);
		}
	}
	// ——————上栏——————
	if (nowob != NULL)
	{
		SDL_SetRenderDrawColor(render, nowob->color.r, 0, 0, 255);
		for (i = 0; i < w / 10; i++)
			SDL_RenderDrawLine(render, w / 6.5 + i, h / 40, w / 6.5 + i, h / 40 * 3);
		SDL_SetRenderDrawColor(render, 0, nowob->color.g, 0, 255);
		for (i = 0; i < w / 10; i++)
			SDL_RenderDrawLine(render,
							   w / 6.5 + w / 5 + i, h / 40, w / 6.5 + w / 5 + i, h / 40 * 3);
		SDL_SetRenderDrawColor(render, 0, 0, nowob->color.b, 255);
		for (i = 0; i < w / 10; i++)
			SDL_RenderDrawLine(render,
							   w / 6.5 + w / 5 + w / 5 + i, h / 40,
							   w / 6.5 + w / 5 + w / 5 + i, h / 40 * 3);
		SDL_SetRenderDrawColor(render, nowob->color.r, nowob->color.g, nowob->color.b, 255);
		for (i = 0; i < w / 10; i++)
			SDL_RenderDrawLine(render,
							   w / 6.5 + w / 5 + w / 5 + w / 5 + i, h / 40,
							   w / 6.5 + w / 5 + w / 5 + w / 5 + i, h / 40 * 3);
	}

	else
	{
		SDL_SetRenderDrawColor(render, screen_r, 0, 0, 255);
		for (i = 0; i < w / 10; i++)
			SDL_RenderDrawLine(render, w / 6.5 + i, h / 40, w / 6.5 + i, h / 40 * 3);
		SDL_SetRenderDrawColor(render, 0, screen_g, 0, 255);
		for (i = 0; i < w / 10; i++)
			SDL_RenderDrawLine(render,
							   w / 6.5 + w / 5 + i, h / 40, w / 6.5 + w / 5 + i, h / 40 * 3);
		SDL_SetRenderDrawColor(render, 0, 0, screen_b, 255);
		for (i = 0; i < w / 10; i++)
			SDL_RenderDrawLine(render,
							   w / 6.5 + w / 5 + w / 5 + i, h / 40,
							   w / 6.5 + w / 5 + w / 5 + i, h / 40 * 3);
		SDL_SetRenderDrawColor(render, screen_r, screen_g, screen_b, 255);
		for (i = 0; i < w / 10; i++)
			SDL_RenderDrawLine(render,
							   w / 6.5 + w / 5 + w / 5 + w / 5 + i, h / 40,
							   w / 6.5 + w / 5 + w / 5 + w / 5 + i, h / 40 * 3);
	}
	// ————控制滑屏亮键—————
	SDL_SetRenderDrawColor(render, 10, 150, 10, 255);
	if (tx > w / 10 * 9 && boo == false && theob == 0 && bo == false)
	{
		// 向下翻页
		if (ty > h / 6 * 5 && page < pages)
			for (i = 0; i < h / 6 - 3; i++)
				SDL_RenderDrawLine
					(render, w / 10 * 9 + 2, h / 6 * 5 + 2 + i, w, h / 6 * 5 + 2 + i);
		// 选择该页第三个对象
		else if (ty > h / 3 * 2 && ty < h / 6 * 5 && page * 3 <= obnum)
			for (i = 0; i < h / 6 - 3; i++)
				SDL_RenderDrawLine
					(render, w / 10 * 9 + 2, h / 3 * 2 + 2 + i, w, h / 3 * 2 + 2 + i);
		// 选择该页第二个对象
		else if (ty > h / 2 && ty < h / 3 * 2 && page * 3 - 1 <= obnum)
			for (i = 0; i < h / 6 - 3; i++)
				SDL_RenderDrawLine(render, w / 10 * 9 + 2, h / 2 + 2 + i, w, h / 2 + 2 + i);
		// 选择该页第一个对象
		else if (ty > h / 3 && ty < h / 2 && page * 3 - 2 <= obnum)
			for (i = 0; i < h / 6 - 3; i++)
				SDL_RenderDrawLine(render, w / 10 * 9 + 2, h / 3 + 2 + i, w, h / 3 + 2 + i);
		// 向上翻页
		else if (ty > h / 6 && ty < h / 3 && page > 1)
			for (i = 0; i < h / 6 - 3; i++)
				SDL_RenderDrawLine(render, w / 10 * 9 + 2, h / 6 + 2 + i, w, h / 6 + 2 + i);
		// 取消当前对象
		else if (nowob != NULL && ty < h / 6 && tool > 3)
			for (i = 0; i < h / 6 - 3; i++)
				SDL_RenderDrawLine(render, w / 10 * 9 + 2, 2 + i, w, 2 + i);
	}
	/* 工具栏选择 */
	else if (tx < w / 10)
	{
		// 小窗口
		if (ty < h / 6)
			for (i = 0; i < h / 6 - 3; i++)
				SDL_RenderDrawLine(render, 0, 2 + i, w / 10 - 2, 2 + i);
		// 移动工具
		else if (ty < h / 3)
			for (i = 0; i < h / 6 - 3; i++)
				SDL_RenderDrawLine(render, 0, h / 6 + 2 + i, w / 10 - 2, h / 6 + 2 + i);
		// 旋转工具
		else if (ty < h / 2)
			for (i = 0; i < h / 6 - 3; i++)
				SDL_RenderDrawLine(render, 0, h / 3 + 2 + i, w / 10 - 2, h / 3 + 2 + i);
		// 缩放工具
		else if (ty < h / 3 * 2)
			for (i = 0; i < h / 6 - 3; i++)
				SDL_RenderDrawLine(render, 0, h / 2 + 2 + i, w / 10 - 2, h / 2 + 2 + i);
		// x方向锁定键
		else if (ty < h / 6 * 5)
			for (i = 0; i < h / 6 - 3; i++)
				SDL_RenderDrawLine(render, 0, h / 3 * 2 + 2 + i, w / 10 - 2, h / 3 * 2 + 2 + i);
		// y方向锁定键
		else if (ty < h)
			for (i = 0; i < h / 6 - 3; i++)
				SDL_RenderDrawLine(render, 0, h / 6 * 5 + 2 + i, w / 10 - 2, h / 6 * 5 + 2 + i);
	}
	/* 上栏的点选 */
	else if (ty < h / 10 && tx > w / 10)
	{
		// 红色
		if (tx < w / 10 * 3)
			for (i = 0; i < w / 5 - 3; i++)
				SDL_RenderDrawLine(render, w / 10 + i + 2, 1, w / 10 + i + 2, h / 10 - 2);
		// 绿色
		else if (tx < w / 10 * 5)
			for (i = 0; i < w / 5 - 3; i++)
				SDL_RenderDrawLine
					(render, w / 5 + w / 10 + i + 2, 1, w / 5 + w / 10 + i + 2, h / 10 - 2);
		// 蓝色
		else if (tx < w / 10 * 7)
			for (i = 0; i < w / 5 - 3; i++)
				SDL_RenderDrawLine
					(render, w / 5 * 2 + w / 10 + i + 2, 1,
					 w / 5 * 2 + w / 10 + i + 2, h / 10 - 2);
		// 全局
		else if (tx < w / 10 * 9)
			for (i = 0; i < w / 5 - 3; i++)
				SDL_RenderDrawLine
					(render, w / 5 * 3 + w / 10 + i + 2, 1,
					 w / 5 * 3 + w / 10 + i + 2, h / 10 - 2);
	}
	if (nowob != NULL && ty < h / 6 && tool > 3 && tx > w / 10 * 9)
		for (i = 0; i < h / 6 - 3; i++)
			SDL_RenderDrawLine(render, w / 10 * 9 + 2, 2 + i, w, 2 + i);
	if (theob != 0 && ty < h / 6 && tx > w / 10 * 9)
	{
		SDL_SetRenderDrawColor(render, 150, 10, 10, 255);
		for (i = 0; i < h / 6 - 3; i++)
			SDL_RenderDrawLine(render, w / 10 * 9 + 2, 2 + i, w, 2 + i);
	}
	// ——————输出———————
	SDL_RenderPresent(render);
}