#include "TestScene.h"
#include <iostream>

USING_NS_CC;

MTestScene::~MTestScene() {
    std::vector<MCard*> advCards = adventurer->getAllCards();
    for (int i = 0; i < advCards.size(); i++) {
        if (advCards[i]) delete advCards[i];
    }
    delete adventurer;
}

cocos2d::Scene* MTestScene::createScene() {
    return MTestScene::create();
}

bool MTestScene::init() {
	if(!Scene::init()) return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();

	cocos2d::EventListenerMouse* mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = CC_CALLBACK_1(MTestScene::onMouseDown, this);
    mouseListener->onMouseUp = CC_CALLBACK_1(MTestScene::onMouseUp, this);
    mouseListener->onMouseMove = CC_CALLBACK_1(MTestScene::onMouseMove, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    cocos2d::EventListenerKeyboard* keybordListener = EventListenerKeyboard::create();
    keybordListener->onKeyPressed = CC_CALLBACK_2(MTestScene::onKeyPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keybordListener, this);

    cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
    if (!cache) return false;
    cache->addSpriteFramesWithFile("anim/cards.plist");

    adventurer = new MAdventurer("explorer", "copper_gate");
    adventurer->addCard(new MCard("helicopter_0", "item"));
    adventurer->addCard(new MCard("sandbag_1", "item"));
    adventurer->addCard(new MCard("sphere_1", "artifact"));
    adventurer->addCard(new MCard("sphere_4", "artifact"));
    adventurer->addCard(new MCard("bowl_0", "artifact"));
    adventurer->addCard(new MCard("lion_3", "artifact"));

    if (!hand2.initCards(this, adventurer)) return false;

    cocos2d::Label* label = Label::createWithTTF("Test scene!", "fonts/Marker Felt.ttf", 24);
    label->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    this->addChild(label);

	this->scheduleUpdate();
	
	return true;
}

void MTestScene::update(float delta) {
    hand2.update(delta);
}

void MTestScene::onMouseDown(cocos2d::Event* event) {
    hand2.onMouseDown(event);
}

void MTestScene::onMouseUp(cocos2d::Event* event) {
    hand2.onMouseUp(event);
}

void MTestScene::onMouseMove(cocos2d::Event* event) {
    hand2.onMouseMove(event);
}

void MTestScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
}