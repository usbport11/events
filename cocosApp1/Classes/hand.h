#ifndef handH
#define handH

#include "cocos2d.h"

class MHand {
private:
	cocos2d::Scene* scene;
	int limit;
	std::string prefix;
	std::string noneCard;
	std::vector<std::string> cards;
public:
	MHand();
	~MHand();
	void setLimit(int _limit);
	int getLimit();
	int getSize();
	bool create(cocos2d::Scene* scene, int _limit, const std::string& _prefix, const std::string& _noneCard, cocos2d::Vec2 position);
	bool setCard(int number, const std::string& name);
	bool addCard(const std::string& name);
	void removeCard(const std::string& name);
	void reset();
};

#endif
