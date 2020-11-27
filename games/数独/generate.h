//原文链接不小心丢了。。。
#ifndef GENERATE_H
#define GENERATE_H

#include <vector>
using namespace std;
int map[MMAX][MMAX];
int answer[MMAX][MMAX];
int judgemap[MMAX][MMAX];
bool IsRightPlace(vector<vector<int>> &num, int row, int col)
{
    int n = num[row][col];
    //注意i < row
    for (int i = 0; i < row; i++)
    {
        if (num[i][col] == n)
            return false;
    }
    //注意i < col
    for (int i = 0; i < col; i++)
    {
        if (num[row][i] == n)
            return false;
    }
    int row_start = row / 3;
    row_start *= 3;
    int row_end = row_start + 2;
    int col_start = col / 3;
    col_start *= 3;
    int col_end = col_start + 2;
    int i = row_start, j = col_start;
    //注意 k <= 8
    for (int k = 1; k <= 8; k++)
    {
        if (row != i || col != j)
        {
            if (num[i][j] == n)
                return false;
        }
        else
            break;
        if (j == col_end)
        {
            //注意j = col_start ！不要搞错换行时列的起始点！
            j = col_start;
            i = i + 1;
        }
        else
        {
            j = j + 1;
        }
    }
    return true;
}

bool generate_core(vector<vector<int>> &num, int row, int col)
{
    
    vector<int> number;
    for (int i = 1; i <= 9; i++)
        number.emplace_back(i);
    while (!number.empty())
    {
        int randindex = rand() % number.size();
        int randnum = number[randindex];
        number.erase(number.begin() + randindex);
        num[row][col] = randnum;
        if (IsRightPlace(num, row, col) == false)
            continue;
        if (row == MMAX - 1 && col == MMAX-1)
        {
            return true;
        }
        int nextrow, nextcol;
        if (col == MMAX-1)
        {
            nextrow=row + 1;
            nextcol = 0;
        }
        else
        {
            nextrow = row;
            nextcol = col + 1;
        }
        bool next = generate_core(num, nextrow, nextcol);
        if (next)
            return true;
    }
    if (number.empty())
    {
        num[row][col] = -5;
        return false;
    }		
}

void generate()
{
    vector<vector<int>> num(MMAX, vector<int>(MMAX, -1));
    srand((unsigned)time(NULL));
    if ( generate_core(num, 0, 0) )
    {
        for (int i = 0; i < MMAX; i++)
        for (int j = 0; j < MMAX; j++)
        	map[i][j]=num[i][j];
    }
}
void generate_map(int n)
{
	generate();
	int *p=*map;
	memcpy(answer,map,sizeof(map));
	int a[MMAX*MMAX]; 
	int i, j, k,sum=0; 
	for (i = 0; i < MMAX*MMAX; i++)
		a[i] = i; //生成原始序列
	for (i = 0; i < n+sum&&i<MMAX*MMAX; i++)
	{//将原始序列随机打乱
		j = rand()%(MMAX*MMAX-i)+i; 
		k = a[i]; 
		a[i] = a[j];
		a[j] = k;
		int kk=p[a[i]];
		p[a[i]] = 0;
		char s[MMAX*MMAX]={};
		for (int i=0;i<MMAX*MMAX;i++)
		{
			s[i]=p[i]+48;
		}
		if (solve(s)>1)
		{
			p[a[i]]=kk;
			sum++;
		}
	}
	memcpy(judgemap,map,sizeof(map));
}
#endif