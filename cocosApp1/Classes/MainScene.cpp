#include "MainScene.h"
#include "EndScene.h"
#include "DropCardScene.h"
#include "MenuItemImageExt.h"
#include "utils.h"
#include "logic/area.h"
#include "logic/adventurer.h"
#include "logic/card.h"
#include <iostream>

USING_NS_CC;

Scene* MMainScene::createScene() {
    return MMainScene::create();
}

MProcessor* MMainScene::getProcessor() {
    return &processor;
}

bool MMainScene::endTurn() {
    //exec function
    if(!processor.execFunction("endturn", processor.getCurrentAdventurer()->getName())) return false;

    //update hand
    std::vector<MCard*> cards = processor.getCurrentAdventurer()->getAllCards();
    for (int i = hand.getUsedSize(); i < cards.size(); i++) {
        if (!hand.addCard(cardFrame[cards[i]->getName()])) return false;//here
    }

    //display top card at flood drop deck
    if (!processor.getFloodDropDeck().empty()) floodDeck.setTopCard(floodSprite[processor.getFloodDropDeck().front()]);
    else floodDeck.reset();

    //update areas
    updateAreas();
    //display name of next adventurer
    cocos2d::Label* advLabel = (cocos2d::Label*)this->getChildByName("lblAdventurerName");
    if (advLabel) advLabel->setString(processor.getCurrentAdventurer()->getName());

    if (processor.adventureFailed()) {
        MEndScene* endScene = (MEndScene*)MEndScene::createScene();
        if (!endScene) return false;
        endScene->setMainScene(this);
        endScene->setMessage("Adventure failed!");
        Director::getInstance()->pushScene(endScene);
    }

    if (hand.getUsedSize() > 5) {
        MDropCardScene* dropCardScene = (MDropCardScene*)MDropCardScene::createScene();
        if (!dropCardScene) return false;
        dropCardScene->setMainScene(this);
        if (!dropCardScene->setCards(hand.getNotEmptyCards())) return false;
        Director::getInstance()->pushScene(dropCardScene);
    }

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

    //pg.setDiagonalMovement(false);

    updateAreas();
    gridMap.clearAreaLimit();

    cocos2d::Sprite* sp;
    std::list<MArea*> areas;
    MAdventurer* adventurer = processor.getCurrentAdventurer();
    if (!adventurer) return false;
    if (adventurer->canUseDiagonal()) {
        areas = adventurer->getArea()->getAllActiveNeighbors();
        //pg.setDiagonalMovement(true);
    }
    else areas = adventurer->getArea()->getDirectActiveNeighbors();
    if (areas.empty()) {
        //currentAction = "";
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
    if (processor.getCurrentAdventurer()->hasCard("helicopter") && processor.allActiveAdventurersOnArea("adventurers_circle") && processor.allArifactsCollected()) {
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
bool MMainScene::skip() {
    if (processor.actionNumberLimitReached()) {
        std::cout << "Can't action. Limit reached" << std::endl;
        return false;
    }

    if (!processor.execFunction("skip", processor.getCurrentAdventurer()->getName())) return false;
    currentAction = "";

    return true;
}

bool MMainScene::getArtifact() {
    //todo
    currentAction = "";
    return false;
}

bool MMainScene::discard(std::list<int> cards) {
    int removed = 0;
    for (std::list<int>::iterator it = cards.begin(); it != cards.end(); it++) {
        MAdventurer* adventurer = processor.getCurrentAdventurer();
        if (!adventurer) return false;
        MCard* card = processor.getCurrentAdventurer()->getCardByNumber(*it - removed);
        if (!card) return false;
        if (!processor.execFunction("discard", adventurer->getName() + " " + card->getName())) return false;
        hand.removeCard(*it - removed);
        itemDeck.setTopCard("itm_" + cardFrame[processor.getItemDropDeck().front()]);//need to fix prefix
        removed ++;
    }
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
            //pg.removeCollision(NVector2(area->getIndex()[0], area->getIndex()[1]));
            break;
        case 1:
            sp->setVisible(true);
            sp->setColor(blue);
            //pg.removeCollision(NVector2(area->getIndex()[0], area->getIndex()[1]));
            break;
        case 2:
            sp->setVisible(false);
            //pg.addCollision(NVector2(area->getIndex()[0], area->getIndex()[1]));
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
    std::vector<std::string> adventurers = processor.getActiveAdventurers();
    for (int i = 0; i < adventurers.size(); i++) {
        MAdventurer* adventurer = processor.findAdventurer(adventurers[i]);
        if (!adventurer) return false;
        MArea* area = processor.findArea(adventurer->getStartArea());
        if (!area) return false;
        cocos2d::Sprite* sp = gridMap.getSpriteByCell(area->getIndex()[0], area->getIndex()[1]);
        if (!sp) return false;
        this->getChildByName("anim_fox")->setPosition(sp->getPosition());
        this->getChildByName("anim_fox")->setVisible(true);
        gridMap.setCurrentCell(gridMap.getCellByCoordinates(sp->getPosition()));
        break;
    }
    return true;
}

bool MMainScene::initHand() {
    if(!hand.reset()) return false;
    MAdventurer* adventurer = processor.getCurrentAdventurer();
    std::vector<MCard*> cards = adventurer->getAllCards();
    for (int i = 0; i < cards.size(); i++) {
        if(!hand.addCard(cardFrame[cards[i]->getName()])) return false;
    }
    return true;
}

/*
bool MMainScene::initPathGenerator() {
    pg.clearCollisions();
    //need make nicer (in sitaution when map is not standard type)
    for (int i = 0; i < 2; i++) {
        //x
        pg.addCollision(NVector2((gridSize - 1) * i - 0, 0));
        pg.addCollision(NVector2((gridSize - 1) * i - 0, 1));
        pg.addCollision(NVector2((gridSize - 1) * i - 1, 0));
        //y
        pg.addCollision(NVector2((gridSize - 1) * i - 0, (gridSize - 1)));
        pg.addCollision(NVector2((gridSize - 1) * i - 1, (gridSize - 1)));
        pg.addCollision(NVector2((gridSize - 1) * i - 0, (gridSize - 2)));
    }
    return true;
}
*/

bool MMainScene::initVisual() {
    if (!createAnimSpriteFromPlist(this, "anim/out.plist", "selection", "pt", 4, 0.2f)) {
        return false;
    }
    this->getChildByName("selection")->setPosition(0, 0);
    this->getChildByName("selection")->setScale(1.5);//1.0
    this->getChildByName("selection")->setVisible(false);
    if (!createAnimSpriteFromPlist(this, "anim/fox.plist", "anim_fox", "fox_pt", 4, 0.1f)) {
        return false;
    }
    this->getChildByName("anim_fox")->setPosition(0, 0);//(394, 332);//200 + 48*3
    this->getChildByName("anim_fox")->setVisible(false);
    this->getChildByName("anim_fox")->setScale(2.0);
    this->getChildByName("anim_fox")->setVisible(true);

    if (!createAnimSpriteFromPlist(this, "anim/water.plist", "anim_water", "water", 4, 0.1f)) {
        return false;
    }
    this->getChildByName("anim_water")->setVisible(false);
    cocos2d::Sprite* waterBack = Sprite::create("water_back.png");
    waterBack->setVisible(false);
    this->addChild(waterBack, 0, "water_back");
    cocos2d::Sprite* glass = Sprite::create("glass.png");
    //bad realization (must be calculated)
    glass->setPosition(cocos2d::Vec2(964, 592));//592//624
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
    if (!itemDeck.create(this, "anim/cards.plist", "Item deck", cocos2d::Vec2(850, 170), itemCards, "itm_no_right")) return false;
    if (!itemDeck.setCardNames("itm_card%d", "itm_back")) return false;
    if (!floodDeck.create(this, "anim/floods.plist", "Flood deck", cocos2d::Vec2(650, 170), floodCards, "fld_no_right")) return false;
    if (!floodDeck.setCardNames("fld_card%d", "fld_back")) return false;

    //pg.setWorldSize(NVector2(gridSize, gridSize));
    //pg.setDiagonalMovement(false);

    if (!gridMap.create(this, "anim/cells.plist", gridSize, 24, cocos2d::Size(250, 200), cocos2d::Size(96, 96))) return false;

    if (!hand.create(this, 5, 7, "hand%d", "card1", cocos2d::Vec2(100, 100))) return false; //maybe 5+2?
    if (!menu.create(this)) return false;

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

    if (!processor.execFunction("start")) return false;
    if (!gridMap.init()) return false;
    //initPathGenerator();
    if (!updateAreas()) return false;
    gridMap.clearAreaLimit();
    if (!initAdventurers()) return false;
    if (!initHand()) return false;
    floodDeck.setTopCard(floodSprite[processor.getFloodDropDeck().front()]);
    //clear all collisions except standard one
    //show adventurer name
    cocos2d::Label* advLabel = (cocos2d::Label*)this->getChildByName("lblAdventurerName");
    if (advLabel) advLabel->setString(processor.getCurrentAdventurer()->getName());
    currentAction = "";

    return true;
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
    //is it good to be here?
    waterLevel.setCurrent(processor.getFloodLevel());
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
        if (cell == gridMap.getCurrentCell()) {
            return;
        }
        if (currentAction == "move") {
            if (!processor.execFunction("move", processor.getCurrentAdventurer()->getName() + " " + processor.getAreaByIndex(cell.x, cell.y)->getName())) {
                std::cout << "[MainScene] failed to move adventurer!" << std::endl;
                return;
            }
            getChildByName("anim_fox")->setPosition(gridMap.getSpriteByCell(cell.x, cell.y)->getPosition());
            currentAction = "";
        }
        if (currentAction == "abfluss") {
            if (!processor.execFunction("abfluss", processor.getCurrentAdventurer()->getName() + " " + processor.getAreaByIndex(cell.x, cell.y)->getName())) {
                std::cout << "[MainScene] failed to abluss by adventurer!" << std::endl;
                return;
            }
            currentAction = "";
        }
    }
    if (mouseEvent->getMouseButton() == cocos2d::EventMouse::MouseButton::BUTTON_RIGHT) {
        //pg.setDiagonalMovement(false);
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
    if (keyCode == EventKeyboard::KeyCode::KEY_D) {
        MDropCardScene* dropCardScene = (MDropCardScene*)MDropCardScene::createScene();
        if (!dropCardScene) return;
        dropCardScene->setMainScene(this);
        if (!dropCardScene->setCards(hand.getNotEmptyCards())) return;
        Director::getInstance()->pushScene(dropCardScene);
    }
}

void MMainScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
}