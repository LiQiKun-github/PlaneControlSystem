#ifndef __CLI_SOCKET_H__
#define __CLI_SOCKET_H__

#include"user.h"

void tcp_broken(int sig);
int user_Identity_Test(struct user *u);
int socket_Tcp_Init(short port,char *strip);
void cli_start();







#endif
