#include "graphics.h"
#include "art.h"
#include "mystruct.h"
#include "AI.h"
#include "user.h"
#include <stack>

extern int map_wzq[15][15];
extern Status userstatus;
extern char user[20];
extern std::stack<chesspiece*> chess;
int cc;

bool is_win(int x,int y,int c)
{
    int next[8][2]= {1,0,-1,0,0,1,0,-1,1,1,-1,-1,1,-1,-1,1};
    int tx,ty,sum;
    for(int i=0; i<4; i++)
    {
        sum=0;
        for(int j=0; j<2; j++)
        {
            tx=x;
            ty=y;
            while(1)
            {
                tx+=next[i*2+j][0];
                ty+=next[i*2+j][1];
                if(tx<0||ty<0||tx>=15||ty>=15) break;
                if(map_wzq[tx][ty]==c)
                    sum+=1;
                else
                    break;
            }
        }
        if(sum>=4) return true;
    }
    return false;
}

int start()
{
    ege::mouse_msg msg= {0};
    while(1)
    {
        while (!msg.is_down())
            msg = ege::getmouse();
        while (!msg.is_up())
            msg = ege::getmouse();
        if(msg.is_left())
        {
            if(msg.x>=780&&msg.y>=37&&msg.x<=890&&msg.y<=81)
            {
                PIMAGE newgame=ege::newimage(110,151);
                ege::getimage(newgame,780,37,890,188);
                PIMAGE mode=ege::newimage(109,150);
                ege::getimage(mode,"mode.jpg",109,150);
                ege::putimage(780,37,mode);
                while(1)
                {
                    while (!msg.is_down())
                        msg = ege::getmouse();
                    while (!msg.is_up())
                        msg = ege::getmouse();
                    if(msg.x>=780&&msg.y>=91&&msg.x<=889&&msg.y<=134)
                    {
                        ege::putimage(780,37,newgame);
                        PIMAGE first=ege::newimage(109,150);
                        ege::getimage(first,"first.jpg",109,150);
                        ege::putimage(780,37,first);
                        ege::delimage(first);
                        while(1)
                        {
                            while (!msg.is_down())
                                msg = ege::getmouse();
                            while (!msg.is_up())
                                msg = ege::getmouse();
                            if(msg.x>=780&&msg.y>=37&&msg.x<=889&&msg.y<=80)
                            {
                                ege::putimage(780,37,newgame);
                                ege::delimage(newgame);
                                ege::delimage(mode);
                                return 1;
                            }
                            else if(msg.x>=780&&msg.y>=91&&msg.x<=889&&msg.y<=134)
                            {
                                ege::putimage(780,37,newgame);
                                ege::delimage(newgame);
                                ege::delimage(mode);
                                return 2;
                            }
                            else if(msg.x>=780&&msg.y>=144&&msg.x<=889&&msg.y<=187)
                            {
                                ege::putimage(780,37,mode);
                                break;
                            }
                            else if(msg.x>=780&&msg.y>=691&&msg.x<=890&&msg.y<=735)
                                return 0;
                        }
                    }
                    else if(msg.x>=780&&msg.y>=37&&msg.x<=889&&msg.y<=80)
                    {
                        ege::putimage(780,37,newgame);
                        ege::delimage(newgame);
                        ege::delimage(mode);
                        return 3;
                    }
                    else if(msg.x>=780&&msg.y>=144&&msg.x<=889&&msg.y<=187)
                    {
                        ege::putimage(780,37,newgame);
                        ege::delimage(newgame);
                        ege::delimage(mode);
                        return 4;
                    }
                    else if(msg.x>=780&&msg.y>=691&&msg.x<=890&&msg.y<=735)
                        return 0;
                }
            }
            else if(msg.x>=780&&msg.y>=691&&msg.x<=890&&msg.y<=735)
                return 0;
        }
    }
}

