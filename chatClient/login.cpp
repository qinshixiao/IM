#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    sockfd = -1;
    ui->setupUi(this);
    connect(ui->btn_login,SIGNAL(clicked()),this,SLOT(loginTo()));
    connect(ui->btn_register,SIGNAL(clicked()),this,SLOT(registerTo()));
}

Login::~Login()
{
    delete ui;
}

void Login::loginTo()
{
    if(sockfd != -1)
        cs.closeSockfd(sockfd);
    sockfd = cs.connectTo("127.0.0.1",8888);
    if(-1 == sockfd)
        return;

    loginInfo lf;
    lf.flag = 1;
    strcpy(lf.id ,ui->lineEdit_account->text().toStdString().c_str());
    strcpy(lf.pwd,ui->lineEdit_pwd->text().toStdString().c_str());
    if(!strcmp(lf.id,"") || !strcmp(lf.pwd,""))
    {
        qDebug() << "pwd or id is empty!" << endl;
        return;
    }
    ssize_t size = send(sockfd,(void*)&lf,sizeof(lf),0);
    if(-1 == size)
    {
        return;
    }
    int flag = 0;
    size = recv(sockfd,&flag,sizeof(int),0);
    if(1 == flag)
    {
        qDebug() << "login successful" << endl;
        main_face = new interface(sockfd,atoi(lf.id));
        main_face->show();
        this->close();
        //delete this;
    }
    else
    {
        return;
    }
}

void Login::registerTo()
{
    if(-1 == sockfd)
    {
        connectServer cs;
        sockfd = cs.connectTo("127.0.0.1",8888);
        if(-1 == sockfd)
        {
            qDebug() << "register don't connect server" << endl;
            return;
        }
    }
    regist = new Register(sockfd);
    regist->show();
}
