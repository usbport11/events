#include "deck.h"

MDeck::MDeck() {
	card = "card%d";
	cardBack = "back";
	scene = nullptr;
	cardsNumber = 0;
    lastCard = 0;
}

bool MDeck::create(cocos2d::Scene* _scene, const std::string& plistFile, const std::string& deckName, cocos2d::Vec2 position, tCardsMap& cardsMap) {
	if(!_scene) return false;
	scene = _scene;
	
	if(plistFile.empty()) return false;
	if(deckName.empty()) return false;
	
    cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
    if (!cache) return false;
    cache->addSpriteFramesWithFile(plistFile);

    cocos2d::Label* labelLevel = cocos2d::Label::createWithTTF(deckName, "fonts/Marker Felt.ttf", 24);
    if (!labelLevel) return false;
    labelLevel->setPosition(position);
    scene->addChild(labelLevel);

    cocos2d::Vec2 pos2;
    for (auto it = cardsMap.begin(); it != cardsMap.end(); it ++ ) {
        auto sp = cocos2d::Sprite::createWithSpriteFrame(cache->getSpriteFrameByName(cardsMap[it->first]["listName"]));
        if (!sp) return false;
        sp->getTexture()->setAliasTexParameters();
        if (cardsMap[it->first]["pos"] == "left") {
            pos2 = cocos2d::Vec2(position.x - 50, position.y - 70);
        }
        else if (cardsMap[it->first]["pos"] == "right") {
            pos2 = cocos2d::Vec2(position.x + 50, position.y - 70);
        }
        else {
            return false;
        }
        sp->setPosition(pos2);
        sp->setVisible((bool)std::stoi(cardsMap[it->first]["visible"]));
        scene->addChild(sp, std::stoi(cardsMap[it->first]["zOrder"]), it->first);
    }
    lastCard = 0;
	cardsNumber = cardsMap.size() - 3;

    return true;
}

bool MDeck::setCardNames(const std::string& _card, const std::string& _cardBack) {
    if (_card.empty() || _cardBack.empty()) return false;
    card = _card;
    cardBack = _cardBack;
    return true;
}

void MDeck::nextCard() {
    if (lastCard > cardsNumber - 1) {
        return;
    }
    char buffer[16] = { 0 };
    snprintf(buffer, 16, card.c_str(), lastCard);
    scene->getChildByName(buffer)->setVisible(true);
    if (lastCard >= 1) {
        snprintf(buffer, 16, card.c_str(), lastCard - 1);
        scene->getChildByName(buffer)->setVisible(false);
    }
    lastCard = lastCard + 1;
    if (lastCard == cardsNumber) {
        scene->getChildByName(cardBack)->setVisible(false);
    }
}

void MDeck::reset() {
	scene->getChildByName(cardBack)->setVisible(true);
    char buffer[16];
	for(int i=0; i<cardsNumber; i++) {
        memset(buffer, 0, 16);
		snprintf(buffer, 16, card.c_str(), i);
        scene->getChildByName(buffer)->setVisible(false);
	}
    lastCard = 0;
}

std::string MDeck::getCard(int number) {
    char buffer[16];
    memset(buffer, 0, 16);
    snprintf(buffer, 16, card.c_str(), number);
    return buffer;
}