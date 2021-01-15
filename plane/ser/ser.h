#ifndef __SER_H__
#define __SER_H__


#define PORT 9001
#define IP "127.0.0.1"

struct cli_t
{
  int cfd;
  struct sockaddr_in addr;
  int close_flag;
  int count;
  struct cli_t *next;
};


#endif
