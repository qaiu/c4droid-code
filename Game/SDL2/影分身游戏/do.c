void addtons(float x,float y)
{
//—————链表的初始化————
tons *s=NULL;
tons *p=NULL;
p=head;
while(p->next!=NULL)
  p=p->next;
s= (tons *) malloc (sizeof(tons));
if (s==NULL)
  exit(0);
//—————对象初始化—————
s->x=x;
s->y=y;
s->next=NULL;//链表尾
//—————接入链表——————
p->next=s;
}