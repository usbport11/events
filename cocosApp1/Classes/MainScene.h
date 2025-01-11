#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include "PathGenerator.h"
#include "gridMap.h"
#include "deck.h"
#include "hand.h"
#include "waterLevel.h"
#include "menu.h"
#include "logic/processor.h"

class MMainScene: public cocos2d::Scene {
private:
    MProcessor processor;

    MGridMap gridMap;
    MDeck itemDeck;
    MDeck floodDeck;
    MWaterLevel waterLevel;
    MPathGenerator pg;
    MHand hand;
    MMenu menu;
    std::vector<NVector2> path;
    int gridSize;
    std::map<std::string, std::string> cardFrame;

    cocos2d::Vec2 speed;
    bool moving;
    void moveSprite(cocos2d::Sprite* sprite, cocos2d::Vec2 destination);

    bool initVisual();

public:
    MProcessor* getProcessor();
    bool initAreas();
    bool initAdventurers();
    bool initHand();

    static cocos2d::Scene* createScene();
    virtual bool init();
    void update(float delta);
    void onMouseMove(cocos2d::Event* event);
    void onMouseDown(cocos2d::Event* event);
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    CREATE_FUNC(MMainScene);
};

#endif
