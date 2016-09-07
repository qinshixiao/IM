/*
用户链表，负责维护当前所有在线的用户，有新用户上线则添加到链表中，有用户下线则从该链表中删除
*/
#include "userlist.h"

UserList::UserList():
    head(NULL),m_length(0)
{
    //头结点
    Node* node = (Node*)new(Node);
    node->next = NULL;
    head = node;
}

UserList::~UserList()
{

}

//添加新上线用户
void UserList::push(User user)
{
    Node* node = (Node*)new(Node);
    node->user = user;
    node->next = this->head->next;
    this->head->next = node;
    m_length++;
}

//删除下线用户
void UserList::pop(int csockfd)
{
    Node* cur_node = head;
    while(cur_node->next!=NULL)
    {
        if(cur_node->next->user.csockfd == csockfd)
        {
            Node* q = cur_node->next;
            cur_node->next = q->next;
            delete q;
            m_length--;
            return;
        }
        else
            cur_node = cur_node->next;
    }
}

//当前有多少用户在线
int UserList::length()
{
   return this->m_length;
}

//根据用户的id来查询用户的套接字描述符，用于一对一通信
int UserList::selectSockfd(const char* id)
{
    Node* cur_node = head->next;
    while(cur_node!=NULL)
    {
        if(!strcmp(cur_node->user.id.c_str(),id))
            return cur_node->user.csockfd;
        else
            cur_node = cur_node->next;
    }
    return -1;
}

Node* & UserList::get_head_node()
{
    return this->head;
}

void UserList::show()
{
    cout << "show function:" << endl;
    Node* cur_node = head->next;
    while(cur_node!=NULL)
    {
        cout << "userID: " << cur_node->user.id << "  userSockfd: " << cur_node->user.csockfd << endl;
        cur_node = cur_node->next;
    }
}