bool pvp(int c)
{
    cc = 3;
    ege::mouse_msg msg= {0};
    ege::setcolor(LIGHTBLUE);
    ege::setfont(20, 0, "宋体");
    ege::setbkmode(TRANSPARENT);
    ege::outtextxy(780,240,"轮到：");
    while(1)
    {
        if(c==1)
        {
            ege::setcolor(EGERGB(0xE2, 0x7B, 0x1C));
            ege::setfillcolor(EGERGB(0xE2, 0x7B, 0x1C));
            bar(840,230,890,260);
            ege::setcolor(BLACK);
            ege::setfont(30, 0, "楷体");
            ege::outtextxy(840,230,"黑");
        }
        else
        {
            ege::setcolor(EGERGB(0xE2, 0x7B, 0x1C));
            ege::setfillcolor(EGERGB(0xE2, 0x7B, 0x1C));
            bar(840,230,890,260);
            ege::setcolor(WHITE);
            ege::setfont(30, 0, "楷体");
            ege::outtextxy(840,230,"白");
        }
        while (!msg.is_down())
            msg = ege::getmouse();
        while (!msg.is_up())
            msg = ege::getmouse();
        if(msg.x>=15&&msg.y>=16&&msg.x<=765&&msg.y<=766)
        {
            if(putchesspieces((msg.x-15)/50,(msg.y-16)/50,c))
            {
                if(is_win((msg.x-15)/50,(msg.y-16)/50,c))
                {
                    if(c==1)
                    {
                        ege::setcolor(DARKGRAY);
                        ege::setfont(60, 0, "楷体");
                        ege::setbkmode(TRANSPARENT);
                        ege::outtextxy(312, 311, "黑方胜!");
                    }
                    else
                    {
                        ege::setcolor(LIGHTCYAN);
                        ege::setfont(60, 0, "楷体");
                        ege::setbkmode(TRANSPARENT);
                        ege::outtextxy(312, 311, "白方胜!");
                    }
                    return false;
                }
                else if(chess.size()==225)
                {
                    ege::setcolor(LIGHTGREEN);
                    ege::setfont(60, 0, "楷体");
                    ege::setbkmode(TRANSPARENT);
                    ege::outtextxy(312, 311, "平局!");
                    return false;
                }
                if(c==1) c=2;
                else c=1;
            }
        }
        else if(msg.x>=780&&msg.y>=565&&msg.x<=890&&msg.y<=609)
        {
            backchesspieces();
            if(c==1) c=2;
            else c=1;
        }
        else if(msg.x>=780&&msg.y>=37&&msg.x<=890&&msg.y<=81)
        {
            reload();
            return false;
        }
        else if(msg.x>=780&&msg.y>=691&&msg.x<=890&&msg.y<=735)
            logout(user);
    }
}

