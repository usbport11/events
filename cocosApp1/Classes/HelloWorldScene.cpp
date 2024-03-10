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
    
    const int size = 64;
    const int half = 64 / 2;
    int rows = 2, cols = 2;
    for(int i = 0; i< rows; i++) {
        for (int j = 0; j < cols; j++) {
            auto spQuad = Sprite::create("quad64.png");
            if (spQuad != nullptr) {
                spQuads.push_back(spQuad);
                spQuad->setPosition(Vec2(origin.x + half + i * size, origin.y + half + j * size));
                this->addChild(spQuad, 0);
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
    float x = e->getCursorX();
    float y = e->getCursorY();
    int rows = 2, cols = 2;
    Vec2 point;
    const int half = 64 / 2;
    int num;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            num = i * rows + j;
            point = spQuads[num]->getPosition();
            if (x > point.x - half && x < point.x + half &&
                y > point.y - half && y < point.y + half) {
                spQuads[num]->setColor(Color3B(128, 128, 128));
            }
            else {
                spQuads[num]->setColor(Color3B(255, 255, 255));
            }
        }
    }
}