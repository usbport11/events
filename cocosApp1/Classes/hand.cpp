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
int MHand::getSize() {
	return cards.size();
}
bool MHand::create(cocos2d::Scene* _pScene, int _limit, const std::string& _handMask, const std::string& _noneCard, cocos2d::Vec2 position) {
	if (!_pScene) return false;
	if (_limit <= 0) return false;
	if (_noneCard.empty()) return false;
	pScene = _pScene;
	limit = _limit;
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
bool MHand::addCard(const std::string& name) {
	std::vector<std::string>::iterator it;

	//check diplicate ?
	//it = std::find(cards.begin(), cards.end(), name);
	//if (it != cards.end()) {
	//	std::cout << " [hand] Card alredy added" << std::endl;
	//	return true;
	//}

	//check limit
	it = std::find(cards.begin(), cards.end(), noneCard);
	if (it == cards.end()) {
		std::cout << " [hand] Hand is full or array empty" << std::endl;
		return false;
	}
	int num = it - cards.begin();
	*it = name;

	char buffer[16] = {0};
	cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
	sprintf(buffer, handMask.c_str(), num);
	cocos2d::Sprite* sp = (cocos2d::Sprite*)pScene->getChildByName(buffer);
	cocos2d::SpriteFrame* frame = cache->getSpriteFrameByName(name);
	if (!sp || !frame) {
		return false;
	}
	sp->setSpriteFrame(frame);
	
	return true;
}
bool MHand::removeCard(const std::string& name) {
	std::vector<std::string>::iterator it = std::find(cards.begin(), cards.end(), name);
	if (it == cards.end()) {
		std::cout << " [hand] Cant find card or array empty" << std::endl;
		return false;
	}
	int num = it - cards.begin();
	*it = name;

	char buffer[16] = { 0 };
	cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
	sprintf(buffer, handMask.c_str(), num);
	cocos2d::Sprite* sp = (cocos2d::Sprite*)pScene->getChildByName(buffer);
	cocos2d::SpriteFrame* frame = cache->getSpriteFrameByName(noneCard);
	if (!sp || !frame) {
		return false;
	}
	sp->setSpriteFrame(frame);

	cards.erase(it);

	return true;
}
bool MHand::reset() {
	cards.clear();
	for (int i = 0; i < limit; i++) {
		cards.push_back(noneCard);
	}

	char buffer[16];
	cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
	for (int i = 0; i < limit; i++) {
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