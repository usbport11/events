#include "AdventurerMenu.h"
#include "MainScene.h"
#include <iostream>

MAdventurerMenu::MAdventurerMenu() {
	pMainScene = nullptr;
}

MAdventurerMenu::~MAdventurerMenu() {
	numberAdventurer.clear();
	adventurerSprite.clear();
}

void MAdventurerMenu::menuAdv0Callback(cocos2d::Ref* pSender) {
	selectMenuItem(pSender);
	//pMainScene->advClicked(numberAdventurer[0]);
}
void MAdventurerMenu::menuAdv1Callback(cocos2d::Ref* pSender) {
	selectMenuItem(pSender);
	//pMainScene->advClicked(numberAdventurer[1]);
}
void MAdventurerMenu::menuAdv2Callback(cocos2d::Ref* pSender) {
	selectMenuItem(pSender);
	//pMainScene->advClicked(numberAdventurer[2]);
}
void MAdventurerMenu::menuAdv3Callback(cocos2d::Ref* pSender) {
	selectMenuItem(pSender);
	//pMainScene->advClicked(numberAdventurer[3]);
}
void MAdventurerMenu::menuAdv4Callback(cocos2d::Ref* pSender) {
	selectMenuItem(pSender);
	//pMainScene->advClicked(numberAdventurer[4]);
}
void MAdventurerMenu::menuAdv5Callback(cocos2d::Ref* pSender) {
	selectMenuItem(pSender);
	//pMainScene->advClicked(numberAdventurer[5]);
}

bool MAdventurerMenu::create(MMainScene* _pMainScene, const std::string& pListFile, std::vector<std::string> adventurers) {
    if (!_pMainScene) {
        return false;
    }
    pMainScene = _pMainScene;
	
	cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
    if (!cache) return false;
    cache->addSpriteFramesWithFile(pListFile);
	
	std::map<std::string, std::string> advNameFrame = {
		{"diver", "adv0"},
		{"engineer", "adv1"},
		{"explorer", "adv2"},
		{"liaison", "adv3"},
		{"navigator", "adv4"},
		{"pilot", "adv5"}
	};
	
	//cocos2d::SpriteFrame* spriteFrame;
	//cocos2d::Sprite* sprite;
	//for(int i=0; i<adventurers.size(); i++) {
	//	spriteFrame = cache->getSpriteFrameByName(advNameFrame[adventurers[i]]);
	//	sprite = cocos2d::Sprite::createWithSpriteFrame(spriteFrame);
	//	adventurerSprite[adventurers[i]] = sprite;
	//}
	
	//create items
    std::map<int, cocos2d::ccMenuCallback> menuCallback = {
		{0, CC_CALLBACK_1(MAdventurerMenu::menuAdv0Callback, this)},
		{1, CC_CALLBACK_1(MAdventurerMenu::menuAdv1Callback, this)},
		{2, CC_CALLBACK_1(MAdventurerMenu::menuAdv2Callback, this)},
		{3, CC_CALLBACK_1(MAdventurerMenu::menuAdv3Callback, this)},
		{4, CC_CALLBACK_1(MAdventurerMenu::menuAdv4Callback, this)},
		{5, CC_CALLBACK_1(MAdventurerMenu::menuAdv5Callback, this)},
	};
	cocos2d::Vector<cocos2d::MenuItem*> menuItems;
	cocos2d::Vec2 offset = cocos2d::Vec2(50, 300);
	int rows = 2;
	int pos[2];
	//for(int i=0; i<adventurerSprite.size(); i++) {
	for (int i = 0; i < adventurers.size(); i++) {
		cocos2d::MenuItemImage* menuItem = cocos2d::MenuItemImage::create("empty_card.png", "", "", menuCallback[i]);
        if(!menuItem) {
            return false;
        }
		pos[0] = i / rows;
		pos[1] = i - pos[0] * rows;
        menuItem->setPosition(offset.x + pos[0] * 64 - 10, offset.y - pos[1] * 64 - 10);//<--- need calculate
		menuItem->setVisible(false);
		menuItem->setEnabled(false);
        menuItems.pushBack(menuItem);
	}

	//create menu
    cocos2d::Menu* menu = cocos2d::Menu::createWithArray(menuItems);
    if(!menu) {
        return false;
    }
    menu->setPosition(cocos2d::Vec2::ZERO);
    menu->setName("adventurerMenu");
    pMainScene->addChild(menu, 1);
    menuCallback.clear();

    return true;
}

bool MAdventurerMenu::init(std::vector<std::string> activeAdventurers) {
	numberAdventurer.clear();

	std::map<std::string, std::string> advNameFrame = {
		{"diver", "adv0"},
		{"engineer", "adv1"},
		{"explorer", "adv2"},
		{"liaison", "adv3"},
		{"navigator", "adv4"},
		{"pilot", "adv5"}
	};
	cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
	if (!cache) return false;
	cocos2d::SpriteFrame* spriteFrame;
	cocos2d::Sprite* sprite;

	cocos2d::Node* node = pMainScene->getChildByName("adventurerMenu");
	if(!node) return false;
    cocos2d::Vector<cocos2d::Node*> items = node->getChildren();
	cocos2d::MenuItemImage* menuItem;
    for (int i = 0; i < items.size(); i++) {
		menuItem = (cocos2d::MenuItemImage*)items.at(i);
		if(i < activeAdventurers.size()) {
			spriteFrame = cache->getSpriteFrameByName(advNameFrame[activeAdventurers[i]]);
			sprite = cocos2d::Sprite::createWithSpriteFrame(spriteFrame);

			//adventurerSprite[adventurers[i]] = sprite;
			//cocos2d::Sprite* sp = adventurerSprite[activeAdventurers[i]];

			menuItem->setVisible(true);
			menuItem->setEnabled(true);
			menuItem->setNormalImage(sprite);//sp
			menuItem->setSelectedImage(sprite);//sp
			numberAdventurer[i] = activeAdventurers[i];
			if(i == 0) {
				menuItem->setSelectedImage(sprite);//sp
			}
		}
		else {
			menuItem->setVisible(false);
			menuItem->setEnabled(false);
		}
	}
	return true;
}

void MAdventurerMenu::unselectMenuAll() {
    cocos2d::Node* node = pMainScene->getChildByName("adventurerMenu");
    cocos2d::Vector<cocos2d::Node*> items = node->getChildren();
	cocos2d::MenuItemImage* menuItem;
    for (int i = 0; i < items.size(); i++) {
		menuItem = (cocos2d::MenuItemImage*)items.at(i);
		if(!menuItem->isEnabled()) break;
		menuItem->setColor(cocos2d::Color3B(255, 255, 255));
    }
}

void MAdventurerMenu::selectMenuItem(cocos2d::Ref* pSender) {
    unselectMenuAll();
	cocos2d::MenuItemImage* menuItem = (cocos2d::MenuItemImage*)pSender;
    menuItem->setColor(cocos2d::Color3B(0, 255, 0));
    menuItem->selected();
}