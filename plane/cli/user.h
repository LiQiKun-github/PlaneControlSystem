#ifndef __USER_H__
#define __USER_H__

struct user 
{
  int id;
  char password[32];
};



struct user *create_User();



#endif
