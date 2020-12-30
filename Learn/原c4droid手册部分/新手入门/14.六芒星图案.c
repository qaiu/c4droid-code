/*c4droid代码手册
 *六芒星
 *雪夜鼠编写
 *TTHHR收集整理
 *转载请说明出处
*/
#include<iostream>
#include<ctime>
using namespace std;
void s(char*a,char*b){
int c;
for(c=0;*(a+c)!=0;c++)
*(b+c)=*(a+c);
}
void p(int n){
int a=0,b=0;
char d[50][50]={0},c[50]={0},k=0;
for(a=1;a<n-1;a++)
c[a]=' ';
c[0]=c[n-1]='*';
s(c,*(d+0));
for(a=0,b=n-1;b>=0;a++,b--){
k=c[a];c[a]=c[a+1];c[a+1]=k;
k=c[b];c[b]=c[b-1];c[b-1]=k;
s(c,*(d+a+1));
}
for(a=0;a<n/2;a++){
 for(b=0;b<n;b++)
 cout<<' ';
 cout<<d[n/2+a]<<endl;
}
for(a=0;a<n;a++){
if(a==0||a==n-1)
 for(b=0;b<n;b++)
 cout<<'*';
 else
 cout<<d[a];
 if(a==0||a==n-1)
 for(b=0;b<n;b++)
 cout<<'*';
 else 
 for(b=0;b<n;b++)
 cout<<' ';
 if(a==0||a==n-1)
 for(b=0;b<n;b++)
 cout<<'*';
 else
 cout<<d[a];
 cout<<endl;
}
for(a=0;a<n/2;a++){
 for(b=0;b<n;b++)
 cout<<' ';
 cout<<d[a]<<endl;
}
}
int main(){
clock_t st=clock();
int g=clock()-st;
int a=0,b[30],c,d,e;
for(c=6;c<18;c++){
b[c-6]=c;
if(c==17)
for(d=18;d>5;d--)
b[30-d]=d;
}
for(;;){
if(g%170000==0){
 p(b[a]);
 a++;
 if(a==24) a=0;
 g=clock()-st;
}
else g=clock()-st;
}
}