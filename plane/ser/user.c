#include"common.h"
#include"user.h"
#include"pack.h"
#include"user_mysql.h"


static struct user *head=NULL;

struct user *take_Off_Head_Node()
{
  struct user *p=head;
  if(head!=NULL) head=head->next;
  return p;
}


struct user *create_user_node(char *id,char*name,char*password)
{
  struct user *u=(struct user*)malloc(sizeof(struct user)); 
  u->id=string_to_int(id);
  strcpy(u->name,name);
  strcpy(u->password,password);
  return u;
}


void print_User_Link()
{
  struct user *p=head;
  while(p!=NULL)
  {
	printf("id : %d\n",p->id);
	p=p->next;
  }
}
void add_User_Node(char *row[])
{
  struct user *u=create_user_node(row[0],row[1],row[2]);
  u->next=head;
  head=u;
}
