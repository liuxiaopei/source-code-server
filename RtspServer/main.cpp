#include <iostream>
#include "./NetServer/TCPServer.h"

using namespace std;

int main()
{
    cout << "Hello World!" << endl;
    TCPServer   tcp;
    tcp.SetNetServerInfo(8554, "192.168.36.34");
    tcp.InitNetServer();
    return 0;
}

