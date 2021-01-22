#include"common.h"
#include"envirmoni.h"

struct pack_head
{
  short len;//追加的数据包长度
  short type;//数据包类型
  int ver;//版本
  char data[0];//追加的地址
};

struct order_t
{
  int relative_time;    //相对时间
  int action_id;    //动作ID
  int frequency;    //次数
  int time;         //间隔
  char data[96];    //操作数据
  struct order_t *next;
};


struct equipment_t
{
  int id;
  int fd;
  struct equipment_t *next;
};

//打包数据
//参数 ：类型   数据长度    版本    数据内容
//return    打包后的地址
struct pack_head *pack_Make(short type,int len,int ver,void *data)
{
  struct pack_head *p; 
  p=(struct pack_head *)malloc(8+len);
  p->type=type;
  p->len=len;
  p->ver=ver;
  memcpy(p->data,data,len);
  return p;
}       


//socket tcp 初始化
//port:端口号   strip:ip地址
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


int main()
{
  int fd;
  fd=socket_Tcp_Init(9001,"127.0.0.1");
  if(fd<0) return -1;
  struct pack_head *p;
  struct equipment_t equ;
  struct order_t o;
  equ.id=100;
  p=pack_Make(3000,sizeof(struct equipment_t),1,(void *)&equ);
  int ret=write(fd,p,sizeof(struct equipment_t)+8);
  if(ret<0)
  {
	perror("error");
	return -1;
  }
  else printf("write ret : %d\n",ret);
  while(1)
  {
	ret = read(fd,p,8);
	if(ret<0)
	{
	  perror("read head");
	  continue;
	}
	else if(ret==0)
	{
	  printf("\n");
	  continue;
	}
	ret = read(fd,&o,sizeof(struct order_t));
	if(ret<0)
	{
	  perror("read data");
	}
	if(p->type==1004)
	{
	  printf("equ->action_id : %d\n",o.action_id);
	  printf("equ->data : %s\n",o.data);
	}
  }


  sleep(1000);
}
