#ifndef handH
#define handH

#include "cocos2d.h"

class MHand {
private:
	cocos2d::Scene* pScene;
	int limit;
	int max;
	std::string handMask;
	std::string noneCard;
	std::vector<std::string> cards;//frameNames
	std::map<std::string, cocos2d::SpriteFrame*> cardFrame;
	std::map<int, cocos2d::Sprite*> numberSprite;
	void refresh();
public:
	MHand(); 
	~MHand();
	void setLimit(int _limit);
	int getLimit();
	int getUsedSize();
	int getSize();
	bool create(cocos2d::Scene* _pScene, int _limit, int _max, const std::string& _prefix, const std::string& _noneCard, cocos2d::Vec2 position);
	bool create(cocos2d::Scene* _pScene, MHand* hand, const std::string namePrefix, cocos2d::Vec2 position);
	std::vector<std::string> getCards();
	std::vector<std::string> getNotEmptyCards();
	bool addCard(const std::string& name);
	bool removeCard(int number);
	bool removeCard(const std::string& name);
	bool reset();
	void setVisible(bool visible);
	std::string getCard(int number);
	int getMax();
	std::string getHandMask();
	std::string getNoneCard();
	std::map<int, cocos2d::Sprite*> getNumberSprites();
	std::map<std::string, cocos2d::SpriteFrame*> getCardFrames();
	cocos2d::SpriteFrame* getCardFrame(const std::string& cardFrameName);
};

#endif
