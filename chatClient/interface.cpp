#include "interface.h"
#include "ui_interface.h"

interface::interface(int sockfd, int my_id, QWidget *parent) :
    sockfd(sockfd),my_id(my_id),QWidget(parent),
    ui(new Ui::interface)
{
    ui->setupUi(this);

    chatType = 0;
    to_user_id = 0;
    msg.to_user_id = 0;
    msg.from_user_id = this->my_id;

    ui->btn_all->setEnabled(false);

    ui->tableWidget_userlist->setColumnCount(2);//设置column为2
    QStringList list;
    list << "userID" << "name";
    ui->tableWidget_userlist->setHorizontalHeaderLabels(list);
    ui->tableWidget_userlist->verticalHeader()->setVisible(false); //隐藏行表头
    ui->tableWidget_userlist->setSelectionBehavior(QAbstractItemView::SelectRows);  //单击选择一行
    ui->tableWidget_userlist->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget_userlist->setEditTriggers(QAbstractItemView::NoEditTriggers);   //设置每行内容不可更改


    ui->tableWidget_msgPage->setColumnCount(1);
    ui->tableWidget_msgPage->setShowGrid(false);
    ui->tableWidget_msgPage->setColumnWidth(0,500);
    ui->tableWidget_msgPage->verticalHeader()->setVisible(false);
    ui->tableWidget_msgPage->horizontalHeader()->setVisible(false);
    ui->tableWidget_msgPage->setEditTriggers(QAbstractItemView::NoEditTriggers);

    recv_Online_userinfo();

    thread = new recvMsgThread(sockfd);
    thread->start();

    connect(ui->tableWidget_userlist,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,SLOT(getItem(QTableWidgetItem*)));
    connect(ui->btn_one,SIGNAL(clicked()),this,SLOT(btn_one_slot()));
    connect(ui->btn_all,SIGNAL(clicked()),this,SLOT(btn_all_slot()));
    connect(ui->btn_send,SIGNAL(clicked()),this,SLOT(btn_send_slot()));
    connect(thread,SIGNAL(sendMsg_signal(Msg*)),this,SLOT(recv_msg(Msg*)));
}

interface::~interface()
{
    delete ui;
}

void interface::closeEvent(QCloseEvent *event)
{
    connectServer::closeSockfd(sockfd);
}

void interface::recv_Online_userinfo()
{
    int length = 0;
    recv(sockfd,&length,sizeof(int),0);
    qDebug() << "cur online user count is:" << length << endl;
    online_userinfo ouf;

    int i = 0;
    while(length--)
    {
        recv(sockfd,&ouf,sizeof(ouf),0);
        ui->tableWidget_userlist->insertRow(i);
        ui->tableWidget_userlist->setItem(i,0,new QTableWidgetItem(ouf.id));
        ui->tableWidget_userlist->setItem(i,1,new QTableWidgetItem(ouf.name));
        i++;
        qDebug() << "userID: " << ouf.id << "  name: " << ouf.name << endl;
    }

}

void interface::getItem(QTableWidgetItem *item)
{
    if(1 == chatType)
    {
        int row_number = ui->tableWidget_userlist->currentRow();
        to_user_id = ui->tableWidget_userlist->item(row_number,0)->data(Qt::DisplayRole).toString().toInt();
        to_user_name = ui->tableWidget_userlist->item(row_number,1)->data(Qt::DisplayRole).toString();
        ui->chatuser->setText(to_user_name);
        msg.to_user_id = to_user_id;
        ui->btn_send->setEnabled(true);
    }
}

void interface::btn_one_slot()
{
    ui->btn_all->setEnabled(true);
    ui->btn_one->setEnabled(false);
    this->ui->tableWidget_userlist->setSelectionMode(QAbstractItemView::SingleSelection); //设置只能选择一行，不能多行选中
    chatType = 1;

    ui->btn_send->setEnabled(false);

    delete_Msg();
}

void interface::btn_all_slot()
{
    ui->btn_all->setEnabled(false);
    ui->chatuser->setText("everyone");
    ui->btn_one->setEnabled(true);
    ui->btn_send->setEnabled(true);
    chatType = 0;
    to_user_id = 0;
    msg.to_user_id = to_user_id;

    delete_Msg();
}

void interface::btn_send_slot()
{
    QString msg_str = ui->lineEdit_msg->text();
    if(msg_str == "")
        return;
    msg.to_user_id = to_user_id;
    strcpy(msg.msg,msg_str.toStdString().c_str());
    send(sockfd,&msg,sizeof(msg),0);
    int row_count = ui->tableWidget_msgPage->rowCount();
    ui->tableWidget_msgPage->insertRow(row_count);
    ui->tableWidget_msgPage->setItem(row_count,0,new QTableWidgetItem(QString(msg.msg) + " :我"));
    QTableWidgetItem* item = ui->tableWidget_msgPage->item(row_count,0);
    item->setTextAlignment(Qt::AlignRight);
}

void interface::recv_msg(Msg* msg)
{
    if(chatType == 1)//one mode
    {
        if(0 == msg->to_user_id)
        {
            return;
        }
        to_user_id = msg->from_user_id;
        ui->btn_send->setEnabled(true);
    }
    if(0 == chatType) //everyone mode
    {
        if(0 != msg->to_user_id)
        {
            return;
        }
    }

    if(msg->from_user_id == this->msg.from_user_id)
        return;
    int row_count = ui->tableWidget_msgPage->rowCount();
    ui->tableWidget_msgPage->insertRow(row_count);
    ui->tableWidget_msgPage->setItem(row_count,0,new QTableWidgetItem(QString(msg->from_user_name) +": " + QString(msg->msg)));
}

void interface::delete_Msg()
{
    int row_count = ui->tableWidget_msgPage->rowCount();
    while(row_count)
    {
        row_count--;
        ui->tableWidget_msgPage->removeRow(row_count);
    }
}
