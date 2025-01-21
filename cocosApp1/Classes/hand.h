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
	void refresh();
public:
	MHand();
	~MHand();
	void setLimit(int _limit);
	int getLimit();
	int getUsedSize();
	int getSize();
	bool create(cocos2d::Scene* _pScene, int _limit, int _max, const std::string& _prefix, const std::string& _noneCard, cocos2d::Vec2 position);
	std::vector<std::string> getCards();
	std::vector<std::string> getNotEmptyCards();
	bool addCard(const std::string& name);
	bool removeCard(int number);
	bool removeCard(const std::string& name);
	bool reset();
};

#endif
