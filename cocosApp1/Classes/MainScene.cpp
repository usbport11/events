#include "MainScene.h"
#include "EndScene.h"
#include "DropCardScene.h"
#include "HandoverScene.h"
#include "MainMenuScene.h"
#include "utils.h"
#include "logic/area.h"
#include "logic/adventurer.h"
#include "logic/card.h"
#include <iostream>
#include <algorithm>

USING_NS_CC;

Scene* MMainScene::createScene() {
    return MMainScene::create();
}

MProcessor* MMainScene::getProcessor() {
    return &processor;
}

MMainScene::~MMainScene() {
    cardFrame.clear();
    floodSprite.clear();
    artifactSprite.clear();
    adventurerHand.clear();
    for (int i = 0; i < hands.size(); i++) {
        if (hands[i]) delete hands[i];
    }
    hands.clear();
    adventurerSprite.clear();
}

bool MMainScene::endTurn() {
    //exec function
    MAdventurer* adventurer = processor.getCurrentAdventurer();
    MHand* hand = adventurerHand[adventurer->getName()];
    adventurerSprite[adventurer->getName()]->setColor(cocos2d::Color3B(255, 255, 255));
    if(!processor.execFunction("endturn", adventurer->getName())) return false;

    //update hand current adventurer
    std::vector<MCard*> cards = adventurer->getAllCards();
    for (int i = hand->getUsedSize(); i < cards.size(); i++) {
        if (!hand->addCard(cardFrame[cards[i]->getName()])) return false;
    }

    //display top card at flood drop deck
    if (!processor.getFloodDropDeck().empty()) floodDeck.setTopCard(floodSprite[processor.getFloodDropDeck().front()]);
    else floodDeck.reset();

    //update areas
    updateAreas();

    //update adventurer positions (in case that area drawn)
    std::vector<std::string> adventurers = processor.getActiveAdventurers();
    MAdventurer* nextAdventurer;
    int pos[2];
    int num;
    cocos2d::Vec2 advPos;
    for (int i = 0; i < adventurers.size(); i++) {
        nextAdventurer = processor.findAdventurer(adventurers[i]);
        if (!nextAdventurer) return false;
        num = getAdventurerNumber(adventurers[i]);
        pos[0] = (num / 2);
        pos[0] = pos[0] * 32 - 16;
        pos[1] = 16 - (num % 2) * 32;
        advPos = gridMap.getSpriteByAreaName(nextAdventurer->getArea()->getName())->getPosition();
        adventurerSprite[adventurers[i]]->setPosition(advPos.x + pos[0], advPos.y + pos[1]);
    }   

    //remove adventurer if it totaly flooded (sprite on grid, sprite on panel)
    //todo

    if (processor.adventureFailed()) {
        MEndScene* endScene = (MEndScene*)MEndScene::createScene();
        if (!endScene) return false;
        endScene->setMainScene(this);
        endScene->setMessage("Adventure failed!");
        Director::getInstance()->pushScene(endScene);
    }

    if(hand->getUsedSize() > 5) {
        //move to separate function
        std::cout << "Drop cards for: " << adventurer->getName() << std::endl;//important! current adveturer is changed in processor!
        MDropCardScene* dropCardScene = (MDropCardScene*)MDropCardScene::createScene();
        if (!dropCardScene) return false;
        dropCardScene->setMainScene(this);
        if (!dropCardScene->setCards(hand->getNotEmptyCards())) return false;
        if (!dropCardScene->setAdventurer(adventurer)) return false;
        Director::getInstance()->pushScene(dropCardScene);
    }

    //change adventurer
    adventurer = processor.getCurrentAdventurer();
    adventurerSprite[adventurer->getName()]->setColor(cocos2d::Color3B(255, 128, 128));

    //change hand
    adventurerClicked(adventurer->getName());
    adventurerMenu.selectByName(adventurer->getName());

    //display name of next adventurer
    cocos2d::Label* advLabel = (cocos2d::Label*)this->getChildByName("lblAdventurerName");
    if (advLabel) advLabel->setString(processor.getCurrentAdventurer()->getName());

    //update wather level
    waterLevel.setCurrent(processor.getFloodLevel());

    //update menu items
    menu.updateStatuses(processor.getAvailableActions(processor.getCurrentAdventurer()));
    currentAction = "";
    return true;
}

