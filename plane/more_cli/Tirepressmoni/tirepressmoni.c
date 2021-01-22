#include"common.h"
#include"tirepressmoni.h"

struct pack_head
{
  short len;//追加的数据包长度
  short type;//数据包类型
  int ver;//版本
  char data[0];//追加的地址
};


struct equipment_t
{
  int id;
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
  struct pack_head *p;
  struct equipment_t *equ;
  memset(equ,0x00,sizeof(struct equipment_t));
  equ->id=300;

  p=pack_Make(3000,sizeof(struct equipment_t),1,(void *)equ);
  int ret=write(fd,p,sizeof(struct equipment_t)+8);
  if(ret<0)
  {
	perror("error");
	return -1;
  }
  else printf("write ret : %d\n",ret);

}
