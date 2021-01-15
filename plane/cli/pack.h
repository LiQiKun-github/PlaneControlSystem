#ifndef __PACK_H__
#define __PACK_H__


struct pack_head
{
  short len;//追加的数据包长度
  short type;//数据包类型
  int ver;//版本
  char data[0];//追加的地址
};

struct login_answer_t
{
  int flag;
};



struct pack_head *pack_Make(short type,int len,int ver,void *data);



#define PACK_HEAD_LEN 8
#define PACK_TYPE_USER 1
#define PACK_VER_1 1

#define USER_TYPE 66
#define USER_VER 1



#endif
