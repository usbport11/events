#include "HelloWorldScene.h"
#include "MenuItemImageExt.h"

USING_NS_CC;

Scene* HelloWorld::createScene() {
    return HelloWorld::create();
}

static void problemLoading(const char* filename) {
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool HelloWorld::createAnimSpriteFromPlist(const std::string& fileName, const std::string& spriteName, const std::string& prefix, int count, float step) {
    if (fileName.empty() || spriteName.empty() || prefix.empty() || count <= 0) {
        return false;
    }

    auto spritecache = SpriteFrameCache::getInstance();
    if (!spritecache) {
        return false;
    }

    Vector<SpriteFrame*> animFrames;
    spritecache->addSpriteFramesWithFile(fileName);
    std::string key;
    char buffer[10];
    for (int i = 0; i < count; i++) {
        memset(buffer, 0, 10);
        snprintf(buffer, 10, "%d", i);
        key = prefix + buffer;
        animFrames.pushBack(spritecache->getSpriteFrameByName(key));
    }

    cocos2d::Sprite* animSprite_fox = Sprite::createWithSpriteFrame(animFrames.front());
    animSprite_fox->setName(spriteName);
    animSprite_fox->getTexture()->setAliasTexParameters();
    cocos2d::Animation* animation_fox = Animation::createWithSpriteFrames(animFrames, step);
    cocos2d::Animate* animate_fox = Animate::create(animation_fox);
    animSprite_fox->runAction(RepeatForever::create(animate_fox));
    this->addChild(animSprite_fox, 1);
    animFrames.clear();

    return true;
}

bool HelloWorld::init() {
    if(!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    offset = cocos2d::Vec2(0, 0);
    cellSize = cocos2d::Vec2(64, 64);
    halfSize = cocos2d::Vec2(32, 32);
    cellsCount = cocos2d::Vec2(4, 4);
    speed = cocos2d::Vec2(2, 2);
    currentCell = cocos2d::Vec2(0, 0);
    gridRect = cocos2d::Rect(0, 0, cellSize.x * cellsCount.x, cellSize.y * cellsCount.y);
    moving = false;

    cocos2d::EventListenerMouse* mouseListener = EventListenerMouse::create();
    mouseListener->onMouseMove = CC_CALLBACK_1(HelloWorld::onMouseMove, this);
    mouseListener->onMouseDown = CC_CALLBACK_1(HelloWorld::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    cocos2d::EventListenerKeyboard* keybordListener = EventListenerKeyboard::create();
    keybordListener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
    keybordListener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keybordListener, this);

    if (!createAnimSpriteFromPlist("anim/out.plist", "selection", "pt", 4, 0.2f)) {
        return false;
    }
    this->getChildByName("selection")->setPosition(0, 0);
    this->getChildByName("selection")->setScale(1.0);
    this->getChildByName("selection")->setVisible(false);
    if (!createAnimSpriteFromPlist("anim/fox.plist", "anim_fox", "fox_pt", 4, 0.1f)) {
        return false;
    }
    this->getChildByName("anim_fox")->setPosition(32, 32);
    this->getChildByName("anim_fox")->setScale(2.0);
    this->getChildByName("anim_fox")->setVisible(true);

    if (!createMenu()) {
        return false;
    }
    if (!createDeck()) {
        return false;
    }
    if (!createCells((int)cellsCount.x, (int)cellsCount.y)) {
        return false;
    }
    pg.setWorldSize(NVector2((int)cellsCount.x, (int)cellsCount.y));
    pg.setDiagonalMovement(false);

    this->scheduleUpdate();

    return true;
}

bool HelloWorld::createCells(int countX, int countY) {
    if (countX < 0 || countY < 0) {
        return false;
    }
    auto cache = SpriteFrameCache::getInstance();
    if (!cache) {
        return false;
    }
    cache->addSpriteFramesWithFile("anim/cell.plist");

    std::string key;
    std::string cellName;
    char buffer[16];
    for (int i = 0; i < countX; i++) {
        for (int j = 0; j < countY; j++) {
            memset(buffer, 0, 16);
            snprintf(buffer, 16, "cell_%d_%d", i, j);
            key = buffer;
            memset(buffer, 0, 16);
            snprintf(buffer, 16, "cell%d", i* countX  + j);
            cellName = buffer;
            auto sp = Sprite::createWithSpriteFrame(cache->getSpriteFrameByName(cellName));
            if (!sp) {
                return false;
            }
            sp->setScale(1.0);
            sp->getTexture()->setAliasTexParameters();
            sp->setPosition(Vec2(offset.x + i * cellSize.x + halfSize.x, offset.y + j * cellSize.y + halfSize.y));
            this->addChild(sp, 0, key);
        }
    }
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
            currentCell = cocos2d::Vec2((float)nv2.x, (float)nv2.y);
            cocos2d::Vec2 destination = cocos2d::Vec2(offset.x + nv2.x * cellSize.x + halfSize.x, offset.y + nv2.y * cellSize.y + halfSize.y);
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

cocos2d::Vec2 HelloWorld::getCellUnderMouse(cocos2d::Event* event) {
    cocos2d::EventMouse* e = (EventMouse*)event;
    cocos2d::Vec2 cell = cocos2d::Vec2(e->getCursorX(), e->getCursorY());
    if (gridRect.containsPoint(cell)) {
        cell = cocos2d::Vec2((int)(cell.x / cellSize.x), (int)(cell.y / cellSize.y));
    }
    else {
        cell = cocos2d::Vec2(-1, -1);
    }
    return cell;
}

void HelloWorld::selectCell(cocos2d::Vec2 cell, std::string name) {
    if (cell.x < 0 || cell.y < 0 || cell.x >= cellsCount.x || cell.y >= cellsCount.y) {
        this->getChildByName(name)->setVisible(false);
        return;
    }
    this->getChildByName(name)->setPosition(getCoordsByCell(cell));
    this->getChildByName(name)->setVisible(true);
}

cocos2d::Vec2 HelloWorld::getCoordsByCell(cocos2d::Vec2 cell) {
    return cocos2d::Vec2(offset.x + cell.x * cellSize.x + halfSize.x, offset.y + cell.y * cellSize.y + halfSize.y);
}

cocos2d::Vec2 HelloWorld::sign(cocos2d::Vec2 vec) {
    return cocos2d::Vec2((0 < vec.x) - (vec.x < 0), (0 < vec.y) - (vec.y < 0));
}

void HelloWorld::moveSprite(cocos2d::Sprite* sprite, cocos2d::Vec2 destination) {
    cocos2d::Vec2 direction = sign(destination - sprite->getPosition());
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
    selectCell(getCellUnderMouse(event), "selection");
}

void HelloWorld::onMouseDown(cocos2d::Event* event) {
    if (moving) {
        return;
    }
    cocos2d::Vec2 cell = getCellUnderMouse(event);

    if (cell.x < 0 || cell.y < 0 || cell.x >= cellsCount.x || cell.y >= cellsCount.y) {
        return;
    }
    if (cell == currentCell) {
        return;
    }

    path.clear(); 
    path = pg.findPath(NVector2(currentCell.x, currentCell.y), NVector2(cell.x, cell.y));
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