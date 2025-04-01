#ifndef __CREDITS_SCENE_H__
#define __CREDITS_SCENE_H__

#include "cocos2d.h"

class MCreditsScene: public cocos2d::Scene {
private:
	float step;
	std::vector<cocos2d::Label*> creditsLabels;
public:
    static cocos2d::Scene* createScene();
	virtual bool init();
	void update(float delta);
    void onMouseDown(cocos2d::Event* event);
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    CREATE_FUNC(MCreditsScene);
};

#endif

