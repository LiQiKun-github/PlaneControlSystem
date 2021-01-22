#ifndef __USER_MYSQL_H__
#define __USER_MYSQL_H__

#include"pack.h"

int user_login_check(int id,char *password);
void mysql_Start();

void mysql_Go(char *query);
int user_login_check(int id,char *password);
void print_res();
void close_my_mysql();
int add_User_Mysql(struct user *u);
int show_One_User_Mysql(struct user *u,struct user *bakc_u);
int del_User_Mysql(struct user *u);
int change_User_Mysql(struct user *u);
int show_All_User_Mysql(struct user *u);
int string_to_int(char *n);
int experiment_Mysql(char *expname,int delflag,char *starttime,int userid);


#endif
