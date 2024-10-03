#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "PathGenerator.h"

class HelloWorld : public cocos2d::Scene {
private:
    cocos2d::Vec2 offset;
    cocos2d::Rect gridRect;
    cocos2d::Vec2 cellsCount;
    cocos2d::Vec2 cellSize;
    cocos2d::Vec2 halfSize;
    cocos2d::Vec2 speed;
    cocos2d::Vec2 currentCell;
    bool moving;
    MPathGenerator pg;
    std::vector<NVector2> path;
    int lastCard;

    cocos2d::Vec2 getCellUnderMouse(cocos2d::Event* event);
    void selectCell(cocos2d::Vec2 cell, std::string name);
    cocos2d::Vec2 getCoordsByCell(cocos2d::Vec2 cell);
    cocos2d::Vec2 HelloWorld::sign(cocos2d::Vec2 vec);
    void moveSprite(cocos2d::Sprite* sprite, cocos2d::Vec2 destination);
    bool createCells(int x, int y);
    bool createDeck();
    void nextCard();
    void createMenu();

    bool createAnimSpriteFromPlist(const std::string& fileName, const std::string& spriteName, const std::string& prefix, int count, float step);

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
