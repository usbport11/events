#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Scene
{
private:
    std::vector<cocos2d::Sprite*> spQuads;
    cocos2d::Label* label;
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    virtual void update(float delta);
    void menuCloseCallback(cocos2d::Ref* pSender);
    void onMouseMove(cocos2d::Event* event);
    CREATE_FUNC(HelloWorld);
};

#endif
