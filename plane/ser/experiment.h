#ifndef __EXPERIMENT_H__
#define __EXPERIMENT_H__



struct order_t
{
  int relative_time;	//相对时间
  int action_id;	//动作ID
  int frequency;	//次数
  int time;			//间隔
  char data[96];	//操作数据
  struct order_t *next;
};




struct order_t * experiment_Manage(int user_id,char *filename);
int order_Link_Reduce();
int exe_Order_Link();
void print_Order_Link();







#endif
