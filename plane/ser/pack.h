#ifndef __PACK_H__
#define __PACK_H__

struct pack_head
{
  short len;
  short type;
  int ver;
  char data[0];
};

struct login_answer_t
{   
  int flag;
};

struct pack_head *pack_Make(int len,int type,int ver,char *data);



#define LOGIN_SUCCESS_FLAG 1
#define LOGIN_FAIL_FLAG 0
#define LOGIN_BREAK_FLAG -1

#define PACK_HEAD_LEN 8


#define PACK_TYPE_LOGIN_ANSWER 25
#define PACK_VER_1 1
#endif
