#include"common.h"
#include"pack.h"
#include"ser.h"
#include"cli_link.h"
#include"user_mysql.h"

//13信号函数
void tcp_broken(int sig)
{
	printf("tcp broken.\n");
	return;
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

static int read_User(int fd,struct user *u)
{
  int ret;
  ret=read(fd,u,sizeof(struct user));
  if(ret<0)
  {
	perror("read user");
	return -1;
  }
  return ret;
}


static int write_User(int fd,struct user *u,int type)
{
  int ret;
  struct pack_head *pph;
  pph=pack_Make(sizeof(struct user),type,PACK_VER_1,(void *)u);
  ret = write(fd,pph,PACK_HEAD_LEN+sizeof(struct user));
  if(ret < 0)
  {
  	perror("write user");
  	return -1;
  }
  return ret;
}

//客户端用户登陆
//p:客户端节点
//return:	出错返回-1，成功返回1
int user_Login(struct cli_t *p)
{ 
  int ret;
  struct login_answer_t lat;
  struct pack_head *pph;
  struct user *u=(struct user*)malloc(sizeof(struct user));
  if(!read_User(p->cfd,u))return -1;
  printf("u->id : %d\n",u->id);
  printf("u->password : %s\n",u->password);
  p->count++;
  printf("count : %d\n",p->count);
  int flag=user_login_check(u->id,u->password);
  free(u);
  if(flag==1)
  {
  	lat.flag=LOGIN_SUCCESS_FLAG;
	p->count=0;
  }
  else if(p->count>=3)
  {
	lat.flag=LOGIN_BREAK_FLAG;
	p->count=0;
  }
  else lat.flag=LOGIN_FAIL_FLAG;
	pph=pack_Make(sizeof(struct login_answer_t),PACK_TYPE_LOGIN_ANSWER,PACK_VER_1,(void *)&lat);
	ret = write(p->cfd,pph,PACK_HEAD_LEN+sizeof(struct login_answer_t));
	if(ret < 0)
	{
	  perror("write");
	  return -1;
	}
	return 0;
}

//添加用户
//p: 客户端客户端socket文件描述符socket文件描述符
static void user_Add(struct cli_t *p)
{
  int ret;
  struct user u;
  struct login_answer_t lat;
  struct pack_head *pph;
  ret=read(p->cfd,&u,sizeof(struct user));
  if(ret<0)
  {
    perror("read2");
    return ;
  }
printf("u->id : %d\n",u.id);
printf("u->password : %s\n",u.password);
printf("u->name : %s\n",u.name);
  int id=add_User_Mysql(&u);
printf("id = %d\n",id);
  if(id>0)
  {
printf("add success!!!\n");
	lat.flag=id;
	pph=pack_Make(sizeof(struct login_answer_t),PACK_TYPE_LOGIN_ANSWER,PACK_VER_1,(void *)&lat);
	ret = write(p->cfd,pph,PACK_HEAD_LEN+sizeof(struct login_answer_t));
  }
  else printf("add error!!!\n");
}


//对客户端传来的的显示一个用户信息数据包进行处理
//p:客户端节点
static void  show_One_User(struct cli_t *p)
{
  struct user u;
  struct user *back_u=(struct user *)malloc(sizeof(struct user));
  if(!read_User(p->cfd,&u))return ;//read用户信息
  if(!show_One_User_Mysql(&u,back_u))
  {
	back_u->id=-1;//让客户端知道没有此人
	printf("别闹孩子，没有这个人!!!\n");
  }
  else
  {
printf("###############show one user####################\n");
	printf("id:%d\n",back_u->id);
	if(strlen(back_u->name)!=0)printf("name:%s\n",back_u->name);
	else printf("name:用户名为空\n");
	printf("password:%s\n",back_u->password);
printf("###############show one user####################\n");
  }
  write_User(p->cfd,back_u,PACK_TYPE_SHOW_ONE_USER);
  free(back_u);
}


static void del_User(struct cli_t *p)
{
  int ret;
  struct user u;
  struct pack_head *pph;
  struct user *back_u=(struct user *)malloc(sizeof(struct user));
  if(!read_User(p->cfd,&u))return ;
  if(del_User_Mysql(&u))
  {
	printf("################del success\n");
  }
  else printf("##################del fail");
}



//对客户端传来的数据包进行分类
//p:客户端链表节点
//return	read失败返回-1，客户端断裂返回-2，成功返回0
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
printf("head ret = %d\n",ret);
printf("type : %hd\n",ph.type);
printf("len : %hd\n",ph.len);
printf("ver : %d\n",ph.ver);

  if(ph.type==USER_LOGIN_TYPE)
  {
printf("user_Login\n");
	user_Login(p);
  }
  else if(ph.type==3)
  {
printf("user_Add\n");
	user_Add(p);
  }
  else if(ph.type==7)
  {
	show_One_User(p);
  }
  else if(ph.type==4)
  {
	del_User(p);
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
	//perror("socket");
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
	  add_cli_node(nfd,caddr);

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













