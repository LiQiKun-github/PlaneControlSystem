#include"common.h"
#include"cli_socket.h"
#include"menu.h"
#include"pc.h"
#include"user.h"
#include"pack.h"


static int fd;


//信号13
void tcp_broken(int sig)
{
  printf("tcp broken\n");
  return ;
}


//删除单个用户
//u: 要删除的用户信息（id）
int del_User_To_Ser(struct user *u)
{
  printf("#################del user start\n");
  int ret;
  struct pack_head *p;

  p=pack_Make(DEL_USER_TYPE,sizeof(struct user),PACK_VER_1,(void *)u);
  ret=write(fd,p,sizeof(struct user)+PACK_HEAD_LEN);
  if(ret<0)
  {
	perror("write");
	return -1;
  }
  free(p);
  //ret=read(read);
  return -1;
}




//显示单个用户信息
//u: 要显示的用户id
//
int show_One_User_To_Ser(struct user *u)
{
  int ret;
  struct pack_head *p;
  struct pack_head ph;
  struct user back_u;
  p=pack_Make(SHOW_ONE_USER_TYPE,sizeof(struct user),PACK_VER_1,(void *)u);
  ret = write(fd,p,sizeof(struct user)+PACK_HEAD_LEN);
  if(ret<0)
  {
	perror("write");
	return -1;
  }

  free(p);
  ret =read(fd,&ph,PACK_HEAD_LEN);
  if(ret<0)
  {
	perror("show one user back user read head");
	return -1;
  }
  if(ret>0&&ph.type==SHOW_ONE_USER_BACK_TYPE)
  {
	ret=read(fd,&back_u,sizeof(struct user));
	if(ret<0) 
	{
	  perror("show one user back read data");
	  return -1;
	}
	if(back_u.id<0)
	{
	  printf("别闹孩子，没有这个人!!!\n");
	  return 0;
	}
	printf("id:%d\n",back_u.id);
	if(strlen(back_u.name)!=0)printf("name:%s\n",back_u.name);
	else printf("name:用户名为空\n");
    printf("password:%s\n",back_u.password);
  }
  return 0;
}



//添加用户信息
//u:用户信息结构体
int add_User_To_Ser(struct user *u)
{
  int ret;
  struct pack_head *p; 
  struct pack_head ph;
  struct answer_t lat;
  p=pack_Make(ADD_USER_TYPE,sizeof(struct user),PACK_VER_1,(void *)u);
  ret = write(fd,p,sizeof(struct user)+PACK_HEAD_LEN);
  if(ret < 0)
  {
    perror("write");
    return -1;
  }
  free(p);
  ret = read(fd,&ph,PACK_HEAD_LEN);
  if(ret < 0)
  {
	perror("add write");
	return -1;
  }
  if(ret>0&&ph.type==PACK_TYPE_LOGIN_ANSWER)
  {
	ret = read(fd,&lat,sizeof(struct answer_t));
	if(ret>0) return lat.flag;
	else perror("answer read");
  }
  return -1;
}
   

//用户身份验证
//id    用户id
//passowrd  用户密码
int user_Identity_Test(struct user *u,int pack_type)
{
  int ret;
  struct pack_head *p;
  struct pack_head ph;
  struct answer_t lat;
  p=pack_Make(pack_type,sizeof(struct user),PACK_VER_1,(void *)u);
  ret = write(fd,p,sizeof(struct user)+PACK_HEAD_LEN);
  if(ret < 0)
  {
    perror("write");
    return -2;
  }
  free(p);
  free(u);
  ret = read(fd,&ph,PACK_HEAD_LEN);
  if(ret>0&&ph.type==PACK_TYPE_LOGIN_ANSWER)
  {
    ret = read(fd,&lat,sizeof(struct answer_t));
    if(ret>0) return lat.flag;
  }
  return -2;
}



//socket tcp 初始化
//port:端口号	strip:ip地址
//return:成功返回socket文件描述符，失败返回-1
int socket_Tcp_Init(short port,char *strip)
{
  int fd;
  struct sockaddr_in myaddr;
  int ret;

  //socket create
  fd = socket(AF_INET,SOCK_STREAM,0);
  if(fd<0)
  {
    perror("socket");
    return -1;
  }

  //connect
  myaddr.sin_family = AF_INET;
  myaddr.sin_port = htons(port);
  myaddr.sin_addr.s_addr = inet_addr(strip);
  ret = connect(fd,(struct sockaddr*)&myaddr,sizeof(myaddr));
  if(ret < 0)
  {
    perror("connect");
    return -1;
  }
  return fd;
}




//客户端tcp连接和信号处理，主要为了给mian函数提供接口
void cli_start()
{
  signal(13,tcp_broken);
  fd=socket_Tcp_Init(PORT,IP);
}

