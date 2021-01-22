#include"common.h"
#include"experiment.h"
#include"ser.h"
#include"user_mysql.h"
#include <sys/time.h>


static struct order_t *head=NULL;
static struct timeval start_time;
static struct timeval end_time;


void add_Order_Node(struct order_t *o)
{
  if(head==NULL)
  {
	o->next=head;
	head=o;
	return;
  }
  int flag=0;
  struct order_t *p=head;
  struct order_t *pp=p->next;
  if(pp==NULL)
  {
	if(o->relative_time<=p->relative_time)
	{
	  o->next=head;
	  head=o;
	  return;
	}
	else
	{
	  p->next=o;
	  o->next=NULL;
	  return;
	}
  }
  while(pp!=NULL)
  {
	if(o->relative_time<=pp->relative_time)
	{
		p->next=o;
		o->next=pp;
		flag=1;
		return;
	}
	p=p->next;
	pp=p->next;
  }
  if(flag==0)
  {
	p->next=o;
	o->next=pp;
	return;
  }
}



//主要用于代码测试
void print_Order_Link()
{
  struct order_t *p=head;
  if(p==NULL) printf("空空如也!!!!\n");
  while(p!=NULL)
  {
	printf("[%d : %d]\n",p->action_id,p->relative_time);
	p=p->next;
  }
  printf("\n");
}

void delete_Order_Link()
{
  struct order_t *d=NULL;
  while(head!=NULL)
  {
	d=head;
	head=head->next;
	free(d);
	printf("##########delete....\n");
  }
}

void frequency_Add_Order_Node(struct order_t *temp)
{
  int n=0;
  while(n<temp->frequency)
  {
	struct order_t *o=(struct order_t *)malloc(sizeof(struct order_t));
	memcpy(o,temp,sizeof(struct order_t));
	o->relative_time=temp->relative_time+o->time*n;
	add_Order_Node(o);
	n++;
  }
}

struct order_t *del_Order_Head_Node()
{
  struct order_t *p=head;
  head=p->next;
printf("#############del order node : [%d : %d]\n",p->action_id,p->relative_time);
  free(p);
  return head;
}


void write_Report()
{
  struct order_t *h=head;
  FILE *fp;
  char buf[1024]="相对时间：  动作ID：  操作数据：\n";
  fp=fopen("../ser/Report/report","w+");
  if(fp==NULL)
  {
	perror("fopen");
	return ;
  }
  fputs(buf,fp);
  while(h!=NULL)
  {
	memset(buf,0x00,1024);
    sprintf(buf,"%-10d%-10d%-10s\n",h->relative_time,h->action_id,h->data);
    fputs(buf,fp);
	h=h->next;
  }
  fclose(fp);
}



struct order_t *experiment_Manage(int user_id,char *filename)
{
  FILE *fp;
  char buf[128];//最后一个操作数据可能很大。。。记得改
  char file[128];
  sprintf(file,"/home/shangqian/桌面/plane/ser/Expriment/%s",filename);
  fp=fopen(file,"r");
  if(fp==NULL)
  {
	printf("打开实验文件错误!!!\n");
	return NULL;
  }
  while((fgets(buf,sizeof(buf),fp))!=NULL)
  {
	struct order_t *o=(struct order_t*)malloc(sizeof(struct order_t));
	sscanf(buf,"%d%10d%10d%10d%10s\n",&(o->relative_time),&(o->action_id),&(o->frequency),&(o->time),(o->data));
	frequency_Add_Order_Node(o);
  }
  print_Order_Link();
  write_Report();
  gettimeofday(&start_time,NULL);
  printf("##############%d\n",experiment_Mysql(filename,0,ctime(&start_time.tv_sec),user_id));
printf("start_time : %s",ctime(&start_time.tv_sec));
  return head;
}

int time_Minus_Time(struct timeval t1,struct timeval t2)
{
  int sec=t1.tv_sec-t2.tv_sec;
  int usec=t1.tv_usec-t2.tv_usec;
  return sec*1000+usec/1000;
}

int exe_Order_Link()
{
  struct order_t *h=head;
  struct timeval t;
  gettimeofday(&t,NULL);
  if(h==NULL)
  {
	return -1;
  }
  while(h!=NULL)
  {
	if(time_Minus_Time(t,start_time)>=h->relative_time)
	{
	  write_Data_To_Equipment(h);
	  h=h->next;
	  del_Order_Head_Node();
	  if(h==NULL)
	  {
	    gettimeofday(&end_time,NULL);
printf("end_start : %s",ctime(&end_time.tv_sec));
        
        
	  }
	  continue;
	}
	else break;
  }
  return 1;
}

