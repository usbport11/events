#ifndef __SETTINGS_SCENE_H__
#define __SETTINGS_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class MSettingsScene: public cocos2d::Scene {
private:
	std::vector<cocos2d::Label*> creditsLabels;
	void adventurersNumberTouchCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void waterLevelTouchCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void musicTouchCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void sumbitCallback(cocos2d::Ref* pSender);
	void cancelCallback(cocos2d::Ref* pSender);
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	int floorPercent(int percent);
public:
    static cocos2d::Scene* createScene();
	virtual bool init();
    CREATE_FUNC(MSettingsScene);
};

#endif