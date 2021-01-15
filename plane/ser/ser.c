#include"common.h"
#include"pack.h"
#include"usermysql.h"
#include"ser.h"


static struct cli_t *head=NULL;
const int id=123;
const char password[32]="123456";


//13信号函数
void tcp_broken(int sig)
{
	printf("tcp broken.\n");
	return;
}


//打印链表
void print_link()
{
  struct cli_t *p=head;
  if(p==NULL) printf("空空如也!!!\n");
  while(p!=NULL)
  {
	printf("fd : %d\n",p->cfd);
	p=p->next;
  }
}

//sockt初始化
//return 成功返回socket描述符	失败返回-1
int socket_Tcp_Init(int port,char *strip)
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

  //bind
  myaddr.sin_family = AF_INET;
  myaddr.sin_port = htons(port);
  myaddr.sin_addr.s_addr = inet_addr(strip);
  ret = bind(fd,(struct sockaddr*)&myaddr,sizeof(myaddr));
  if(ret < 0)
  {
	perror("bind");
	close(fd);
	return -1;
  }

  //listen
  ret = listen(fd,20);
  if(ret < 0)
  {
	perror("lieten");
	close(fd);
	return -1;
  }
  return fd;
}


//在链表中查找某个描述符的节点
//cfd	要查找的文件描述符
//return	找到返回找到的节点，未找到返回NULL
struct cli_t *cli_find(int cfd)
{
  struct cli_t *p=head;
  while(p!=NULL)
  {
	if(p->cfd==cfd)
	  return p;
	p=p->next;
  }
  return NULL;
}



//删除某个节点
//参数：	last：所要删除节点的上一个节点	p：要删除的节点	
void delete_cli_node(struct cli_t *last,struct cli_t *p)
{
  if(last==NULL) head=p->next;
  else last->next=p->next;
  free(p);
}


//查找某个节点的上一个节点
//temp：所要查找节点的地址
//return：返回上一个节点的地址
struct cli_t *last_node(struct cli_t *temp)
{
  struct cli_t *p=head;
  if(NULL==p) return NULL;
  else if(temp==p) return NULL;
  while(p->next!=NULL)
  {
	if(p->next==temp) return p;
	p=p->next;
  }
  return NULL;
}


//关闭链表当中需要关闭的描述符
void cli_close()
{
  struct cli_t *p=head;
  struct cli_t *last;
  while(p!=NULL)
  {
	if(p->close_flag==1)
	{
	  printf("OK\n");
	  close(p->cfd);
	  last=last_node(p);
	  delete_cli_node(last,p);
	}
	p=p->next;
  }
}


int user_Login(struct cli_t *p,int fd)
{ 
  int ret;
  struct user u;
  struct login_answer_t lat;
  struct pack_head *pph;
  ret=read(fd,&u,sizeof(struct user)); 
  if(ret<0)
  {
	perror("read2");
	return -1;
  }
  printf("u->id : %d\n",u.id);
  printf("u->password : %s\n",u.password);
  p->count++;
  printf("count : %d\n",p->count);
  if(u.id==id&&strcmp(u.password,password)==0)
  {
  	lat.flag=LOGIN_SUCCESS_FLAG;
	p->count=0;
  }
  else if(p->count>=3)
  {
	lat.flag=LOGIN_BREAK_FLAG;
  }
  else lat.flag=LOGIN_FAIL_FLAG;
	pph=pack_Make(sizeof(struct login_answer_t),PACK_TYPE_LOGIN_ANSWER,PACK_VER_1,(void *)&lat);
	ret = write(fd,pph,PACK_HEAD_LEN+sizeof(struct login_answer_t));
	if(ret < 0)
	{
	  perror("write");
	  return -1;
	}
	return 0;
}

int cli_recv(struct cli_t *p)
{
  struct pack_head ph;
  int ret;
  int rfd=p->cfd;
  
  ret=read(rfd,&ph,PACK_HEAD_LEN);
  if(ret<0)
  {
	perror("read1");
	p->close_flag=1;
	return -1;
  }
  else if(ret==0)
  {
	p->close_flag=1;
	return -2;
  }
  printf("type : %hd\n",ph.type);
  printf("len : %hd\n",ph.len);
  printf("ver : %d\n",ph.ver);

  if(ph.type==66)
  {
	user_Login(p,rfd);
  }

  return 0;
}


//epoll
int epoll_Go()
{
  int fd,efd,nfd;
  //int ret;
  int num_epoll;//处于准备状态的个数
  struct sockaddr_in caddr;
  struct epoll_event events[20],ev;
  struct cli_t *p=NULL;
  unsigned int addr_len;

  fd = socket_Tcp_Init(PORT,IP);//socket初始化
  if(fd < 0)
  {
	perror("socket");
	return -1;
  }

  efd = epoll_create(20);//epoll创建
  if(efd < 0)
  {
	perror("spoll_create");
	return -1;
  }

  ev.data.fd=fd;
  ev.events=EPOLLIN;

  epoll_ctl(efd,EPOLL_CTL_ADD,fd,&ev);
  while(1)
  {
	printf("epoll wait..\n");
	num_epoll = epoll_wait(efd,events,20,-1);
	printf("num_epoll : %d\n",num_epoll);
	if(num_epoll==0)
	{
	  printf("time over\n");
	  continue;
	}

	//遍历所有已经准备好的文件描述符
	for(int i=0;i<num_epoll;i++)
	{
	  if(events[i].data.fd==fd)
	  {
	  addr_len=sizeof(caddr);
	  printf("accept..\n");
      nfd = accept(fd,(struct sockaddr *)&caddr,&addr_len);//接收
	  if(nfd<0)
	  {
		perror("accept..");
		close(fd);
		return -1;
	  }

	  //添加节点
	  p=(struct cli_t*)malloc(sizeof(struct cli_t));
	  p->cfd=nfd;
	  p->addr=caddr;
	  p->count=0;
	  p->close_flag=0;
	  p->next=head;
	  head=p;

		//add epoll
		ev.data.fd=nfd;
		ev.events=EPOLLIN;
		epoll_ctl(efd,EPOLL_CTL_ADD,nfd,&ev);
	  }
	  else
	  {

		//查找产生事件的文件描述符节点
		p=cli_find(events[i].data.fd);
		if(p!=NULL)
		  cli_recv(p);//处理事件
	  }
	}
	cli_close();//关闭无用的文件描述符
	//print_link();
  }

  return 1;
}




int main()
{
  signal(SIGPIPE,tcp_broken);
  epoll_Go();
}













