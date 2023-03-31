/*
  来源：QQ群 172725648
  上传者：未知
  整理：QAIU
*/


// 推箱子游戏 初学写的有不规范的地方见谅
// 优先读取PushBox.txt的地图 其次是自带地图
// 地图中的符号  #:地图分隔 换行符:地图换行 0:空地 1:目标印记 2:箱子 3:放好的箱子 4:人 5:人站在印记上 6:墙  其它:无意义不影响读取
#include <stdio.h>
#include <stdlib.h>
#define N 33 // 地图最大边长
// 记录状态 px,py:人的坐标 nLevel:关卡数
int px, py, nBoxes, nReached, nSteps, nLevel=1;
// 物体 数组下标 0:空 1:目标印记 2:箱子 3:放好的箱子 4:人 5:人站在印记上 6:墙
const char items[][6]={" ·"," ×"," □"," ■"," g"," g"," =","  "};
const int L=sizeof(items)/sizeof(items[0]);
// 翻译打印地图
void printMap(char a[N][N]) {
	int i, j, ix;
	nBoxes=nReached=0;
	for (i=0; i<N; i++) { // 双层循环打印二维的地图
		for (j=0; j<N; j++) {
			if (a[i][j]<0||L<a[i][j]) break;
			ix=(0<=a[i][j]&&a[i][j]<L)?a[i][j]:L; // ix为数组中数字，处理一下防止items数组越界
			printf("%s",items[ix]); // 将数组中的数字翻译为物体
			switch (a[i][j]) { // 顺便统计游戏状态
			case 3:nReached++;
			case 2:nBoxes++;break;
			case 4:case 5:px=i;py=j;break;
			}
		}
		printf("\n");
		if (j==0) break;
	}
}
// 打印界面
void print(char a[N][N]) {
	system("clear")&&system("cls");
	printf("推箱子  第%d关\n",nLevel);
	printMap(a);
	printf("到达的箱子数:%d/%d  步数统计:%d\n\n",nReached,nBoxes,nSteps);
	if (nReached<nBoxes) {
	printf("[菜单] 临时存档：#  读取存档：@  选择关卡:^\n");
	printf("[操作] 上：w/2  下：s/8  左：a/4  右：d/6\n\n请输入：");
	}else {
		printf("恭喜您过关！\n\n回车进入下一关. . . ");
	}
}
// 复制数组b到a 用于复制地图
void mapCopy(char a[N][N],char b[N][N]) {
	int i, j;
	for (i=0; i<N; i++)
		for (j=0; j<N; j++) a[i][j]=b[i][j];
}
// 移动坐标(x,y)处的物体到(x+mx,y+my) 仅移动一个物体一步 返回是否移动成功
int move(char a[N][N],int x,int y,int mx,int my) {
	int x2=x+mx, y2=y+my; // 目的坐标(x2,y2)
	if (x2<0||N<=x2||y2<0||N<=y2||a[x2][y2]<0||L<a[x2][y2]) return 0;
	if (5<a[x][y]||1<a[x2][y2]) return 0; // 判断是否能移动
	a[x2][y2]+=a[x][y]&6; // 按位与结果 2:代表箱子 4:代表人
	a[x][y]-=a[x][y]&6;
	return 1;
}
// 人从坐标(px,py)移动到(px+mx,py+my) 一次仅移动一步
void movePeople(char a[N][N],int mx,int my) {
	int x=px+mx, y=py+my; // 人移动后的坐标(x,y)不能越界
	if (x<0||N<=x||y<0||N<=y||a[x][y]<0||L<a[x][y]) return;
	if (a[x][y]<2) { // 人往空处走
		if (move(a,px,py,mx,my)) { // 人走一步
			px=x;py=y;nSteps++;
		}
	}else if (a[x][y]==2||a[x][y]==3) { // 人推箱子
		if (move(a,x,y,mx,my)&&move(a,px,py,mx,my)) { // 先移箱子，如果成功了再移人
			px=x;py=y;nSteps++;
		}
	}
}
// 播放地图 返回下一个关卡数
void playMap(char a[N][N], int n) {
	char b[N][N], act[1024], c;
	int i, steps=nSteps=0;
	if (nLevel<1||n<nLevel) return;
	mapCopy(b,a); // 存档
	do  {
		print(a);
		if (nReached==nBoxes) break; // 箱子推完则退出
		while ((c=getchar())!='\n') // 解析并执行操作
			switch (c) {
			case 'w':case '2':movePeople(a,-1,0);break;
			case 's':case '8':movePeople(a,1,0);break;
			case 'a':case '4':movePeople(a,0,-1);break;
			case 'd':case '6':movePeople(a,0,1);break;
			/*case '1':movePeople(a,-1,-1);break;//允许斜着走
			case '3':movePeople(a,-1,1);break;
			case '9':movePeople(a,1,1);break;
			case '7':movePeople(a,1,-1);break;*/
			case '#':mapCopy(b,a);steps=nSteps;break;
			case '@':mapCopy(a,b);nSteps=steps;break;
			case '^':
				printf("可选关卡为%d~%d关,请输入关卡序号：",nLevel>0?1:0,n);
				fflush(stdin);scanf("%d",&nLevel);
				return;
			}
	}while (1);
	getchar();
}
// 在地图中寻找关卡n的地址
char* locateLevel(char* map,int n) {
	int level=0, f=1;
	char *p;
	for (p=map; *p!=0; p++) { // 统计关卡数
		if ('0'<=*p&&*p<'0'+L) {
			if (f!=0&&(++level)==n) return p;
			f=0;
		}else if (*p=='#') f=1;
	}
	return 0;
}
// 读取地图到矩阵a
int getMap(char a[N][N],int level,char* map) {
	int i, j;nLevel=level;
	char *p=locateLevel(map,level);
	if (p==0) return 0;
	for (i=0; i<N; i++)
		for (j=0; j<N; j++) a[i][j]=-1; // 清空矩阵a
	for (i=j=0; *p!=0&&*p!='#'; p++) // #或\0代表一个地图的结束
		if ('0'<=*p&&*p<'0'+L) a[i][j++]=*p-'0';
		else if (*p=='\n') {i++;j=0;}
	return 1;
}
// 播放游戏
void play(char *map) {
	char a[N][N], *p=map;
	int f=1, levels=locateLevel(p,1)!=0?1:0;
	while ((p=locateLevel(p,2))!=0) levels++;
	while (nLevel<=levels) {
		getMap(a,nLevel,map);
		playMap(a,levels);
		if (nReached==nBoxes) nLevel++;
	}
}
int playFile(char* file) {
	char *fmap;
	int fLen;
	FILE *fp=fopen(file,"r");
	if (fp==0) return 0;
	fseek(fp,0,2);fLen=ftell(fp);fseek(fp,0,0);
	fmap=(char*)malloc(fLen);
	fread(fmap,fLen,1,fp);
	play(fmap);
	free(fmap);fclose(fp);
	return 1;
}
// 主方法
int main(char **argv,int argc) {
	//  #:地图分隔 0:空 1:目标位置 2:箱子 3:放好的箱子 4:人 5:人站在印记上 6:墙  \n:换行  其它值:无意义
	char map[]=
		"000000000000\n" // 第一关地图
		"006666660000\n"
		"006400066600\n"
		"066066201660\n"
		"060002021160\n"
		"066020201160\n"
		"060006666660\n"
		"001666000000\n"
		"000000000000#"
		"060000000000000\n" // 第二关地图
		"000066666660000\n"
		"066660000066060\n"
		"060001666403006\n"
		"060606000060000\n"
		"060602026106600\n"
		"06060030060600\n"
		"06016202060600\n"
		"066000060606660\n"
		"006066610000060\n"
		"006000006600060\n"
		"006666666666660\n"
		"000000000000000#"
		"0066666070\n" // 第三关地图
		"0064006660\n"
		"0662620060\n"
		"0600006060\n"
		"0601122160\n"
		"0660106660\n"
		"0066006000\n"
		"7706666070";
	if (!playFile("PushBox.txt")) play(map);
	printf("\n游戏结束 祝您 Have a good time !\n");
}