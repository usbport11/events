#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#include "cocos2d.h"

class MMainScene;

class MMainMenuScene: public cocos2d::Scene {
private:
	MMainScene* pMainScene;
	void startCallback(cocos2d::Ref* pSender);
	void resumeCallback(cocos2d::Ref* pSender);
	void restartCallback(cocos2d::Ref* pSender);
	void settingsCallback(cocos2d::Ref* pSender);
	void creditsCallback(cocos2d::Ref* pSender);
	void quitCallback(cocos2d::Ref* pSender);
public:
    static cocos2d::Scene* createScene();
	virtual bool init();
	bool create(MMainScene* _pMainScene, bool begin);
    CREATE_FUNC(MMainMenuScene);
};

#endif

