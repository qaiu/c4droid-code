/* 负责各种对象操作, 主要是控制链表 */

/* 添加对象 */
node *addobject(node * head, int id)
{
	// —————链表的初始化————
	node * s = NULL;
	node *p = NULL;
	p = head;
	while (p->next != NULL)
		p = p->next;
	s = (node *) malloc(sizeof(node));
	if (s == NULL)
		exit(0);
	// —————对象初始化—————
	s->id = id;					// ID
	s->x = w / 2;				// x坐标
	s->y = h / 2;				// y坐标
	s->h = 0;					// 旋转角度
	s->mark = obnum + 1;		// 对象号
	s->next = NULL;				// 链表尾
	s->color.r = 255;
	s->color.g = 255;
	s->color.b = 255;
	switch (id)
	{
	case 0:
		s->size.a = 100;		// 矩形长
		s->size.b = 50;
		break;					// 矩形宽
	case 1:
		s->size.a = 100;		// 圆的半径
		s->size.b = 16;			// 圆分段
	}
	// —————接入链表——————
	p->next = s;
	p = s;
	obnum += 1;					// 总数加一
	// —————看看页码——————
	if (s->mark > pages * 3)
	{
		page += 1;
		pages += 1;
	}
	// —————链表返回——————
	return head;
}

/* 删除对象(这手续真多啊) */
node *cutobject(node * head, int mark)
{
	int i;						// 循环体
	// ———————————————
	// —使s指向删除对象,p指向前一个—
	node *s;
	node *p;
	p = head;
	for (i = 0; i < mark - 1; i++)
		p = p->next;
	s = p->next;
	// ———删除对象,分类讨论————
	if (s->next != NULL)
		p->next = s->next;
	else
		p->next = NULL;
	free(s);
	// —————处理后事—————
	/* 对象总数少一个 */
	obnum -= 1;
	/* 看看页码 */
	if (mark - mark / 3 * 3 == 1 && mark != 1)
	{
		page -= 1;
		pages -= 1;
	}
	/* 重置当前对象 */
	if (obnum != 0)
	{
		if (p->next != NULL)
			nowob = p->next;
		else
			nowob = p;
	}
	else
		nowob = NULL;
	/* 重新编号 */
	for (p = head, i = 1; p->next != NULL; p = p->next, i++)
		p->next->mark = i;
}

/* 新建链表 */
node *createnode(void)
{
	node *head = NULL;			// 头结点
	head = (node *) malloc(sizeof(node));	// 申请内存空间
	if (head == NULL)
		exit(0);
	head->next = NULL;			// 建立头结点
	return (head);
}