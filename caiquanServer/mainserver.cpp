#include "mainserver.h"
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Logging.h>
#include <muduo/base/TimeZone.h>

#include <boost/bind.hpp>

#include "stdio.h"

const int kHighWaterMask = 64*1024;
const int kMaxConnections = 30000;
#define MAX_BUF_SIZE 16*1024

enum {
    HEAD_TICK = 0x0001,
    HEAD_LOGIN = 0x0002,
    HEAD_LOGIN_ACK = 0x0003,
    HEAD_LOGOUT = 0x0004,
    HEAD_LOGOUT_ACK = 0x0005,
    HEAD_GET = 0x0006,
    HEAD_GET_ACK = 0x0007
} e_HEAD;

MainServer::MainServer(EventLoop *loop, const InetAddress &listenAddr)
    :server_(loop, listenAddr, "mainServer"),
    loop_(loop),
    numConnected_(0){
    server_.setConnectionCallback(
                    boost::bind(&MainServer::onConnection, this, _1));
    server_.setMessageCallback(
                    boost::bind(&MainServer::onMessage, this, _1,_2,_3));
    srand(time(NULL));
}

void MainServer::start() {
    server_.start();
}

void MainServer::onConnection(const TcpConnectionPtr &conn) {
    LOG_INFO << conn->localAddress().toIpPort() << " -> "
                 << conn->peerAddress().toIpPort() << " is "
                 << (conn->connected() ? "UP" : "DOWN");
    if (conn->connected()){
            ++ numConnected_;
            if ( numConnected_ > kMaxConnections){
                conn->shutdown();
            }

            conn->setTcpNoDelay(true);

            conn->setCloseCallback(boost::bind(&MainServer::onCloseConn, this, _1));

            connections_.insert(conn);
        }else{
            --numConnected_;

            connections_.erase(conn);
        }
}

void MainServer::onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp time) {
    LOG_INFO<< "onMessage: " << conn->name();
    const int kHeaderLen = 2;
    while (buf->readableBytes() >= kHeaderLen) // len---buffer len--uint16
    {
      // FIXME: use Buffer::peekInt32()
      const void* data = buf->peek();
      //int32_t be32 = *static_cast<const int32_t*>(data); // SIGBUS
      //const int32_t len = muduo::net::sockets::networkToHost32(be32);
      int pos = 0;
      int16_t len = 0;
      memcpy(&len, buf->peek(), sizeof(len));
      //int len = get_uint32((char*)data, pos);
      if (len > MAX_BUF_SIZE || len <= 0){
        LOG_ERROR << "Invalid length: " << len;
        //conn->shutdown();  // FIXME: disable reading
        buf->retrieve(kHeaderLen);
        continue;
      }else if (buf->readableBytes() >= (size_t)len) {
        muduo::string message(buf->peek(), len);
        //buf = len(2) + cmd(2) + content(len)
        int16_t cmd = 0;
        memcpy(&cmd, &(buf->peek()[2]), sizeof(cmd));
        LOG_INFO << "len: " << len << "  cmd: " << cmd;
//get caiquan hand
        if (cmd == HEAD_GET) {

            //send ack
            char ackbuf[10];
            memset(ackbuf, 0, sizeof(ackbuf));
            int16_t ack_len = 2+2+2;
            int16_t ack_cmd = HEAD_GET_ACK;
            int16_t ack_hand = rand() % 3; //0-1-2
LOG_INFO << "hand: " <<ack_hand;
            int pos = 0;
            *(int16_t*)(ackbuf + pos) = ack_len;
            pos += sizeof(int16_t);
            *(int16_t*)(ackbuf + pos) = ack_cmd;
            pos += sizeof(int16_t);
            *(int16_t*)(ackbuf + pos) = ack_hand;
            pos += sizeof(int16_t);

            conn->send(ackbuf, ack_len);

            buf->retrieve(len);
        } else if (cmd == HEAD_LOGIN) {

            buf->retrieve(len);
        } else if (cmd == HEAD_LOGOUT) {

            buf->retrieve(len);
        } else if (cmd == HEAD_TICK) {

            buf->retrieve(len);
        }  else {
            buf->retrieve(kHeaderLen);
        }



        //cmdCallback_(conn, message, receiveTime);
      }
      else
      {
        break;
      }
    }
}

void MainServer::onCloseConn(const TcpConnectionPtr &conn) {
    LOG_INFO << "colseConn: " << conn->name();
}