bool MMainScene::startMove() {
    if (processor.actionNumberLimitReached()) {
        std::cout << "Can't action. Limit reached" << std::endl;
        return false;
    }
    if (currentAction == "move") {
        std::cout << "Can't action. Alredy selected" << std::endl;
        return false;
    }

    updateAreas();
    gridMap.clearAreaLimit();

    cocos2d::Sprite* sp;
    std::list<MArea*> areas;
    MAdventurer* adventurer = processor.getCurrentAdventurer();
    if (!adventurer) return false;
    if (adventurer->canUseDiagonal()) {
        areas = adventurer->getArea()->getAllActiveNeighbors();
    }
    else areas = adventurer->getArea()->getDirectActiveNeighbors();
    if (areas.empty()) {
        std::cout << " [MainScene] no available to move areas" << std::endl;
        return false;
    }
    for (std::list<MArea*>::iterator it = areas.begin(); it!= areas.end(); it++) {
        gridMap.addAreaLimit((*it)->getIndex()[0] * gridSize + (*it)->getIndex()[1]);
        sp = gridMap.getSpriteByAreaName((*it)->getName());
        if (!sp) return false;
        //coloring flooded area we are loosing flood status of area
        sp->setColor(cocos2d::Color3B(128,255,128));
    }
    std::cout << " [MainScene] hightlight available to move areas" << std::endl;
    currentAction = "move";

    return true;
}

bool MMainScene::startAbfluss() {
    if (processor.actionNumberLimitReached()) {
        std::cout << "Can't action. Limit reached" << std::endl;
        return false;
    }
    if (currentAction == "abfluss") {
        std::cout << "Can't action. Alredy selected" << std::endl;
        return false;
    }

    updateAreas();
    gridMap.clearAreaLimit();

    cocos2d::Sprite* sp;
    std::list<MArea*> areas;
    MAdventurer* adventurer = processor.getCurrentAdventurer();
    if (!adventurer) return false;
    if (adventurer->canUseDiagonal()) {
        areas = adventurer->getArea()->getAllFloodedNeighbors();
    }
    else areas = adventurer->getArea()->getDirectFloodedNeighbors();
    if (areas.empty()) {
        //currentAction = "";
        std::cout << " [MainScene] no available to abfluss areas" << std::endl;
        return false;
    }
    for (std::list<MArea*>::iterator it = areas.begin(); it != areas.end(); it++) {
        gridMap.addAreaLimit((*it)->getIndex()[0] * gridSize + (*it)->getIndex()[1]);
        sp = gridMap.getSpriteByAreaName((*it)->getName());
        if (!sp) return false;
        //coloring flooded area we are loosing flood status of area
        sp->setColor(cocos2d::Color3B(128, 255, 128));
    }
    std::cout << " [MainScene] hightlight available to abfluss areas" << std::endl;
    currentAction = "abfluss";

    return true;
}

