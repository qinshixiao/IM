#include "user.h"

User::User(string id, string name, int csockfd):
    id(id),name(name),csockfd(csockfd)
{

}

User::~User()
{

}