bool pve(int c)
{
    cc = 2;
    ege::mouse_msg msg= {0};
    ege::setcolor(LIGHTBLUE);
    ege::setfont(20, 0, "宋体");
    ege::setbkmode(TRANSPARENT);
    ege::outtextxy(780,240,"轮到：");
    while(1)
    {
        if(!chess.empty())
        {
            if(is_win(chess.top()->x,chess.top()->y,c))
            {
                if(c==1)
                {
                    ege::setcolor(DARKGRAY);
                    ege::setfont(60, 0, "楷体");
                    ege::setbkmode(TRANSPARENT);
                    ege::outtextxy(312, 311, "黑方胜!");
                    userstatus.win += 1;
                }
                else
                {
                    ege::setcolor(LIGHTCYAN);
                    ege::setfont(60, 0, "楷体");
                    ege::setbkmode(TRANSPARENT);
                    ege::outtextxy(312, 311, "白方胜!");
                    userstatus.lose += 1;
                }
                return false;
            }
            else if(chess.size()==225)
            {
                ege::setcolor(LIGHTGREEN);
                ege::setfont(60, 0, "楷体");
                ege::setbkmode(TRANSPARENT);
                ege::outtextxy(312, 311, "平局!");
                return false;
            }
            if(c==1) c=2;
            else c=1;
        }
        if(c==1)
        {
            ege::setcolor(EGERGB(0xE2, 0x7B, 0x1C));
            ege::setfillcolor(EGERGB(0xE2, 0x7B, 0x1C));
            bar(840,230,890,260);
            ege::setcolor(BLACK);
            ege::setfont(30, 0, "楷体");
            ege::outtextxy(840,230,"黑");
        }
        else
        {
            ege::setcolor(EGERGB(0xE2, 0x7B, 0x1C));
            ege::setfillcolor(EGERGB(0xE2, 0x7B, 0x1C));
            bar(840,230,890,260);
            ege::setcolor(WHITE);
            ege::setfont(30, 0, "楷体");
            ege::outtextxy(840,230,"白");
        }
        if(c==2)
        {
            //AI2(c,0,3,3);
            //AI2(c,0,2,2);
            AIthinking(c);
            continue;
        }
        while (!msg.is_down())
            msg = ege::getmouse();
        while (!msg.is_up())
            msg = ege::getmouse();
        if(msg.x>=15&&msg.y>=16&&msg.x<=765&&msg.y<=766)
        {
            if(!putchesspieces((msg.x-15)/50,(msg.y-16)/50,c))
                c=2;
        }
        else if(msg.x>=780&&msg.y>=565&&msg.x<=890&&msg.y<=609)
        {
            if(chess.size()>1)
                for(int i=0; i<2; i++)
                    backchesspieces();
            if(c==1) c=2;
            else c=1;
        }
        else if(msg.x>=780&&msg.y>=37&&msg.x<=890&&msg.y<=81)
        {
            userstatus.lose += 1;
            reload();
            return false;
        }
        else if(msg.x>=780&&msg.y>=691&&msg.x<=890&&msg.y<=735)
            logout(user);
        else
        {
            if(c==1) c=2;
            else c=1;
        }
    }
}


bool evp(int c)
{
    cc = 1;
    ege::mouse_msg msg= {0};
    ege::setcolor(LIGHTBLUE);
    ege::setfont(20, 0, "宋体");
    ege::setbkmode(TRANSPARENT);
    ege::outtextxy(780,240,"轮到：");
    while(1)
    {
        if(!chess.empty())
        {
            if(is_win(chess.top()->x,chess.top()->y,c))
            {
                if(c==1)
                {
                    ege::setcolor(DARKGRAY);
                    ege::setfont(60, 0, "楷体");
                    ege::setbkmode(TRANSPARENT);
                    ege::outtextxy(312, 311, "黑方胜!");
                    userstatus.lose += 1;
                }
                else
                {
                    ege::setcolor(LIGHTCYAN);
                    ege::setfont(60, 0, "楷体");
                    ege::setbkmode(TRANSPARENT);
                    ege::outtextxy(312, 311, "白方胜!");
                    userstatus.win += 1;
                }
                return false;
            }
            else if(chess.size()==225)
            {
                ege::setcolor(LIGHTGREEN);
                ege::setfont(60, 0, "楷体");
                ege::setbkmode(TRANSPARENT);
                ege::outtextxy(312, 311, "平局!");
                return false;
            }
            if(c==1) c=2;
            else c=1;
        }
        if(c==1)
        {
            ege::setcolor(EGERGB(0xE2, 0x7B, 0x1C));
            ege::setfillcolor(EGERGB(0xE2, 0x7B, 0x1C));
            bar(840,230,890,260);
            ege::setcolor(BLACK);
            ege::setfont(30, 0, "楷体");
            ege::outtextxy(840,230,"黑");
        }
        else
        {
            ege::setcolor(EGERGB(0xE2, 0x7B, 0x1C));
            ege::setfillcolor(EGERGB(0xE2, 0x7B, 0x1C));
            bar(840,230,890,260);
            ege::setcolor(WHITE);
            ege::setfont(30, 0, "楷体");
            ege::outtextxy(840,230,"白");
        }
        if(c==1)
        {
            AIthinking(c);
            //AI2(c,0,2,2);
            //AI2(c,0,3,3);
            continue;
        }
        while (!msg.is_down())
            msg = ege::getmouse();
        while (!msg.is_up())
            msg = ege::getmouse();
        if(msg.x>=15&&msg.y>=16&&msg.x<=765&&msg.y<=766)
        {
            if(!putchesspieces((msg.x-15)/50,(msg.y-16)/50,c))
                c=1;
        }
        else if(msg.x>=780&&msg.y>=565&&msg.x<=890&&msg.y<=609)
        {
            if(chess.size()>1)
                for(int i=0; i<2; i++)
                    backchesspieces();
            if(c==1) c=2;
            else c=1;
        }
        else if(msg.x>=780&&msg.y>=37&&msg.x<=890&&msg.y<=81)
        {
            userstatus.lose += 1;
            reload();
            return false;
        }
        else if(msg.x>=780&&msg.y>=691&&msg.x<=890&&msg.y<=735)
            logout(user);
        else
        {
            if(c==1) c=2;
            else c=1;
        }
    }
}

