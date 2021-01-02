/*获取触屏
**左列是工具栏,依次为:
**生成对象窗口,移动,旋转,缩放, **x锁定,y锁定。
**右列分别是
**特殊键,对象列表(五格)
**上,下是翻页,中间是这页的对象
**对象列表中的对象可以拖出来 
**拖到特殊键上删除
**选择颜色栏时点击特殊键以取消所
**选对象
**从特殊键上开始拖移可以提高精度
**选择缩放工具,选择圆,从特殊键
**上拖移可以调整圆的分段
**上栏是颜色栏,为红,绿,蓝,整体
**选中后在主界面上下拖移调整 
**空对象时调节背景色
**手指划过摁键时若可进行操作则摁
**键亮起
**左上角摁键可以对触屏事件进行提
**示
**点击下方长摁键进入全屏
**点同样位置退出全屏
**删除对象时特殊键为红色
**差不多就这样,双拼可真累(;一_一) */
void *gettouch(void *data)
{
	SDL_Event event;			// 事件
	float xx, yy, hh, aa, bb, cr, cg, cb;
	// 储存对象原来的信息,在操作时使用
	int i;						// 循环体

	while (1)
	{
		while (SDL_PollEvent(&event))
		{
			if (nowob != NULL)
				// 对象操作(触屏和移动)的前提
			{


				/* 获取特殊键的开启———————
				   **获取从列表中选中对象—————
				   **通过else_if缩小范围的方法—— */
				if (event.type == SDL_FINGERDOWN)
					// 触屏事件
				{
					tx = event.tfinger.x * w;
					ty = event.tfinger.y * h;
					// 获取坐标
					if (tx > w / 10 && tx < w / 10 * 9 &&
						ty > h / 10 && ty < h / 10 * 9 && tool != 0 && bo == false && theob == 0)
						boo = true;	// 操作键开
					if (tx > w / 10 * 9 && ty < h / 6 * 5)
						// 范围：右列上五格
					{
						if (ty < h / 6 && theob == 0)
							// 右列第一格
							bo = true;	// 特殊键开
						else if (ty > h / 3 * 2)
							// 右列第五格
						{
							if ((page * 3) <= obnum)
								theob = page * 3;
						}
						else if (ty > h / 2)
							// 右列第四格
						{
							if ((page * 3 - 1) <= obnum)
								theob = page * 3 - 1;
						}
						else if (ty > h / 3)
							// 右列第三格
							theob = page * 3 - 2;
					}
				}


				/* 移动,旋转,缩放的获取—————
				   **包括特殊键开启时的操作————
				   **乘以box,boy表示配合锁定键—— */
				if (event.type == SDL_FINGERMOTION)
					// 滑屏事件
				{
					// ——获取坐标和滑屏距离——
					dx = event.tfinger.dx * w;
					dy = event.tfinger.dy * h;
					tx = event.tfinger.x * w;
					ty = event.tfinger.y * h;
					// ——初始化储存对象信息——
					xx = nowob->x;
					yy = nowob->y;
					hh = nowob->h;
					aa = nowob->size.a;
					bb = nowob->size.b;
					cr = nowob->color.r;
					cg = nowob->color.g;
					cb = nowob->color.b;
					// ——移动,旋转,缩放的获取——
					if (boo == true)
					{
						switch (tool)
						{
							/* 移动 */
						case 1:
							nowob->x = dx * box + xx;
							nowob->y = dy * boy + yy;
							break;
							/* 旋转 */
						case 2:
							nowob->h = hh +
								(dy * (tx - xx < 0) +
								 dx * (ty - yy > 0) -
								 dy * (tx - xx > 0) - dx * (ty - yy < 0)) / 3 * 2;
							break;
							/* 缩放 */
						case 3:
							switch (nowob->id)
							{
							case 0:
								nowob->size.a = aa + dx * box;
								nowob->size.b = bb - dy * boy;
								break;
							case 1:
								nowob->size.a = aa + (dx - dy) * (boy | box);
							}
							break;
							// 红色
						case 4:
							nowob->color.r = cr - dy / 4;
							if (nowob->color.r > 255)
								nowob->color.r = 255;
							if (nowob->color.r < 0)
								nowob->color.r = 0;
							break;
							// 绿色
						case 5:
							nowob->color.g = cg - dy / 4;
							if (nowob->color.g > 255)
								nowob->color.g = 255;
							if (nowob->color.g < 0)
								nowob->color.g = 0;
							break;
							// 蓝色
						case 6:
							nowob->color.b = cb - dy / 4;
							if (nowob->color.b > 255)
								nowob->color.b = 255;
							if (nowob->color.b < 0)
								nowob->color.b = 0;
							break;
							// 全局
						case 7:
							nowob->color.r = cr - dy / 4;
							if (nowob->color.r > 255)
								nowob->color.r = 255;
							if (nowob->color.r < 0)
								nowob->color.r = 0;
							nowob->color.g = cg - dy / 4;
							if (nowob->color.g > 255)
								nowob->color.g = 255;
							if (nowob->color.g < 0)
								nowob->color.g = 0;
							nowob->color.b = cb - dy / 4;
							if (nowob->color.b > 255)
								nowob->color.b = 255;
							if (nowob->color.b < 0)
								nowob->color.b = 0;
							break;
						}
					}
					// ———特殊键下的操作———
					if (bo == true)
					{
						switch (tool)
						{
							/* 高精度移动 */
						case 1:
							nowob->x = dx / 10 * box + xx;
							nowob->y = dy / 10 * boy + yy;
							break;
							/* 高精度旋转 */
						case 2:
							nowob->h = hh +
								(dy * (tx - xx < 0) +
								 dx * (ty - yy > 0) -
								 dy * (tx - xx > 0) - dx * (ty - yy < 0)) / 25;
							break;
							/* 高精度缩放以及圆的分段 */
						case 3:
							if (box == false && boy == false && nowob->id == 1)
								/* 圆在x,y方向都锁定时
								   **用特殊键改变分段 */
							{
								nowob->size.b = -dy / 50 + bb;
								if (nowob->size.b <= 3)
									nowob->size.b = 3;
								break;
							}
							/* 高精度缩放 */
							switch (nowob->id)
							{
							case 0:
								nowob->size.a = aa + dx / 10 * box;
								nowob->size.b = bb - dy / 10 * boy;
								break;
							case 1:
								nowob->size.a = aa + (dx / 10 - dy / 10) * (box | boy);
							}
						}
					}
				}
			}


			/* 在没有当前对象时的获取————
			   **以及背景色的计算—————— */
			else if (event.type == SDL_FINGERMOTION)
			{
				// ————背景色预备——————
				cr = screen_r;
				cg = screen_g;
				cb = screen_b;
				// ——————获取坐标—————
				dx = event.tfinger.dx * w;
				dy = event.tfinger.dy * h;
				tx = event.tfinger.x * w;
				ty = event.tfinger.y * h;
				// ————计算背景色——————
				switch (tool)
				{
					// 红
				case 4:
					screen_r = cr - dy / 4;
					if (screen_r > 255)
						screen_r = 255;
					if (screen_r < 0)
						screen_r = 0;
					break;
					// 绿
				case 5:
					screen_g = cg - dy / 4;
					if (screen_g > 255)
						screen_g = 255;
					if (screen_g < 0)
						screen_g = 0;
					break;
					// 蓝
				case 6:
					screen_b = cb - dy / 4;
					if (screen_b > 255)
						screen_b = 255;
					if (screen_b < 0)
						screen_b = 0;
					break;
					// 全局
				case 7:
					screen_r = cr - dy / 4;
					if (screen_r > 255)
						screen_r = 255;
					if (screen_r < 0)
						screen_r = 0;

					screen_g = cg - dy / 4;
					if (screen_g > 255)
						screen_g = 255;
					if (screen_g < 0)
						screen_g = 0;

					screen_b = cb - dy / 4;
					if (screen_b > 255)
						screen_b = 255;
					if (screen_b < 0)
						screen_b = 0;
					break;
				}
			}


			/* 工具栏的点选—————————
			   **对象列表的使用以及删除对象—
			   **同样通过else_if缩小范围的方法 */
			if (event.type == SDL_FINGERUP)
				// 手指抬起事件
			{
				// ————获取坐标—————
				tx = event.tfinger.x * w;
				ty = event.tfinger.y * h;
				// ——生成对象的小窗口———
				if (tool == 0)
				{
					if (tx < h / 2)
					{
						/* 添加矩形 */
						if (ty < h / 4)
						{
							/* 添加对象矩形(doobs.c) */
							allob = addobject(allob, 0);
							/* 当前对象更新至最后 */
							nowob = allob;
							while (nowob->next != NULL)
								nowob = nowob->next;
						}
						/* 添加圆 */
						else if (ty < h / 2)
						{
							/* 添加对象圆(doobs.c) */
							allob = addobject(allob, 1);
							/* 当前对象更新至最后 */
							nowob = allob;
							while (nowob->next != NULL)
								nowob = nowob->next;
						}
					}
					/* 结束小窗口,转移动工具 */
					tool = 1;
				}
				// ———对象列表的操作———
				else if (tx > w / 10 * 9 && obnum != 0 && bo == false && boo == false)
				{
					/* 向下翻页 */
					if (ty > h / 6 * 5)
						page += 1 * (page < pages);
					/* 选择该页第三个对象 */
					else if (ty > h / 3 * 2)
					{
						nowob = allob->next;
						for (i = 1; i < page * 3; i++)
							if (nowob->next != NULL)
								nowob = nowob->next;
					}
					/* 选择该页第二个对象 */
					else if (ty > h / 2)
					{
						nowob = allob->next;
						for (i = 1; i < page * 3 - 1; i++)
							if (nowob->next != NULL)
								nowob = nowob->next;
					}
					/* 选择该页第一个对象 */
					else if (ty > h / 3)
					{
						nowob = allob->next;
						for (i = 1; i < page * 3 - 2; i++)
							if (nowob->next != NULL)
								nowob = nowob->next;
					}
					/* 向上翻页 */
					else if (ty > h / 6)
						page -= 1 * (page != 1);
					/* 把对象拖到特殊键时删除 */
					else if (theob != 0)
						cutobject(allob, theob);
				}
				// ————工具栏选择————
				else if (tx < w / 10)
				{
					/* 小窗口 */
					if (ty < h / 6)
						tool = 0;
					/* 移动工具 */
					else if (ty < h / 3)
						tool = 1;
					/* 旋转工具 */
					else if (ty < h / 2)
						tool = 2;
					/* 缩放工具 */
					else if (ty < h / 3 * 2)
						tool = 3;
					/* x方向锁定键 */
					else if (ty < h / 6 * 5)
						box = !box;
					/* y方向锁定键 */
					else if (ty < h)
						boy = !boy;
				}
				// ————上栏的点选————
				else if (ty < h / 10 && tx > w / 10)
				{
					if (tx < w / 10 * 3)
						tool = 4;
					else if (tx < w / 10 * 5)
						tool = 5;
					else if (tx < w / 10 * 7)
						tool = 6;
					else if (tx < w / 10 * 9)
						tool = 7;
				}
				// 下栏的点选
				else if (ty > h / 10 * 9 && tx < w / 10 * 9
						 && theob == 0 && bo == false && boo == false)
				{
					if (tool != 8)
						tool = 8;
					else
						tool = 1;
				}
				// ————结束所有开关———
				if (bo == true)
					bo = false;
				if (boo == true)
					boo = false;

				// ————取消当前对象———
				if (boo == false && theob == 0 && tool > 3 && tx > w / 10 * 9 && ty < h / 6)
					nowob = NULL;
				// ———取消选中对象号———
				theob = 0;
				// ————触碰点清零————
				tx = ty = dx = dy = 0;
			}
		}
	}
}