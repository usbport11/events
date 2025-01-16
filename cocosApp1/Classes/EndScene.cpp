#include "EndScene.h"
#include "MenuItemImageExt.h"

USING_NS_CC;

Scene* MEndScene::createScene() {
    return MEndScene::create();
}

bool MEndScene::init() {
    if(!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	cocos2d::Label* label = Label::createWithTTF("some_text", "fonts/Marker Felt.ttf", 48);
    if (!label) return false;
    label->setName("lblMessage");
    label->setPosition(visibleSize.width/2, visibleSize.height/2);
    this->addChild(label);

    MenuItemImageExt* closeItem = MenuItemImageExt::create("back_off.png", "back_on.png", "", CC_CALLBACK_1(MEndScene::menuCloseCallback, this));
    if (!closeItem) {
        return false;
    }
    closeItem->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 100);
    cocos2d::Label* itemLabel = Label::createWithTTF("New game", "fonts/Marker Felt.ttf", 24);
    if (!itemLabel) {
        return false;
    }
    itemLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 100);
    this->addChild(itemLabel, 2);
    cocos2d::Menu* menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    return true;
}

void MEndScene::setMessage(const std::string& message) {
    cocos2d::Label* label = (cocos2d::Label*)getChildByName("lblMessage");
    if (!label) return;
    label->setString(message);
}

void MEndScene::menuCloseCallback(cocos2d::Ref* pSender) {
    Director::getInstance()->popScene();
}