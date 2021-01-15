#include"common.h"
#include"pack.h"

//打包数据
//参数 ：类型	数据长度	版本	数据内容
//return	打包后的地址
struct pack_head *pack_Make(short type,int len,int ver,void *data)
{
  struct pack_head *p;
  p=(struct pack_head *)malloc(PACK_HEAD_LEN+len);
  p->type=type;
  p->len=len;
  p->ver=ver;
  memcpy(p->data,data,len);
  return p;
}
