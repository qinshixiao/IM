#include "server.h"

Server::Server()
{
    init();
    list = new UserList();
    db = new DBMysql("127.0.0.1","root","hch10499104","IM");
    db->db_connect();
}

Server::~Server()
{
    delete db;
}

//初始化服务器到监听状态
void Server::init()
{
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(-1 == sockfd)
    {
        return;
    }

    int ret = bind(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    if(-1 == ret)
    {
        close(sockfd);
        sockfd = -1;
        return;
    }
    ret = listen(sockfd,1024);
    if(-1 == ret)
    {
        close(sockfd);
        sockfd = -1;
        return;
    }
}

//接受客户端的请求，如果一个用户的用户名和密码都正确才算是连接上服务器。负责就删除已连接套接字描述符
bool Server::acceptClient()
{
    if(-1 == sockfd)
        return false;
    //struct sockaddr_in caddr;
    //socklen_t len = sizeof(caddr);
    int csockfd = accept(sockfd,NULL,NULL);
    if(-1 == csockfd)
    {
        return false;
    }
    userInfo lf;
	
    //接收客户端
    ssize_t size = recv(csockfd,(void*)&lf,sizeof(lf),0);
    if(-1 == size || 0 == size)
    {
        close(csockfd);
        return false;
    }
    if(1 == lf.flag)//login
    {
        //判断用户名和密码是否正确，实际应用从数据库查询
        string sqlstr("select * from user where id = '");
        sqlstr.append(lf.id_name).append("'");
        char** row = db->db_select(sqlstr.c_str());
        int login_flag = 0;
        if(row == NULL)
        {
            login_flag = 0;
            size =send(csockfd,&login_flag,sizeof(int),0);
            close(csockfd);
            return false;
        }
        else
        {
            if(!strcmp(lf.id_name,row[0])&&!strcmp(lf.pwd,row[2]))
            {
                //给客户端返回登录状态，正确登录返回1
                cout << "confirmation successful" << endl;
                login_flag = 1;

                //将已经登录的用户保存到online用户列表中
                User user(lf.id_name,row[1],csockfd);
                list->push(user);
                list->show();

                //start a thread session for curr user
                size =send(csockfd,&login_flag,sizeof(int),0);
                Thread* thread = new Thread(user,list);
                thread->start();
                cout << "user " << row[0] << ":" << row[1] << "  online...." << endl;
                return true;
            }
            else
            {
                login_flag = 0;
                size =send(csockfd,&login_flag,sizeof(int),0);
                close(csockfd);
                return false;
            }
        }
    }
    else //register
    {
        cout << "user name:" << lf.id_name << endl << "user pwd:" << lf.pwd << endl;
        char account[10] ={0};
        ifstream in("auto_account.dat");
        in.read(account,sizeof(account));
        in.close();

        int ac = atoi(account);
        ac++;
        string str;
        stringstream ss;
        ss << ac;
        ss >> str;
        ofstream out("auto_account.dat");
        out.write(str.c_str(),strlen(str.c_str()));
        if(!out)
        {
            cout << "write fail" << endl;
        }
        cout << account << endl;

        send(csockfd,account,sizeof(account),0);
        string sqlstr("insert into user values('");
        sqlstr.append(account).append("','").append(lf.id_name).append("','").append(lf.pwd).append("')");
        db->db_insert(sqlstr.c_str());
        close(csockfd);
    }
    return true;
}

void Server::closeServer()
{
    close(sockfd);
}
