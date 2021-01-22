#include"common.h"
#include"cli_link.h"
#include"pack.h"
#include"ser.h"


static struct cli_t *head=NULL;


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


//在链表中查找某个描述符的节点
//cfd   要查找的文件描述符
//return    找到返回找到的节点，未找到返回NULL
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
//参数：    last：所要删除节点的上一个节点  p：要删除的节点
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



void add_cli_node(int nfd,struct sockaddr_in caddr)
{
  struct cli_t *p;
  p=(struct cli_t*)malloc(sizeof(struct cli_t));
  p->cfd=nfd;
  p->addr=caddr;
  p->count=0;
  p->close_flag=0;
  p->equipment_id=-1;
  p->next=head;
  head=p;
}




