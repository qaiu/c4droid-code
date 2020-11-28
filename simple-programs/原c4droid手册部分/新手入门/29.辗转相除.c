/*c4droid代码手册
 *辗转相除
 *TTHHR编写
 *转载请说明出处
*/
#include<stdio.h>
main()
{
	int m,n,r;
	scanf("%d,%d",&m,&n);
	for (r=n;r>0;)
	{
		r=m%n;
		m=n;
		n=r;
	}
	printf("%d",m);
	return 0;
}