bool MMainScene::extract() {
    if (processor.actionNumberLimitReached()) {
        std::cout << "Can't action. Limit reached" << std::endl;
        return false;
    }
    MAdventurer* adventurer = processor.getCurrentAdventurer();
    if (!adventurer) return false;
    if (adventurer->hasCard("helicopter") && processor.allActiveAdventurersOnArea("adventurers_circle") && processor.allArifactsCollected()) {
        if (!processor.execFunction("extract")) {
            std::cout << "[MainScene] failed to extract!" << std::endl;
            return false;
        }
        MEndScene* endScene = (MEndScene*)MEndScene::createScene();
        if (!endScene) return false;
        endScene->setMainScene(this);
        endScene->setMessage("Congratulations!");
        Director::getInstance()->pushScene(endScene);
    }
    else {
        std::cout << "[MainScene] can't extract. Some condition failed" << std::endl;
        return false;
    }
    currentAction = "";

    return true;
}
bool MMainScene::getArtifact() {
    if (processor.actionNumberLimitReached()) {
        std::cout << "Can't action. Limit reached" << std::endl;
        return false;
    }
    MAdventurer* adventurer = processor.getCurrentAdventurer();
    std::map<std::string, MObject*> artifacts = processor.getArtifacts();
    for (std::map<std::string, MObject*>::iterator it = artifacts.begin(); it != artifacts.end(); it++) {
        std::vector<MCard*> artifactCards = adventurer->getArtifactCards(it->first);
        if (artifactCards.size() >= 4) {
            if(!processor.execFunction("getartifact", adventurer->getName() + " " + it->first)) return false;
            for (int i=0; i < 4; i++) {
                MHand* hand = adventurerHand[adventurer->getName()];
                if (!hand->removeCard(cardFrame[it->first + "0"])) return false; //hand not matter real card name
                itemDeck.setTopCard("itm_" + cardFrame[processor.getItemDropDeck().front()]);//need to fix prefix
            }
            cocos2d::Sprite* sp = (cocos2d::Sprite*)this->getChildByName(artifactSprite[it->first]);
            if (sp) sp->setColor(cocos2d::Color3B(0, 255, 0));
        } 
    }

    //update menu items
    menu.updateStatuses(processor.getAvailableActions(processor.getCurrentAdventurer()));
    currentAction = "";
    return true;
}

bool MMainScene::discard(MAdventurer* adventurer, std::list<int> cards) {
    if (!adventurer) return false;
    int removed = 0;
    cards.sort();//important!

    std::cout << "Drop cards for: " << adventurer->getName() << std::endl;
    std::vector<MCard*> cardsR = adventurer->getAllCards();
    if (!adventurer) return false;
    if (cards.back() >= cardsR.size()) return false;

    MHand* hand = adventurerHand[adventurer->getName()];
    MCard* card;

    for (std::list<int>::iterator it = cards.begin(); it != cards.end(); it++) {
        card = adventurer->getCardByNumber(*it - removed);//wrong card after delete
        if (!card) return false;
        if (!processor.execFunction("discard", adventurer->getName() + " " + card->getName())) return false; //error!
        if (!hand->removeCard(*it - removed)) return false; //hand not matter real card name
        itemDeck.setTopCard("itm_" + cardFrame[processor.getItemDropDeck().front()]);//need to fix prefix
        removed++;
    }
    currentAction = "";
    return true;
}

bool MMainScene::startHandover() {
    if (processor.actionNumberLimitReached()) {
        std::cout << "Can't action. Limit reached" << std::endl;
        return false;
    }
    if (adventurerHand.size() <= 1) return false;
    
    MAdventurer* srcAdventurer = processor.getCurrentAdventurer();

    MArea* area = srcAdventurer->getArea();
    std::vector<std::string> adventurers = processor.getActiveAdventurers();
    std::vector<std::string> dstAdventurers;
    for (int i = 0; i < adventurers.size(); i++) {
        if (srcAdventurer->getName() == adventurers[i]) continue;
        if (srcAdventurer->getName() == "liaison") {
            dstAdventurers.push_back(adventurers[i]);
        }
        else {
            if (srcAdventurer->getArea() == processor.findAdventurer(adventurers[i])->getArea()) {
                dstAdventurers.push_back(adventurers[i]);
            }
        }
    }
    if (dstAdventurers.empty()) return false;
    MHandoverScene* handoverScene = (MHandoverScene*)MHandoverScene::createScene();
    if (!handoverScene->create(this, srcAdventurer, dstAdventurers)) return false;
    Director::getInstance()->pushScene(handoverScene);
    currentAction = "";
    return true;
}

