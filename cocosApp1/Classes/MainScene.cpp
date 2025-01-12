#include "MainScene.h"
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

bool MMainScene::initAreas() {
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
            pg.removeCollision(NVector2(area->getIndex()[0], area->getIndex()[1]));
            break;
        case 1:
            sp->setVisible(true);
            sp->setColor(blue);
            pg.removeCollision(NVector2(area->getIndex()[0], area->getIndex()[1]));
            break;
        case 2:
            sp->setVisible(false);
            pg.addCollision(NVector2(area->getIndex()[0], area->getIndex()[1]));
            break;
        }
    }
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
    cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
    if (!cache) return false;

    std::string handMask = "hand%d";
    char buffer[16];
    cocos2d::Sprite* sp;

    //clear hand
    for (int i = 0; i < 5; i++) {
        memset(buffer, 0, 16);
        sprintf(buffer, handMask.c_str(), i);
        sp = (cocos2d::Sprite*)this->getChildByName(buffer);
        cocos2d::SpriteFrame* frame = cache->getSpriteFrameByName("card1");
        if (sp && frame) sp->setSpriteFrame(frame);
    }

    //fill hand
    std::vector<std::string> adventurers = processor.getActiveAdventurers();
    MAdventurer* adventurer = processor.findAdventurer(adventurers[0]);
    std::vector<MCard*> cards = adventurer->getAllCards();
    for (int i = 0; i < cards.size(); i++) {
        memset(buffer, 0, 16);
        sprintf(buffer, handMask.c_str(), i);
        sp = (cocos2d::Sprite*)this->getChildByName(buffer);
        cocos2d::SpriteFrame* frame = cache->getSpriteFrameByName(cardFrame[cards[i]->getName()]);
        if (sp && frame) sp->setSpriteFrame(frame);
    }
}

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
    this->getChildByName("anim_fox")->setPosition(394, 332);//200 + 48*3
    this->getChildByName("anim_fox")->setVisible(false);
    this->getChildByName("anim_fox")->setScale(2.0);
    this->getChildByName("anim_fox")->setVisible(true);

    if (!createAnimSpriteFromPlist(this, "anim/water.plist", "anim_water", "water", 4, 0.1f)) {
        return false;
    }
    this->getChildByName("anim_water")->setVisible(false);
    //this->getChildByName("anim_water")->setPositionZ(1);
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
        {"itm_card0", {{"listName", "card2"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
        {"itm_card1", {{"listName", "card3"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
        {"itm_card2", {{"listName", "card4"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
        {"itm_card3", {{"listName", "card5"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
        {"itm_card4", {{"listName", "card6"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
        {"itm_card5", {{"listName", "card7"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
        {"itm_card6", {{"listName", "card8"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
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
    if (!itemDeck.create(this, "anim/cards.plist", "Item deck", cocos2d::Vec2(850, 170), itemCards)) return false;
    if (!itemDeck.setCardNames("itm_card%d", "itm_back")) return false;
    if (!floodDeck.create(this, "anim/floods.plist", "Flood deck", cocos2d::Vec2(650, 170), floodCards)) return false;
    if (!floodDeck.setCardNames("fld_card%d", "fld_back")) return false;

    pg.setWorldSize(NVector2(gridSize, gridSize));
    pg.setDiagonalMovement(false);
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

    if (!gridMap.create(this, "anim/cells.plist", gridSize, 24, cocos2d::Size(250, 200), cocos2d::Size(96, 96))) return false;

    if (!hand.create(this, 5, "hand%d", "card1", cocos2d::Vec2(160, 100))) return false;
    if (!menu.create(this, &gridMap)) return false;

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

    return true;
}

bool MMainScene::init() {
    if(!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    speed = cocos2d::Vec2(2, 2);
    moving = false;
    gridSize = 6;

    cocos2d::EventListenerMouse* mouseListener = EventListenerMouse::create();
    mouseListener->onMouseMove = CC_CALLBACK_1(MMainScene::onMouseMove, this);
    mouseListener->onMouseDown = CC_CALLBACK_1(MMainScene::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    cocos2d::EventListenerKeyboard* keybordListener = EventListenerKeyboard::create();
    keybordListener->onKeyPressed = CC_CALLBACK_2(MMainScene::onKeyPressed, this);
    keybordListener->onKeyReleased = CC_CALLBACK_2(MMainScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keybordListener, this);

    //logic
    if (!processor.execFunction("start")) return false;
    //visual
    if (!initVisual()) return false;

    //start
    if (!gridMap.init()) return false;
    initAreas();
    initAdventurers();
    initHand();
    //set flood deck - show last top card
    std::deque<std::string> floodDropDeck = processor.getFloodDropDeck();
    std::cout << "Top card at flood drop deck: "<< floodDropDeck.front() <<std::endl;
    floodDeck.setTopCard(floodSprite[floodDropDeck.front()]);

    this->scheduleUpdate();

    return true;
}

void MMainScene::update(float delta) {
    if (moving) {
        if (path.empty()) {
            moving = false;
        }
        else {
            NVector2 nv2 = path.front();
            gridMap.setCurrentCell(cocos2d::Vec2((float)nv2.x, (float)nv2.y));
            cocos2d::Vec2 destination = gridMap.getNewPoint(nv2.x, nv2.y);
            if (this->getChildByName("anim_fox")->getPosition() == destination) {
                path.erase(path.begin());
            }
            else {
                moveSprite((cocos2d::Sprite*)this->getChildByName("anim_fox"), destination);
            }
        }
    }
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
    if (moving) {
        return;
    }
    cocos2d::Vec2 cell = gridMap.getCellUnderMouse(event);
    if (!gridMap.cellCheck(cell)) {
        return;
    }
    if (cell == gridMap.getCurrentCell()) {
        return;
    }
    path.clear(); 
    path = pg.findPath(NVector2(gridMap.getCurrentCell().x, gridMap.getCurrentCell().y), NVector2(cell.x, cell.y));
    moving = true;
}

void MMainScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    if (keyCode == EventKeyboard::KeyCode::KEY_N) {
        //itemDeck.nextCard();
        //floodDeck.nextCard();
    }
    if (keyCode == EventKeyboard::KeyCode::KEY_R) {
        itemDeck.reset();
        floodDeck.reset();
        waterLevel.reset();
        if(!processor.execFunction("start")) return;
        if(!gridMap.init()) return;
        initAreas();
        initAdventurers();
        initHand();
        //set flood deck - show last top card
        std::deque<std::string> floodDropDeck = processor.getFloodDropDeck();
        std::cout << "Top card at flood drop deck: " << floodDropDeck.front() << std::endl;
        floodDeck.setTopCard(floodSprite[floodDropDeck.front()]);
    }
    if (keyCode == EventKeyboard::KeyCode::KEY_W) {
        waterLevel.increase();
    }
}

void MMainScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
}
