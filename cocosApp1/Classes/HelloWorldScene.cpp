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
    this->getChildByName("selection")->setScale(1.5);//1.0
    this->getChildByName("selection")->setVisible(false);
    if (!createAnimSpriteFromPlist(this, "anim/fox.plist", "anim_fox", "fox_pt", 4, 0.1f)) {
        return false;
    }
    this->getChildByName("anim_fox")->setPosition(332, 332);//300 + 32
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
    glass->setPosition(cocos2d::Vec2(964, 592));//464
    glass->setScaleY(5.0);
    this->addChild(glass, 2);

    if (!waterLevel.create(this, "anim_water", "water_back", cocos2d::Vec2(964, 464), cocos2d::Size(64, 64), 5)) return false;

    if (!createMenu()) return false;
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

    if (!gridMap.create(this, "anim/cells.plist", cocos2d::Size(4, 4), cocos2d::Size(300, 300), cocos2d::Size(96, 96))) return false;//64/64
    pg.setWorldSize(NVector2((int)gridMap.getCellsCount().x, (int)gridMap.getCellsCount().y));
    pg.setDiagonalMovement(false);

    cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
    if (!cache) return false;
    cocos2d::Label* labelHand = cocos2d::Label::createWithTTF("Hand", "fonts/Marker Felt.ttf", 24);
    if (!labelHand) return false;
    labelHand->setPosition(cocos2d::Vec2(230, 170));
    this->addChild(labelHand);
    cocos2d::Sprite* hand;
    char buffer[16];
    for (int i = 0; i < 5; i++) {
        memset(buffer, 0, 16);
        sprintf(buffer, "hand%d", i);
        hand = cocos2d::Sprite::createWithSpriteFrame(cache->getSpriteFrameByName("card1"));
        if (!hand) return false;
        hand->setPosition(cocos2d::Vec2(90 + 70 * i, 100));
        this->addChild(hand, 0, buffer);
    }
    cocos2d::Sprite* spA = (cocos2d::Sprite*)this->getChildByName("hand0");
    spA->setSpriteFrame(cache->getSpriteFrameByName("flood2"));

    this->scheduleUpdate();

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

void HelloWorld::menuAbflussCallback(cocos2d::Ref* pSender) {
    selectMenuItem(pSender);
}

void HelloWorld::menuEndTurnCallback(cocos2d::Ref* pSender) {
    selectMenuItem(pSender);
}

void HelloWorld::menuMoveCallback(cocos2d::Ref* pSender) {
    selectMenuItem(pSender);
    //highlight available cells
    char buffer[16] = {0};
    cocos2d::Vec2 curr = gridMap.getCurrentCell();

    //clear alredy higlighted
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            snprintf(buffer, 16, "cell_%d_%d", i, j);
            cocos2d::Sprite* sp = (cocos2d::Sprite*)this->getChildByName(buffer);
            if (!sp) continue;
            sp->setColor(cocos2d::Color3B(255, 255, 255));
        }
    }

    //hightlight needed
    int ind[4][2] = { {-1,0},{1,0},{0,-1},{0, 1} };
    for (int i = 0; i<4; i++) {
        memset(buffer, 0, 16);
        cocos2d::Vec2 neigh = curr + cocos2d::Vec2(ind[i][0], ind[i][1]);
        if (neigh.x < 0 || neigh.y < 0 || neigh.x >= 4 || neigh.y >= 4) continue;
        snprintf(buffer, 16, "cell_%d_%d", (int)neigh.x, (int)neigh.y);
        cocos2d::Sprite* sp = (cocos2d::Sprite*) this->getChildByName(buffer);
        if (!sp) continue;
        sp->setColor(cocos2d::Color3B(0, 255, 0));
    }
}

/*
void HelloWorld::menuExitCallback(Ref* pSender) {
    Director::getInstance()->end();
}
*/

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

void HelloWorld::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    if (keyCode == EventKeyboard::KeyCode::KEY_N) {
        itemDeck.nextCard();
        floodDeck.nextCard();
    }
    if (keyCode == EventKeyboard::KeyCode::KEY_R) {
        itemDeck.reset();
        floodDeck.reset();
        waterLevel.reset();
    }
    if (keyCode == EventKeyboard::KeyCode::KEY_W) {
        waterLevel.increase();
    }
}

void HelloWorld::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
}

bool HelloWorld::createMenu() {
    const std::string nameList[3] = {"EndTurn", "Move", "Abfluss"};
    std::map<std::string, ccMenuCallback> menuCallback;
    menuCallback["EndTurn"] = CC_CALLBACK_1(HelloWorld::menuEndTurnCallback, this);
    menuCallback["Move"] = CC_CALLBACK_1(HelloWorld::menuMoveCallback, this);
    menuCallback["Abfluss"] = CC_CALLBACK_1(HelloWorld::menuAbflussCallback, this);
    //menuCallback["Exit"] = CC_CALLBACK_1(HelloWorld::menuExitCallback, this);

    float topOffset = this->getContentSize().height - 30;
    const std::string btnBackPng[2] = { "back_off.png", "back_on.png" };
    const std::string fontTTF = "fonts/Marker Felt.ttf";

    //create menu
    int num = 0;
    cocos2d::Vector<cocos2d::MenuItem*> menuItems;
    for (int i = 0; i < sizeof(nameList) / sizeof(std::string); i++) {
        MenuItemImageExt* menuItem = MenuItemImageExt::create(btnBackPng[0], btnBackPng[1], "", menuCallback[nameList[i]]);
        if(!menuItem) {
            return false;
        }
        cocos2d::Vec2 itemPosition = Vec2(menuItem->getContentSize().width / 2, topOffset - num * menuItem->getContentSize().height);
        menuItem->setPosition(itemPosition);
        menuItem->setName(nameList[i]);
        menuItems.pushBack(menuItem);
        cocos2d::Label* itemLabel = Label::createWithTTF(nameList[i], fontTTF, 24);
        if (!itemLabel) {
            return false;
        }
        itemLabel->setPosition(itemPosition);
        this->addChild(itemLabel, 2);
        num ++;
    }
    cocos2d::Menu* menu = Menu::createWithArray(menuItems);
    if(!menu) {
        return false;
    }
    menu->setPosition(Vec2::ZERO);
    menu->setName("MainMenu");
    this->addChild(menu, 1);
    menuCallback.clear();

    return true;
}

void HelloWorld::unselectMenuAll() {
    cocos2d::Node* node = this->getChildByName("MainMenu");
    cocos2d::Vector<cocos2d::Node*> items = node->getChildren();
    for (int i = 0; i < items.size(); i++) {
        items.at(i)->setColor(cocos2d::Color3B(255, 255, 255));
    }
}

void HelloWorld::selectMenuItem(cocos2d::Ref* pSender) {
    unselectMenuAll();
    MenuItemImageExt* menuItem = (MenuItemImageExt*)pSender;
    menuItem->setColor(cocos2d::Color3B(255, 0, 0));
    menuItem->selected();
    log(menuItem->getName().c_str());
}