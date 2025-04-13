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
    floodSprite.clear();
    artifactSprite.clear();
    adventurerHand2.clear();
    for (int i = 0; i < hands2.size(); i++) {
        if (hands2[i]) delete hands2[i];
    }
    hands2.clear();
    adventurerSprite.clear();
}

bool MMainScene::endTurn() {
    MAdventurer* adventurer = processor.getCurrentAdventurer();
    if (!adventurer) return false;
    logStream << "Acton 'end turn' called for: " << adventurer->getName() << std::endl;

    MHand2* hand2 = adventurerHand2[adventurer];
    adventurerSprite[adventurer->getName()]->setColor(cocos2d::Color3B(255, 255, 255));
    if(!processor.execFunction("endturn", adventurer->getName())) return false;

    //update hand current adventurer
    std::vector<MCard*> cards = adventurer->getAllCards();
    for (int i = hand2->getCardsNumber(); i < cards.size(); i++) {
        if (!hand2->addCard(cards[i])) return false;
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

    if (hand2->getCardsNumber() > 5) {
        //move to separate function
        std::cout << "Drop cards for: " << adventurer->getName() << std::endl;//important! current adveturer is changed in processor!
        MDropCardScene* dropCardScene = (MDropCardScene*)MDropCardScene::createScene();
        if (!dropCardScene) return false;
        dropCardScene->setMainScene(this);
        if (!dropCardScene->setCardFrames(hand2->getCardFrames())) return false;
        if (!dropCardScene->setAdventurer(adventurer)) return false;
        Director::getInstance()->pushScene(dropCardScene);
    }

    //change adventurer
    adventurer = processor.getCurrentAdventurer();
    adventurerSprite[adventurer->getName()]->setColor(cocos2d::Color3B(255, 128, 128));

    //change hand
    adventurerClicked(adventurer);
    advMenu.selectByName(adventurer->getName());

    //display name of next adventurer
    cocos2d::Label* label;
    label = (cocos2d::Label*)this->getChildByName("lblAdventurerName");
    if (label) label->setString(processor.getCurrentAdventurer()->getNameU());
    label = (cocos2d::Label*)this->getChildByName("lblAdventurerDescription");
    if (label) label->setString(processor.getCurrentAdventurer()->getDescription());

    //update wather level
    waterLevel.setCurrent(processor.getFloodLevel());

    //update menu items
    menu.updateStatuses(processor.getAvailableActions(processor.getCurrentAdventurer()));
    currentAction = "";
    return true;
}

bool MMainScene::startMove() {
    MAdventurer* adventurer = processor.getCurrentAdventurer();
    if (!adventurer) return false;
    logStream << "Acton 'start move' called for: " << adventurer->getName() << std::endl;

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

bool MMainScene::startAbfluss(bool itemUse) {
    MAdventurer* adventurer = processor.getCurrentAdventurer();
    if (!adventurer) return false;
    logStream << "Acton 'start abfluss' called for: " << adventurer->getName() << std::endl;

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
    if (adventurer->canUseDiagonal()) {
        areas = adventurer->getArea()->getAllFloodedNeighbors();
        std::cout << "adventurer can use diagonal" << std::endl;
    }
    else {
        areas = adventurer->getArea()->getDirectFloodedNeighbors();
        std::cout << "adventurer can not use diagonal" << std::endl;
    }
    if (adventurer->getArea()->getFloodLevel() == 1) areas.push_back(adventurer->getArea());
    if (areas.empty()) {
        std::cout << " [MainScene] no available to abfluss areas" << std::endl;
        if (itemUse) {
            MCard* card = adventurerHand2[adventurer]->getReleasedCard();
            adventurerHand2[adventurer]->clearReleasedCard();
            adventurerHand2[adventurer]->showCard(card);
            adventurerHand2[adventurer]->enable();
            advMenu.enable();
            currentAction = "";
        }
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
    if(!itemUse) currentAction = "abfluss";
    else currentAction = "use_card_sandbag_abfluss";

    return true;
}

bool MMainScene::extract() {
    MAdventurer* adventurer = processor.getCurrentAdventurer();
    if (!adventurer) return false;
    logStream << "Acton 'extract' called for: " << adventurer->getName() << std::endl;

    if (processor.actionNumberLimitReached()) {
        std::cout << "Can't action. Limit reached" << std::endl;
        return false;
    }

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
    MAdventurer* adventurer = processor.getCurrentAdventurer();
    logStream << "Acton 'get artifact' called for: " << adventurer->getName() << std::endl;

    if (processor.actionNumberLimitReached()) {
        std::cout << "Can't action. Limit reached" << std::endl;
        return false;
    }
    
    std::map<std::string, MObject*> artifacts = processor.getArtifacts();
    for (std::map<std::string, MObject*>::iterator it = artifacts.begin(); it != artifacts.end(); it++) {
        std::vector<MCard*> artifactCards = adventurer->getArtifactCards(it->first);
        if (artifactCards.size() >= 4) {
            if(!processor.execFunction("getartifact", adventurer->getName() + " " + it->first)) return false;
            for (int i=0; i < 4; i++) {
                MHand2* hand2 = adventurerHand2[adventurer];
                if (!hand2->removeCardByMask(it->first)) return false; //hand not matter real card name
                //itemDeck.setTopCard("itm_" + cardFrame[processor.getItemDropDeck().front()]);//need to fix prefix
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
    logStream << "Discard card(s) for: " << adventurer->getName() << std::endl;
    
    int removed = 0;
    cards.sort();//important!

    std::cout << "Discard card(s) for: " << adventurer->getName() << std::endl;
    std::vector<MCard*> cardsR = adventurer->getAllCards();
    if (!adventurer) return false;
    if (cards.back() >= cardsR.size()) return false;

    MHand2* hand2 = adventurerHand2[adventurer];
    MCard* card;

    for (std::list<int>::iterator it = cards.begin(); it != cards.end(); it++) {
        card = adventurer->getCardByNumber(*it - removed);//wrong card after delete
        if (!card) return false;
        if (!processor.execFunction("discard", adventurer->getName() + " " + card->getName())) return false; //error!
        if (!hand2->removeCard(card)) return false; //hand not matter real card name
        //itemDeck.setTopCard("itm_" + cardFrame[processor.getItemDropDeck().front()]);//need to fix prefix
        removed++;
    }
    currentAction = "";
    return true;
}

bool MMainScene::startHandover() {
    MAdventurer* srcAdventurer = processor.getCurrentAdventurer();
    if (!srcAdventurer) return false;
    logStream << "Acton 'start handover' called for: " << srcAdventurer->getName() << std::endl;

    if (processor.actionNumberLimitReached()) {
        std::cout << "Can't action. Limit reached" << std::endl;
        return false;
    }
    if (adventurerHand2.size() <= 1) {
        std::cout << "Adventurer is alone" << std::endl;
        return false;
    }

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
    logStream << "Submit handover" << std::endl;

    if (!adventurer || cardNumber < 0 || cardNumber >= 5) {
        return false;
    }
    MAdventurer* srcAdventurer = processor.getCurrentAdventurer();
    MCard* srcCard = srcAdventurer->getCardByNumber(cardNumber);
    if (!processor.execFunction("handover", srcAdventurer->getName() + " " + adventurer->getName() + " " + srcCard->getName())) return false;
    adventurerHand2[srcAdventurer]->removeCard(srcCard);
    adventurerHand2[adventurer]->addCard(srcCard);

    //NOT TESTED!
    //if destination adventurer hand has more than 5 cards - must be call drop card scene
    if (adventurerHand2[adventurer]->getCardsNumber() > 5) {
        std::cout << "Drop cards after submit for: " << adventurer->getName() << std::endl;
        MDropCardScene* dropCardScene = (MDropCardScene*)MDropCardScene::createScene();
        if (!dropCardScene) return false;
        dropCardScene->setMainScene(this);
        if (!dropCardScene->setCardFrames(adventurerHand2[adventurer]->getCardFrames())) return false;
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
    logStream << "Acton 'start fly' called for pilot" << std::endl;

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
    logStream << "Acton 'start swim' called for diver" << std::endl;

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

bool MMainScene::startMoveOther(bool itemUse) {
    if (processor.getCurrentAdventurer()->getName() != "navigator") return false;
    logStream << "Acton 'start move other' called for navigator" << std::endl;

    MAdventurer* adventurer;
    cocos2d::Sprite* sp;
    updateAreas();
    gridMap.clearAreaLimit();
    std::vector<std::string> activeAdventurers = processor.getActiveAdventurers();
    for(int i=0; i<activeAdventurers.size(); i++) {
        if(activeAdventurers[i] == processor.getCurrentAdventurer()->getName()) continue;
        adventurer = processor.findAdventurer(activeAdventurers[i]);
        if (!adventurer) return false;
        gridMap.addAreaLimit(adventurer->getArea()->getIndex()[0] * gridSize + adventurer->getArea()->getIndex()[1]);
        sp = gridMap.getSpriteByAreaName(adventurer->getArea()->getName());
        if (!sp) return false;
        sp->setColor(cocos2d::Color3B(128, 255, 128));
    }
    if(!itemUse) currentAction = "moveOther_selectAdventurer";
    else currentAction = "moveOther_selectAdventurer_helicopter";
    return true;
}

bool MMainScene::selectDoubleCard(const std::string& value) {
    doubleCardMenu.hide();
    if (value == "card9") {
        //not tested
        MAdventurer* adventurer = processor.getCurrentAdventurer();
        MCard* card = adventurerHand2[adventurer]->getReleasedCard();
        if (!card) return false;
        if (!processor.execFunction("usedoublecard", processor.getCurrentAdventurer()->getName() + " " + card->getName() + " extract")) {
            std::cout << "[MainScene] failed to extract!" << std::endl;
            return false;
        }
        MEndScene* endScene = (MEndScene*)MEndScene::createScene();
        if (!endScene) return false;
        endScene->setMainScene(this);
        endScene->setMessage("Congratulations!");
        Director::getInstance()->pushScene(endScene);
        return true;
    }
    if (value == "card10") {
        //not tested
        startMoveOther(true);
        return true;
    }
    return false;
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
        adventurerSprite[it->first]->setColor(cocos2d::Color3B(255, 255, 255));
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
    adventurerHand2.clear();
    MAdventurer* adventurer;
    std::vector<std::string> activeAdventurers = processor.getActiveAdventurers();
    if (activeAdventurers.empty()) return false;
    logStream << "Init cards in hands. Adventurers: " << activeAdventurers.size() << std::endl;
    for (int i=0; i < activeAdventurers.size(); i++) {
        logStream << "Init adventurer hand: " << activeAdventurers[i] << std::endl;
        adventurer = processor.findAdventurer(activeAdventurers[i]);
        if (!adventurer) return false;
        adventurerHand2[adventurer] = hands2[i];
        if (!adventurerHand2[adventurer]->initCards(this, adventurer, -250)) return false;
        if (i > 0) {
            hands2[i]->hide();
        }
    }
    return true;
}

bool MMainScene::initVisual() {
    logStream << "Create selection srpires" << std::endl;
    if (!createAnimSpriteFromPlist(this, "anim/advsel.plist", "adv_selection", "advs", 4, 0.2f)) {
        return false;
    }
    this->getChildByName("adv_selection")->setLocalZOrder(5);
    this->getChildByName("adv_selection")->setVisible(false);

    if (!createAnimSpriteFromPlist(this, "anim/out.plist", "selection", "pt", 4, 0.2f)) {
        return false;
    }
    this->getChildByName("selection")->setPosition(0, 0);
    this->getChildByName("selection")->setScale(1.5);//1.0
    this->getChildByName("selection")->setVisible(false);

    //artifacts
    cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
    if (!cache) return false;

    logStream << "Create artifacts sprites" << std::endl;
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

    logStream << "Create water panel" << std::endl;
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

    logStream << "Create decks" << std::endl;
    cache->addSpriteFramesWithFile("anim/cards2.plist");//cards.plist
    cache->addSpriteFramesWithFile("anim/floods.plist");
    //decks
    if (!itemDeck.create(this, "Item deck", cocos2d::Vec2(800, 170), "itm_no_right", "item")) return false;//850
    if (!itemDeck.setCardNames("itm_card%d", "itm_back")) return false;
    if (!floodDeck.create(this, "Flood deck", cocos2d::Vec2(600, 170), "fld_no_right", "flood")) return false;//650
    if (!floodDeck.setCardNames("fld_card%d", "fld_back")) return false;

    logStream << "Create grid" << std::endl;
    //grid
    if (!gridMap.create(this, "anim/cells.plist", gridSize, 24, cocos2d::Size(250, 200), cocos2d::Size(96, 96))) return false;

    logStream << "Create adventurers menu and empty hands" << std::endl;
    //hands + adventurers sprites
    std::map<std::string, MObject*> adventurers = processor.getAdventurers();
    std::vector<std::string> advs;
    int i = 0;
    for (std::map<std::string, MObject*>::iterator it = adventurers.begin(); it != adventurers.end(); it++) {
        hands2.push_back(new MHand2);
        if (!createAnimSpriteFromPlist(this, "anim/adven1.plist", it->first + "_anim", "adven", 4, 0.2f)) {
            return false;
        }
        adventurerSprite[it->first] = (cocos2d::Sprite*) this->getChildByName(it->first + "_anim");
        adventurerSprite[it->first]->setPosition(0, 0);
        adventurerSprite[it->first]->setVisible(false);
        adventurerSprite[it->first]->setScale(2.0);
        advs.push_back(it->first);
    }

    //adventurers menu
    if (!advMenu.create(this, "anim/adventurers.plist", advs)) return false;
    advs.clear();

    logStream << "Create menu" << std::endl;
    //menu
    if (!menu.create(this)) return false;

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

    logStream << "Create labels" << std::endl;
    cocos2d::Label* label;
    label = Label::createWithTTF("some_name", "fonts/Marker Felt.ttf", 24);
    if (!label) return false;
    label->setName("lblAdventurerName");
    label->setAnchorPoint(cocos2d::Vec2(0, 0));
    label->setPosition(20, 28);
    this->addChild(label);
    label = Label::createWithTTF("some_description", "fonts/Marker Felt.ttf", 24);
    if (!label) return false;
    label->setName("lblAdventurerDescrition");
    label->setAnchorPoint(cocos2d::Vec2(0, 0));
    label->setPosition(20, 4);
    this->addChild(label);

    cocos2d::Label* actLabel = Label::createWithTTF("Action: 0", "fonts/Marker Felt.ttf", 24);
    if (!actLabel) return false;
    actLabel->setName("lblActionNumber");
    actLabel->setPosition(250, 740);//250,25
    this->addChild(actLabel);
    
    if(!doubleCardMenu.create(this, cocos2d::Vec2(180, 250))) return false;

    return true;
}

bool MMainScene::reset() {
    std::cout << "##############################################################################" << std::endl;
    std::cout << "###  Next start" << std::endl;
    std::cout << "##############################################################################" << std::endl;

    
    itemDeck.reset();
    floodDeck.reset();
    waterLevel.reset();

    processor.setAdventurersNumber(3);

    logStream << "##############################################################################" << std::endl;
    logStream << "Exec start" << std::endl;
    if (!processor.execFunction("start")) return false;
    logStream << "Init grid" << std::endl;
    if (!gridMap.init()) return false;
    logStream << "Update areas" << std::endl;
    if (!updateAreas()) return false;
    gridMap.clearAreaLimit();
    logStream << "Init adventurers" << std::endl;
    if (!initAdventurers()) return false;
    logStream << "Init hand" << std::endl;
    if (!initHand()) return false;//error here in release

    floodDeck.setTopCard(floodSprite[processor.getFloodDropDeck().front()]);
    //show adventurer name
    cocos2d::Label* label;
    MAdventurer* adventurer = processor.getCurrentAdventurer();
    label = (cocos2d::Label*)this->getChildByName("lblAdventurerName");
    if (label) label->setString(adventurer->getNameU());
    label = (cocos2d::Label*)this->getChildByName("lblAdventurerDescrition");
    if (label) label->setString(adventurer->getDescription());
    currentAction = "";
    moveAdventurer = "";
    abflussNumber = 0;

    logStream << "Init adventurers menu" << std::endl;
    if (!advMenu.init(processor.getActiveAdventurers())) return false;
    advMenuAdventurer = adventurer;
    adventurerSprite[adventurer->getName()]->setColor(cocos2d::Color3B(255, 128, 128));
    advMenu.selectByName(adventurer->getName());
    this->getChildByName("adv_selection")->setPosition(advMenu.getAdventurerMenuImage(adventurer->getName())->getPosition());
    this->getChildByName("adv_selection")->setVisible(true);

    //update menu items
    logStream << "Set available menu actions" << std::endl;
    menu.updateStatuses(processor.getAvailableActions(adventurer));

    return true;
}

void MMainScene::adventurerClicked(MAdventurer* adventurer) {
    logStream << "Adventurer menu clicked" << std::endl;
    if (!adventurer) return;
    for (std::map<MAdventurer*, MHand2*>::iterator it = adventurerHand2.begin(); it != adventurerHand2.end(); it++) {
        it->second->hide();
    }
    adventurerHand2[adventurer]->show();
    advMenuAdventurer = adventurer;
    //change label
    cocos2d::Label* label;
    label = (cocos2d::Label*)this->getChildByName("lblAdventurerName");
    if (label) label->setString(advMenuAdventurer->getNameU());
    label = (cocos2d::Label*)this->getChildByName("lblAdventurerDescrition");
    if (label) label->setString(advMenuAdventurer->getDescription());
    this->getChildByName("adv_selection")->setPosition(advMenu.getAdventurerMenuImage(adventurer->getName())->getPosition());
}

void MMainScene::adventurerClicked(const std::string& name) {
    adventurerClicked(processor.findAdventurer(name));
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

MHand2* MMainScene::getAdventurerHand2(MAdventurer* adventurer) {
    if (adventurerHand2.find(adventurer) != adventurerHand2.end()) return adventurerHand2[adventurer];
    return nullptr;
}

MAdventurerMenu* MMainScene::getAdventurerMenu() {
    return &advMenu;
}

bool MMainScene::init() {
    if (!Scene::init()) {
        return false;
    }

    logStream.open("game.log", std::ios::out);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //speed = cocos2d::Vec2(2, 2);
    gridSize = 6;//need get from processor

    logStream << "Init mouse" << std::endl;
    cocos2d::EventListenerMouse* mouseListener = EventListenerMouse::create();
    mouseListener->onMouseMove = CC_CALLBACK_1(MMainScene::onMouseMove, this);
    mouseListener->onMouseDown = CC_CALLBACK_1(MMainScene::onMouseDown, this);
    mouseListener->onMouseUp = CC_CALLBACK_1(MMainScene::onMouseUp, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    logStream << "Init keyboard" << std::endl;
    cocos2d::EventListenerKeyboard* keybordListener = EventListenerKeyboard::create();
    keybordListener->onKeyPressed = CC_CALLBACK_2(MMainScene::onKeyPressed, this);
    keybordListener->onKeyReleased = CC_CALLBACK_2(MMainScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keybordListener, this);

    if (!initVisual()) return false;

    logStream << "Schedule update" << std::endl;
    this->scheduleUpdate();

    return true;
}

void MMainScene::update(float delta) {
    updateActionNumber();
    adventurerHand2[advMenuAdventurer]->update(delta);
}

/*
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
*/

void  MMainScene::lbmGridProcess(cocos2d::Event* event) {
    cocos2d::Vec2 cell = gridMap.getCellUnderMouse(event);
    if (!gridMap.cellCheck(cell)) {
        return;
    }

    MAdventurer* adventurer = processor.getCurrentAdventurer();
    if (currentAction == "move" || currentAction == "fly" || currentAction == "swim") {
        if (gridMap.getCellByCoordinates(adventurerSprite[adventurer->getName()]->getPosition()) == cell) {
            std::cout << "[MainScene] Can't " << currentAction << " to same cell" << std::endl;
            return;
        }
        if (!processor.execFunction(currentAction, adventurer->getName() + " " + processor.getAreaByIndex(cell.x, cell.y)->getName())) {
            std::cout << "[MainScene] failed to " << currentAction << " adventurer!" << std::endl;
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
        updateAreas();
        currentAction = "";
        return;
    }
    if (currentAction == "abfluss") {
        abflussNumber++;
        if (!processor.execFunction("abfluss", adventurer->getName() + " " + processor.getAreaByIndex(cell.x, cell.y)->getName(), abflussNumber - 2)) {
            std::cout << "[MainScene] failed to abluss by adventurer!" << std::endl;
            return;
        }
        if (adventurer->getName() == "engineer" && abflussNumber < 2) {
            std::cout << "[MainScene] call second abluss by engineer" << std::endl;
            menu.updateStatuses(processor.getAvailableActions(processor.getCurrentAdventurer()));
            updateAreas();
            currentAction = "";
            startAbfluss();
            return;
        }
        //update menu items
        menu.updateStatuses(processor.getAvailableActions(processor.getCurrentAdventurer()));
        updateAreas();
        currentAction = "";
        abflussNumber = 0;
        return;
    }
    if(currentAction == "use_card_sandbag_abfluss") {
        MCard* card = adventurerHand2[adventurer]->getReleasedCard();
        if (!card) return;
        if (!processor.execFunction("usecard", adventurer->getName() + " " + card->getName() + " " + processor.getAreaByIndex(cell.x, cell.y)->getName())) {
            std::cout << "[MainScene] failed to use card sandbag by adventurer!" << std::endl;
            return;
        }
        adventurerHand2[adventurer]->clearReleasedCard();
        adventurerHand2[adventurer]->removeCard(card);
        adventurerHand2[adventurer]->enable();
        advMenu.enable();
        updateAreas();
        currentAction = "";
        return;
    }
    if (currentAction.find("moveOther_selectAdventurer") != std::string::npos) {
    //if (currentAction == "moveOther_selectAdventurer") {
        //need change logic of selection
        //if area hold more than one adventurer - first will be selected
        moveAdventurer = "";
        MArea* area = processor.getAreaByIndex(cell.x, cell.y);
        if (!area) return;
        std::vector<std::string> activeAdventurers = processor.getActiveAdventurers();
        for (int i = 0; i < activeAdventurers.size(); i++) {
            if (activeAdventurers[i] == processor.getCurrentAdventurer()->getName()) continue;
            adventurer = processor.findAdventurer(activeAdventurers[i]);
            if (adventurer) {
                if (adventurer->getArea() == area) {
                    moveAdventurer = adventurer->getName();
                    std::cout << "Selected to moveOther action: " << moveAdventurer << std::endl;
                    break;
                }
            }
        }
        if (moveAdventurer == "") {
            currentAction = "";
            return;
        }

        updateAreas();
        gridMap.clearAreaLimit();

        std::list<MArea*> areas;
        if (currentAction == "moveOther_selectAdventurer") {
            areas = adventurer->getArea()->getDirectActiveNeighbors2();
            if (areas.empty()) {
                std::cout << " [MainScene] no available to move areas" << std::endl;
                return;
            }
        }
        if (currentAction == "moveOther_selectAdventurer_helicopter") {
            areas = processor.getActiveAreas();
            if (areas.empty()) {
                std::cout << " [MainScene] no available to move areas" << std::endl;
                return;
            }
        }
        cocos2d::Sprite* sp;
        for (std::list<MArea*>::iterator it = areas.begin(); it != areas.end(); it++) {
            gridMap.addAreaLimit((*it)->getIndex()[0] * gridSize + (*it)->getIndex()[1]);
            sp = gridMap.getSpriteByAreaName((*it)->getName());
            if (!sp) return;
            sp->setColor(cocos2d::Color3B(128, 255, 128));
        }

        if (currentAction == "moveOther_selectAdventurer") currentAction = "moveOther_selectArea";
        if (currentAction == "moveOther_selectAdventurer_helicopter") currentAction = "moveOther_selectArea_helicopter";
        return;
    }
    if (currentAction.find("moveOther_selectArea") != std::string::npos) {
    //if (currentAction == "moveOther_selectArea") {
        if (moveAdventurer == "") {
            currentAction = "";
            return;
        }

        if (gridMap.getCellByCoordinates(adventurerSprite[moveAdventurer]->getPosition()) == cell) {
            std::cout << "[MainScene] Can't move other to same cell" << std::endl;
            return;
        }

        if (currentAction == "moveOther_selectArea") {
            if (!processor.execFunction("moveother", processor.getCurrentAdventurer()->getName() + " " + moveAdventurer + " " + processor.getAreaByIndex(cell.x, cell.y)->getName())) {
                std::cout << "[MainScene] failed to move other adventurer!" << std::endl;
                return;
            }
        }
        if (currentAction == "moveOther_selectArea_helicopter") {
            MCard* card = adventurerHand2[adventurer]->getReleasedCard();
            if (!card) return;
            if (!processor.execFunction("usedoublecard", processor.getCurrentAdventurer()->getName() + " " + card->getName() + " moveother " + moveAdventurer + " " + processor.getAreaByIndex(cell.x, cell.y)->getName())) {
                std::cout << "[MainScene] failed to move other adventurer!" << std::endl;
                return;
            }
        }

        int pos[2];
        int num = getAdventurerNumber(moveAdventurer);
        pos[0] = (num / 2);
        pos[0] = pos[0] * 32 - 16;
        pos[1] = 16 - (num % 2) * 32;
        cocos2d::Vec2 advPos = gridMap.getSpriteByCell(cell.x, cell.y)->getPosition();
        adventurerSprite[moveAdventurer]->setPosition(advPos.x + pos[0], advPos.y + pos[1]);

        //update menu items
        menu.updateStatuses(processor.getAvailableActions(processor.getCurrentAdventurer()));
        updateAreas();
        currentAction = "";
        return;
    }
}
void  MMainScene::rbmGridProcess(cocos2d::Event* event) {
    logStream << "RBM clicked. Clear" << std::endl;
    updateAreas();
    gridMap.clearAreaLimit();
    menu.unselectMenuAll();
    currentAction = "";
    moveAdventurer = "";
    return;
}

void MMainScene::onMouseMove(cocos2d::Event* event) {
    adventurerHand2[advMenuAdventurer]->onMouseMove(event);
    if (adventurerHand2[advMenuAdventurer]->getCardHold()) return;

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
        adventurerHand2[advMenuAdventurer]->onMouseDown(event);
        lbmGridProcess(event);
    }
    if (mouseEvent->getMouseButton() == cocos2d::EventMouse::MouseButton::BUTTON_RIGHT) {
        if (currentAction.find("use_card") != std::string::npos || currentAction == "moveOther_selectAdventurer_helicopter" || currentAction == "moveOther_selectArea_helicopter") {
            MAdventurer* adventurer = processor.getCurrentAdventurer();
            MCard* card = adventurerHand2[adventurer]->getReleasedCard();
            adventurerHand2[adventurer]->clearReleasedCard();
            adventurerHand2[adventurer]->showCard(card);
            adventurerHand2[adventurer]->enable();
            advMenu.enable();
            doubleCardMenu.hide();
        }
        rbmGridProcess(event);
    }
}

void MMainScene::onMouseUp(cocos2d::Event* event) {
    adventurerHand2[advMenuAdventurer]->onMouseUp(event);
    MAdventurer* adventurer = processor.getCurrentAdventurer();
    if (!adventurer) return;
    if (advMenuAdventurer == adventurer && currentAction == "") {
        MCard* card = adventurerHand2[adventurer]->getReleasedCard();
        if (!card) return;
        if (card->getType() == "item") {
            advMenu.disable();
            adventurerHand2[adventurer]->disable();
            adventurerHand2[adventurer]->hideCard(card);
            if (card->getName().find("sandbag") != std::string::npos) {
                //set currentAction in startAbfluss function
                startAbfluss(true);
            }
            if (card->getName().find("helicopter") != std::string::npos) {
                bool canExtract = false;
                if (adventurer->hasCard("helicopter") && processor.allActiveAdventurersOnArea("adventurers_circle") && processor.allArifactsCollected()) {
                    canExtract = true;
                }
                if (!doubleCardMenu.reset("card9", "card10", "card11", canExtract, true)) return;
                currentAction = "use_card_helicopter";
            }
        }
    }
}

void MMainScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
        logStream << "Main menu called" << std::endl;
        MMainMenuScene* mainMenuScene = (MMainMenuScene*)MMainMenuScene::createScene();
        if (!mainMenuScene) return;
        if (!mainMenuScene->create(this, false)) return;
        Director::getInstance()->pushScene(mainMenuScene);
    }
}

void MMainScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
}