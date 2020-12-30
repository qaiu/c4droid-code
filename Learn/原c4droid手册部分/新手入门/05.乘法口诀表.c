/*c4droid代码手册 
*乘法口诀表
 *TTHHR编写
  *转载请说明出处 */
   #include<stdio.h> 
   int main() 
   { 
   int i,j; 
   for (i=1;i<=9;i++)
    { 
    for (j=1;j<=i;j++)
     printf("%d*%d=%d ",j,i,i*j);
     puts("");
     } 
     return 0;
  }
