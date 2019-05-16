#include "graphics.h"
#include "mystruct.h"
#include "rule.h"
#include <stack>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "AI.h"
#include "art.h"

extern int map_wzq[15][15];
extern std::stack<chesspiece*> chess;


void reload()
{
  while(!chess.empty()) backchesspieces();
  ege::cleardevice();
  for(int i=0;i<15;i++)
    for(int j=0;j<15;j++)
      map_wzq[i][j]=0;
  PIMAGE img=ege::newimage(900,780);
  ege::getimage(img,"map.jpg",900,780);
  ege::putimage(0,0,img);
  ege::delimage(img);
}

bool putchesspieces(int x,int y,int c)
{
  if(map_wzq[x][y]) return false;
  chesspiece *s;
  char str[4];
  map_wzq[x][y]=c;
  s=(chesspiece*)malloc(sizeof(chesspiece));
  s->x=x;
  s->y=y;
  s->_back=ege::newimage(45,45);
  ege::getimage(s->_back,15+x*50,16+y*50,61+x*50,62+y*50);
  chess.push(s);
  PIMAGE img=ege::newimage(45,45);
  if(c==1)
    ege::getimage(img,"black.png",45,45);
  else if(c==2)
    ege::getimage(img,"white.png",45,45);
  ege::putimage(15+x*50,16+y*50,img);
  ege::delimage(img);
  sprintf(str,"%u",chess.size());
  if(c==1) ege::setcolor(LIGHTGREEN);
  else ege::setcolor(LIGHTBLUE);
  ege::setfont(20, 0, "ËÎÌו");
  ege::setbkmode(TRANSPARENT);
  ege::outtextxy(40+x*50-strlen(str)*8,29+y*50, str);
  return true;
}

void backchesspieces()
{
  if(!chess.empty())
  {
    map_wzq[chess.top()->x][chess.top()->y]=0;
    ege::putimage(15+(chess.top()->x)*50,16+(chess.top()->y)*50,(chess.top()->_back));
    ege::delimage(chess.top()->_back);
    free(chess.top());
    chess.pop();
  }
}

void testprint()
{
  ege::setcolor(LIGHTGREEN);
  ege::setfont(20, 0, "ËÎÌו");
  ege::setbkmode(TRANSPARENT);
  char str[2];
  for(int y=0;y<15;y++)
    for(int x=0;x<15;x++)
    {
    sprintf(str,"%d",map_wzq[x][y]);
    ege::outtextxy(40+x*50-strlen(str)*8,29+y*50, str);
    }
}
