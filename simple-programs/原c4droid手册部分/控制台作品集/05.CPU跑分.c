/*c4droid代码手册
*cpu跑分
*作者 未知
*转载请注明出处
*/
#include<iostream>
#include"time.h"//clock()所属头文件
#include"conio.h"//clrscr()和sleep()所属头文件
#include"dirent.h"//读取文件操作所属头文件
#include"stdlib.h"//srand()所属头文件
using namespace std;

const int N_qsort=10000;//快排的数据规模
const int M=20000,N=50000;//整点、浮点运算的规模
const int N_pi=100000000;//计算圆周率的运算规模
const int N_read_dir=50000;//读取根目录的规模
const int N_FPS=20000;//FPS测试规模

double s_int,s_float,s_pi,s_sort,s_read,s_FPS;

void int_comp(void);//整点运算
void float_comp(void);//浮点运算
void pi_comp(void);//泰勒级数推论式计算圆周率
void Qsort(int a[],int low,int high);//快排算法
void qsort(void);//调用快排算法的函数
void read_dir(void);//读取根文件夹算法
void FPS(void);//通过显示及清屏模拟FPS测试
void ZB(int);
int main()
{
	cout<<"即将开始进行处理器运算性能测试\n"
	<<"   可能会花费您几分钟的时间"<<endl;
	sleep(5);
	ZB(10);
int_comp();//整点运算
float_comp();//浮点运算
pi_comp();//泰勒级数推论式计算圆周率
qsort();//快速排序
read_dir();//读取根目录
FPS();//模拟测FPS
clrscr();
cout<<"测试完毕！"<<endl;
cout<<"整点运算得分:"<<"\033[31;40;1m"<<s_int<<"\033[0m"<<endl;
cout<<"浮点运算得分:"<<"\033[31;40;1m"<<s_float<<"\033[0m"<<endl;
cout<<"泰勒级数推论式计算圆周率运算得分:"<<"\033[31;40;1m"<<s_pi<<"\033[0m"<<endl;
cout<<"排序运算得分:"<<"\033[31;40;1m"<<s_sort<<"\033[0m"<<endl;
cout<<"读取文件运算得分:"<<"\033[31;40;1m"<<s_read<<"\033[0m"<<endl;
cout<<"模拟计算FPS运算得分:"<<"\033[31;40;1m"<<s_FPS<<"\033[0m"<<endl;
cout<<"总得分:"<<"\033[32;40;1m"<<s_int+s_float+s_pi+s_sort+s_read+s_FPS<<"\033[0m"<<endl;
return 0;
}

void int_comp(void)//整点加法
{
clrscr();
cout<<"即将进入整点加法运算测试!"<<endl;
sleep(3);
ZB(5);
cout<<"整点运算测试中(运算次数为:"<<(double)M*N<<"!请稍后!)......"<<endl;
clock_t start,end;
int i,j;
start=clock();
for(i=0;i<M;i++)
for(j=0;j<N;j++);
end=clock();
double duration=(double)(end-start)/CLOCKS_PER_SEC;
double score=(M*N)/duration;
clrscr();

/*注：score本身即为运算速度，数量级一般在亿，
为方便起见，本程序的分数均采用运算速度除以
一万后的结果！除特殊说明，后面类同！*/
s_int=score/10000;
cout<<"整点运算测试完毕!分数:"<<"\033[32;40;1m"<<s_int<<"\033[0m"<<endl; 
sleep(5);
}

void float_comp(void)//浮点加法
{
clrscr();
cout<<"即将进入浮点加法运算测试!"<<endl;
sleep(3);
ZB(5);
cout<<"浮点运算测试中(运算次数为:"<<(double)M*N<<"!请稍后!)......"<<endl;
clock_t start,end;
float i,j;
start=clock();
for(i=0;i<M;i++)
for(j=0;j<N;j++);
end=clock();
double duration=(double)(end-start)/CLOCKS_PER_SEC;
double score=(M*N)/duration;
clrscr();
s_float=score/10000;
cout<<"浮点运算测试完毕!分数:""\033[32;40;1m"<<s_float<<"\033[0m"<<endl;
sleep(5);
}

void pi_comp(void)
{
	clrscr();
	cout<<"即将进入泰勒级数推论式计算圆周率运算测试!"<<endl;
	sleep(3);
	ZB(5);
	cout<<"泰勒级数推论式计算圆周率中(运算次数为:"<<N_pi<<"!请稍后!)......"<<endl;
	int m,i=1;
	double s=0;
	clock_t start,end;
	start=clock();
	for(m=1;m<N_pi;m+=2)
	{
		s+=i*(1.0/m);
		i=-i;
	}
	end=clock();
	double duration=(double)(end-start)/CLOCKS_PER_SEC;
	double score=N_pi/duration;
	clrscr();
//下面一行可输出计算出来的圆周率
	//cout<<"pi="<<4*s<<endl;
	s_pi=score/10000;
	cout<<"泰勒级数推论式计算圆周率完毕!分数:""\033[32;40;1m"<<s_pi<<"\033[0m"<<endl;
	sleep(5);
}

void Qsort(int a[],int low,int high)//快排算法
{
if(low>=high) return;
int first=low;
int last=high;
int key=a[first];
while(first<last)
{
while(first<last&&a[last]>=key) --last;
a[first]=a[last];
while(first<last&&a[first]<=key) ++first;
a[last]=a[first];
}
a[first]=key;
Qsort(a,low,first-1);
Qsort(a,first+1,high);
}

