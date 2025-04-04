#include "DropCardScene.h"
#include "MenuItemImageExt.h"
#include "MainScene.h"
#include <iostream>

USING_NS_CC;

Scene* MDropCardScene::createScene() {
    return MDropCardScene::create();
}

bool MDropCardScene::init() {
    if(!Scene::init()) {
        return false;
    }

	pMainScene = nullptr;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	menuName = "dropCardMenu";
	selectedCards.clear();
	//sourceCards.clear();
	sourceCardFrames.clear();
	selectedCardsNumber = 0;
	cardsLimit = 5;
	totalCardsLimit = 7;

	//create empty holder
	//create horizontal menu from card sprites [0] [1] [2] [3] [4] [5] [6]
	std::map<int, ccMenuCallback> menuCallback = {
		{0, CC_CALLBACK_1(MDropCardScene::card0Callback, this)}, 
		{1, CC_CALLBACK_1(MDropCardScene::card1Callback, this)}, 
		{2, CC_CALLBACK_1(MDropCardScene::card2Callback, this)},  
		{3, CC_CALLBACK_1(MDropCardScene::card3Callback, this)}, 
		{4, CC_CALLBACK_1(MDropCardScene::card4Callback, this)}, 
		{5, CC_CALLBACK_1(MDropCardScene::card5Callback, this)}, 
		{6, CC_CALLBACK_1(MDropCardScene::card6Callback, this)},
		{7, CC_CALLBACK_1(MDropCardScene::menuCloseCallback, this)}
	};
	
    cocos2d::Vector<cocos2d::MenuItem*> menuItems;
	MenuItemImage* menuItem;
	cocos2d::Vec2 itemPosition;
	float offset;
    for (int i = 0; i < totalCardsLimit; i++) {
		menuItem = MenuItemImage::create("empty_card.png", "", menuCallback[i]);
		offset = (visibleSize.width - (menuItem->getContentSize().width + 20) * totalCardsLimit) / 2 + (menuItem->getContentSize().width + 20) / 2;
        if(!menuItem) {
            return false;
        }
        itemPosition = Vec2(offset + i * (menuItem->getContentSize().width + 20), visibleSize.height / 2);
        menuItem->setPosition(itemPosition);
        menuItems.pushBack(menuItem);
    }
	
	//create close item
	//closeItem = MenuItemImage::create("back_off.png", "back_on.png", menuCallback[totalCardsLimit]);
	closeItem = MenuItemImageExt::create("back_off.png", "back_on.png", "back_dis.png", menuCallback[totalCardsLimit]);
    if(!closeItem) {
		return false;
    }
	offset = (visibleSize.width - closeItem->getContentSize().width) / 2;
	itemPosition = Vec2(offset + closeItem->getContentSize().width/2, visibleSize.height / 2 - 150);
	
	closeItem->setEnabled(false);
	closeItem->setName("closeButton");
    closeItem->setPosition(itemPosition);
    menuItems.pushBack(closeItem);
	cocos2d::Label* itemLabel = Label::createWithTTF("Submit", "fonts/Marker Felt.ttf", 24);
    if (!itemLabel) {
		return false;
    }
    itemLabel->setPosition(itemPosition);
    this->addChild(itemLabel, 2);
	
	//create menu based on items
    cocos2d::Menu* menu = Menu::createWithArray(menuItems);
    if(!menu) {
        return false;
    }
    menu->setPosition(Vec2::ZERO);
    menu->setName(menuName);
    this->addChild(menu, 1);
	menuCallback.clear();

	cocos2d::Label* messageLabel = Label::createWithTTF("Select cards to discard", "fonts/Marker Felt.ttf", 24);
	if (!messageLabel) {
		return false;
	}
	messageLabel->setPosition(visibleSize.width/2, visibleSize.height/2 + 150);
	this->addChild(messageLabel, 2);

    return true;
}

void MDropCardScene::setMainScene(MMainScene* _pMainScene) {
	pMainScene = _pMainScene;
}

bool MDropCardScene::setCardFrames(std::vector<cocos2d::SpriteFrame*> _cardFrames) {
	sourceCardFrames = _cardFrames;

	auto visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Node* node = this->getChildByName(menuName);
	cocos2d::Vector<cocos2d::Node*> items = node->getChildren();
	cocos2d::Sprite* sp;
	int offset = (visibleSize.width - (items.at(0)->getContentSize().width + 20) * sourceCardFrames.size()) / 2 + (items.at(0)->getContentSize().width + 20) / 2;

	for (int i = 0; i < items.size() - 1; i++) {
		MenuItemImage* item = (MenuItemImage*)items.at(i);
		if (i < sourceCardFrames.size()) {
			sp = cocos2d::Sprite::createWithSpriteFrame(sourceCardFrames[i]);
			if (!sp) {
				return false;
			}
			item->setNormalImage(sp);
			item->setSelectedImage(sp);
			item->setPosition(offset + i * (sp->getContentSize().width + 20), visibleSize.height / 2);
		}
		else {
			item->setVisible(false);
		}
	}

	return true;
}

bool MDropCardScene::setAdventurer(MAdventurer* _pAdventurer) {
	if (!_pAdventurer) return false;
	pAdventurer = _pAdventurer;
	return true;
}

void MDropCardScene::selectMenuItem(cocos2d::Ref* pSender, int number) {
	if (number >= totalCardsLimit ) return;
	if (number >= sourceCardFrames.size()) return;

	MenuItemImageExt* menuItem = (MenuItemImageExt*)pSender;
	if (menuItem->getColor() == cocos2d::Color3B(255, 0, 0)) {
		menuItem->setColor(cocos2d::Color3B(255, 255, 255));
		selectedCards.remove(number);
	}
	else {
		menuItem->setColor(cocos2d::Color3B(255, 0, 0));
		selectedCards.push_back(number);
	}

	if (sourceCardFrames.size() - selectedCards.size() == cardsLimit) closeItem->setEnabled(true);
	else closeItem->setEnabled(false);
}

void MDropCardScene::card0Callback(cocos2d::Ref* pSender) {
	selectMenuItem(pSender, 0);
}

void MDropCardScene::card1Callback(cocos2d::Ref* pSender) {
	selectMenuItem(pSender, 1);
}

void MDropCardScene::card2Callback(cocos2d::Ref* pSender) {
	selectMenuItem(pSender, 2);
}

void MDropCardScene::card3Callback(cocos2d::Ref* pSender) {
	selectMenuItem(pSender, 3);
}

void MDropCardScene::card4Callback(cocos2d::Ref* pSender) {
	selectMenuItem(pSender, 4);
}

void MDropCardScene::card5Callback(cocos2d::Ref* pSender) {
	selectMenuItem(pSender, 5);
}

void MDropCardScene::card6Callback(cocos2d::Ref* pSender) {
	selectMenuItem(pSender, 6);
}

void MDropCardScene::menuCloseCallback(cocos2d::Ref* pSender) {
	pMainScene->discard(pAdventurer, selectedCards);
	Director::getInstance()->popScene();
}