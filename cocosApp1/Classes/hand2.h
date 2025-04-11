#ifndef hand2H
#define hand2H

#include "cocos2d.h"

class MCard;
class MAdventurer;

struct stCard {
  MCard* card;
  cocos2d::SpriteFrame* spf;
  cocos2d::Sprite* sp;
  stCard() {
	  card = nullptr;
	  spf = nullptr;
	  sp = nullptr;
  };
  stCard(MCard* _card, cocos2d::SpriteFrame* _spf, cocos2d::Sprite* _sp) {
	  card = _card;
	  spf = _spf;
	  sp = _sp;
  };
};

class MHand2 {
private:
  cocos2d::Scene* pScene;
  cocos2d::Sprite* selectedCardSprite;
  cocos2d::Sprite* hoverCardSprite;
  cocos2d::Sprite* prevHoverCardSprite;
  cocos2d::Vec2 cardOriginPos;
  cocos2d::Vec2 cardMousePos;
  cocos2d::Size visibleSize;
  MCard* releasedCard;
  int limit;
  float scale;
  float offset;
  cocos2d::Rect totalRect;
  std::list<stCard> cards;
  std::map<std::string, cocos2d::SpriteFrame*> cardFrame;
  void repositionCards();
  bool visible;
  bool enabled;
  float originY;
public:
  MHand2();
  ~MHand2();
  bool cardAlredyAdded(MCard* card);
  bool coordsInHand(float x, float y);
  bool initCards(cocos2d::Scene* _pScene, MAdventurer* adventurer, float _offset=0);
  bool addCard(MCard* card);
  bool removeCard(MCard* card);
  bool removeCard(const std::string& name);
  bool removeCardByMask(const std::string& mask);
  bool removeCardByNumber(int i);
  cocos2d::SpriteFrame* getCardSpriteFrame(const std::string& name);
  cocos2d::SpriteFrame* getCardSpriteFrame(MCard* card);
  cocos2d::Sprite* getCardSprite(const std::string& name);
  cocos2d::Sprite* getCardSprite(MCard* card);
  cocos2d::Sprite* getCardSpriteByCoord(float x, float y);
  int getCardsNumber();
  MCard* getCardByNumber(int i);
  MCard* getCardBySprite(cocos2d::Sprite* sprite);
  cocos2d::SpriteFrame* getCardSpriteFrameByNumber(int i);
  cocos2d::Sprite* getCardSpriteByNumber(int i);
  std::vector<cocos2d::SpriteFrame*> getCardFrames();
  std::vector<cocos2d::Sprite*> getCardSprites();
  void disable();
  void enable();
  void hide();
  void show();
  bool needDiscard();
  bool getCardHold();
  void update(float delta);
  void onMouseDown(cocos2d::Event* event);
  void onMouseUp(cocos2d::Event* event);
  void onMouseMove(cocos2d::Event* event);
  MCard* MHand2::getReleasedCard();
  void MHand2::hideCard(MCard* card);
  void MHand2::showCard(MCard* card);
  void clearReleasedCard();

};

#endif