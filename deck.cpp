#include "deck.h"
#include <iostream>

MDeck::MDeck() {
	card = "card%d";
	cardBack = "back";
	pScene = nullptr;
	cardsNumber = 0;
    lastName = "";//"itm_no_right";
}

bool MDeck::create(cocos2d::Scene* _pScene, const std::string& plistFile, const std::string& deckName, cocos2d::Vec2 position, tCardsMap& cardsMap, const std::string& _noRight) {
	if(!_pScene) return false;
    pScene = _pScene;
	
	if(plistFile.empty()) return false;
	if(deckName.empty()) return false;

    noRight = _noRight;
    lastName = _noRight;
	
    cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
    if (!cache) return false;
    cache->addSpriteFramesWithFile(plistFile);

    cocos2d::Label* labelLevel = cocos2d::Label::createWithTTF(deckName, "fonts/Marker Felt.ttf", 24);
    if (!labelLevel) return false;
    labelLevel->setPosition(position);
    pScene->addChild(labelLevel);

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
        pScene->addChild(sp, std::stoi(cardsMap[it->first]["zOrder"]), it->first);
    }
	cardsNumber = cardsMap.size() - 3;

    return true;
}

bool MDeck::create(cocos2d::Scene* _pScene, const std::string& deckName, cocos2d::Vec2 position, const std::string& _noRight, const std::string& type) {
    if (!_pScene) return false;
    pScene = _pScene;

    if (deckName.empty()) return false;

    tCardsMap cards;
    if (type != "item" && type != "flood") return false;
    if (type == "item") {
        cards = {
            {"itm_back", {{"listName", "card0"}, {"pos", "left"}, {"visible", "1"}, {"zOrder", "2"}}},
            {"itm_no_left", {{"listName", "card1"}, {"pos", "left"}, {"visible", "1"}, {"zOrder", "1"}}},
            {"itm_no_right", {{"listName", "card1"}, {"pos", "right"}, {"visible", "1"}, {"zOrder", "1"}}},
            {"itm_card2", {{"listName", "card2"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
            {"itm_card3", {{"listName", "card3"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
            {"itm_card4", {{"listName", "card4"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
            {"itm_card5", {{"listName", "card5"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
            {"itm_card6", {{"listName", "card6"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
            {"itm_card7", {{"listName", "card7"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
            {"itm_card8", {{"listName", "card8"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
        };
    }
    if (type == "flood") {
        cards = {
            {"fld_back", {{"listName", "flood0"}, {"pos", "left"}, {"visible", "1"}, {"zOrder", "2"}}},
            {"fld_no_left", {{"listName", "flood1"}, {"pos", "left"}, {"visible", "1"}, {"zOrder", "1"}}},
            {"fld_no_right", {{"listName", "flood1"}, {"pos", "right"}, {"visible", "1"}, {"zOrder", "1"}}},
            {"fld_card0", {{"listName", "flood2"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
            {"fld_card1", {{"listName", "flood3"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
            {"fld_card2", {{"listName", "flood4"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
            {"fld_card3", {{"listName", "flood5"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
            {"fld_card4", {{"listName", "flood6"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
            {"fld_card5", {{"listName", "flood7"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
            {"fld_card6", {{"listName", "flood8"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
            {"fld_card7", {{"listName", "flood9"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
            {"fld_card8", {{"listName", "flood10"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
            {"fld_card9", {{"listName", "flood11"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
            {"fld_card10", {{"listName", "flood12"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
            {"fld_card11", {{"listName", "flood13"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
            {"fld_card12", {{"listName", "flood14"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
            {"fld_card13", {{"listName", "flood15"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
            {"fld_card14", {{"listName", "flood16"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
            {"fld_card15", {{"listName", "flood17"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
            {"fld_card16", {{"listName", "flood18"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
            {"fld_card17", {{"listName", "flood19"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
            {"fld_card18", {{"listName", "flood20"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
            {"fld_card19", {{"listName", "flood21"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
            {"fld_card20", {{"listName", "flood22"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
            {"fld_card21", {{"listName", "flood23"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
            {"fld_card22", {{"listName", "flood24"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
            {"fld_card23", {{"listName", "flood25"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
        };
    }

    noRight = _noRight;
    lastName = _noRight;

    cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
    if (!cache) return false;

    cocos2d::Label* labelLevel = cocos2d::Label::createWithTTF(deckName, "fonts/Marker Felt.ttf", 24);
    if (!labelLevel) return false;
    labelLevel->setPosition(position);
    pScene->addChild(labelLevel);

    cocos2d::Vec2 pos2;
    for (auto it = cards.begin(); it != cards.end(); it++) {
        auto sp = cocos2d::Sprite::createWithSpriteFrame(cache->getSpriteFrameByName(cards[it->first]["listName"]));
        if (!sp) return false;
        sp->getTexture()->setAliasTexParameters();
        if (cards[it->first]["pos"] == "left") {
            pos2 = cocos2d::Vec2(position.x - 50, position.y - 70);
        }
        else if (cards[it->first]["pos"] == "right") {
            pos2 = cocos2d::Vec2(position.x + 50, position.y - 70);
        }
        else {
            return false;
        }
        sp->setPosition(pos2);
        sp->setVisible((bool)std::stoi(cards[it->first]["visible"]));
        pScene->addChild(sp, std::stoi(cards[it->first]["zOrder"]), it->first);
    }
    cardsNumber = cards.size() - 3;
    cards.clear();

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
    //if (lastName != "itm_no_right") {
    if (lastName != noRight) {
        sp = (cocos2d::Sprite*)pScene->getChildByName(lastName);
        if (sp) {
            sp->setVisible(false);
        }
        else std::cout << "no sp for no right: " << name << std::endl;
    }
    if (!name.empty()) {
        sp = (cocos2d::Sprite*)pScene->getChildByName(name);
        if (sp) {
            sp->setVisible(true);
            lastName = name;
        }
        else std::cout << "no sp for no right: " << name << std::endl;
    }
}

bool MDeck::reset() {
    if (!pScene) return false;

    cocos2d::Sprite* sp;
    sp = (cocos2d::Sprite*)pScene->getChildByName(cardBack);
    if (!sp) return false;
    sp->setVisible(true);
    char buffer[16];
	for(int i=0; i<cardsNumber; i++) {
        memset(buffer, 0, 16);
		snprintf(buffer, 16, card.c_str(), i);
        sp = (cocos2d::Sprite*)pScene->getChildByName(buffer);
        if (!sp) return false;
        sp->setVisible(false);
	}
    //lastName = "itm_no_right";
    lastName = noRight;
    return true;
}
