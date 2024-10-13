#include "hand.h"

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
bool MHand::create(cocos2d::Scene* _scene, int _limit, const std::string& _prefix, const std::string& _noneCard, cocos2d::Vec2 position) {
	if (!_scene) return false;
	if (limit <= 0) return false;
	if (_noneCard.empty()) return false;
	scene = _scene;
	limit = _limit;
	noneCard = _noneCard;
	prefix = _prefix;
	reset();

	cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
	if (!cache) return false;

	cocos2d::Label* labelHand = cocos2d::Label::createWithTTF("Hand", "fonts/Marker Felt.ttf", 24);
	if (!labelHand) return false;
	labelHand->setPosition(cocos2d::Vec2(position.x + 70 * limit/2, position.y + 70));
	scene->addChild(labelHand);

	cocos2d::Sprite* hand;
	char buffer[16];
	for (int i = 0; i < limit; i++) {
		memset(buffer, 0, 16);
		snprintf(buffer, 16, prefix.c_str(), i);
		hand = cocos2d::Sprite::createWithSpriteFrame(cache->getSpriteFrameByName(noneCard));
		if (!hand) return false;
		hand->setPosition(cocos2d::Vec2(position.x + 70 * i, position.y));
		scene->addChild(hand, 0, buffer);
	}
	return true;
}
bool MHand::setCard(int number, const std::string& name) {
	if(number< 0 || number >= limit) return false;
	if(name.empty()) return false;
	cards[number] = name;
	return true;
}
bool MHand::addCard(const std::string& name) {
	if(name.empty()) return false;
	cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
    if (!cache) return false;
	cards.push_back(name);
	char buffer[16] = { 0 };
    snprintf(buffer, 16, prefix.c_str(), cards.size() - 1);
	cocos2d::Sprite* cardSprite = (cocos2d::Sprite*)scene->getChildByName(buffer);
	if(cardSprite) {
		cardSprite->setSpriteFrame(cache->getSpriteFrameByName(name));
	}
	return true;
}
void MHand::removeCard(const std::string& name) {
	auto card = std::remove(cards.begin(), cards.end(), name);
	if (card != cards.end()) {
		int i = card - cards.begin();
		char buffer[16] = { 0 };
		snprintf(buffer, 16, prefix.c_str(), i);
		cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
		if (!cache) return;
		cocos2d::Sprite* cardSprite = (cocos2d::Sprite*)scene->getChildByName(buffer);
		if (!cardSprite) return;
		cardSprite->setSpriteFrame(cache->getSpriteFrameByName(noneCard));
		cards.erase(card, cards.end());
	}
}
void MHand::reset() {
	cards.clear();
	for(int i=0; i<limit; i++) {
		cards[i] = noneCard;
	}
}