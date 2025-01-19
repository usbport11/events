#ifndef handH
#define handH

#include "cocos2d.h"

class MHand {
private:
	cocos2d::Scene* pScene;
	int limit;
	std::string handMask;
	std::string noneCard;
	std::vector<std::string> cards;
public:
	MHand();
	~MHand();
	void setLimit(int _limit);
	int getLimit();
	int getSize();
	bool create(cocos2d::Scene* _pScene, int _limit, const std::string& _prefix, const std::string& _noneCard, cocos2d::Vec2 position);
	std::vector<std::string> getCards();
	bool addCard(const std::string& name);
	bool removeCard(const std::string& name);
	bool reset();
};

#endif
