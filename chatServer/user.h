#ifndef USER
#define USER
#include <arpa/inet.h>
#include <string>
using namespace std;

struct userInfo  //loginInfo or registerInfo
{
  char id_name[20]={0};
  char pwd[20]={0};
  int flag;//1 or 2
};

class User
{
    public:
        User(){}
        User(string id,string name,int csockfd);
        ~User();

        string id;
        string name;
        int csockfd;
};

#endif // USER
