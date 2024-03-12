#include "HelloWorldScene.h"
#include "MenuItemImageExt.h"

USING_NS_CC;

Scene* HelloWorld::createScene() {
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename) {
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
    if(!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto listener = EventListenerMouse::create(); 
    listener->onMouseMove = CC_CALLBACK_1(HelloWorld::onMouseMove, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    auto closeItem = MenuItemImageExt::create("CloseNormal.png", "CloseSelected.png", "", CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0) {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr) {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else {
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
          origin.y + visibleSize.height - label->getContentSize().height));
        this->addChild(label, 1);
    }

    auto sprite = Sprite::create("HelloWorld.png");
    if (sprite == nullptr) {
        problemLoading("'HelloWorld.png'");
    }
    else {
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
        this->addChild(sprite, 0);
    }
    
    areasOffset = Vec2(32, 32);
    halfSize = Size(32, 32);
    fullSize = Size(64, 64);
    cellsNumber[0] = cellsNumber[1] = 2;
    areasRect = Rect(0, 0, cellsNumber[0], cellsNumber[1]);
    overNum[0] = overNum[1] = -1;

    for (int i = 0; i < cellsNumber[0]; i++) {
        for (int j = 0; j < cellsNumber[1]; j++) {
            cocos2d::Sprite* sp = Sprite::create("quad64.png");
            if (sp) {
                sp->setPosition(Vec2(areasOffset.x + i * fullSize.width, areasOffset.y + j * fullSize.height));
                this->addChild(sp, 0);
                cells.push_back(sp);
            }
        }
    }

    return true;
}

void HelloWorld::update(float delta) {
}

void HelloWorld::menuCloseCallback(Ref* pSender) {
    Director::getInstance()->end();
}

void HelloWorld::onMouseMove(cocos2d::Event* event) {
    EventMouse* e = (EventMouse*)event;
    Vec2 mc = Vec2(e->getCursorX(), e->getCursorY());
    overNum[0] = mc.x / fullSize.width;
    overNum[1] = mc.y / fullSize.height;
    for (int i = 0; i < cells.size(); i++) {
        cells[i]->setColor(Color3B(255, 255, 255));
    }
    if (overNum[0] < 0 || overNum[1] < 0 || overNum[0] >= cellsNumber[0] || overNum[1] >= cellsNumber[1]) return;
    int num = overNum[0] * cellsNumber[0] + overNum[1];
    cells[num]->setColor(Color3B(128, 128, 128));
}