#include "graphics.h"
#include "user.h"
#include "mystruct.h"
#include "art.h"
#include<cstdio>
#include<string>
#include<stack>

extern Status userstatus;
extern std::stack<chesspiece*> chess;
extern int cc;
int nowc;

char *login()
{
    char user[20];
    FILE *fp;
    fp = fopen("account.txt", "r");
    if(fp == NULL)
    {
        do
        {
            strcpy(user,reg());
        }while(strlen(user) == 0);
        return user;
    }
    char mode;
    char password[20];
    int ok;
    account b;
    setfont(30, 0, "宋体");
	for (;ege::is_run();ege::delay_fps(60))
    {
        ege::outtextrect(300, 300, 600, 90, "是否拥有账号？\n按键选择\n是(y)/否(n)");
        mode = getch();
        if(mode == 'Y' || mode == 'y')
        {
            ege::inputbox_getline("登陆","输入账号(回车确认)", user, 20);
            while(1)
            {
                ok = 0;
                ege::inputbox_getline("登陆","输入密码(回车确认)", password, 20);
                while(~fscanf(fp,"%s%s",b.user,b.password))
                {
                    if(!strcmp(b.user,user))
                    {
                        ok = 1;
                        break;
                    }
                }
                if(!ok)
                {
                    ege::outtextrect(300, 400, 600, 30, "账号不存在");
                    strcpy(user,"");
                    return user;
                }
                else
                {
                    if(!strcmp(b.password,password))
                    {
                        fclose(fp);
                        return user;
                    }
                    else
                    {
                        ege::outtextrect(300, 400, 600, 30, "密码错误");
                        strcpy(user,"");
                        return user;
                    }
                }
            }
        }
        else if(mode == 'N' || mode == 'n')
        {
            do
            {
                strcpy(user,reg());
            }while(strlen(user) == 0);
            fclose(fp);
            return user;
        }
    }
}

char *reg()
{
    FILE *fp;
    account b;
    fp = fopen("account.txt", "a+");
    char user[20];
    char password[20];
    char confim[20];
    int ok;
    setfont(30, 0, "宋体");
    while(1)
    {
        ok = 1;
        ege::inputbox_getline("注册","输入注册的账号(回车确认)", user, 20);
        fseek(fp,0,SEEK_SET);
        while(~fscanf(fp,"%s%s",b.user,b.password))
        {
            if(!strcmp(b.user,user))
            {
                ok = 0;
                break;
            }
        }
        if(ok)
            break;
        else
        {
            ege::outtextrect(300, 700, 600, 30, "账号已被注册");
            strcpy(user,"");
            return user;
        }
    }
    while(1)
    {
        ege::inputbox_getline("注册","输入注册的密码(回车确认)", password, 20);
        ege::inputbox_getline("注册","再次确认密码(回车确认)", confim, 20);
        if(!strcmp(password,confim))
            break;
        else
            ege::outtextrect(300, 600, 600, 30, "前后密码不一致，请重新输入");
    }
    fprintf(fp,"%s %s\n", user, password);
    fclose(fp);
    fp = fopen("status.txt","a+");
    fprintf(fp,"%s 0 0\n", user);
    fclose(fp);
    return user;
}

Status status(char *user)
{
    Status temp;
    FILE *fp;
    char tempuser[20];
    fp = fopen("status.txt","r");
    while(~fscanf(fp, "%s%d%d", tempuser, &temp.win, &temp.lose))
    {
        if(!strcmp(tempuser, user))
            break;
    }
    fclose(fp);
    return temp;
}

void logout(char *user)
{
    FILE *fp, *tempfp;
    Status temp;
    char tempuser[20];
    fp = fopen("status.txt","r");
    tempfp = fopen("temp.txt","w+");
    while(~fscanf(fp, "%s%d%d", tempuser, &temp.win, &temp.lose))
    {
        if(!strcmp(tempuser, user))
            fprintf(tempfp, "%s %d %d\n", tempuser, userstatus.win, userstatus.lose);
        else
            fprintf(tempfp, "%s %d %d\n", tempuser, temp.win, temp.lose);
    }
        fclose(fp);
        fclose(tempfp);
        remove("status.txt");
        rename("temp.txt","status.txt");
        if(!chess.empty())
        {
        std::stack<chesspiece*> tempa;
        while(!chess.empty())
        {
            tempa.push(chess.top());
            chess.pop();
        }
        char save[100];
        sprintf(save,".\\save\\%s.txt",user);
        fp = fopen(save, "w+");
        fprintf(fp,"%d\n", cc);
        while(!tempa.empty())
        {
            fprintf(fp,"%d %d\n",tempa.top()->x ,tempa.top()->y);
            free(tempa.top());
            tempa.pop();
        }
        fclose(fp);
    }
    exit(0);
}

int load(char *user)
{
    FILE *fp;
    char save[100];
    int c = 1;
    sprintf(save,".\\save\\%s.txt",user);
    fp = fopen(save, "r");
    if(fp == NULL)
        return 0;
    fscanf(fp,"%d", &cc);
    int x,y;
    while(~fscanf(fp,"%d %d",&x,&y))
    {
        putchesspieces(x,y,c);
        c = (c==2 ? 1 : 2);
    }
    fclose(fp);
    nowc = c;
    return cc;
}