void qsort(void)//调用快排算法的函数
{
clrscr();
cout<<"即将进入整型数排序运算测试!"<<endl;
sleep(3);
ZB(5);
int a[N_qsort];
for(int i=N_qsort;i>0;i--) a[N_qsort-1]=i;
cout<<"排序运算中(对"<<N_qsort<<"个数进行快速排序!请稍后!)......"<<endl;//采用最坏时间方案
clock_t start,end;
start=clock();
Qsort(a,0,N_qsort-1);
end=clock();
double duration=(double)(end-start)/CLOCKS_PER_SEC;
double score=(N_qsort*N_qsort)/duration;
clrscr();
s_sort=score/10000;
cout<<"排序运算测试完毕!分数:""\033[32;40;1m"<<s_sort<<"\033[0m"<<endl;
sleep(5);
}

void read_dir(void)//读取根文件夹算法
{
	clrscr();
	cout<<"即将进入读取系统文件运算测试!"<<endl;
	sleep(3);
	ZB(5);
	int num=0;//记录文件夹数目
	DIR *dir;
	struct dirent *ptr;
	cout<<"接下来测试读取文件夹的速度!请查看下面读取出来的根"<<endl<<
	"目录列表!若列表有误,则本次测试的文件读取速速度失效!"<<endl;//其实这项是装装逼，一定读取无误，只是想让你参与测试之中
	sleep(5);
	dir=opendir("/sdcard");
	while((ptr=readdir(dir))!=NULL) 
	{
		num++;
		cout<<ptr->d_name<<endl;
	}
	closedir(dir);
	cout<<"您可查看该根目录列表是否读取成功!15秒后程序继续进行测试!\n\n\n\n\n"<<endl;
	sleep(15);
	cout<<"读取根目录运算中(总读取次数为:"<<N_read_dir<<"!请稍后!)......"<<endl;
	clock_t start,end;
	start=clock();
	for(int i=0;i<N_read_dir;i++)
	{
		dir=opendir("/sdcard");
	    closedir(dir);
	}
	end=clock();
	double duration1=(double)(end-start)/CLOCKS_PER_SEC;
	start=clock();
	for(int i=0;i<N_read_dir;i++);
	end=clock();
	double duration2=(double)(end-start)/CLOCKS_PER_SEC;
	double score=(N_read_dir*num)/(duration1-duration2);
	clrscr();
	s_read=score/1000;//此处除以1000
	cout<<"读取根目录测试完毕!分数:"<<"\033[32;40;1m"<<s_read<<"\033[0m"<<endl;
    sleep(5);
}

void FPS(void)
{
    srand(time(0));
	clrscr();
	cout<<"即将进入模拟测试屏幕帧率运算测试!"<<endl;
	sleep(3);
	ZB(5);
	int i,j,k,c[10];
	//由于速度太快,此处给出可视化，仅供娱乐
	for(i=0;i<15;i++)
	{
		cout<<"FPS测试中(测试规模为"<<N_FPS<<"帧)......"<<endl;
		for(j=0;j<10;j++) c[j]=rand()%60;
		cout<<"\033[34;40;1m";
		for(j=0;j<10;j++)
		{
			for(k=0;k<c[j];k++) cout<<"█";
			cout<<c[j]<<endl;
		}
		sleep(1);
		cout<<"\033[0m";
		clrscr();
	}
	cout<<"FPS测试中(测试规模为"<<N_FPS<<"!请稍后!)......"<<endl;
	cout<<"【请稍等，卖力监测中!下面会出现黑屏一段时间,属正常现象】"<<endl;//此处可改成红屏什么的
	sleep(5);
	//此处正式开始测试
	clock_t start,end;
	start=clock();
	for(i=0;i<N_FPS;i++)
	{
		cout<<"█";
		clrscr();
	}
	end=clock();
	double duration1=(double)(end-start)/CLOCKS_PER_SEC;
	start=clock();
	for(i=0;i<N_FPS;i++);
	end=clock();
	double duration2=(double)(end-start)/CLOCKS_PER_SEC;
	double score=N_FPS/(duration1-duration2);
	s_FPS=score;
	cout<<"FPS测试完毕!分数:"<<"\033[32;40;1m"<<s_FPS<<"\033[0m"<<endl;
//此处不除以10000
}

void ZB(int n)//n适合等于3、5、10、15、20、30、60
{
	cout<<"\033[32;40;1m";
	int i,j;
	for(i=0;i<=n;i++)
	{
		for(j=0;j<60/n*i;j++) cout<<"█";
		cout<<endl;
		sleep(1);
		clrscr();
	}
	cout<<"\033[0m";
};
/*注：以上代码可经过删减精简成直接快速测试！上面
的代码浪费了很多时间，只是为了使测试者看到可视化
界面，参与其中！实际意义不大！所测结果也从一定
程度上可以比较手机处理器性能的高低优劣！经测试，
粗略可分为6个等级：(0,2000)渣渣，(2000,3000)低端，
(3000,4000)中端，(4000,5000)高端，(5000,6000)超高端，
(6000,……)机皇(或一般PC端的安卓模拟器);在以上代码中
最后结果没有给出相关等级判断，可以自行添上！*/