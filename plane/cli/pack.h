#ifndef __PACK_H__
#define __PACK_H__


struct pack_head
{
  short len;//追加的数据包长度
  short type;//数据包类型
  int ver;//版本
  char data[0];//追加的地址
};

struct answer_t
{
  int flag;
};


struct experiment_t
{
  int id;
  int start_flag;
};


struct pack_head *pack_Make(short type,int len,int ver,void *data);



#define PACK_HEAD_LEN 8	//包头长度
#define PACK_VER_1 1	//打包版本



/*发送数据类型*/
#define USER_LOGIN_TYPE 66	//用户登陆
#define ADMIN_TYPE 2	//管理员登陆
#define PACK_TYPE_LOGIN_ANSWER 25	//登陆回馈
#define SHOW_ONE_USER_BACK_TYPE 1001	//查询单个用户回馈的数据包类型
#define SHOW_ALL_USER_BACK_TYPE 1002	//查询所有用户回馈的数据包类型
#define PACK_TYPE_START_EXPERIMENT 2000	//开始实验
#define ADD_USER_TYPE 3	//添加用户
#define DEL_USER_TYPE 4	//删除用户
#define CHANGE_USER_TYPE 5	//更改用户信息
#define SHOW_ALL_USER_TYPE 6	//查询所有用户信息
#define SHOW_ONE_USER_TYPE 7	//查询单个用户信息




#endif
