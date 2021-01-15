#include"common.h"
#include"user.h"

struct user *create_User()
{
  struct user *u = (struct user *)malloc(sizeof(struct user));
  printf("Please input id :");
  scanf("%d",&(u->id));
  while(getchar()!='\n');
  printf("Please input password :");
  scanf("%s",u->password);
  while(getchar()!='\n');
  return u;
}