bool MMainScene::sumbitHandover(MAdventurer* adventurer, int cardNumber) {
    if (!adventurer || cardNumber < 0 || cardNumber >= 5) {
        return false;
    }
    MAdventurer* srcAdventurer = processor.getCurrentAdventurer();
    MCard* srcCard = srcAdventurer->getCardByNumber(cardNumber);
    if (!processor.execFunction("handover", srcAdventurer->getName() + " " + adventurer->getName() + " " + srcCard->getName())) return false;
    adventurerHand[srcAdventurer->getName()]->removeCard(cardFrame[srcCard->getName()]);
    adventurerHand[adventurer->getName()]->addCard(cardFrame[srcCard->getName()]);

    //NOT TESTED!
    //if destination adventurer hand has more than 5 cards - must be call drop card scene
    if (adventurerHand[adventurer->getName()]->getUsedSize() > 5) {
        std::cout << "Drop cards after submit for: " << adventurer->getName() << std::endl;
        MDropCardScene* dropCardScene = (MDropCardScene*)MDropCardScene::createScene();
        if (!dropCardScene) return false;
        dropCardScene->setMainScene(this);
        if (!dropCardScene->setCards(adventurerHand[adventurer->getName()]->getNotEmptyCards())) return false;
        if (!dropCardScene->setAdventurer(adventurer)) return false;
        Director::getInstance()->pushScene(dropCardScene);
    }

    //update menu items
    menu.updateStatuses(processor.getAvailableActions(processor.getCurrentAdventurer()));
    return true;
}

bool MMainScene::startFly() {
    MAdventurer* adventurer = processor.getCurrentAdventurer();
    if (adventurer->getName() != "pilot") return false;

    if (processor.actionNumberLimitReached()) {
        std::cout << "Can't action. Limit reached" << std::endl;
        return false;
    }
    if (currentAction == "fly") {
        std::cout << "Can't action. Alredy selected" << std::endl;
        return false;
    }

    updateAreas();
    gridMap.clearAreaLimit();

    cocos2d::Sprite* sp;
    MArea* area;
    std::map<std::string, MObject*> areas = processor.getAreas();
    for (std::map<std::string, MObject*>::iterator it = areas.begin(); it != areas.end(); it++) {
        area = (MArea*)it->second;
        if (!area) return false;
        if (area->getFloodLevel() <= 2) {
            gridMap.addAreaLimit(area->getIndex()[0] * gridSize + area->getIndex()[1]);
            sp = gridMap.getSpriteByAreaName(area->getName());
            if (!sp) return false;
            sp->setColor(cocos2d::Color3B(128, 255, 128));
        }
    }
    currentAction = "fly";
    return true;
}

bool MMainScene::startSwim() {
    //need check available area calculation
    MAdventurer* adventurer = processor.getCurrentAdventurer();
    if (adventurer->getName() != "diver") return false;

    if (processor.actionNumberLimitReached()) {
        std::cout << "Can't action. Limit reached" << std::endl;
        return false;
    }
    if (currentAction == "swim") {
        std::cout << "Can't action. Alredy selected" << std::endl;
        return false;
    }

    updateAreas();
    gridMap.clearAreaLimit();

    MArea* area;
    cocos2d::Sprite* sp;
    std::vector<std::string> swimAreas;
    processor.getSwimAreas(adventurer->getArea(), swimAreas);
    for (int i = 0; i < swimAreas.size(); i++) {
        area = processor.findArea(swimAreas[i]);
        gridMap.addAreaLimit(area->getIndex()[0] * gridSize + area->getIndex()[1]);
        sp = gridMap.getSpriteByAreaName(area->getName());
        if (!sp) return false;
        sp->setColor(cocos2d::Color3B(128, 255, 128));
    }
    
    currentAction = "swim";
    return true;
}

bool MMainScene::updateAreas() {
    MArea* area;
    cocos2d::Sprite* sp;
    std::map<std::string, MObject*> areas = processor.getAreas();
    cocos2d::Color3B blue = cocos2d::Color3B(96, 96, 255);
    cocos2d::Color3B white = cocos2d::Color3B(255, 255, 255);
    cocos2d::Color3B black = cocos2d::Color3B(0, 0, 0);
    for (std::map<std::string, MObject*>::iterator it = areas.begin(); it != areas.end(); it++) {
        area = (MArea*)it->second;
        if (!area) {
            return false;
        }
        sp = gridMap.getSpriteByCell(area->getIndex()[0], area->getIndex()[1]);
        if (!sp) {
            return false;
        }
        switch (area->getFloodLevel()) {
        case 0:
            sp->setVisible(true);
            sp->setColor(white);
            break;
        case 1:
            sp->setVisible(true);
            sp->setColor(blue);
            break;
        case 2:
            sp->setVisible(false);
            break;
        }
    }
    return true;
}

