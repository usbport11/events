#ifndef __RULES_SCENE_H__
#define __RULES_SCENE_H__

#include "cocos2d.h"

class MRulesScene: public cocos2d::Scene {
private:
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
public:
    static cocos2d::Scene* createScene();
	virtual bool init();
    CREATE_FUNC(MRulesScene);
};

#endif