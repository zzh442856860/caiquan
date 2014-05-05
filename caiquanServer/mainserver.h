#ifndef MAINSERVER_H
#define MAINSERVER_H

#include <set>
#include <muduo/net/TcpServer.h>
using namespace muduo;
using namespace muduo::net;

class MainServer
{
public:
    MainServer(muduo::net::EventLoop* loop,
               const muduo::net::InetAddress& listenAddr);
     void start();
 private:
     void onConnection(const muduo::net::TcpConnectionPtr& conn);
     void onMessage(const muduo::net::TcpConnectionPtr& conn,
                    muduo::net::Buffer* buf,
                    muduo::Timestamp time);
     //void onTimerDelIdleConn();
     //void onHighWaterMark(const TcpConnectionPtr& conn, size_t len);
     void onCloseConn(const TcpConnectionPtr& conn);
     //void onThreadInit(EventLoop* loop);

     muduo::net::TcpServer server_;
     EventLoop* loop_;

     //ZCodec codec_;
     //int highWaterMark_;

     typedef std::set<TcpConnectionPtr> ConnectionList;
     ConnectionList connections_;

     int numConnected_;
};

#endif // MAINSERVER_H
