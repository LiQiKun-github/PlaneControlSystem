#include"common.h"
#include"user_mysql.h"



static MYSQL *mysql=NULL;
static MYSQL_RES *res;
static MYSQL_ROW row;

const char mysql_server[] = "localhost";	//数据库主机
const char mysql_user[] = "root";	//mysql管理员用户
const char mysql_password[] = "123456";	//mysql管理员登陆密码
const char mysql_database[] = "plane_ser";	//使用的数据库

int string_to_int(char *n)
{
	int l=strlen(n);
	int i=0,m=0;
	while(i<l)
	{
		m=m*10+n[i]-'0';
		i++;
	}
	return m;
}


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

/*
int insert_User_Mysql()
{

  return 0;
}
*/

int add_User_Mysql(struct user *u)
{
  mysql_Start();
  char query[1024];
  sprintf(query,"insert into user(password,name) value(\"%s\",\"%s\");",u->password,u->name);
  if(mysql_query(mysql,query))
  {
	fprintf(stderr, "%s\n", mysql_error(mysql));
	return 0;
  }
  memset(query,0x00,sizeof(query));
  sprintf(query,"select * from user order by id desc LIMIT 1;");
  if(mysql_query(mysql,query))
  {
	fprintf(stderr, "%s\n", mysql_error(mysql));
	return 0;
  }
  res=mysql_use_result(mysql);
  if(res==NULL)
  {
	printf("NULL\n");
	return -1;
  }
  row=mysql_fetch_row(res);
  int id=string_to_int(row[0]);
  close_my_mysql();
  return id;

}

//mysql查询单个用户信息
//u:要查询的用户信息	back_u:查询结果
//查询成功返回1，失败返回0
int show_One_User_Mysql(struct user *u,struct user *back_u)
{
  mysql_Start();
  char query[1024];
  sprintf(query,"select * from user where id=%d;",u->id);
  if(mysql_query(mysql,query))
  {
	fprintf(stderr, "%s\n", mysql_error(mysql));
	return 0;
  }
  res=mysql_use_result(mysql);
  if(res==NULL)
  {
    return 0; 
  }
  row=mysql_fetch_row(res);
  if(row==NULL) return 1;	//查询结果为空
  back_u->id=string_to_int(row[0]);
  if(row[1]!=NULL)strcpy(back_u->name,row[1]);
  if(row[2]!=NULL)strcpy(back_u->password,row[2]);
  close_my_mysql();
  return 1;
}


//删除用户
int del_User_Mysql(struct user *u)
{
  mysql_Start();
  char query[1024];
  sprintf(query,"delete from user where id=%d",u->id);
  if(mysql_query(mysql,query))
  {
	fprintf(stderr, "%s\n", mysql_error(mysql));
    return 0;
  }
  close_my_mysql();
  return 1;
}

