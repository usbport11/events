#ifndef __DROP_CARD_SCENE_H__
#define __DROP_CARD_SCENE_H__

#include "cocos2d.h"

class MMainScene;
class MAdventurer;

class MDropCardScene: public cocos2d::Scene {
private:
	MMainScene* pMainScene;
	MAdventurer* pAdventurer;

	std::list<int> selectedCards;
	std::vector<std::string> sourceCards;
	int selectedCardsNumber;
	int cardsLimit;
	int totalCardsLimit;
	//MenuItemImageExt* closeItem;
	cocos2d::MenuItemImage* closeItem;
	std::string menuName;

	void selectMenuItem(cocos2d::Ref* pSender, int number);

	void card0Callback(cocos2d::Ref* pSender);
	void card1Callback(cocos2d::Ref* pSender);
	void card2Callback(cocos2d::Ref* pSender);
	void card3Callback(cocos2d::Ref* pSender);
	void card4Callback(cocos2d::Ref* pSender);
	void card5Callback(cocos2d::Ref* pSender);
	void card6Callback(cocos2d::Ref* pSender);
	void menuCloseCallback(cocos2d::Ref* pSender);
public:
    static cocos2d::Scene* createScene();
	virtual bool init();
	void setMainScene(MMainScene* _pMainScene);
	bool setCards(std::vector<std::string> _sourceCards);
	bool setAdventurer(MAdventurer* _pAdventurer);
    CREATE_FUNC(MDropCardScene);
};

#endif
