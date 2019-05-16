#ifndef USER_H
#define USER_H
#include "mystruct.h"
char *login();
char *reg();
Status status(char *user);
void logout(char *user);
int load(char *user);
#endif // USER_H
