#ifndef __INTRO_SCENE_H__
#define __INTRO_SCENE_H__

#include "cocos2d.h"

class MIntroScene: public cocos2d::Scene {
private:
    int color;
    int sign;
    cocos2d::Label* pressLabel;
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void update(float delta);
    void onMouseDown(cocos2d::Event* event);
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    CREATE_FUNC(MIntroScene);
};

#endif
