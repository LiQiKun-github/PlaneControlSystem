#include"common.h"
#include"user_mysql.h"


static MYSQL *mysql=NULL;
static MYSQL_RES *res;
static MYSQL_ROW row;

const char mysql_server[] = "localhost";	//数据库主机
const char mysql_user[] = "root";	//mysql管理员用户
const char mysql_password[] = "123456";	//mysql管理员登陆密码
const char mysql_database[] = "plane_ser";	//使用的数据库


void mysql_Start()
{
  if(mysql!=NULL) return ;
  mysql=mysql_init(NULL);
  if(!mysql_real_connect(mysql,mysql_server,mysql_user,mysql_password,mysql_database,0,NULL,0))
  {
	fprintf(stderr,"%s\n",mysql_error(mysql));
	mysql_close(mysql);
	mysql=NULL;
  }
}


void mysql_Go(char *query)
{
  if(mysql==NULL)
  {
	perror("mysql connect");
	return ;
  }
  if(mysql_query(mysql,query))
  {
	fprintf(stderr, "%s\n", mysql_error(mysql));
	return ;
  }
  res=mysql_use_result(mysql);
}




void print_res()
{
  while((row = mysql_fetch_row(res))!=NULL)
  {
	printf("id : %s\npassword : %s\n",row[0],row[1]);
  }
}


void close_my_mysql()
{
  mysql_close(mysql);
  mysql=NULL;
  res=NULL;
}


int user_login_check(int id,char *password)
{
  int flag=0;
  mysql_Start();
  char query[1024];
  sprintf(query,"select * from user where id='%d' and password='%s' ", id,password);
  mysql_query(mysql,query);
  res=mysql_use_result(mysql);
  while((row = mysql_fetch_row(res))!=NULL)
  {
	flag=1;
  }
  close_my_mysql();
  return flag;
}