bool MMainScene::updateActionNumber() {
    cocos2d::Label* actLabel = (cocos2d::Label*)this->getChildByName("lblActionNumber");
    if (!actLabel) return false;
    char buffer[24] = { 0 };
    sprintf(buffer, "Action: %d", processor.getCurrentActionNumber());
    actLabel->setString(buffer);
    return true;
}

bool MMainScene::initAdventurers() {
    std::map<std::string, MObject*> adventurers = processor.getAdventurers();
    for (std::map<std::string, MObject*>::iterator it = adventurers.begin(); it != adventurers.end(); it++) {
        adventurerSprite[it->first]->setVisible(false);
    }

    int pos[2];

    std::vector<std::string> activeAdventurers = processor.getActiveAdventurers();
    for (int i = 0; i < activeAdventurers.size(); i++) {
        MAdventurer* adventurer = processor.findAdventurer(activeAdventurers[i]);
        if (!adventurer) return false;
        MArea* area = processor.findArea(adventurer->getStartArea());
        if (!area) return false;
        cocos2d::Sprite* sp = gridMap.getSpriteByCell(area->getIndex()[0], area->getIndex()[1]);
        if (!sp) return false;

        pos[0] = i / 2;
        pos[0] = pos[0] * 32 - 16;
        pos[1] = 16 - (i % 2) * 32;
        adventurerSprite[adventurer->getName()]->setPosition(sp->getPosition().x + pos[0], sp->getPosition().y + pos[1]);

        adventurerSprite[adventurer->getName()]->setVisible(true);
        gridMap.setCurrentCell(gridMap.getCellByCoordinates(sp->getPosition()));
    }
    return true;
}

bool MMainScene::initHand() {
    adventurerHand.clear();
    for (int i=0; i < hands.size(); i++) {
        hands[i]->reset();
        hands[i]->setVisible(false);
    }
    std::vector<std::string> activeAdventurers = processor.getActiveAdventurers();
    for (int i=0; i < activeAdventurers.size(); i++) {
        adventurerHand[activeAdventurers[i]] = hands[i];
        MAdventurer* adventurer = processor.findAdventurer(activeAdventurers[i]);
        if (!adventurer) {
            return false;
        }
        if (i == 0) {
            hands[i]->setVisible(true);
        }
        std::vector<MCard*> cards = adventurer->getAllCards();
        for (int j = 0; j < cards.size(); j++) {
            if (!hands[i]->addCard(cardFrame[cards[j]->getName()])) return false;
        }
    }
    
    return true;
}