bool eve(int c)
{
    cc = 4;
    ege::mouse_msg msg= {0};
    ege::setcolor(LIGHTBLUE);
    ege::setfont(20, 0, "宋体");
    ege::setbkmode(TRANSPARENT);
    ege::outtextxy(780,240,"轮到：");
    while(1)
    {
        if(!chess.empty())
        {
            if(is_win(chess.top()->x,chess.top()->y,c))
            {
                if(c==1)
                {
                    ege::setcolor(DARKGRAY);
                    ege::setfont(60, 0, "楷体");
                    ege::setbkmode(TRANSPARENT);
                    ege::outtextxy(312, 311, "黑方胜!");
                }
                else
                {
                    ege::setcolor(LIGHTCYAN);
                    ege::setfont(60, 0, "楷体");
                    ege::setbkmode(TRANSPARENT);
                    ege::outtextxy(312, 311, "白方胜!");
                }
                return false;
            }
            else if(chess.size()==225)
            {
                ege::setcolor(LIGHTGREEN);
                ege::setfont(60, 0, "楷体");
                ege::setbkmode(TRANSPARENT);
                ege::outtextxy(312, 311, "平局!");
                return false;
            }
            if(c==1) c=2;
            else c=1;
        }
        if(c==1)
        {
            ege::setcolor(EGERGB(0xE2, 0x7B, 0x1C));
            ege::setfillcolor(EGERGB(0xE2, 0x7B, 0x1C));
            bar(840,230,890,260);
            ege::setcolor(BLACK);
            ege::setfont(30, 0, "楷体");
            ege::outtextxy(840,230,"黑");
        }
        else
        {
            ege::setcolor(EGERGB(0xE2, 0x7B, 0x1C));
            ege::setfillcolor(EGERGB(0xE2, 0x7B, 0x1C));
            bar(840,230,890,260);
            ege::setcolor(WHITE);
            ege::setfont(30, 0, "楷体");
            ege::outtextxy(840,230,"白");
        }
        while (!msg.is_down())
            msg = ege::getmouse();
        while (!msg.is_up())
            msg = ege::getmouse();
        if(msg.x>=15&&msg.y>=16&&msg.x<=765&&msg.y<=766)
        {
//      AIthinking(c);
            if(c==2) AIthinking(c);
            else AIthinking(c);
        }
        else if(msg.x>=780&&msg.y>=565&&msg.x<=890&&msg.y<=609)
        {
            if(chess.size()>1)
                for(int i=0; i<2; i++)
                {
                    backchesspieces();
                }
            if(c==1) c=2;
            else c=1;
        }
        else if(msg.x>=780&&msg.y>=37&&msg.x<=890&&msg.y<=81)
        {
            reload();
            return false;
        }
        else if(msg.x>=780&&msg.y>=691&&msg.x<=890&&msg.y<=735)
            logout(user);
        else
        {
            if(c==1) c=2;
            else c=1;
        }
    }
}
