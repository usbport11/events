#ifndef HandoverSceneH
#define HandoverSceneH

#include "cocos2d.h"

class MMainScene;
class MAdventurer;
class MHand;
class MenuItemImageExt;

class MHandoverScene: public cocos2d::Scene {
private:
	int selectedCard;
	int selectedAdventurer;
	MMainScene* pMainScene;
	MAdventurer* pAdventurer;
	MenuItemImageExt* btnSubmit;
	std::vector<MHand*> hands;
	cocos2d::Vector<cocos2d::MenuItem*> cardItems;
	cocos2d::Vector<cocos2d::MenuItem*> adventurerItems;
	std::map<int, MAdventurer*> numberAdventurer;
	void selectAdventurer(int number);
	void selectSourceCard(int number);
	//main menu buttons
	void handoverSumbit(cocos2d::Ref* pSender);
	void handoverCancel(cocos2d::Ref* pSender);
	//destination adventurers menu
	void adventurer0Select(cocos2d::Ref* pSender);
	void adventurer1Select(cocos2d::Ref* pSender);
	void adventurer2Select(cocos2d::Ref* pSender);
	void adventurer3Select(cocos2d::Ref* pSender);
	void adventurer4Select(cocos2d::Ref* pSender);
	//source cards menu
	void sourceCard0Select(cocos2d::Ref* pSender);
	void sourceCard1Select(cocos2d::Ref* pSender);
	void sourceCard2Select(cocos2d::Ref* pSender);
	void sourceCard3Select(cocos2d::Ref* pSender);
	void sourceCard4Select(cocos2d::Ref* pSender);
public:
	MHandoverScene();
	~MHandoverScene();
	static cocos2d::Scene* createScene();
	virtual bool init();
	bool create(MMainScene* _pMainScene, MAdventurer* srcAdventurer, std::vector<std::string> dstAdventurers);

	CREATE_FUNC(MHandoverScene);
};

#endif