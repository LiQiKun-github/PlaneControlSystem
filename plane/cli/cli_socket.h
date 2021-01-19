#ifndef __CLI_SOCKET_H__
#define __CLI_SOCKET_H__

#include"user.h"

void tcp_broken(int sig);
int user_Identity_Test(struct user *u,int type);
int socket_Tcp_Init(short port,char *strip);
void cli_start();
int add_User_To_Ser(struct user *u);
int show_One_User_To_Ser(struct user *u);
int del_User_To_Ser(struct user *u);
int change_User_To_Ser(struct user *u);
int show_All_To_Ser(struct user *u);







#endif
