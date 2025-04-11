#ifndef __DOUBLE_CARD_MENU_H__
#define __DOUBLE_CARD_MENU_H__

#include "cocos2d.h"

class MDoubleCardMenu {
private:
	cocos2d::Scene* pScene;
	cocos2d::Menu* menu;
	cocos2d::Vec2 position;
	cocos2d::Sprite* sprites[2];
	void card1Callback(cocos2d::Ref* pSender);
	void card2Callback(cocos2d::Ref* pSender);
public:
	MDoubleCardMenu();
	~MDoubleCardMenu();
	bool create(cocos2d::Scene* _pScene, cocos2d::Vec2 _position);
	bool reset(cocos2d::SpriteFrame* card1SpriteFrame, cocos2d::SpriteFrame* card2SpriteFrame);
	void hide();
	void show();
};

#endif