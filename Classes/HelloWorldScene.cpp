#include "HelloWorldScene.h"
#include "socket/BaseConnect.h"
USING_NS_CC;



CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	//caiquan result
	resultLable = CCLabelTTF::create();
	resultLable->setFontSize(40);
	resultLable->setPosition(ccp(visibleSize.width/2, visibleSize.height-30));
	addChild(resultLable);
	//bg
	CCSprite* bg = CCSprite::create("background.jpg");
	bg->setAnchorPoint(CCPointZero);
	bg->setOpacity(100);
	addChild(bg, 0, 1);

	//init mySprite adn enemySprite
	enemySprite = CCSprite::create("quan1.png");
	enemySprite->setScale(0.5);
	enemySprite->setAnchorPoint( ccp(1,0));
	enemySprite->setPosition( ccp(visibleSize.width, 150));
	addChild(enemySprite, 1, 2);
	CCActionInterval* action1 = CCBlink::create(15,10);
	enemySprite->runAction( CCRepeatForever::create(action1));

	CCLabelTTF *enemyLabel = CCLabelTTF::create();
	enemyLabel->setString("Enemy");
	enemyLabel->setFontSize(40);
	enemyLabel->setAnchorPoint( ccp(1,0));
	enemyLabel->setPosition(ccp(visibleSize.width, 150+80));
	addChild(enemyLabel);

	mySprite = CCSprite::create("jian2.png");
	mySprite->setAnchorPoint(CCPointZero);
	mySprite->setScale(0.5f);
	mySprite->setPosition( ccp(0, 140));
	addChild(mySprite, 1, 3);
	CCActionInterval* action2 = CCBlink::create(15, 10);
	mySprite->runAction( CCRepeatForever::create(action2));

	//menu
	CCMenuItemImage *item1 = CCMenuItemImage::create("quan.png",
		"quan.png", this, menu_selector(HelloWorld::quan) );
	CCMenuItemImage *item2 = CCMenuItemImage::create("bao.png",
		"bao.png", this, menu_selector(HelloWorld::bao) );
	CCMenuItemImage *item3 = CCMenuItemImage::create("jian.png",
		"jian.png", this, menu_selector(HelloWorld::jian) );

	CCMenu* menu = CCMenu::create(item1, item2, item3, NULL);
	menu->alignItemsHorizontally();
	menu->setPosition( ccp(100, 35));
	addChild( menu, 1, 4);

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    //CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);
    //
    //// position the label on the center of the screen
    //pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
    //                        origin.y + visibleSize.height - pLabel->getContentSize().height));

    //// add the label as a child to this layer
    //this->addChild(pLabel, 1);

    //// add "HelloWorld" splash screen"
    //CCSprite* pSprite = CCSprite::create("HelloWorld.png");

    //// position the sprite on the center of the screen
    //pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    //// add the sprite as a child to this layer
    //this->addChild(pSprite, 0);
    myValue = -1;
	enemyValue = -1;

	baseConn = new BaseConnect(
		CCString::create("192.168.1.107"), 60000, 1);

	baseConn->login();

	scheduleUpdate();

    return true;
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
	

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}
//quan--0; jian--1; bao--2;
void HelloWorld::quan(CCObject* p)
{
	CCLOG("quan");
	if (myValue == -1) {
		myValue = 0;
		baseConn->getHand();
	}
	
}

void HelloWorld::jian(CCObject* p)
{
	CCLOG("jian");
	if (myValue == -1) {
		myValue = 1;
		baseConn->getHand();
	}
}

void HelloWorld::bao(CCObject* p)
{
	CCLOG("bao");
	if (myValue == -1) {
		myValue = 2;
		baseConn->getHand();
	}
}

void HelloWorld::update(float dt)
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	if (myValue != -1) {
		enemyValue = baseConn->getCurrHand();
		if (enemyValue != -1) {
			CCLOG("enemyValue != -1 : %d", enemyValue);

			removeChildByTag(2, true);
			removeChildByTag(3, true);

			CCSprite *myResult;
			CCSprite *enemyResult;

			switch(enemyValue) {
			case 0:
				enemyResult = CCSprite::create("quan1.png");
				break;
			case 1:
				enemyResult = CCSprite::create("jian1.png");
				break;
			case 2:
				enemyResult = CCSprite::create("bao1.png");
				break;
			default:break;
			}

			switch(myValue) {
			case 0:
				myResult = CCSprite::create("quan2.png");
				break;
			case 1:
				myResult = CCSprite::create("jian2.png");
				break;
			case 2:
				myResult = CCSprite::create("bao2.png");
				break;
			default:break;
			}

			myResult->setScale(0.5f);
			myResult->setAnchorPoint(CCPointZero);
			myResult->setPosition( ccp(0, 140));
			addChild(myResult, 1, 2);

			enemyResult->setScale(0.5f);
			enemyResult->setAnchorPoint(ccp(1, 0));
			enemyResult->setPosition( ccp(visibleSize.width, 150));
			addChild(enemyResult, 1, 3);

			
			if (myValue == enemyValue) {
				resultLable->setString("Draw");
			} else if (myValue == enemyValue-1 || (myValue == 2 && enemyValue==0) ) {
				resultLable->setString("Win");
			} else {
				resultLable->setString("Lose");
			}
			

			myValue = -1;
			enemyValue = -1;
		}
	}
}
