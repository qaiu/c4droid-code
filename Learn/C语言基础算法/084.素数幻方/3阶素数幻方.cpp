//http://bbs.bccn.net/m.thread.php?tid=357392


#include<stdio.h>
#include<string.h>
#include<math.h>
#include<malloc.h>
int prime(int sum){
    int i,j,k=1;
    i=(int)sqrt(sum);
    for(j=2;j<=i;j++){
        if((sum%j)==0){
            k=0;
            break;
        }
    }
    return k;
}                                                                                      /*判断是否为素数*/                                                       

int main()
{
    int a[9999],m,n,i,j,k,flag=0,x1,x2,x3,x4,x5,x6,x7,x8,x9,fin,ss=0,max;
    printf("请输入要计算的素数范围不要超过10000\n如:1 999 则计算999以内的素数\n");
    scanf("%d%d",&m,&n);
    for(i=m;i<=n;i++){
        if(prime(i)){
            a[flag]=i;
            flag++;
        }
    }                                                                                 /*求出给定范围内所有的素数*/                                                        

    max=a[flag-1];
    for(i=0;i<flag;i++){
        x5=a[i];fin=3*x5;                                                             /*因为三阶幻方的和等于中间项的三倍所以先选择中间项*/                                              

        for(j=i+1;j<flag;j++){
            x1=a[j];                                                                   /*左上角的数字*/                                                   

            for(k=j+1;k<flag;k++){
                x3=a[k];                                                               /*右上角的数字*/
                if(prime(2*x5-x1)==1){                                  

                    x9=2*x5-x1;                                                       /*这是右下角的数字*/
                    if(prime(2*x5-x3)==1){                                 

                        x7=2*x5-x3;                                                   /*左下角的数字*/
                            if(x1!=x7&&x1!=x9){               

                            if(x3!=x7&&x3!=x9){
                            if(x7!=x5&&x7!=x9){
                            if(x5!=x9){                                               /*控制四个角落的数字不能相同*/
                        x2=fin-x1-x3;x4=fin-x1-x7;x6=fin-x3-x9;x8=fin-x7-x9;           /*上下左右四个方向的数字*/
                        if(x2>m&&x4>m&&x6>m&&x8>m)
                        if(x1>0&&x2>0&&x3>0&&x4>0&&x5>0&&x6>0&&x7>0&&x8>0&&x9>0)
                        if(x1<max&&x2<max&&x3<max&&x4<max&&x5<max&&x6<max&&x7<max&&x8<max&&x9<max)
                        if((prime(x4)==1)&&(prime(x2)==1)&&
                        (prime(x8)==1)&&(prime(x6))==1){ 
                        printf("第%d组\n%3d  %3d   %3d  \n%3d  %3d   %3d  \n%3d  %3d   %3d  \n\n",ss,x1,x2,x3,x4,x5,x6,x7,x8,x9);                  /*当满足一系列条件之后说明可以满足幻方的要求*/         

                            ss+=1;
                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    printf("共%d个素数幻方",ss);
    return 0;
}