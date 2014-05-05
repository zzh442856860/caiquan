#ifndef __BASECONNECT__
#define __BASECONNECT__



#include "cocos2d.h"

//#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
#include "TCPSocket.h"

USING_NS_CC;
class BaseConnect
{
public:
	BaseConnect(CCString* ipaddres,int port,int iptype);
	~BaseConnect(void);

	//全是设置的recv消息对应的回调函数。//
	//send 自己写。直接SendPacket//


	// 注册单个协议回调函数（样例)，参数：SOCKET_ID标识，数据包//
    void process_login(int _tag, WorldPacket & packet);
	
    // 消息包更新//
    void process_openbackpack(int _tag, WorldPacket & packet);
    // 注册单个协议回调函数（样例)，参数：SOCKET_ID标识，协议头，数据包//
    bool process_all(int _tag, int _cmd, WorldPacket & packet);
    // 连接事件//
    void OnConnect(int _tag, bool isConnect);
    // 断线事件//
    void onDisconnect(int _tag);

	//send cmd
	void login();
	void getHand();
	//recv ack
	void ack_login(int tag, WorldPacket& packet);
	void ack_getHand(int tag, WorldPacket& packet);

	int getCurrHand() {
		int ret = currHand_;
		currHand_ = -1;
		return ret;
	}
private:
	int16 currHand_;
};
//#endif

#endif __BASECONNECT__