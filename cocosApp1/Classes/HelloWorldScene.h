#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "PathGenerator.h"
#include "gridMap.h"
#include "deck.h"

class HelloWorld: public cocos2d::Scene {
private:
    MGridMap gridMap;
    MDeck itemDeck;
    MDeck floodDeck;
    MPathGenerator pg;
    std::vector<NVector2> path;
    cocos2d::Vec2 speed;
    bool moving;
    int lastCard;
    int waterCurrent;
    int waterLimit;
    cocos2d::Vec2 waterPos;

    void moveSprite(cocos2d::Sprite* sprite, cocos2d::Vec2 destination);

    bool createWaterState();
    bool createMenu();

    void menuEndTurnCallback(cocos2d::Ref* pSender);
    void menuMoveCallback(cocos2d::Ref* pSender);
    void menuAbflussCallback(cocos2d::Ref* pSender);
    void menuExitCallback(cocos2d::Ref* pSender);

public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void update(float delta);
    void onMouseMove(cocos2d::Event* event);
    void onMouseDown(cocos2d::Event* event);
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    CREATE_FUNC(HelloWorld);
};

#endif
