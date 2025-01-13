#include "deck.h"

MDeck::MDeck() {
	card = "card%d";
	cardBack = "back";
	scene = nullptr;
	cardsNumber = 0;
    lastName = "itm_no_right";
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
	cardsNumber = cardsMap.size() - 3;

    return true;
}

bool MDeck::setCardNames(const std::string& _card, const std::string& _cardBack) {
    if (_card.empty() || _cardBack.empty()) return false;
    card = _card;
    cardBack = _cardBack;
    return true;
}

void MDeck::setTopCard(const std::string& name) {
    cocos2d::Sprite* sp;
    if (lastName != "itm_no_right") {
        sp = (cocos2d::Sprite*)scene->getChildByName(lastName);
        if (sp) {
            sp->setVisible(false);
        }
    }
    if (!name.empty()) {
        sp = (cocos2d::Sprite*)scene->getChildByName(name);
        if (sp) {
            sp->setVisible(true);
            lastName = name;
        }
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
    lastName = "itm_no_right";
}
