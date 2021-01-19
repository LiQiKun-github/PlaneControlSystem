#ifndef __USER_H__
#define __USER_H__

struct user 
{
  int id;
  char password[32];
  char name[32];
  struct user *next;
};



struct user *create_User();
struct user *create_Complete_User();


#endif
