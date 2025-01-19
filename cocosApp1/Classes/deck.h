#ifndef deckH
#define deckH

#include "cocos2d.h"

typedef std::map<std::string, std::map<std::string, std::string>> tCardsMap;

class MDeck {
private:
	cocos2d::Scene* pScene;
	std::string card;
	std::string cardBack;
	int cardsNumber;
	std::string lastName;
public:
	MDeck();
	bool create(cocos2d::Scene* _scene, const std::string& plistFile, const std::string& deckName, cocos2d::Vec2 position, tCardsMap& cardsMap);
	bool setCardNames(const std::string& card, const std::string& cardBack);
	void setTopCard(const std::string& name);
	bool reset();
};

#endif
