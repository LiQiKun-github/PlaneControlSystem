#include"common.h"
#include"admin.h"
#include"user.h"
#include"cli_socket.h"


void change_User()
{
  char sel;
  struct user *u=(struct user *)malloc(sizeof(struct user));
  printf("请输入你要修改的用户id:");
  scanf("%d",&(u->id));
  while(getchar()!='\n');
  printf("你要修改的内容如下：\n");
  int flag=show_One_User_To_Ser(u);
  if(flag>0)
  {
	printf("id : %d\n",u->id);
    if(strlen(u->name)!=0)printf("name : %s\n",u->name);
    else printf("name:用户名为空\n");
    printf("password : %s\n",u->password);
  }
  else if(flag==0) printf("别闹孩子，没有这个人\n");
  else printf("查询出错...\n");
  while(1)
  {
	printf("%s",CHANGEMENU);
	scanf("%c",&sel);
	while(getchar()!='\n');
	switch(sel)
	{
	  case '1':printf("\n请输入你要更改后的用户名:"); scanf("%s",u->name); while(getchar()!='\n');break;
	  case '2':printf("\n请输入你要更改后的密码:"); scanf("%s",u->password); while(getchar()!='\n');break;
	  case '3':goto end;
	}
  }

end:
  change_User_To_Ser(u);
  free(u);
}


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
//注意：chang_User调用了这个函数
void show_One_User()
{
  struct user *u=(struct user *)malloc(sizeof(struct user));
  memset(u,0x00,sizeof(struct user));
  printf("请输入你要查看用户的id:");
  scanf("%d",&(u->id));
  while(getchar()!='\n');
  int flag=show_One_User_To_Ser(u);
  if(flag>0)
  {
    printf("id : %d\n",u->id);                                                                                                                                                                             
    if(strlen(u->name)!=0)printf("name : %s\n",u->name);
    else printf("name:用户名为空\n");
    printf("password : %s\n",u->password);
  }
  else if(flag==0) printf("别闹孩子，没有这个人\n");
  else printf("查询出错...\n"); 
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




//显示所有用户
void show_All_User()
{
printf("sjow");
  struct user *u=(struct user *)malloc(sizeof(struct user));
  memset(u,0x00,sizeof(struct user));
  show_All_To_Ser(u);
  free(u);
}
