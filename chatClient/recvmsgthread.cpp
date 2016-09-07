#include "recvmsgthread.h"

recvMsgThread::recvMsgThread(int sockfd, QObject *parent):
    sockfd(sockfd),QObject(parent)
{

}

recvMsgThread::~recvMsgThread()
{

}

void recvMsgThread::start()
{
    int ret = pthread_create(&thread,NULL,run,this);
    if(0!= ret)
        qDebug() << "create thread is fail" << endl;
}

void* recvMsgThread::run(void* arg)
{
    recvMsgThread* th = (recvMsgThread*)(arg);
    Msg msg;
    while(1)
    {
        //memset(&msg,0,sizeof(msg));
        int ret = recv(th->sockfd,&msg,sizeof(msg),0);
        if(-1 == ret || 0 == ret)
            break;
        emit th->sendMsg_signal(&msg);
        qDebug() << "recv a message from " << msg.from_user_id << ": " << msg.msg << endl;
    }
}
