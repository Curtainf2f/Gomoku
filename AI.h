#ifndef AI_H
#define AI_H
bool piececlosed(int x,int y);
int countscore(thinkpiece piece,int c);
int analysis(char s[]);
void AIthinking(int c);
int value(int depth, int c);
int Min(int depth,int maxdepth, int c);
int Max(int depth,int maxdepth, int c);
void AI2(int c,int depth);
#endif
