#include"common.h"
#include"menu.h"
#include"pc.h"
#include"user.h"
#include"pack.h"
#include"cli_socket.h"
#include"admin.h"
#include"expriment.h"


static int user_id;





//管理员登陆检查
//return: 登陆成功返回1，登陆失败次数过多返回-1；
int admin_Login_Check()
{
  while(1)
  {
	struct user *u=create_User();
	int flag=user_Identity_Test(u,USER_LOGIN_TYPE);//用户信息核对
	if(flag ==1)
	{
	  printf("恭喜你登陆成功！！！\n");
	  return 1;
	}
	else if(flag==0)
	{
	  printf("登陆失败（用户名或密码错误）！！！\n");
	  continue;
	}
	else if(flag==-1)
	{
	  printf("输入次数达到三次\n");
	  return -1;
	}
	else {
	  return -1;
	}
  }
	return -1;
}


//管理员登陆
void admin_Login()
{
  if(admin_Login_Check()==-1) return ;
  while(1)
  {
	char sel;
	printf("%s",ADMINMENU);
	sel = getchar();
	while(getchar() != '\n');
	switch(sel)
	{
	  case '1':add_User(); break;
	  case '2':del_User(); break;
	  case '3':change_User(); break;
	  case '4':show_All_User(); break;
	  case '5':show_One_User(); break;
	  case '6':return;
	}
  }
}



//管理员登陆检查
//return: 登陆成功返回1，登陆失败次数过多返回-1；
static int user_Login_Check()
{
  while(1)
  {
    struct user *u=create_User();
	user_id=u->id;
    int flag=user_Identity_Test(u,USER_LOGIN_TYPE);
	free(u);
    if(flag ==1)
    {
      printf("恭喜你登陆成功！！！\n");
      return 1;
    }
    else if(flag==0)
    {
      printf("登陆失败（用户名或密码错误）！！！\n");
      continue;
    }
    else
    {
      printf("输入次数达到三次\n");
      return -1;
    }
  }
    return -1;

}



//实验案例菜单
static void experiment_Menu()
{
  char sel;
  while(1)
  {
	printf("%s",EXPERIMENTMENU);
	scanf("%c",&sel);
	while(getchar()!='\n');
	switch(sel)
	{
	  case '1':add_Expriment();break;
	  case '2':printf("1\n");break;
	  case '3':printf("1\n");break;
	  case '4':printf("1\n");break;
	  case '5':printf("1\n");break;
	  case '6':start_Experiment(user_id);break;
	  case '7':printf("1\n");return;
	}
  }
}

//用户登陆
static void user_Login()
{
  if(!user_Login_Check())return;
  experiment_Menu();

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





