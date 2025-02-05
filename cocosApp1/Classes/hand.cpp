#include "hand.h"
#include <iostream>

MHand::MHand() {
	limit = 0;
}
MHand::~MHand() {
	cards.clear();
	cardFrame.clear();
	numberSprite.clear();
}
void MHand::setLimit(int _limit) {
	limit = _limit;
}
int MHand::getLimit() {
	return limit;
}
int MHand::getUsedSize() {
	std::vector<std::string>::iterator it = std::find(cards.begin(), cards.end(), noneCard);
	if (it == cards.end()) {
		return cards.size();
	}
	int num = it - cards.begin();
	return num;
}
int MHand::getSize() {
	return cards.size();
}
bool MHand::create(cocos2d::Scene* _pScene, int _limit, int _max, const std::string& _handMask, const std::string& _noneCard, cocos2d::Vec2 position) {
	if (!_pScene) return false;
	if (_limit <= 0) return false;
	if (_max <= 0) return false;
	if (_limit >= _max) return false;
	if (_noneCard.empty()) return false;
	pScene = _pScene;
	limit = _limit;
	max = _max;
	noneCard = _noneCard;
	handMask = _handMask;

	cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
	if (!cache) return false;

	cocos2d::Label* labelHand = cocos2d::Label::createWithTTF("Hand", "fonts/Marker Felt.ttf", 24);
	if (!labelHand) return false;
	labelHand->setPosition(cocos2d::Vec2(position.x + 70 * limit/2 - 30, position.y + 70));
	pScene->addChild(labelHand);

	cardFrame = {
		{"card0", cache->getSpriteFrameByName("card0")},
		{"card1", cache->getSpriteFrameByName("card1")},
		{"card2", cache->getSpriteFrameByName("card2")},
		{"card3", cache->getSpriteFrameByName("card3")},
		{"card4", cache->getSpriteFrameByName("card4")},
		{"card5", cache->getSpriteFrameByName("card5")},
		{"card6", cache->getSpriteFrameByName("card6")},
		{"card7", cache->getSpriteFrameByName("card7")},
		{"card8", cache->getSpriteFrameByName("card8")}
	};

	char buffer[32];
	for (int i = 0; i < limit; i++) {
		memset(buffer, 0, 32);
		snprintf(buffer, 32, handMask.c_str(), i);
		numberSprite[i] = cocos2d::Sprite::createWithSpriteFrame(cardFrame[noneCard]);
		if (!numberSprite[i]) return false;
		numberSprite[i]->setName(buffer);
		numberSprite[i]->setPosition(cocos2d::Vec2(position.x + 70 * i, position.y));
		pScene->addChild(numberSprite[i]);
	}

	return true;
}
bool MHand::create(cocos2d::Scene* _pScene, MHand* hand, const std::string namePrefix, cocos2d::Vec2 position) {
	if (!_pScene) return false;
	if (!hand) return false;
	if (namePrefix.empty()) return false;

	pScene = _pScene;
	handMask = hand->getHandMask();
	noneCard = hand->getNoneCard();
	cards = hand->getCards();
	cardFrame = hand->getCardFrame();
	limit = hand->getLimit();
	max = hand->getMax();
	numberSprite = hand->getNumberSprite();
	for (std::map<int, cocos2d::Sprite*>::iterator it = numberSprite.begin(); it != numberSprite.end(); it++) {
		numberSprite[it->first] = cocos2d::Sprite::createWithSpriteFrame(it->second->getSpriteFrame());
		if (!numberSprite[it->first]) return false;
		numberSprite[it->first]->setPosition(cocos2d::Vec2(position.x + 70 * it->first, position.y));
		numberSprite[it->first]->setName(namePrefix + numberSprite[it->first]->getName());
		pScene->addChild(numberSprite[it->first]);
	}
	return true;
}
std::vector<std::string> MHand::getCards() {
	return cards;
}
std::vector<std::string> MHand::getNotEmptyCards() {
	std::vector<std::string> noEmpty;
	for (int i = 0; i<cards.size(); i++) {
		if (cards[i] != noneCard) noEmpty.push_back(cards[i]);
		else return noEmpty;
	}
	return noEmpty;
}
bool MHand::addCard(const std::string& name) {
	//check limit
	std::vector<std::string>::iterator it;
	it = std::find(cards.begin(), cards.end(), noneCard);
	if (it == cards.end()) {
		std::cout << " [hand] Hand is full or array empty" << std::endl;
		return false;
	}
	int num = it - cards.begin();
	*it = name;

	//there are only 5 cards to display and sprites too
	if (num < limit) {
		numberSprite[num]->setSpriteFrame(cardFrame[name]);
	}
	
	return true;
}
bool MHand::removeCard(int number) {
	numberSprite[number]->setSpriteFrame(cardFrame[noneCard]);
	cards[number] = noneCard;

	//split
	int num = 0;
	for (int i = 0; i < cards.size(); i++) {
		if (cards[i] == noneCard) num++;
	}
	if (num >= cards.size()) return true;

	int emptyPos = -1;
	int fullPos = -1;
	for (int i = 0; i < cards.size(); i++) {
		if (cards[i] == noneCard) emptyPos = i;
		else fullPos = i;
		if (emptyPos != -1 && fullPos != -1 && fullPos > emptyPos) {
			cards[emptyPos] = cards[fullPos];
			cards[fullPos] = noneCard;
			i = emptyPos;
			emptyPos = -1;
			fullPos = -1;
		}
	}

	refresh();

	return true;
}
bool MHand::removeCard(const std::string& name) {
	std::vector<std::string>::iterator it = std::find(cards.begin(), cards.end(), name);
	if (it == cards.end()) return false;
	int num = it - cards.begin();
	return removeCard(num);
}
void MHand::refresh() {
	for (int i = 0; i < limit; i++) {
		if (cardFrame.find(cards[i]) == cardFrame.end()) {
			std::cout<<"can't find: " << cards[i] << std::endl;
			return;
		}
		numberSprite[i]->setSpriteFrame(cardFrame[cards[i]]);
	}
}
bool MHand::reset() {
	cards.clear();
	for (int i = 0; i < max; i++) {//limit
		cards.push_back(noneCard);
	}
	for(int i = 0; i < limit; i++) {
		numberSprite[i]->setSpriteFrame(cardFrame[noneCard]);
	}
	return true;
}
void MHand::setVisible(bool visible) {
	for (int i = 0; i < limit; i++) {
		numberSprite[i]->setVisible(visible);
	}
}
std::string MHand::getCard(int number) {
	if (number >= cards.size()) return "";
	return cards[number];
}
int MHand::getMax() {
	return max;
}
std::string MHand::getHandMask() {
	return handMask;
}
std::string MHand::getNoneCard() {
	return noneCard;
}
std::map<std::string, cocos2d::SpriteFrame*> MHand::getCardFrame() {
	return cardFrame;
}
std::map<int, cocos2d::Sprite*> MHand::getNumberSprite() {
	return numberSprite;
}