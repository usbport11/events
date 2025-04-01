#ifndef __TEST_SCENE_H__
#define __TEST_SCENE_H__

#include "cocos2d.h"
#include "hand2.h"
#include "logic/card.h"
#include "logic/adventurer.h"

class MTestScene: public cocos2d::Scene {
private:
    MHand2 hand2;
    MAdventurer* adventurer;
public:
    ~MTestScene();
    static cocos2d::Scene* createScene();
	virtual bool init();
	void update(float delta);
    void onMouseDown(cocos2d::Event* event);
    void onMouseUp(cocos2d::Event* event);
    void onMouseMove(cocos2d::Event* event);
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    CREATE_FUNC(MTestScene);
};

#endif
