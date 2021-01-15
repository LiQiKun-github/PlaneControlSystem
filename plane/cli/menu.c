#include"common.h"
#include"menu.h"
#include"pc.h"
#include"user.h"
#include"pack.h"
#include"cli_socket.h"



//管理员登陆
void admin_Login()
{                                                                                                                                                                                                          
}


//用户登陆
void user_Login()
{
  while(1)
  {
  struct user *u=create_User();
  int flag=user_Identity_Test(u);
  if(flag==-1) return ;
  else if(flag==1) return ;
  else continue;
  }
}  




//菜单
void menu()
{
  while(1)
  {
  char sel;
  printf("%s",LOGINMENU);
  scanf("%c",&sel);
  while(getchar()!='\n');
  switch(sel)
  {
    case '1':admin_Login();    //管理员登陆
             break;
    case '2':user_Login();     //用户登陆
             break;
    case '3':return;
  }
  }
}





