#include <iostream>
#include "./NetServer/TCPServer.h"

using namespace std;

int main()
{
    cout << "Hello World!" << endl;
    TCPServer   tcp;
    tcp.SetNetServerInfo(8554, "");
    tcp.InitNetServer();
    return 0;
}

