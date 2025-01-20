#include "hand.h"
#include <iostream>

MHand::MHand() {
	limit = 0;
}
MHand::~MHand() {
	cards.clear();
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

	cocos2d::Sprite* hand;
	char buffer[16];
	for (int i = 0; i < limit; i++) {
		memset(buffer, 0, 16);
		snprintf(buffer, 16, handMask.c_str(), i);
		hand = cocos2d::Sprite::createWithSpriteFrame(cache->getSpriteFrameByName(noneCard));
		if (!hand) {
			return false;
		}
		hand->setName(buffer);
		hand->setPosition(cocos2d::Vec2(position.x + 70 * i, position.y));
		pScene->addChild(hand);
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

	//display only 5 cards
	if (num < limit) {
		char buffer[16] = { 0 };
		cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
		sprintf(buffer, handMask.c_str(), num);
		cocos2d::Sprite* sp = (cocos2d::Sprite*)pScene->getChildByName(buffer);
		cocos2d::SpriteFrame* frame = cache->getSpriteFrameByName(name);
		if (!sp || !frame) {
			return false;
		}
		sp->setSpriteFrame(frame);
	}
	
	return true;
}
bool MHand::removeCard(int number) {
	char buffer[16] = { 0 };
	cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
	sprintf(buffer, handMask.c_str(), number);
	cocos2d::Sprite* sp = (cocos2d::Sprite*)pScene->getChildByName(buffer);
	cocos2d::SpriteFrame* frame = cache->getSpriteFrameByName(noneCard);
	if (!sp || !frame) {
		return false;
	}
	sp->setSpriteFrame(frame);
	cards.erase(cards.begin() + number);

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

void MHand::refresh() {
	cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
	if (!cache) return;

	cocos2d::Sprite* sp;
	cocos2d::SpriteFrame* frame;
	char buffer[16];
	for (int i = 0; i < cards.size(); i++) {
		memset(buffer, 0, 16);
		snprintf(buffer, 16, handMask.c_str(), i);
		sp = (cocos2d::Sprite*)pScene->getChildByName(buffer);
		frame = cache->getSpriteFrameByName(cards[i]);
		if (!sp || !frame) {
			return;
		}
		sp->setSpriteFrame(frame);
	}
}

bool MHand::reset() {
	cards.clear();
	for (int i = 0; i < max; i++) {//limit
		cards.push_back(noneCard);
	}

	char buffer[16];
	cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
	for (int i = 0; i < limit; i++) {//cards.size()
		memset(buffer, 0, 16);
		sprintf(buffer, handMask.c_str(), i);
		cocos2d::Sprite* sp = (cocos2d::Sprite*)pScene->getChildByName(buffer);
		cocos2d::SpriteFrame* frame = cache->getSpriteFrameByName(noneCard);
		if (!sp || !frame) {
			return false;
		}
		sp->setSpriteFrame(frame);
	}
	return true;
}