bool MMainScene::initVisual() {
    if (!createAnimSpriteFromPlist(this, "anim/out.plist", "selection", "pt", 4, 0.2f)) {
        return false;
    }
    this->getChildByName("selection")->setPosition(0, 0);
    this->getChildByName("selection")->setScale(1.5);//1.0
    this->getChildByName("selection")->setVisible(false);

    //artifacts
    cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
    if (!cache) return false;
    cache->addSpriteFramesWithFile("anim/artifacts.plist");
    char buffer[32];
    cocos2d::Sprite* artSprite;
    for (int i = 0; i < 4; i++) {
        memset(buffer, 0, 32);
        snprintf(buffer, 32, "artifact%d", i);
        artSprite = cocos2d::Sprite::createWithSpriteFrame(cache->getSpriteFrameByName(buffer));
        if (!artSprite) return false;
        artSprite->setName(buffer);
        artSprite->setPosition(cocos2d::Vec2(980, 80 + i * 80 ));
        this->addChild(artSprite);
    }

    //move prep to waterlevel class?
    if (!createAnimSpriteFromPlist(this, "anim/water.plist", "anim_water", "water", 4, 0.1f)) {
        return false;
    }
    this->getChildByName("anim_water")->setVisible(false);
    cocos2d::Sprite* waterBack = Sprite::create("water_back.png");
    waterBack->setVisible(false);
    this->addChild(waterBack, 0, "water_back");
    cocos2d::Sprite* glass = Sprite::create("glass.png");
    //bad realization (must be calculated)
    glass->setPosition(cocos2d::Vec2(964, 592));
    glass->setScaleY(5);//5
    this->addChild(glass, 2);
    if (!waterLevel.create(this, "anim_water", "water_back", cocos2d::Vec2(964, 464), cocos2d::Size(64, 64))) return false;//464 -> 592//496 -> 624

    tCardsMap itemCards = {
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
    tCardsMap floodCards = {
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
    if (!itemDeck.create(this, "anim/cards.plist", "Item deck", cocos2d::Vec2(800, 170), itemCards, "itm_no_right")) return false;//850
    if (!itemDeck.setCardNames("itm_card%d", "itm_back")) return false;
    if (!floodDeck.create(this, "anim/floods.plist", "Flood deck", cocos2d::Vec2(600, 170), floodCards, "fld_no_right")) return false;//650
    if (!floodDeck.setCardNames("fld_card%d", "fld_back")) return false;

    if (!gridMap.create(this, "anim/cells.plist", gridSize, 24, cocos2d::Size(250, 200), cocos2d::Size(96, 96))) return false;

    //hands + adventurers sprites
    std::map<std::string, MObject*> adventurers = processor.getAdventurers();
    std::vector<std::string> advs;
    int i = 0;
    for (std::map<std::string, MObject*>::iterator it = adventurers.begin(); it != adventurers.end(); it++) {
        hands.push_back(new MHand);
        if(!hands[i]->create(this, 5, 7, it->first + "_hand%d", "card1", cocos2d::Vec2(100, 100))) return false;
        i ++;
        if (!createAnimSpriteFromPlist(this, "anim/adven1.plist", it->first + "_anim", "adven", 4, 0.2f)) {
            return false;
        }
        adventurerSprite[it->first] = (cocos2d::Sprite*) this->getChildByName(it->first + "_anim");
        adventurerSprite[it->first]->setPosition(0, 0);
        adventurerSprite[it->first]->setVisible(false);
        adventurerSprite[it->first]->setScale(2.0);
        advs.push_back(it->first);
    }

    if (!menu.create(this)) return false;
    if (!adventurerMenu.create(this, "anim/adventurers.plist", advs)) return false;
    advs.clear();

    cardFrame = { {"crystal0", "card5"},
        {"crystal1", "card5"},
        {"crystal2", "card5"},
        {"crystal3", "card5"},
        {"crystal4", "card5"},
        {"sphere0", "card3"},
        {"sphere1", "card3"},
        {"sphere2", "card3"},
        {"sphere3", "card3"},
        {"sphere4", "card3"},
        {"lion0", "card4"},
        {"lion1", "card4"},
        {"lion2", "card4"},
        {"lion3", "card4"},
        {"lion4", "card4"},
        {"bowl0", "card2"},
        {"bowl1", "card2"},
        {"bowl2", "card2"},
        {"bowl3", "card2"},
        {"bowl4", "card2"},
        {"helicopter0", "card8"},
        {"helicopter1", "card8"},
        {"sandbag0", "card7"},
        {"sandbag1", "card7"},
        {"flood0", "card6"},
        {"flood1", "card6"},
        {"flood2", "card6"} };

    floodSprite = {
        {"temple_of_the_moon", "fld_card0"},
        {"temple_of_the_sun", "fld_card1"},
        {"ghost_rock", "fld_card2"},
        {"bridge_of_the_brave", "fld_card3"},
        {"treacherous_dunes", "fld_card4"},
        {"lost_lagoon", "fld_card5"},
        {"fire_cave", "fld_card6"},
        {"cave_of_shadows", "fld_card7"},
        {"observatory", "fld_card8"},
        {"twilight_hollow", "fld_card9"},
        {"watchtower", "fld_card10"},
        {"abandoned_cliffs", "fld_card11"},
        {"whispering_garden", "fld_card12"},
        {"howling_garden", "fld_card13"},
        {"golden_gate", "fld_card14"},
        {"adventurers_circle", "fld_card15"},
        {"iron_gate", "fld_card16"},
        {"scarlet_forest", "fld_card17"},
        {"tidal_castle", "fld_card18"},
        {"coral_castle", "fld_card19"},
        {"bronze_gate", "fld_card20"},
        {"silver_gate", "fld_card21"},
        {"copper_gate", "fld_card22"},
        {"foggy_marshes", "fld_card23"}};

    artifactSprite = {
        {"crystal", "artifact3"},
        {"sphere", "artifact1"},
        {"lion", "artifact2"},
        {"bowl", "artifact0"},
    };

    cocos2d::Label* advLabel = Label::createWithTTF("some_name", "fonts/Marker Felt.ttf", 24);
    if (!advLabel) return false;
    advLabel->setName("lblAdventurerName");
    advLabel->setPosition(100, 25);
    this->addChild(advLabel);

    cocos2d::Label* actLabel = Label::createWithTTF("Action: 0", "fonts/Marker Felt.ttf", 24);
    if (!actLabel) return false;
    actLabel->setName("lblActionNumber");
    actLabel->setPosition(250, 25);
    this->addChild(actLabel);

    return true;
}

bool MMainScene::reset() {
    itemDeck.reset();
    floodDeck.reset();
    waterLevel.reset();

    std::cout << "##############################################################################" << std::endl;
    std::cout << "###  Next start" << std::endl;
    std::cout << "##############################################################################" << std::endl;

    processor.setAdventurersNumber(3);
    if (!processor.execFunction("start")) return false;
    if (!gridMap.init()) return false;
    if (!updateAreas()) return false;
    gridMap.clearAreaLimit();
    if (!initAdventurers()) return false;
    if (!initHand()) return false;
    floodDeck.setTopCard(floodSprite[processor.getFloodDropDeck().front()]);
    //show adventurer name
    cocos2d::Label* advLabel = (cocos2d::Label*)this->getChildByName("lblAdventurerName");
    if (advLabel) advLabel->setString(processor.getCurrentAdventurer()->getName());
    currentAction = "";

    if (!adventurerMenu.init(processor.getActiveAdventurers())) return false;
    adventurerMenuSelected = processor.getCurrentAdventurer()->getName();
    adventurerSprite[processor.getCurrentAdventurer()->getName()]->setColor(cocos2d::Color3B(255, 128, 128));

    //update menu items
    menu.updateStatuses(processor.getAvailableActions(processor.getCurrentAdventurer()));

    return true;
}

void MMainScene::adventurerClicked(const std::string& name) {
    if (adventurerMenuSelected == name) return;
    adventurerHand[adventurerMenuSelected]->setVisible(false);
    adventurerHand[name]->setVisible(true);
    adventurerMenuSelected = name;
}

int MMainScene::getAdventurerNumber(const std::string& name) {
    std::vector<std::string> activeAdventurers = processor.getActiveAdventurers();
    for (int i = 0; i < activeAdventurers.size(); i++) {
        if (activeAdventurers[i] == name) return i;
    }
    return -1;
}

MAdventurer* MMainScene::getCurrentAdventurer() {
    return processor.getCurrentAdventurer();
}

MHand* MMainScene::getAdventurerHand(const std::string& name) {
    if (adventurerHand.find(name) != adventurerHand.end()) return adventurerHand[name];
    return nullptr;
}

MAdventurerMenu* MMainScene::getAdventurerMenu() {
    return &adventurerMenu;
}

bool MMainScene::init() {
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    speed = cocos2d::Vec2(2, 2);
    moving = false;
    gridSize = 6;//need get from processor

    cocos2d::EventListenerMouse* mouseListener = EventListenerMouse::create();
    mouseListener->onMouseMove = CC_CALLBACK_1(MMainScene::onMouseMove, this);
    mouseListener->onMouseDown = CC_CALLBACK_1(MMainScene::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    cocos2d::EventListenerKeyboard* keybordListener = EventListenerKeyboard::create();
    keybordListener->onKeyPressed = CC_CALLBACK_2(MMainScene::onKeyPressed, this);
    keybordListener->onKeyReleased = CC_CALLBACK_2(MMainScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keybordListener, this);

    if (!initVisual()) return false; //need check this for some reasons

    if (!reset()) return false;

    this->scheduleUpdate();

    return true;
}

void MMainScene::update(float delta) {
    updateActionNumber();
}

void MMainScene::moveSprite(cocos2d::Sprite* sprite, cocos2d::Vec2 destination) {
    cocos2d::Vec2 direction = gridMap.sign(destination - sprite->getPosition());
    cocos2d::Vec2 step = cocos2d::Vec2(speed.x * direction.x, speed.y * direction.y);
    cocos2d::Vec2 nextPos = sprite->getPosition() + step;
    float cur_length = sprite->getPosition().distance(destination);
    float nex_length = nextPos.distance(destination);
    if (cur_length < nex_length) {
        sprite->setPosition(destination);
    }
    else {
        sprite->setPosition(nextPos);
    }
}

void MMainScene::onMouseMove(cocos2d::Event* event) {
    cocos2d::Vec2 cell = gridMap.getCellUnderMouse(event);
    if (!gridMap.cellCheck(cell)) {
        this->getChildByName("selection")->setVisible(false);
        return;
    }
    this->getChildByName("selection")->setPosition(gridMap.getCoordsByCell(cell));
    this->getChildByName("selection")->setVisible(true);
}

void MMainScene::onMouseDown(cocos2d::Event* event) {
    cocos2d::EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
    if (!mouseEvent) return;

    if (mouseEvent->getMouseButton() == cocos2d::EventMouse::MouseButton::BUTTON_LEFT) {
        cocos2d::Vec2 cell = gridMap.getCellUnderMouse(event);
        if (!gridMap.cellCheck(cell)) {
            return;
        }
        //why check this? previous check not pass here
        MAdventurer* adventurer = processor.getCurrentAdventurer();
        if(gridMap.getCellByCoordinates(adventurerSprite[adventurer->getName()]->getPosition()) == cell) {
            return;
        }
        if (currentAction == "move" || currentAction == "fly" || currentAction == "swim") {
            if (!processor.execFunction(currentAction, adventurer->getName() + " " + processor.getAreaByIndex(cell.x, cell.y)->getName())) {
                std::cout << "[MainScene] failed to "<< currentAction <<" adventurer!" << std::endl;
                return;
            }
            //position of adventurer must be 1/N adventurers number (max N = 4)
            //16px and 32px must be calculated
            int pos[2];
            int num = getAdventurerNumber(adventurer->getName());
            pos[0] = (num / 2);
            pos[0] = pos[0] * 32 - 16;
            pos[1] = 16 - (num % 2) * 32;
            cocos2d::Vec2 advPos = gridMap.getSpriteByCell(cell.x, cell.y)->getPosition();
            adventurerSprite[adventurer->getName()]->setPosition(advPos.x + pos[0], advPos.y + pos[1]);

            //update menu items
            menu.updateStatuses(processor.getAvailableActions(processor.getCurrentAdventurer()));
            currentAction = "";
        }
        if (currentAction == "abfluss") {
            if (!processor.execFunction("abfluss", adventurer->getName() + " " + processor.getAreaByIndex(cell.x, cell.y)->getName())) {
                std::cout << "[MainScene] failed to abluss by adventurer!" << std::endl;
                return;
            }

            //update menu items
            menu.updateStatuses(processor.getAvailableActions(processor.getCurrentAdventurer()));
            currentAction = "";
        }
    }
    if (mouseEvent->getMouseButton() == cocos2d::EventMouse::MouseButton::BUTTON_RIGHT) {
        updateAreas();
        gridMap.clearAreaLimit();
        menu.unselectMenuAll();
        currentAction = "";
    }
}

void MMainScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    if (keyCode == EventKeyboard::KeyCode::KEY_R) {
        if(!reset()) return;
    }
    if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
        MMainMenuScene* mainMenuScene = (MMainMenuScene*)MMainMenuScene::createScene();
        if (!mainMenuScene) return;
        if (!mainMenuScene->create(this, false)) return;
        Director::getInstance()->pushScene(mainMenuScene);
    }
}

void MMainScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
}