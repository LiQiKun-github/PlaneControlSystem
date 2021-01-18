#include"common.h"
#include"admin.h"
#include"user.h"
#include"cli_socket.h"


//管理员添加用户
void add_User()
{
  struct user *u=create_Complete_User();
  int id = add_User_To_Ser(u);
  printf("添加的用户信息如下：\n");
  printf("id : %d\nname : %s\npassword : %s\n",id,u->name,u->password);
  free(u);
}



//管理员查看单个用户
void show_One_User()
{
  struct user *u=(struct user *)malloc(sizeof(struct user));
  memset(u,0x00,sizeof(struct user));
  printf("请输入你要查看用户的id:");
  scanf("%d",&(u->id));
  while(getchar()!='\n');
  show_One_User_To_Ser(u);
  free(u);
}


//删除用户
void del_User()
{
  struct user *u=(struct user *)malloc(sizeof(struct user));
  memset(u,0x00,sizeof(struct user));
  printf("请输入你要删除的用户id:");
  scanf("%d",&(u->id));
  while(getchar()!='\n');
  del_User_To_Ser(u);
  free(u);
}
