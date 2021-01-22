#include"common.h"
#include"expriment.h"
#include"menu.h"
#include"cli_socket.h"


static int write_In_File(char *filename,char *buf,int len)
{
  FILE *fp;
  fp=fopen(filename,"a");
  if(fp==NULL)
  {
	perror("open");
	return 0;
  }
  strcat(buf,"\n");
  fwrite(buf,sizeof(char),len+1,fp);
  fclose(fp);
  return 1;
}

void write_Order_In_Experiment_File(char *filename,int action_id)
{
  char buf[1024];
  int relative_time;
  int frequency;
  int time;
  char *data=(char *)malloc(96);
  memset(data,0x00,96);
  printf("请输入相对时间:");scanf("%d",&relative_time);while(getchar()!='\n');
  printf("请输入次数:");scanf("%d",&frequency);while(getchar()!='\n');
  printf("请输入时间间隔:");scanf("%d",&time);while(getchar()!='\n');
  printf("请输入操作数据");scanf("%s",data);while(getchar()!='\n');
  sprintf(buf,"%-10d%-10d%-10d%-10d%-10s",relative_time,action_id,frequency,time,data);
  if(!write_In_File(filename,buf,strlen(buf))){free(data);return;}//写入失败退出
  free(data);
}

void environmental_Simulation(char *filename)
{
  int action_id;
  while(1)
  {
	char sel;
	printf("%s",ENVIRMONIMENU);
	scanf("%c",&sel); while(getchar()!='\n');
	switch(sel)
	{
	  case '1':action_id=101;break;
	  case '2':action_id=102;break;
	  case '3':return;
	  default:continue;
	}
	write_Order_In_Experiment_File(filename,action_id);
  }
}

void planewheel_Simulation(char *filename)
{
  int action_id;
  while(1)
  {
    char sel;
    printf("%s",PLANEWHEELMONIMENU);
    scanf("%c",&sel); while(getchar()!='\n');
    switch(sel)
    {   
      case '1':action_id=201;break;
      case '2':return;
	  default:continue;
    }   
    write_Order_In_Experiment_File(filename,action_id);
  }
}



void tirepress_Simulation(char *filename)
{
  int action_id;
  while(1)
  {
    char sel;
    printf("%s",TIREPRESSMONIMENU);
    scanf("%c",&sel); while(getchar()!='\n');
    switch(sel)
    {
      case '1':action_id=301;break;
      case '2':return;
	  default:continue;
    }
    write_Order_In_Experiment_File(filename,action_id);
  }
}



static void add_Equipment(char *filename)
{
  char sel;
  while(1)
  {
	printf("%s",EQUIPMENTMENU);
	scanf("%c",&sel);while(getchar()!='\n');
	switch(sel)
	{
	  case '1':environmental_Simulation(filename);break;
	  case '2':planewheel_Simulation(filename);break;
	  case '3':tirepress_Simulation(filename);break;
	  case '4':printf("暂无功能\n");break;
	  case '5':printf("暂无功能\n");break;
	  case '6':return;
	}
  }
}

//创建实验案例
void add_Expriment()
{
  char exp_name[96];	//实验案例的名称
  printf("请输入实验案例名称:");scanf("%s",exp_name);while(getchar()!='\n');
  char filename[1024];	//创建文件的名称
  sprintf(filename,"./Expriment/%s",exp_name);
  if(access(filename,F_OK)==0)
  {
	printf("该实验案例已经存在！！！\n");
	return ;
  }
  else
  {
	creat(filename,0777);
  }
  while(1)
  {
	char sel;
	printf("%s",INSTRUCTMENU);
	scanf("%c",&sel);while(getchar()!='\n');
	switch(sel)
	{
	  case '1':add_Equipment(filename);break;
	  case '2':printf("2\n");break;
	  case '3':printf("3\n");break;
	  case '4':printf("4\n");return;
	}
  }
}


void start_Experiment(int user_id)
{
  printf("请输入你要开始的实验案例：");
  char filename[96];
  scanf("%s",filename);
  while(getchar()!='\n');
  char buf[1024];
  char file[200];
  sprintf(file,"/home/shangqian/桌面/plane/cli/Expriment/%s",filename);
  sprintf(buf,"scp shangqian@127.0.0.1:/home/shangqian/桌面/plane/cli/Expriment/%s /home/shangqian/桌面/plane/ser/Expriment/",filename);
  if(access(file,F_OK)==0)system(buf);
  else
  {
	printf("没有这个实验案例...\n");
	return ;
  }
  start_Experiment_To_Ser(user_id,filename);
}

