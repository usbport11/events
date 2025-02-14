#include "HandOverScene.h"
#include "MainScene.h"
#include "MenuItemImageExt.h"
#include "logic/adventurer.h"
#include "logic/card.h"
#include <iostream>

USING_NS_CC;

MHandoverScene::MHandoverScene() {
	pMainScene = nullptr;
	selectedCard = -1;
	selectedAdventurer = -1;
}
MHandoverScene::~MHandoverScene() {
	for(int i=0; i<hands.size(); i++) {
		if(hands[i]) delete hands[i];
	}
	hands.clear();
	cardItems.clear();
	adventurerItems.clear();
	numberAdventurer.clear();
}

Scene* MHandoverScene::createScene() {
	return MHandoverScene::create();
}
bool MHandoverScene::init() {
	return Scene::init();
}
bool MHandoverScene::create(MMainScene* _pMainScene, MAdventurer* srcAdventurer, std::vector<std::string> dstAdventurers) {
	if (!_pMainScene) return false;
	if (!srcAdventurer) return false;
	if (dstAdventurers.empty()) return false;
	pMainScene = _pMainScene;
	pAdventurer = srcAdventurer;

	std::map<int, ccMenuCallback> cardsCallback = {
		{0, CC_CALLBACK_1(MHandoverScene::sourceCard0Select, this)},
		{1, CC_CALLBACK_1(MHandoverScene::sourceCard1Select, this)},
		{2, CC_CALLBACK_1(MHandoverScene::sourceCard2Select, this)},
		{3, CC_CALLBACK_1(MHandoverScene::sourceCard3Select, this)},
		{4, CC_CALLBACK_1(MHandoverScene::sourceCard4Select, this)}
	};

	std::map<std::string, ccMenuCallback> buttonsCallback = {
		{"Submit", CC_CALLBACK_1(MHandoverScene::handoverSumbit, this)},
		{"Cancel", CC_CALLBACK_1(MHandoverScene::handoverCancel, this)},
	};
	std::string buttons[2] = { "Submit" , "Cancel" };
	
	std::map<int, ccMenuCallback> adventurersCallback = {
		{0, CC_CALLBACK_1(MHandoverScene::adventurer0Select, this)}, 
		{1, CC_CALLBACK_1(MHandoverScene::adventurer1Select, this)}, 
		{2, CC_CALLBACK_1(MHandoverScene::adventurer2Select, this)},  
		{3, CC_CALLBACK_1(MHandoverScene::adventurer3Select, this)}, 
		{4, CC_CALLBACK_1(MHandoverScene::adventurer4Select, this)}
	};

	cocos2d::Vector<cocos2d::MenuItem*> menuItems;
	MenuItemImage* menuItem;
	//MenuItemImage* buttonItem;
	MenuItemImageExt* buttonItem;
	MenuItemImage* closeItem;
	cocos2d::Menu* menu;
	cocos2d::Vec2 itemPosition;
	cocos2d::Sprite* sp;
	cocos2d::SpriteFrame* spf;
	float offset;

	cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();

	//============================================================================================
	//create adventurers menu and hands
	MAdventurer* nextAdventurer;
	for (int i = 0; i < dstAdventurers.size(); i++) {
		nextAdventurer = pMainScene->getProcessor()->findAdventurer(dstAdventurers[i]);
		if (!nextAdventurer) return false;
		numberAdventurer[i] = nextAdventurer;

		MHand* hand = new MHand;
		hand->create(this, pMainScene->getAdventurerHand(numberAdventurer[i]->getName()), numberAdventurer[i]->getName(), cocos2d::Vec2((visibleSize.width - 350)/2 + 35, 450));
		if (i > 0) {
			hand->setVisible(false);
		}
		std::vector<std::string> data = hand->getCards();
		hands.push_back(hand);

		//adventurer icon items
		menuItem = MenuItemImage::create("empty_card.png", "", adventurersCallback[i]);
		if (!menuItem) {
			return false;
		}
		sp = pMainScene->getAdventurerMenu()->getAdventurerSprite(nextAdventurer->getName());
		if (!sp) return false;
		sp = cocos2d::Sprite::createWithSpriteFrame(sp->getSpriteFrame());
		if (!sp) return false;
		offset = (visibleSize.width - (sp->getContentSize().width + 20) * (dstAdventurers.size() - 1)) / 2;
		itemPosition = Vec2(offset + i * (sp->getContentSize().width + 20), 600);
		menuItem->setPosition(itemPosition);
		if (!sp) return false;
		menuItem->setNormalImage(sp);
		menuItem->setSelectedImage(sp);
		adventurerItems.pushBack(menuItem);
	}
	//create menu based on items
	menu = Menu::createWithArray(adventurerItems);
	if (!menu) {
		return false;
	}
	menu->setPosition(Vec2::ZERO);
	menu->setName("adventurersMenu");
	this->addChild(menu, 1);
	menuItems.clear();

	//select first adventurer
	selectAdventurer(0);
	
	//============================================================================================
	//create source cards menu
	std::vector<MCard*> cards = pAdventurer->getAllCards();
	MHand* hand = pMainScene->getAdventurerHand(pAdventurer->getName());
    for (int i = 0; i < cards.size(); i++) {
		menuItem = MenuItemImage::create("empty_card.png", "", cardsCallback[i]);
		offset = (visibleSize.width - (menuItem->getContentSize().width + 20) * cards.size()) / 2 + (menuItem->getContentSize().width + 20) / 2;
        if(!menuItem) {
            return false;
        }
        itemPosition = Vec2(offset + i * (menuItem->getContentSize().width + 20), 300);
        menuItem->setPosition(itemPosition);

		spf = hand->getCardFrame(hand->getCard(i));
		if (!spf) return false;
		sp = cocos2d::Sprite::createWithSpriteFrame(spf);
		if(!sp) return false;

		menuItem->setNormalImage(sp);
		menuItem->setSelectedImage(sp);
        menuItems.pushBack(menuItem);
		cardItems.pushBack(menuItem);
    }
	
	//create close items
	for(int i=0; i<2; i++) {
		//buttonItem = MenuItemImage::create("back_off.png", "back_on.png", buttonsCallback[buttons[i]]);
		buttonItem = MenuItemImageExt::create("back_off.png", "back_on.png", "back_dis.png", buttonsCallback[buttons[i]]);
		if(!buttonItem) {
			return false;
		}
		offset = (visibleSize.width - (buttonItem->getContentSize().width + 20) * 2) / 2 + (buttonItem->getContentSize().width + 20) / 2;
		itemPosition = Vec2(offset + i * (buttonItem->getContentSize().width + 20), 150);
		
		buttonItem->setName("btn_" + buttons[i]);
		buttonItem->setPosition(itemPosition);
		menuItems.pushBack(buttonItem);
		
		cocos2d::Label* itemLabel = Label::createWithTTF(buttons[i], "fonts/Marker Felt.ttf", 24);
		if (!itemLabel) {
			return false;
		}
		itemLabel->setPosition(itemPosition);
		this->addChild(itemLabel, 2);
	}
	
	//create menu based on items
    menu = Menu::createWithArray(menuItems);
    if(!menu) {
        return false;
    }
    menu->setPosition(Vec2::ZERO);
    menu->setName("sourceCardsMenu");
	this->addChild(menu, 1);
	menuItems.clear();

	return true;
}
void MHandoverScene::handoverSumbit(cocos2d::Ref* pSender) {
	if(selectedCard < 0 || selectedCard >= 5) return;
	if(selectedAdventurer < 0 || selectedAdventurer >= hands.size()) return;
	//in theory this must bit different
	//card can be transmit to adventurer with full hand, but after this must be discard
	if(hands[selectedAdventurer]->getUsedSize() >= 5) return;
	pMainScene->sumbitHandover(numberAdventurer[selectedAdventurer], selectedCard);
	std::cout << "Submit clicked!" << std::endl;
	Director::getInstance()->popScene();
}
void MHandoverScene::handoverCancel(cocos2d::Ref* pSender) {
	std::cout<<"Cancel clicked!" << std::endl;
	Director::getInstance()->popScene();
}
void MHandoverScene::selectAdventurer(int number) {
	if (number >= numberAdventurer.size()) return;
	if (numberAdventurer[number]->getArtifactCards().empty()) return;
	if (selectedAdventurer >= 0) adventurerItems.at(selectedAdventurer)->setColor(cocos2d::Color3B(255,255,255));
	if (selectedAdventurer >= 0) hands.at(selectedAdventurer)->setVisible(false);
	adventurerItems.at(number)->setColor(cocos2d::Color3B(0,255,0));
	hands.at(number)->setVisible(true);
	selectedAdventurer = number;
}
void MHandoverScene::adventurer0Select(cocos2d::Ref* pSender) {
	selectAdventurer(0);
}
void MHandoverScene::adventurer1Select(cocos2d::Ref* pSender) {
	selectAdventurer(1);
}
void MHandoverScene::adventurer2Select(cocos2d::Ref* pSender) {
	selectAdventurer(2);
}
void MHandoverScene::adventurer3Select(cocos2d::Ref* pSender) {
	selectAdventurer(3);
}
void MHandoverScene::adventurer4Select(cocos2d::Ref* pSender) {
	selectAdventurer(4);
}
void MHandoverScene::selectSourceCard(int number) {
	if(number < 0) return;
	if(selectedCard >= 0) cardItems.at(selectedCard)->setColor(cocos2d::Color3B(255,255,255));
	cardItems.at(number)->setColor(cocos2d::Color3B(0,255,0));
	selectedCard = number;
}
void MHandoverScene::sourceCard0Select(cocos2d::Ref* pSender) {
	selectSourceCard(0);
}
void MHandoverScene::sourceCard1Select(cocos2d::Ref* pSender) {
	selectSourceCard(1);
}
void MHandoverScene::sourceCard2Select(cocos2d::Ref* pSender) {
	selectSourceCard(2);
}
void MHandoverScene::sourceCard3Select(cocos2d::Ref* pSender) {
	selectSourceCard(3);
}
void MHandoverScene::sourceCard4Select(cocos2d::Ref* pSender) {
	selectSourceCard(4);
}
