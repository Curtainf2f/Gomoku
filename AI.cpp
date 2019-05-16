#include "graphics.h"
#include "art.h"
#include "mystruct.h"
#include "AI.h"
#include <stack>
#include <queue>
#include <cstring>
#include "rule.h"
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <map>
#include <set>
#include <string>
#include <ctime>
using namespace std;
//#define DEBUG

clock_t start_time;
const int timelimit = 930;

extern int map_wzq[15][15];
extern std::stack<chesspiece*> chess;
thinkpiece *tempchess;

bool piececlosed(int x,int y)
{
    int next[8][2]= {1,0,-1,0,0,1,0,-1,1,1,-1,-1,1,-1,-1,1};
    int tx,ty,sum;
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<2; j++)
        {
            sum=1;
            tx=x;
            ty=y;
            while(1)
            {
                if(sum>3) break;
                tx+=next[i*2+j][0];
                ty+=next[i*2+j][1];
                if(tx<0||ty<0||tx>=15||ty>=15) break;
                if(map_wzq[tx][ty]==0)
                    sum+=1;
                else
                    return true;
            }
        }
    }
    return false;
}

int analysis(char s[])
{
///长连
    if (strstr(s,"OOOOO")!=NULL)
        return 0;
///活四
    else if (strstr(s,"_OOOO_")!=NULL)
        return 1;
    else if (strstr(s,"O_OOO_O")!=NULL)
        return 1;
///冲四
    else if (strstr(s,"OOO_O")!=NULL)
        return 2;
    else if (strstr(s,"_OOOOH")!=NULL)
        return 2;
    else if (strstr(s,"OO_OO")!=NULL)
        return 2;
///活三
    else if (strstr(s,"__OOO_")!=NULL)
        return 3;
    else if (strstr(s,"_OO_O_")!=NULL)
        return 3;
///眠三
    else if (strstr(s,"OHHH_O")!=NULL)
        return 4;
    else if (strstr(s,"OHH_HO")!=NULL)
        return 4;
    else if (strstr(s,"__OOOH")!=NULL)
        return 4;
    else if (strstr(s,"_O_OOH")!=NULL)
        return 4;
    else if (strstr(s,"_OO_OH")!=NULL)
        return 4;
    else if (strstr(s,"O__OO")!=NULL)
        return 4;
    else if (strstr(s,"O_O_O")!=NULL)
        return 4;
    else if (strstr(s,"H_OOO_H")!=NULL)
        return 4;
///活二
    else if (strstr(s,"__OO__")!=NULL)
        return 5;
    else if (strstr(s,"__O_O_")!=NULL)
        return 5;
    else if (strstr(s,"_O__O_")!=NULL)
        return 5;
    else if (strstr(s,"___OO_")!=NULL)
        return 5;
///眠二
    else if (strstr(s,"___OOH")!=NULL)
        return 6;
    else if (strstr(s,"__O_OH")!=NULL)
        return 6;
    else if (strstr(s,"_O__OH")!=NULL)
        return 6;
    else if (strstr(s,"H_O_O_H")!=NULL)
        return 6;
    else if (strstr(s,"H_OO__H")!=NULL)
        return 6;
    return 7;
}

bool vct;

int countscore(thinkpiece piece,int c)
{
    int position[15][15] =
    {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
        { 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0 },
        { 0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0 },
        { 0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0 },
        { 0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0 },
        { 0, 1, 2, 3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, 0 },
        { 0, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, 0 },
        { 0, 1, 2, 3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, 0 },
        { 0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0 },
        { 0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0 },
        { 0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0 },
        { 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0 },
        { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    };
    int v=0;
    int tx,ty;
    char s[4][13];
    int len[4];
    int situation[8];
    int next[8][2]= {1,0,-1,0,0,1,0,-1,1,1,-1,-1,1,-1,-1,1};
    v+=position[piece.x][piece.y];
    memset(situation,0,sizeof(situation));
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<2; j++)
        {
            len[j]=0;
            tx=piece.x;
            ty=piece.y;
            while(1)
            {
                if(len[j]>4) break;
                tx+=next[i*2+j][0];
                ty+=next[i*2+j][1];
                if(tx>=0&&ty>=0&&tx<15&&ty<15)
                {
                    if(map_wzq[tx][ty]==0)
                        s[j][len[j]++]='_';
                    else if(map_wzq[tx][ty]==c)
                        s[j][len[j]++]='O';
                    else
                    {
                        s[j][len[j]++]='H';
                    }
                }
                else
                {
                    s[j][len[j]++]='H';
                    break;
                }
            }
            s[j][len[j]]='\0';
        }
        len[2]=0;
        len[3]=0;
        for(int k=len[0]-1; k>=0; k--)
            s[2][len[2]++]=s[0][k];
        s[2][len[2]++]='O';
        for(int k=0; k<len[1]; k++)
            s[2][len[2]++]=s[1][k];
        s[2][len[2]]='\0';
        for(int k=len[2]-1; k>=0; k--)
            s[3][len[3]++]=s[2][k];
        s[3][len[3]]='\0';
        situation[std::min(analysis(s[2]),analysis(s[3]))]++;
    }
    //长连0 活四1 眠四2 活三3 眠三4 活二5 眠二6
    if(situation[0]) v+=1000000;
    else if(situation[1]||situation[2]>=2||(situation[2]&&situation[3])) v+=100000;
    else if(situation[3]>=2) v+=50000;
    else if(situation[2] && vct) v+=6000;
    else if(situation[3] && (situation[4] || situation[5])) v += 5500;
    else if(situation[2] && (situation[4] || situation[5])) v += 5500;
    else if(situation[3] || situation[2]) v+=5000;
    else if(situation[4] >= 2) v+=400*situation[4];
    else if(situation[4]) v+=200 + situation[5]*20;
    else if(situation[5]) v+=80*situation[5];
    else if(situation[6]) v+=10*situation[6];
    return v;
}

