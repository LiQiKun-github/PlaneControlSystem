#include"common.h"
#include"cli_socket.h"
#include"menu.h"
#include"pc.h"
#include"user.h"
#include"pack.h"

static int fd;





int start_Experiment_To_Ser(int user_id,char *filename)
{
  
  int ret;
  struct pack_head *pph;
  struct experiment_t exp;
  exp.id=user_id;
  exp.start_flag=1;
  strcpy(exp.filename,filename);
printf("%s\n",exp.filename);
  pph=pack_Make(PACK_TYPE_START_EXPERIMENT,sizeof(struct experiment_t),PACK_VER_1,(void *)&exp);
  ret=write(fd,pph,PACK_HEAD_LEN+sizeof(struct experiment_t));
  if(ret<0)
  {
	perror("start experiment write");
	return -1;
  }
  return 1;
  
}








//信号13
void tcp_broken(int sig)
{
  printf("tcp broken\n");
  return ;
}



static int write_User(int fd,struct user *u,int type)
{
  int ret;
  struct pack_head *pph;
  pph=pack_Make(type,sizeof(struct user),PACK_VER_1,(void *)u);
  ret = write(fd,pph,PACK_HEAD_LEN+sizeof(struct user));
  if(ret < 0)
  {
    perror("write user");
    return -1;
  }
  return ret;
}


int show_All_To_Ser(struct user *u)
{
  int ret;
  ret=write_User(fd,u,SHOW_ALL_USER_TYPE);
  if(ret<0) return 1;
  struct pack_head ph;
  struct user uu;
  int flag=1;
  while(flag)
  {
    ret=read(fd,&ph,PACK_HEAD_LEN);
    if(ret<0)
    {
	  perror("show all to ser read head");
	  return -1;
    }
	if(ret>0&&ph.type==SHOW_ALL_USER_BACK_TYPE)
	{
	  ret=read(fd,&uu,sizeof(struct user));
	  if(ret<0)
	  {
		perror("show all user read data");
		continue;
	  }
	  if(uu.id>0)
	  {
		printf("id : %-10d",uu.id);
		printf("name : %-10s",uu.name);
		printf("password : %-10s\n",uu.password);
	  }
	  else flag=0;
	}
  }
  return 1;
}

int change_User_To_Ser(struct user *u)
{
  int ret=write_User(fd,u,CHANGE_USER_TYPE);
  printf("change write ret: %d\n",ret);
  if(ret>0) return 1;
  else return -1;
}

//删除单个用户
//u: 要删除的用户信息（id）
int del_User_To_Ser(struct user *u)
{
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
	  u->id=-1;
	  return 0;
	}
	u->id=back_u.id;
	strcpy(u->name,back_u.name);
	strcpy(u->password,back_u.password);
  }
  return 1;
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

