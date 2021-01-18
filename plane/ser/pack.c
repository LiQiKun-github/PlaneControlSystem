#include"common.h"
#include"pack.h"

struct pack_head *pack_Make(int len,int type,int ver,char *data)
{
    struct pack_head *p;
	p = malloc(sizeof(struct pack_head) + len);
	p->len = len;
	p->type = type;
	p->ver = ver;
	memcpy(p->data,data,len);
	
	return p;
}

