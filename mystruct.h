#ifndef MYSTRUCT_H
#define MYSTRUCT_H

struct thinkpiece
{
    int x,y,v;
    bool friend operator < (const thinkpiece &a,const thinkpiece &b)
    {
        if(a.v > b.v)
            return true;
        else
            return false;
    }
    thinkpiece(int x, int y):x(x), y(y) {}
    thinkpiece(int x, int y, int v):x(x), y(y), v() {}
    thinkpiece() {}
};

struct chesspiece
{
    int x, y;
    chesspiece(int x, int y):x(x), y(y) {}
    chesspiece() {}
    PIMAGE _back;
};

struct account
{
    char user[20];
    char password[20];
};

struct Status
{
    int win;
    int lose;
};
#endif
