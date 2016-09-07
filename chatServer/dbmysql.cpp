#include "dbmysql.h"

DBMysql::DBMysql(const char* host,const char* user,const char* passwd,const char* db_name)
    :host(host),user(user),passwd(passwd),db_name(db_name)
{

}

DBMysql::~DBMysql()
{

}

int DBMysql::db_connect()
{
    if (NULL == mysql_init(&mysql)) {    //分配和初始化MYSQL对象
            cout << "mysql_init(): " << mysql_error(&mysql) << endl;
            return -1;
        }
    //尝试与运行在主机上的MySQL数据库引擎建立连接
    if (NULL == mysql_real_connect(&mysql,host,user,passwd,db_name,0,NULL,0))
    {
        cout << "mysql_real_connect():" << mysql_error(&mysql) << endl;
        return -1;
    }

    cout << "Connected MySQL successful! \n" << endl;
    return 0;
}

void DBMysql::db_insert(const char* sqlstr)
{
    int flag = mysql_real_query(&mysql,sqlstr,strlen(sqlstr));
    if(flag != 0)//if successful return 0
    {
        cout << " mysql_real_query(): " << mysql_error(&mysql) << endl;
        return;
    }
}

MYSQL_ROW DBMysql::db_select(const char *sqlstr)
{
    int flag = mysql_real_query(&mysql,sqlstr,strlen(sqlstr));
    if(flag != 0)//if successful return 0
    {
        cout << " mysql_real_query(): " << mysql_error(&mysql) << endl;
        return NULL;
    }

    MYSQL_RES* res = mysql_store_result(&mysql);
    if (NULL == res)
    {
        printf("mysql_restore_result(): %s\n", mysql_error(&mysql));
        return NULL;
    }
    MYSQL_ROW row = mysql_fetch_row(res);
    return row;
}

void DBMysql::db_close()
{
    mysql_close(&mysql);
}
