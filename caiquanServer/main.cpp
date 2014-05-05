#include <iostream>
#include <muduo/net/EventLoop.h>
#include <muduo/net/EventLoopThread.h>
#include <muduo/net/InetAddress.h>
#include <muduo/base/Logging.h>

#include "mainserver.h"
using namespace std;
using namespace muduo;
using namespace muduo::net;

int main()
{
    LOG_INFO << "pid = " << getpid();

    EventLoop loop;

    InetAddress listenAddr(60000);
    MainServer server(&loop, listenAddr);
    server.start();

    loop.loop();
    return 0;
}

