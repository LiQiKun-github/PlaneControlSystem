#include"common.h"
#include"experiment.h"







void experiment_Manage(int user_id,char *filename)
{
  FILE *fp;
  char buf[128];//最后一个操作数据可能很大。。。记得改
  char file[128];
  sprintf(file,"/home/shangqian/桌面/plane/ser/Expriment/%s",filename);
  fp=fopen(file,"r");
  if(fp==NULL)
  {
	printf("打开实验文件错误!!!\n");
	return ;
  }
  while((fgets(buf,sizeof(buf),fp))!=NULL)
  {
	int action_id;
    int relative_time;
    int frequency;
    int time;
    char *data=(char *)malloc(96);
	printf("###################ok");
	sscanf(buf,"%d%10d%10d%10d%10s\n",&relative_time,&action_id,&frequency,&time,data);
	printf("%-10d%-10d%-10d%-10d%-10s\n",relative_time,action_id,frequency,time,data);
  }
}



