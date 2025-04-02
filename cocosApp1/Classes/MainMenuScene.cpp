#include "MainMenuScene.h"
#include "MenuItemImageExt.h"
#include "MainScene.h"
#include "CreditsScene.h"
#include <iostream>

USING_NS_CC;

cocos2d::Scene* MMainMenuScene::createScene() {
    return MMainMenuScene::create();
}

bool MMainMenuScene::init() {
	return Scene::init();
}

bool MMainMenuScene::create(MMainScene* _pMainScene, bool begin) {
	if (!_pMainScene) return false;
	pMainScene = _pMainScene;

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	cocos2d::Vector<cocos2d::MenuItem*> menuItems;
	MenuItemImage* menuItem;
	cocos2d::Vec2 itemPosition;
	cocos2d::Label* itemLabel;
	float offset;

	int len;
	const std::string nameListBegin[4] = { "Start", "Settings", "Credits", "Quit" };
	const std::string nameListResume[5] = { "Resume", "Restart", "Settings", "Credits", "Quit" };
	std::string* nameList;
	if (begin) {
		nameList = (std::string*)nameListBegin;
		len = 4;
	}
	else {
		nameList = (std::string*)nameListResume;
		len = 5;
	}

	std::map<std::string, ccMenuCallback> menuCallback;
	menuCallback["Start"] = CC_CALLBACK_1(MMainMenuScene::startCallback, this);
	menuCallback["Resume"] = CC_CALLBACK_1(MMainMenuScene::resumeCallback, this);
	menuCallback["Restart"] = CC_CALLBACK_1(MMainMenuScene::restartCallback, this);
	menuCallback["Settings"] = CC_CALLBACK_1(MMainMenuScene::settingsCallback, this);
	menuCallback["Credits"] = CC_CALLBACK_1(MMainMenuScene::creditsCallback, this);
	menuCallback["Quit"] = CC_CALLBACK_1(MMainMenuScene::quitCallback, this);

	for (int i = 0; i < len; i++) {
		if (menuCallback.find(nameList[len - i - 1]) == menuCallback.end()) {
			std::cout << " [Menu] Can't find item with name " << nameList[len - i - 1] << std::endl;
		}
		menuItem = MenuItemImageExt::create("back_off.png", "back_on.png", "back_dis.png", menuCallback[nameList[len - i - 1]]);
		if (!menuItem) {
			return false;
		}
		offset = (visibleSize.height - (menuItem->getContentSize().height + 20) * len) / 2 + (menuItem->getContentSize().height + 20) / 2;
		itemPosition = Vec2(visibleSize.width / 2, offset + i * (menuItem->getContentSize().height + 20));
		menuItem->setName(nameList[len - i - 1]);
		menuItem->setPosition(itemPosition);
		menuItems.pushBack(menuItem);

		itemLabel = Label::createWithTTF(nameList[len - i - 1], "fonts/Marker Felt.ttf", 24);
		if (!itemLabel) {
			return false;
		}
		itemLabel->setPosition(itemPosition);
		this->addChild(itemLabel, 2);
	}
	cocos2d::Menu* menu = Menu::createWithArray(menuItems);
	if (!menu) {
		return false;
	}
	menu->setPosition(Vec2::ZERO);
	menu->setName("mainMenu");
	this->addChild(menu, 1);
	menuCallback.clear();

	return true;
}

void MMainMenuScene::startCallback(cocos2d::Ref* pSender) {
	pMainScene->reset();
	Director::getInstance()->popScene();
}

void MMainMenuScene::resumeCallback(cocos2d::Ref* pSender) {
	Director::getInstance()->popScene();
}

void MMainMenuScene::restartCallback(cocos2d::Ref* pSender) {
	pMainScene->reset();
	Director::getInstance()->popScene();
}

void MMainMenuScene::settingsCallback(cocos2d::Ref* pSender) {
	//todo
}

void MMainMenuScene::creditsCallback(cocos2d::Ref* pSender) {
	MCreditsScene* creditsScene = (MCreditsScene*)MCreditsScene::createScene();
	if (!creditsScene) return;
	Director::getInstance()->pushScene(creditsScene);
}

void MMainMenuScene::quitCallback(cocos2d::Ref* pSender) {
	Director::getInstance()->end();
}