bool vctf(thinkpiece piece, int range, int c, int depth);
int delscore(int c);
thinkpiece maxpiece;
bool isme;
int pdf;

#ifdef DEBUG
FILE *fp;
#endif // DEBUG

bool protect(thinkpiece piece, int c, int v, int depth)
{
    clock_t end_time = clock();
    if(static_cast<double>(end_time - start_time)/CLOCKS_PER_SEC*1000 >= timelimit)
    {
        return true;
    }
    int b;
    if(c==1) b=2;
    else b=1;
    queue<thinkpiece> consider;
    int x = piece.x;
    int y = piece.y;
    int next[8][2]= {1,0,-1,0,0,1,0,-1,1,1,-1,-1,1,-1,-1,1};
    int tx,ty,sum;
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<2; j++)
        {
            sum=1;
            tx=x;
            ty=y;
            while(1)
            {
                if(sum>4) break;
                tx+=next[i*2+j][0];
                ty+=next[i*2+j][1];
                if(tx<0||ty<0||tx>=15||ty>=15) break;
                if(map_wzq[tx][ty]==0)
                {
                    consider.push(thinkpiece(tx, ty));
                }
                sum+=1;
            }
        }
    }

    while(!consider.empty())
    {
        thinkpiece now = consider.front();
        consider.pop();
        map_wzq[now.x][now.y] = c;
        int del = delscore(b);
        if(del >= 10000)
        {
            map_wzq[now.x][now.y] = 0;
            continue;
        }
        else
        {

#ifdef DEBUG
            fprintf(fp,"%d %d try to protect\n", now.x, now.y);
            for(int i = 0; i < 15; i ++)
            {
                for(int j = 0; j < 15; j ++)
                {
                    fprintf(fp,"%d ", map_wzq[j][i]);
                }
                fprintf(fp,"\n");
            }
#endif // DEBUG

            if(!vctf(piece, 4, b, depth - 1))
            {
                map_wzq[now.x][now.y] = 0;
                return true;
            }
        }
        map_wzq[now.x][now.y] = 0;
    }
    return false;
}

bool vctf(thinkpiece piece, int range, int c, int depth) // range 2 or 14
{
    clock_t end_time = clock();
    if(static_cast<double>(end_time - start_time)/CLOCKS_PER_SEC*1000 >= timelimit)
    {
        return false;
    }
    if(depth <= 0)
        return false;
    int b;
    if(c==1) b=2;
    else b=1;

    int left, right, up, down;
    if(piece.x - range >= 0) left = piece.x - range;
    else left = 0;
    if(piece.x + range < 15) right = piece.x + range;
    else right = 14;
    if(piece.y - range >= 0) up = piece.y - range;
    else up = 0;
    if(piece.y + range < 15) down = piece.y + range;
    else down = 14;

    for(int y=up; y<=down; y++)
    {
        for(int x=left; x<=right; x++)
        {
            if(!map_wzq[x][y]&&piececlosed(x, y))
            {
                map_wzq[x][y] = c;
                int v = countscore(thinkpiece(x, y), c);
                int del;
                if(v >= 10000)
                {
                    del = delscore(b);
                    if(v - del >= -10)
                    {
#ifdef DEBUG
                        fprintf(fp, "test 10000\n");
                        for(int i = 0; i < 15; i ++)
                        {
                            for(int j = 0; j < 15; j ++)
                            {
                                fprintf(fp,"%d ", map_wzq[j][i]);
                            }
                            fprintf(fp, "\n");
                        }
#endif // DEBUG
                        map_wzq[x][y] = 0;
                        if(isme)
                            maxpiece = thinkpiece(x,y);
                        return true;
                    }
                }
                else if(v >= pdf)
                {
                    del = delscore(b);
                    if(v - del >= -10)
                    {
#ifdef DEBUG
                        fprintf(fp, "test 6000\n");
                        for(int i = 0; i < 15; i ++)
                        {
                            for(int j = 0; j < 15; j ++)
                            {
                                fprintf(fp,"%d ", map_wzq[j][i]);
                            }
                            fprintf(fp, "\n");
                        }
#endif // DEBUG
                        if(!protect(thinkpiece(x, y), b, v, depth - 3))
                        {
                            map_wzq[x][y] = 0;
                            if(isme)
                                maxpiece = thinkpiece(x,y);
                            return true;
                        }
                    }
                }
                else if(v >= 5000)
                {
                    del = delscore(b);
                    if(v - del >= -10)
                    {
#ifdef DEBUG
                        fprintf(fp, "test 5000\n");
                        for(int i = 0; i < 15; i ++)
                        {
                            for(int j = 0; j < 15; j ++)
                            {
                                fprintf(fp,"%d ", map_wzq[j][i]);
                            }
                            fprintf(fp, "\n");
                        }
#endif // DEBUG
                        if(!protect(thinkpiece(x, y), b, v, depth - 1))
                        {
                            map_wzq[x][y] = 0;
                            if(isme)
                                maxpiece = thinkpiece(x,y);
                            return true;
                        }
                    }
                }
                map_wzq[x][y] = 0;
            }
        }
    }
    return false;
}

