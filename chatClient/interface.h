#ifndef INTERFACE_H
#define INTERFACE_H

#include <QWidget>
#include <sys/types.h>
#include <sys/socket.h>
#include <QDebug>
#include <iostream>
#include <QCloseEvent>
#include <QTableWidgetItem>
#include "connectserver.h"
#include "msg.h"
#include "recvmsgthread.h"

using namespace std;

struct online_userinfo{
    char id[20]={0};
    char name[20]={0};
};

namespace Ui {
class interface;
}

class interface : public QWidget
{
    Q_OBJECT

public:
    explicit interface(int sockfd,int my_id,QWidget *parent = 0);
    ~interface();

protected:
    void closeEvent(QCloseEvent * event);
private:
    Ui::interface *ui;
    int sockfd;

    Msg msg;

    int chatType;//0 is all ,1 is one

    int my_id;
    int to_user_id;
    QString to_user_name;

    recvMsgThread* thread;

    void recv_Online_userinfo();
    void delete_Msg();

private slots:
    void getItem(QTableWidgetItem* item);
    void btn_one_slot();
    void btn_all_slot();
    void btn_send_slot();
    void recv_msg(Msg *msg);
};

#endif // INTERFACE_H
