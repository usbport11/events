#include "IntroScene.h"
#include "utils.h"

USING_NS_CC;

Scene* MIntroScene::createScene() {
    return MIntroScene::create();
}

bool MIntroScene::init() {
    if(!Scene::init()) {
        return false;
    }

    sign = -1;
    color = 255;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	cocos2d::EventListenerMouse* mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = CC_CALLBACK_1(MIntroScene::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    cocos2d::EventListenerKeyboard* keybordListener = EventListenerKeyboard::create();
    keybordListener->onKeyPressed = CC_CALLBACK_2(MIntroScene::onKeyPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keybordListener, this);
	
	//create background
	cocos2d::Sprite* backgroungd = Sprite::create("intro.png");
    backgroungd->setPosition(visibleSize.width/2, visibleSize.height/2);
    this->addChild(backgroungd, 2);

	//create and apply animated frame
	if (!createAnimSpriteFromPlist(this, "anim/lights2.plist", "anim_light", "light", 4, 0.4f)) {
        return false;
    }
	this->getChildByName("anim_light")->setPosition(visibleSize.width / 2 + 9, visibleSize.height / 2 + 87);//?
    this->getChildByName("anim_light")->setPositionZ(1);

    //name logo
    cocos2d::Label* nameLabel = Label::createWithTTF("Forbridden Island", "fonts/Marker Felt.ttf", 48);
    if (!nameLabel) return false;
    nameLabel->setPosition(visibleSize.width / 2, visibleSize.height - 100);
    this->addChild(nameLabel);

    //press
    pressLabel = Label::createWithTTF("Press any key to continue ...", "fonts/Marker Felt.ttf", 24);
    if (!pressLabel) return false;
    pressLabel->setPosition(visibleSize.width / 2, 100);
    pressLabel->setColor(cocos2d::Color3B(color, color, color));
    this->addChild(pressLabel);

    this->scheduleUpdate();

    return true;
}

void MIntroScene::update(float delta) {
    color += sign;
    if (color <= 64) sign = 1;
    if (color >= 255) sign = -1;
    pressLabel->setColor(cocos2d::Color3B(color, color, color));
}

void MIntroScene::onMouseDown(cocos2d::Event* event) {
	Director::getInstance()->popScene();
}
void MIntroScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    Director::getInstance()->popScene();
}