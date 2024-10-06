#include "HelloWorldScene.h"
#include "MenuItemImageExt.h"
#include "utils.h"

USING_NS_CC;

Scene* HelloWorld::createScene() {
    return HelloWorld::create();
}

bool HelloWorld::init() {
    if(!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    speed = cocos2d::Vec2(2, 2);
    moving = false;

    cocos2d::EventListenerMouse* mouseListener = EventListenerMouse::create();
    mouseListener->onMouseMove = CC_CALLBACK_1(HelloWorld::onMouseMove, this);
    mouseListener->onMouseDown = CC_CALLBACK_1(HelloWorld::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    cocos2d::EventListenerKeyboard* keybordListener = EventListenerKeyboard::create();
    keybordListener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
    keybordListener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keybordListener, this);

    if (!createAnimSpriteFromPlist(this, "anim/out.plist", "selection", "pt", 4, 0.2f)) {
        return false;
    }
    this->getChildByName("selection")->setPosition(0, 0);
    this->getChildByName("selection")->setScale(1.0);
    this->getChildByName("selection")->setVisible(false);
    if (!createAnimSpriteFromPlist(this, "anim/fox.plist", "anim_fox", "fox_pt", 4, 0.1f)) {
        return false;
    }
    this->getChildByName("anim_fox")->setPosition(32, 32);
    this->getChildByName("anim_fox")->setScale(2.0);
    this->getChildByName("anim_fox")->setVisible(true);

    if (!createAnimSpriteFromPlist(this, "anim/water.plist", "anim_water", "water", 4, 0.1f)) {
        return false;
    }
    this->getChildByName("anim_water")->setVisible(false);
    cocos2d::Sprite* waterBack = Sprite::create("water_back.png");
    waterBack->setVisible(false);
    this->addChild(waterBack, 0, "water_back");

    waterCurrent = 0;
    waterLimit = 5;
    waterPos = cocos2d::Vec2(464, 464);
    char buffer[10];
    for (int i = 1; i <= waterLimit; i++) {
        memset(buffer, 0, 10);
        snprintf(buffer, 10, "Level %d", i);
        auto labelLevel = Label::createWithTTF(buffer, "fonts/Marker Felt.ttf", 24);
        labelLevel->setPosition(cocos2d::Vec2(waterPos.x - 70, (waterPos.y - 64) + 64 * i - 1));
        this->addChild(labelLevel);
    }

    if (!createMenu()) {
        return false;
    }
    if (!createDeck()) {
        return false;
    }
    if (!gridMap.createCells(this, 4, 4)) {
        return false;
    }
    pg.setWorldSize(NVector2((int)gridMap.getCellsCount().x, (int)gridMap.getCellsCount().y));
    pg.setDiagonalMovement(false);

    this->scheduleUpdate();

    return true;
}

bool HelloWorld::createDeck() {
    std::map<std::string, std::map<std::string, std::string>> cards = {
        {"back", {{"listName", "card0"}, {"pos", "left"}, {"visible", "1"}, {"zOrder", "2"}}},
        {"no_left", {{"listName", "card1"}, {"pos", "left"}, {"visible", "1"}, {"zOrder", "1"}}},
        {"no_right", {{"listName", "card1"}, {"pos", "right"}, {"visible", "1"}, {"zOrder", "1"}}},
        {"card0", {{"listName", "card2"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
        {"card1", {{"listName", "card3"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
        {"card2", {{"listName", "card4"}, {"pos", "right"}, {"visible", "0"}, {"zOrder", "2"}}},
    };

    auto cache = SpriteFrameCache::getInstance();
    if (!cache) {
        return false;
    }
    cache->addSpriteFramesWithFile("anim/cards.plist");

    cocos2d::Vec2 pos;
    for (auto it = cards.begin(); it != cards.end(); it ++ ) {
        auto sp = Sprite::createWithSpriteFrame(cache->getSpriteFrameByName(cards[it->first]["listName"]));
        if (!sp) {
            return false;
        }
        sp->getTexture()->setAliasTexParameters();
        if (cards[it->first]["pos"] == "left") {
            pos = cocos2d::Vec2(600, 640);
        }
        else if (cards[it->first]["pos"] == "right") {
            pos = cocos2d::Vec2(700, 640);
        }
        else {
            return false;
        }
        sp->setPosition(pos);
        sp->setVisible((bool)std::stoi(cards[it->first]["visible"]));
        this->addChild(sp, std::stoi(cards[it->first]["zOrder"]), it->first);
    }
    lastCard = 0;

    return true;
}

void HelloWorld::update(float delta) {
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

void HelloWorld::menuEndTurnCallback(cocos2d::Ref* pSender) {
    //todo
}

void HelloWorld::menuMoveCallback(cocos2d::Ref* pSender) {
    //todo
}

void HelloWorld::menuAbflussCallback(cocos2d::Ref* pSender) {
    //todo
}

void HelloWorld::menuExitCallback(Ref* pSender) {
    Director::getInstance()->end();
}

void HelloWorld::moveSprite(cocos2d::Sprite* sprite, cocos2d::Vec2 destination) {
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

void HelloWorld::onMouseMove(cocos2d::Event* event) {
    cocos2d::Vec2 cell = gridMap.getCellUnderMouse(event);
    if (!gridMap.cellCheck(cell)) {
        this->getChildByName("selection")->setVisible(false);
        return;
    }
    this->getChildByName("selection")->setPosition(gridMap.getCoordsByCell(cell));
    this->getChildByName("selection")->setVisible(true);
}

void HelloWorld::onMouseDown(cocos2d::Event* event) {
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

void HelloWorld::nextCard() {
    if (lastCard > 2) {
        return;
    }
    char buffer[16] = { 0 };
    snprintf(buffer, 16, "card%d", lastCard);
    this->getChildByName(buffer)->setVisible(true);
    if (lastCard >= 1) {
        snprintf(buffer, 16, "card%d", lastCard - 1);
        this->getChildByName(buffer)->setVisible(false);
    }
    lastCard = lastCard + 1;
    if (lastCard == 3) {
        this->getChildByName("back")->setVisible(false);
    }
}

void HelloWorld::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    if (keyCode == EventKeyboard::KeyCode::KEY_N) {
        nextCard();
    }
    if (keyCode == EventKeyboard::KeyCode::KEY_R) {
        this->getChildByName("back")->setVisible(true);
        this->getChildByName("card0")->setVisible(false);
        this->getChildByName("card1")->setVisible(false);
        this->getChildByName("card2")->setVisible(false);
        lastCard = 0;

        waterCurrent = 0;
        this->getChildByName("anim_water")->setVisible(false);
        this->getChildByName("water_back")->setVisible(false);
    }
    if (keyCode == EventKeyboard::KeyCode::KEY_W) {
        if (waterCurrent < waterLimit) {
            waterCurrent ++;
            this->getChildByName("anim_water")->setPosition(cocos2d::Vec2(waterPos.x, waterPos.y + 64 * (waterCurrent - 1)));
            this->getChildByName("anim_water")->setVisible(true);
            if (waterCurrent > 1) {
                this->getChildByName("water_back")->setPosition(cocos2d::Vec2(waterPos.x, waterPos.y + 32 * (waterCurrent - 2)));
                this->getChildByName("water_back")->setScaleY(waterCurrent - 1);
                this->getChildByName("water_back")->setVisible(true);
            }
        }
    }
}

void HelloWorld::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
}

bool HelloWorld::createMenu() {
    //prepare
    std::unordered_map<std::string, ccMenuCallback> menuCallback;
    menuCallback["End Turn"] = CC_CALLBACK_1(HelloWorld::menuEndTurnCallback, this);
    menuCallback["Move"] = CC_CALLBACK_1(HelloWorld::menuMoveCallback, this);
    menuCallback["Abfluss"] = CC_CALLBACK_1(HelloWorld::menuAbflussCallback, this);
    menuCallback["Exit"] = CC_CALLBACK_1(HelloWorld::menuExitCallback, this);

    float topOffset = this->getContentSize().height - 30;
    const std::string btnBackPng[2] = { "back_off.png", "back_on.png" };
    const std::string fontTTF = "fonts/Marker Felt.ttf";

    //create menu
    cocos2d::Vector<MenuItem*> MenuItems;
    int num = 0;
    cocos2d::Vec2 itemPosition;
    for (auto it = menuCallback.begin(); it != menuCallback.end(); it++) {
        MenuItemImageExt* memuItem = MenuItemImageExt::create(btnBackPng[0], btnBackPng[1], "", it->second);
        if(!memuItem) {
            return false;
        }
        itemPosition = Vec2(memuItem->getContentSize().width / 2, topOffset - num * memuItem->getContentSize().height);
        memuItem->setPosition(itemPosition);
        MenuItems.pushBack(memuItem);
        cocos2d::Label* itemLabel = Label::createWithTTF(it->first, fontTTF, 24);
        if (!itemLabel) {
            return false;
        }
        itemLabel->setPosition(itemPosition);
        this->addChild(itemLabel, 2);
        num ++;
    }
    cocos2d::Menu* menu = Menu::createWithArray(MenuItems);
    if (!menu) {
        return false;
    }
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    menuCallback.clear();
    return true;
}