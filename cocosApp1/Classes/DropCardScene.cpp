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
	sourceCards.clear();
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
	MenuItemImageExt* menuItem;
	cocos2d::Vec2 itemPosition;
	float offset;
    for (int i = 0; i < totalCardsLimit; i++) {
        menuItem = MenuItemImageExt::create("empty_card.png", "empty_card.png", "", menuCallback[i]);
		offset = (visibleSize.width - (menuItem->getContentSize().width + 20) * totalCardsLimit) / 2 + (menuItem->getContentSize().width + 20) / 2;
        if(!menuItem) {
            return false;
        }
        itemPosition = Vec2(offset + i * (menuItem->getContentSize().width + 20), visibleSize.height / 2);
        menuItem->setPosition(itemPosition);
        menuItems.pushBack(menuItem);

		//for test only
        cocos2d::Label* itemLabel = Label::createWithTTF("card", "fonts/Marker Felt.ttf", 24);
        if (!itemLabel) {
            return false;
        }
        itemLabel->setPosition(itemPosition);
        this->addChild(itemLabel, 2);
    }
	
	//create close item
	closeItem = MenuItemImageExt::create("back_off.png", "back_on.png", "", menuCallback[totalCardsLimit]);
    if(!closeItem) {
		return false;
    }
	offset = (visibleSize.width - closeItem->getContentSize().width) / 2;
	itemPosition = Vec2(offset + closeItem->getContentSize().width/2, visibleSize.height / 2 - 200);
	//closeItem->setEnabled(false);
	closeItem->setName("closeButton");
    closeItem->setPosition(itemPosition);
    menuItems.pushBack(closeItem);
	cocos2d::Label* itemLabel = Label::createWithTTF("Close", "fonts/Marker Felt.ttf", 24);
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

bool MDropCardScene::setCards(std::vector<std::string> _sourceCards) {
	sourceCards = _sourceCards;
	
	cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
    if (!cache) {
        return false;
    }
	
	cocos2d::Node* node = this->getChildByName(menuName);
    cocos2d::Vector<cocos2d::Node*> items = node->getChildren();
	cocos2d::Sprite* sp;
    for (int i = 0; i < items.size() && i < sourceCards.size(); i++) {
		sp = cocos2d::Sprite::createWithSpriteFrame(cache->getSpriteFrameByName(sourceCards[i]));
		if (!sp) {
			return false;
		}
		MenuItemImageExt* item = (MenuItemImageExt*) items.at(i);
		item->setNormalImage(sp);
		item->setSelectedImage(sp);
    }
	
	return true;
}

void MDropCardScene::selectMenuItem(cocos2d::Ref* pSender, int number) {
	if (number >= totalCardsLimit ) return;
	if (number >= sourceCards.size()) return;
	if (sourceCards[number] == "card1") return;

	MenuItemImageExt* menuItem = (MenuItemImageExt*)pSender;
	if (menuItem->getColor() == cocos2d::Color3B(255, 0, 0)) {
		menuItem->setColor(cocos2d::Color3B(255, 255, 255));
		selectedCards.remove(number);
	}
	else {
		menuItem->setColor(cocos2d::Color3B(255, 0, 0));
		selectedCards.push_back(number);
	}

	return;
	if (sourceCards.size() - selectedCards.size() <= cardsLimit) closeItem->setEnabled(true);
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
	//if cards by some reason <= 0 activate button
	//send cards list to mainScene
	//how call function in mainScene after this?
	//may be call function in MainScene from here?
		//processor exec function draw card for current adventurer
		//hide sprite in hand slot for current adventurer that was draw
	//return to mainScene;
	pMainScene->discard(selectedCards);
	Director::getInstance()->popScene();
}