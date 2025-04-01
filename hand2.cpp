#include "hand2.h"
#include "logic/card.h"
#include "logic/adventurer.h"
#include <iostream>

MHand2::MHand2() {
  limit = 5;
  scale = 1;
  offset = 0;
  selectedCardSprite = nullptr;
  prevHoverCardSprite = nullptr;
  visible = true;
  enabled = false;
}
MHand2::~MHand2() {
  cards.clear();
  cardFrame.clear();
}
bool MHand2::initCards(cocos2d::Scene* _pScene, MAdventurer* adventurer, float _offset) {
  if(!pScene) return false;
  pScene = _pScene;
  if(!adventurer) return false;

  visible = true;
  selectedCardSprite = nullptr;
  prevHoverCardSprite = nullptr;
  offset = _offset;
  visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

  for (std::list<stCard>::iterator it = cards.begin(); it != cards.end(); it++) {
	  if(it->sp) pScene->removeChild(it->sp);
  }
  cards.clear();

  cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
  if (!cache) return false;
  
  cardFrame = {
		{"helicopter0", cache->getSpriteFrameByName("card8")},
		{"helicopter1", cache->getSpriteFrameByName("card8")},
		{"sandbag0", cache->getSpriteFrameByName("card7")},
		{"sandbag1", cache->getSpriteFrameByName("card7")},
		{"crystal0", cache->getSpriteFrameByName("card5")},
		{"crystal1", cache->getSpriteFrameByName("card5")},
		{"crystal2", cache->getSpriteFrameByName("card5")},
		{"crystal3", cache->getSpriteFrameByName("card5")},
		{"crystal4", cache->getSpriteFrameByName("card5")},
		{"sphere0", cache->getSpriteFrameByName("card3")},
		{"sphere1", cache->getSpriteFrameByName("card3")},
		{"sphere2", cache->getSpriteFrameByName("card3")},
		{"sphere3", cache->getSpriteFrameByName("card3")},
		{"sphere4", cache->getSpriteFrameByName("card3")},
		{"lion0", cache->getSpriteFrameByName("card4")},
		{"lion1", cache->getSpriteFrameByName("card4")},
		{"lion2", cache->getSpriteFrameByName("card4")},
		{"lion3", cache->getSpriteFrameByName("card4")},
		{"lion4", cache->getSpriteFrameByName("card4")},
		{"bowl0", cache->getSpriteFrameByName("card2")},
		{"bowl1", cache->getSpriteFrameByName("card2")},
		{"bowl2", cache->getSpriteFrameByName("card2")},
		{"bowl3", cache->getSpriteFrameByName("card2")},
		{"bowl4", cache->getSpriteFrameByName("card2")}
  };
  
  std::vector<MCard*> sourceCards = adventurer->getAllCards();
  for (std::vector<MCard*>::iterator it = sourceCards.begin(); it != sourceCards.end(); it++) {
    if(!addCard(*it)) return false;
  }

  enabled = true;
  
  return true;
}
void MHand2::repositionCards() {
  int cardsNumber = cards.size();
  int space = -10;
  int cs_sp = cards.front().sp->getContentSize().width + space;//new
  float center = offset + (visibleSize.width - cs_sp * cardsNumber) / 2 + cs_sp / 2;
  float posX;
  float posY = 100;
  int i = 0;
  for(std::list<stCard>::iterator it = cards.begin(); it != cards.end(); it++) {
	posX = center + i * cs_sp;
	it->sp->setPosition(posX, posY);
	i ++;
  }
  cocos2d::Size size = cocos2d::Size(cs_sp * cardsNumber, cards.front().sp->getContentSize().height);
  cocos2d::Vec2 pos = (cards.front().sp->getPosition() + cards.back().sp->getPosition()) / 2 - cocos2d::Vec2(size.width / 2, size.height / 2);
  totalRect = cocos2d::Rect(pos.x, pos.y, size.width, size.height);
}
bool MHand2::cardAlredyAdded(MCard* card) {
	for (std::list<stCard>::iterator it = cards.begin(); it != cards.end(); it++) {
		if (it->card == card) return true;
	}
	return false;
}
bool MHand2::coordsInHand(float x, float y) {
	return totalRect.containsPoint(cocos2d::Vec2(x, y));
}
bool MHand2::addCard(MCard* card) {
  if (!card) return false;
  if (cardAlredyAdded(card)) return false;
  if (cardFrame.find(card->getName()) == cardFrame.end()) return false;
  if (!cardFrame[card->getName()]) return false;
  cocos2d::Sprite* sp = cocos2d::Sprite::createWithSpriteFrame(cardFrame[card->getName()]);
  if(!sp) return false;
  pScene->addChild(sp);
  stCard _card = stCard(card, cardFrame[card->getName()], sp);
  cards.push_back(_card);
  cards.back().sp->setScale(1);//new
  if (card->getType() == "item") {
	  cards.back().sp->setColor(cocos2d::Color3B(255, 255, 0));
  }
  repositionCards();
  return true;
}
bool MHand2::removeCard(MCard* card) {
  if(!card) return false;
  for(std::list<stCard>::iterator it = cards.begin(); it != cards.end(); it++) {
    if(it->card == card) {
	  pScene->removeChild(it->sp);
	  cards.erase(it);
	  repositionCards();
	  return true;
	}
  }
  return false;
}
bool MHand2::removeCard(const std::string& name) {
  if(name.empty()) return false;
  for(std::list<stCard>::iterator it = cards.begin(); it != cards.end(); it++) {
    if(it->card->getName() == name) {
	  pScene->removeChild(it->sp);
	  cards.erase(it);
	  repositionCards();
	  return true;
	}
  }
  return false;
}
bool MHand2::removeCardByMask(const std::string& mask) {
	if (mask.empty()) return false;
	for (std::list<stCard>::iterator it = cards.begin(); it != cards.end(); it++) {
		if (it->card->getName().find(mask) != std::string::npos) {
			pScene->removeChild(it->sp);
			cards.erase(it);
			repositionCards();
			return true;
		}
	}
	return false;
}
bool MHand2::removeCardByNumber(int i) {
	if (i < 0 || i >= cards.size()) return false;
	std::list<stCard>::iterator it = cards.begin();
	std::advance(it, i);
	it->sp->setVisible(false);
	pScene->removeChild(it->sp);
	cards.erase(it);
}
cocos2d::SpriteFrame* MHand2::getCardSpriteFrame(const std::string& name) {
  if(name.empty()) return nullptr;
  for(std::list<stCard>::iterator it = cards.begin(); it != cards.end(); it++) {
    if(it->card->getName() == name) {
	  return it->spf;
	}
  }
  return nullptr;
}
cocos2d::SpriteFrame* MHand2::getCardSpriteFrame(MCard* card) {
  if(!card) return nullptr;
  for(std::list<stCard>::iterator it = cards.begin(); it != cards.end(); it++) {
    if(it->card == card) {
	  return it->spf;
	}
  }
  return nullptr;
}
cocos2d::Sprite* MHand2::getCardSprite(const std::string& name) {
  if(name.empty()) return nullptr;
  for(std::list<stCard>::iterator it = cards.begin(); it != cards.end(); it++) {
    if(it->card->getName() == name) {
	  return it->sp;
	}
  }
  return nullptr;
}
cocos2d::Sprite* MHand2::getCardSprite(MCard* card) {
  if(!card) return nullptr;
  for(std::list<stCard>::iterator it = cards.begin(); it != cards.end(); it++) {
    if(it->card == card) {
	  return it->sp;
	}
  }
  return nullptr;
}
cocos2d::Sprite* MHand2::getCardSpriteByCoord(float x, float y) {
	cocos2d::Size size;
	cocos2d::Vec2 pos;
	cocos2d::Rect rect;
	for (std::list<stCard>::iterator it = cards.begin(); it != cards.end(); it++) {
		if (!it->sp) return nullptr;
		size = it->sp->getContentSize();//new
		pos = it->sp->getPosition() - cocos2d::Vec2(size.width / 2, size.height / 2);
		rect = cocos2d::Rect(pos.x, pos.y, size.width, size.height);
		if (rect.containsPoint(cocos2d::Vec2(x, y))) return it->sp;
	}
	return nullptr;
}
int MHand2::getCardsNumber() {
  return cards.size();
}
MCard* MHand2::getCardByNumber(int i) {
  if(i<0 || i>=cards.size()) return nullptr;
  std::list<stCard>::iterator it = cards.begin();
  std::advance(it, i);
  return it->card;
}
cocos2d::SpriteFrame* MHand2::getCardSpriteFrameByNumber(int i) {
  if(i<0 || i>=cards.size()) return nullptr;
  std::list<stCard>::iterator it = cards.begin();
  std::advance(it, i);
  return it->spf;
}
cocos2d::Sprite* MHand2::getCardSpriteByNumber(int i) {
  if(i<0 || i>=cards.size()) return nullptr;
  std::list<stCard>::iterator it = cards.begin();
  std::advance(it, i);
  return it->sp;
}
std::vector<cocos2d::SpriteFrame*> MHand2::getCardFrames() {
	std::vector<cocos2d::SpriteFrame*> frames;
	for (std::list<stCard>::iterator it = cards.begin(); it != cards.end(); it++) {
		frames.push_back(it->spf);
	}
	return frames;
}
std::vector<cocos2d::Sprite*> MHand2::getCardSprites() {
	std::vector<cocos2d::Sprite*> sprites;
	for (std::list<stCard>::iterator it = cards.begin(); it != cards.end(); it++) {
		sprites.push_back(it->sp);
	}
	return sprites;
}
void MHand2::disable() {
	enabled = false;
}
void MHand2::enable() {
	enabled = true;
}
void MHand2::hide() {
  for(std::list<stCard>::iterator it = cards.begin(); it != cards.end(); it++) {
    it->sp->setVisible(false);
  }
  visible = false;
}
void MHand2::show() {
  for(std::list<stCard>::iterator it = cards.begin(); it != cards.end(); it++) {
    it->sp->setVisible(true);
  }
  visible = true;
}
bool MHand2::needDiscard() {
  return cards.size() > limit;
}
bool MHand2::getCardHold() {
	return selectedCardSprite != nullptr;
}
void MHand2::update(float delta) {
	if (!enabled) return;
	if (!visible) return;
	if (selectedCardSprite) {
		selectedCardSprite->setPosition(cardMousePos);
	}
	else {
		if (!coordsInHand(cardMousePos.x, cardMousePos.y)) {
			if (prevHoverCardSprite) {
				prevHoverCardSprite->setPosition(prevHoverCardSprite->getPosition() - cocos2d::Vec2(0, 20));
				prevHoverCardSprite = nullptr;
			}
			return;
		}
		hoverCardSprite = getCardSpriteByCoord(cardMousePos.x, cardMousePos.y);
		if (hoverCardSprite && hoverCardSprite != prevHoverCardSprite) {
			hoverCardSprite->setLocalZOrder(10);
			hoverCardSprite->setPosition(hoverCardSprite->getPosition() + cocos2d::Vec2(0, 20));
			if (prevHoverCardSprite) {
				prevHoverCardSprite->setLocalZOrder(1);
				prevHoverCardSprite->setPosition(prevHoverCardSprite->getPosition() - cocos2d::Vec2(0, 20));
			}
			prevHoverCardSprite = hoverCardSprite;
		}
	}
}
void MHand2::onMouseDown(cocos2d::Event* event) {
	if (!visible) return;
	cocos2d::EventMouse* e = (cocos2d::EventMouse*)event;
	selectedCardSprite = getCardSpriteByCoord(e->getCursorX(), e->getCursorY());
	if (selectedCardSprite) {
		cardOriginPos = selectedCardSprite->getPosition();
	}
}
void MHand2::onMouseUp(cocos2d::Event* event) {
	if (!visible) return;
	if (selectedCardSprite) {
		selectedCardSprite->setPosition(cardOriginPos);
		selectedCardSprite = nullptr;
	}
}
void MHand2::onMouseMove(cocos2d::Event* event) {
	if (!visible) return;
	cocos2d::EventMouse* e = (cocos2d::EventMouse*)event;
	cardMousePos = cocos2d::Vec2(e->getCursorX(), e->getCursorY());
}