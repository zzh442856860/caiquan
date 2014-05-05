#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "socket/BaseConnect.h"

class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);


	void quan(CCObject* p);
	void jian(CCObject* p);
	void bao(CCObject* p);

	void update(float dt);

	BaseConnect *baseConn;

	int myValue;
	int enemyValue;

	cocos2d::CCSprite* mySprite;
	cocos2d::CCSprite* enemySprite;
	CCLabelTTF* resultLable;
};

#endif // __HELLOWORLD_SCENE_H__
