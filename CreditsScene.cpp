#include "CreditsScene.h"

USING_NS_CC;

cocos2d::Scene* MCreditsScene::createScene() {
    return MCreditsScene::create();
}

bool MCreditsScene::init() {
	if(!Scene::init()) return false;
	
	float startHeight = -50;
	step = 50;

	const std::string credits[] = {
		"Source:",
		"Table game 'Forbriden Island'",
		"",
		"Lead Programmer:",
		"Dmitriy",
		"",
		"Engine Programmers:",
		"Dmitriy",
		"",
		"Pixel artists:",
		"Dmitriy",
		"",
		"Game Designers:",
		"Dmitriy",
		"",
		"Quality team:",
		"Dmitriy",
		"",
		"Testers:",
		"Dmitriy",
		"Andrey",
		"",
		"Localizers:",
		"English - Dmitriy",
		"",
		"Sound effects:",
		"None",
		"",
		"Music:",
		"None",
		"",
		"Credits autor:",
		"Dmitriy",
		"",
		"Special Thanks:",
		"Family",
		"Friends",
		"Colleagues"
	};

	const int linesNumber = sizeof(credits) / sizeof(std::string);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	cocos2d::EventListenerMouse* mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = CC_CALLBACK_1(MCreditsScene::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    cocos2d::EventListenerKeyboard* keybordListener = EventListenerKeyboard::create();
    keybordListener->onKeyPressed = CC_CALLBACK_2(MCreditsScene::onKeyPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keybordListener, this);

	cocos2d::Label* label;
	for (int i = 0; i < linesNumber; i++) {
		if (credits[i].empty()) continue;
		label = Label::createWithTTF(credits[i], "fonts/Marker Felt.ttf", 24);
		if (!label) return false;
		label->setPosition(visibleSize.width / 2, startHeight - i * 32);
		this->addChild(label);
		creditsLabels.push_back(label);
	}
	
	this->scheduleUpdate();
	
	return true;
}

void MCreditsScene::update(float delta) {
	if (creditsLabels.back()->getPosition().y >= 800) {
		Director::getInstance()->popScene();
	}
	for (int i = 0; i < creditsLabels.size(); i++) {
		creditsLabels[i]->setPosition(creditsLabels[i]->getPosition() + cocos2d::Vec2(0, step * delta));
	}
}

void MCreditsScene::onMouseDown(cocos2d::Event* event) {
	Director::getInstance()->popScene();
}
void MCreditsScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    Director::getInstance()->popScene();
}