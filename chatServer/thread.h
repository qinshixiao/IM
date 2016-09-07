#ifndef THREAD_H
#define THREAD_H
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include "user.h"
#include "userlist.h"
#include "msg.h"

struct online_userinfo{
    char id[20]={0};
    char name[20]={0};
};

class Thread
{
public:
    Thread(User user,UserList* list);
    ~Thread();

    void start();
private:
    pthread_t thread;
    User m_user;
    UserList* m_list;

    static void* run(void* arg);
};

#endif // THREAD_H
