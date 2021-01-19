#ifndef __PACK_H__
#define __PACK_H__


//头数据包结构体
struct pack_head
{
  short len;//data数据的长度
  short type;//data数据类型
  int ver;//版本
  char data[0];
};


//用户登陆回复的结构体
struct login_answer_t
{   
  int flag;
};

//用户信息结构体
struct user
{
  int id; //用户id
  char password[32];//用户密码
  char name[32];//用户名
  struct user *next;
};


struct pack_head *pack_Make(int len,int type,int ver,char *data);


/*客户端登陆成功，失败和登陆次数过多的返回标志*/
#define LOGIN_SUCCESS_FLAG 1
#define LOGIN_FAIL_FLAG 0
#define LOGIN_BREAK_FLAG -1

/* 头数据包的长度*/
#define PACK_HEAD_LEN 8

/*对客户端回复的类型*/
#define PACK_TYPE_LOGIN_ANSWER 25
#define USER_LOGIN_TYPE 66
#define PACK_TYPE_SHOW_ONE_USER 1001
#define PACK_TYPE_SHOW_ALL_USER 1002



#define PACK_VER_1 1 //包版本





#endif
