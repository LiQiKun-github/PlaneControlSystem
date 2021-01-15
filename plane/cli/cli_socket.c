#include"common.h"
#include"cli_socket.h"
#include"menu.h"
#include"pc.h"
#include"user.h"
#include"pack.h"


static int fd;



void tcp_broken(int sig)
{
  printf("tcp broken\n");
  return ;
}
   

//用户身份验证
//id    用户id
//passowrd  用户密码
int user_Identity_Test(struct user *u)
{
  int ret;
  struct pack_head *p;
  struct pack_head ph;
  struct login_answer_t lat;
  p=pack_Make(USER_TYPE,sizeof(struct user),USER_VER,(void *)u);
  ret = write(fd,p,sizeof(struct user)+PACK_HEAD_LEN);
  if(ret < 0)
  {
    perror("write");
    return -1;
  }
  free(p);
  ret = read(fd,&ph,PACK_HEAD_LEN);
  if(ret>0&&ph.type==25)
  {
    ret = read(fd,&lat,sizeof(struct login_answer_t));
    if(ret>0)
    {
      if(lat.flag ==1)
      {
        printf("恭喜你登陆成功！！！\n");
        return 1;
      }
      else if(lat.flag==0)
      {
        printf("登陆失败（用户名或密码错误）！！！\n");
        return 0;
      }
      else
      {
        printf("输入次数达到三次\n");
        return -1;
      }
    }
  }
  return 0;
}




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





void cli_start()
{
  signal(13,tcp_broken);
  fd=socket_Tcp_Init(PORT,IP);
}

