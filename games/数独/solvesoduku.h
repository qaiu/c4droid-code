/*
 *原作者信息,版权归原作者所有,千百度仅做修改
 *原文链接:http://m.blog.chinaunix.net/uid-26456800-id-3380612.html =====================================================================================
 *
 * Filename: sudokou.c
 *
 * Description: 
 *
 * Version: 1.0
 * Created: 10/21/2012 02:58:56 PM
 * Revision: none
 * Compiler: gcc
 *
 * Author: royn.wang.renyuan@gmail.com (), 
 * Organization: 
 *
 * =====================================================================================
 */

#ifndef SOLVESODUKU_H
#define SOLVESODUKU_H

#define MMAX 9
int printmap[MMAX][MMAX];
char M[MMAX][MMAX] = {{0}};
int count_num = 0;
int judge=0;
int getblockid(int row, int col)
{
    return row/3 * 3 + col/3;
}
void getnext(int *row, int *col, int* nrow, int* ncol)
{
    if (*col ==8)
    {
        *ncol = 0;
        *nrow = (*row)+1;
    }
    else
    {
        *nrow = *row;
        *ncol = (*col) +1;
    }
}
void outputmatrix(char mtr[MMAX][MMAX] )
{
    judge++;
    for (int i=0;i<MMAX;i++)
    for (int j = 0; j<MMAX;j++)
    printmap[i][j]=mtr[i][j];
}

void fill(int row, int col, char matrix[MMAX][MMAX], char rowd[][MMAX], char cold[][MMAX], char blod[][MMAX])
{
    count_num ++;
    if (matrix[row][col] != 0)
    {
        if (row == 8 && col == 8)
        {
            outputmatrix(matrix);
            return ;
        }
        int nr, nc;
        getnext(&row,&col,&nr,&nc);
        fill(nr,nc,matrix,rowd,cold,blod);
        return;
    }
    int i = 1;
    char mtr[MMAX][MMAX];
    memcpy(mtr, matrix, MMAX*MMAX*sizeof(char));
    for (;i<= 9;i++){
        //duplicate all data
        int blockid = getblockid(row, col);
        if (
            blod[blockid][i-1]!= (char)1 &&
            rowd[row][i-1]!=(char)1 &&
            cold[col][i-1]!=(char)1
            )
            {  //fill the cell
                matrix[row][col] = i;
                if (row == 8 && col == 8){
                outputmatrix(matrix);
                return;
            }
            //set the restriction
            blod[blockid][i-1] = 1;
            rowd[row][i-1] = 1;
            cold[col][i-1] = 1;
            int nrow,ncol;
            getnext(&row, &col, &nrow, &ncol);
            fill(nrow,ncol,matrix,rowd,cold,blod);
            //restore status
            blod[blockid][i-1] = 0;
            rowd[row][i-1] = 0;
            cold[col][i-1] = 0;
            matrix[row][col] = 0;
        }
    }
}

void trackfixedcell(char fixmtr[MMAX][MMAX], char r[MMAX][MMAX],char c[MMAX][MMAX],char b[MMAX][MMAX]){
    int row,col;
    for (row = 0; row<MMAX; row++)
    {
    	for (col= 0; col<MMAX; col++)
        {
        	if (fixmtr[row][col]!= 0)
        	{
                r[row][(int)fixmtr[row][col]-1] = 1;
                c[col][(int)fixmtr[row][col]-1] = 1;
                b[getblockid(row,col)][(int)fixmtr[row][col]-1] = 1;
            }
        }
    }
}
void readfixed(char mtr[MMAX][MMAX], char* input)
{
    int i = 0;
    for (;i<MMAX*MMAX;i++)
    {
        int row = i/MMAX;
        int col = i%MMAX;
        if (input[i] != '0')
        {
            mtr[row][col] = input[i]-48;
        }
    }
}

int solve(char *str)
{
	count_num=0;
	judge=0;
	char r[MMAX][MMAX] = {{0}};
    char c[MMAX][MMAX] = {{0}};
    char b[MMAX][MMAX] = {{0}};
    memcpy(M,b,sizeof(b));
    readfixed(M,str);
    trackfixedcell(M,r,c,b);
    fill(0,0,M,r,c,b);
    return judge;
}
#endif