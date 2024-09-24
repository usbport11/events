#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

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

    cocos2d::Vec2 getCellUnderMouse(cocos2d::Event* event);
    void selectCell(cocos2d::Vec2 cell, std::string name);
    cocos2d::Vec2 getCoordsByCell(cocos2d::Vec2 cell);
    cocos2d::Vec2 HelloWorld::sign(cocos2d::Vec2 vec);
    void moveSprite(cocos2d::Sprite& sprite, cocos2d::Vec2 destination);
    void createCells(int x, int y);

    bool createAnimSpriteFromPlist(const std::string& fileName, const std::string& spriteName, const std::string& prefix, int count, float step);

public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    virtual void update(float delta);
    void menuCloseCallback(cocos2d::Ref* pSender);
    void onMouseMove(cocos2d::Event* event);
    void onMouseDown(cocos2d::Event* event);
    CREATE_FUNC(HelloWorld);
};

#endif
