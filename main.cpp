#include "graphics.h"
#include "art.h"
#include "mystruct.h"
#include "rule.h"
#include "user.h"
#include<stack>
#define computer1 1
#define player1 2
#define playervsplayer 3
#define computervscomputer 4

int map_wzq[15][15];
char user[20];
extern int nowc;
Status userstatus;
std::stack<chesspiece*> chess;

int main() {
    int zt,savemode;
    ege::setinitmode(INIT_RENDERMANUAL|INIT_NOFORCEEXIT);
    ege::initgraph(900,780);
    do {
        strcpy(user,login());
    } while(strlen(user) == 0);
    userstatus = status(user);
    reload();
    savemode = load(user);
    for (; ege::is_run(); ege::delay_fps(60)) {
        if(savemode != 0)
            zt = savemode;
        else {
            zt=start();
            reload();
        }
        if(zt==computer1) {
            if(savemode != 0)
                evp(nowc == 2?1:2);
            else
                evp(1);
        } else if(zt==player1) {
            if(savemode != 0)
                pve(nowc == 2?1:2);
            else
                pve(1);
        } else if(zt==playervsplayer) {
            if(savemode != 0)
                pvp(nowc);
            else
                pvp(1);
        } else if(zt==computervscomputer) {
            if(savemode != 0)
                eve(nowc == 2?1:2);
            else
                eve(1);
        } else if(!zt)
            logout(user);
        if(savemode != 0)
            savemode = 0;
    }
    while(!chess.empty())
        backchesspieces();
    ege::cleardevice();
    ege::closegraph();
    return 0;
}