int delscore(int c)
{
    queue<thinkpiece> add;
    thinkpiece piece;
    int vmax=-9999999, v;
    for(int y=0; y<15; y++)
        for(int x=0; x<15; x++)
            if(!map_wzq[x][y]&&piececlosed(x,y))
            {
                piece.x=x;
                piece.y=y;
                add.push(piece);
            }
    while(!add.empty())
    {
        map_wzq[add.front().x][add.front().y]=c;
        v=countscore(add.front(),c);
        if(v > vmax)
        {
            vmax = v;
        }
        map_wzq[add.front().x][add.front().y]=0;
        add.pop();
    }
    return vmax;
}

void AIthinking(int c)
{
    if(chess.empty())
    {
        putchesspieces(7,7,c);
        return ;
    }
    else if(chess.size() == 1 && !(chess.top()->x ==7 && chess.top()->y == 7))
    {
        if(chess.top()->x>7)
        {
            putchesspieces(chess.top()->x-1,chess.top()->y,c);
        }
        else
        {
            putchesspieces(chess.top()->x+1,chess.top()->y,c);
        }
        return ;
    }
    int b;
    if(c==1) b=2;
    else b=1;
    thinkpiece piece, vmaxpiece;
    piece.x = chess.top()->x;
    piece.y = chess.top()->y;
    isme = 1;
    vct = 1;
    pdf = 6000;
#ifdef DEBUG
    fp = fopen("1.txt", "w");
#endif // DEBUG
    if(vctf(piece, 14, c, 11))
    {
#ifdef DEBUG
        puts("win");
#endif // DEBUG
        putchesspieces(maxpiece.x,maxpiece.y,c);
        return ;
#ifdef DEBUG
        fclose(fp);
#endif // DEBUG
    }
#ifdef DEBUG
    fclose(fp);
    puts("test1");
#endif // DEBUG
    isme = 0;
    vct = 0;
    pdf = 6000;
    queue<thinkpiece> add;
    int vmax=-9999999, vvmax = -9999999,v;
    for(int y=0; y<15; y++)
        for(int x=0; x<15; x++)
            if(!map_wzq[x][y]&&piececlosed(x,y))
            {
                piece.x=x;
                piece.y=y;
                add.push(piece);
            }
    while(!add.empty())
    {
        map_wzq[add.front().x][add.front().y]=c;
        v=countscore(add.front(),c);
        int del = delscore(b);
        v -= del;
        if(v >= vvmax)
        {
            if(v >= -10)
            {
#ifdef DEBUG
                fp = fopen("23.txt", "a");
#endif // DEBUG
                vct = 1;
                if(!vctf(add.front(), 14, b, 11))
                {
#ifdef DEBUG
                    puts("ok2");
#endif // DEBUG
                    if(v > vvmax)
                    {
                        vvmax = v;
                        vmaxpiece = add.front();
                    }
                    if(v >= vvmax - 1)
                    {
                        if(rand() & 1)
                        {
                            vmaxpiece = add.front();
                        }
                    }
                }
                vct = 0;
#ifdef DEBUG
                fclose(fp);
#endif // DEBUG
            }
            else
            {
                if(v > vmax){
                    vmax = v;
                    maxpiece = add.front();
                }
                if(v >= vmax - 1){
                    if(rand() & 1)
                    {
                        maxpiece = add.front();
                    }
                }
            }
        }
        if(vvmax != -9999999)
        {
            maxpiece = vmaxpiece;
        }
//        printf("[%d %d] +%d -%d =%d\n", add.front().x, add.front().y, addv, del, v);
        map_wzq[add.front().x][add.front().y]=0;
        add.pop();
    }
    putchesspieces(maxpiece.x,maxpiece.y,c);
    return ;
}
