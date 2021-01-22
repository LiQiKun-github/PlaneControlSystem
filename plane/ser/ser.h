#ifndef __SER_H__
#define __SER_H__


#include "experiment.h"

#define PORT 9001
#define IP "127.0.0.1"



struct cli_t
{
  int cfd;
  int equipment_id;
  struct sockaddr_in addr;
  int close_flag;
  int count;
  struct cli_t *next;
};

int find_Equipment_Fd(int action_id);
void write_Data_To_Equipment(struct order_t *temp);

#endif
