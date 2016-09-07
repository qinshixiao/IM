#include <iostream>
#include "server.h"
#include <string>
#include <stdlib.h>

using namespace std;

int main()
{
    Server server;
    while(1)
    {
        int csockfd = server.acceptClient();
        if(-1 == csockfd)
        {
            server.closeServer();
            exit(-1);
        }
    }
    return 0;
}

