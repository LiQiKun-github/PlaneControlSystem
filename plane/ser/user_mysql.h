#ifndef __USER_MYSQL_H__
#define __USER_MYSQL_H__

int user_login_check(int id,char *password);
void mysql_Start();

void mysql_Go(char *query);
int user_login_check(int id,char *password);
void print_res();
void close_my_mysql();




#endif
