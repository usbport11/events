#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Scene
{
private:
    cocos2d::Label* label;
    cocos2d::Vec2 areasOffset;
    cocos2d::Size halfSize;
    cocos2d::Size fullSize;
    int cellsNumber[2];
    int overNum[2];
    std::vector<cocos2d::Sprite*> cells;
    std::vector<int> available;
    int clicked;
    int over;
    cocos2d::Sprite* animSprite;
    cocos2d::Animation* animation;
    cocos2d::Animate* animate;
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
