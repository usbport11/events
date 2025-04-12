#include "DoubleCardMenu.h"
#include "MenuItemImageExt.h"
#include "MainScene.h"
#include "utils.h"

void MDoubleCardMenu::card1Callback(cocos2d::Ref* pSender) {
	pMainScene->selectDoubleCard(cardFrames[0]);
}
void MDoubleCardMenu::card2Callback(cocos2d::Ref* pSender) {
	pMainScene->selectDoubleCard(cardFrames[1]);
}
MDoubleCardMenu::MDoubleCardMenu() {
	pMainScene = nullptr;
	menu = nullptr;
	sprites[0][0] = nullptr;
	sprites[1][0] = nullptr;
	sprites[0][1] = nullptr;
	sprites[1][1] = nullptr;
	cardFrames[0] = cardFrames[1] = cardFrames[2] = "";
}
MDoubleCardMenu::~MDoubleCardMenu() {
	if (sprites[0][0]) pMainScene->removeChild(sprites[0][0]);
	if (sprites[1][0]) pMainScene->removeChild(sprites[1][0]);
	if (sprites[0][1]) pMainScene->removeChild(sprites[0][1]);
	if (sprites[1][1]) pMainScene->removeChild(sprites[1][1]);
}
bool MDoubleCardMenu::create(MMainScene* _pMainScene, cocos2d::Vec2 _position) {
	if(!_pMainScene) return false;
	pMainScene = _pMainScene;
	position = _position;
	
	std::vector<cocos2d::ccMenuCallback> callbacks;
	callbacks.push_back(CC_CALLBACK_1(MDoubleCardMenu::card1Callback, this));
	callbacks.push_back(CC_CALLBACK_1(MDoubleCardMenu::card2Callback, this));
	
	cocos2d::Sprite* sp;
	cocos2d::MenuItemImage* menuItem;
	cocos2d::Vector<cocos2d::MenuItem*> menuItems;
	for(int i=0; i < callbacks.size(); i++) {
		//menuItem = cocos2d::MenuItemImage::create("empty_card.png", "", callbacks[i]);
		menuItem = MenuItemImageExt::create("empty_card.png", "empty_card.png", "", callbacks[i]);
		if (!menuItem) return false;
		menuItems.pushBack(menuItem);
	}
	
	menu = cocos2d::Menu::createWithArray(menuItems);
	if (!menu) return false;
	menu->setPosition(cocos2d::Vec2::ZERO);
	menu->setName("doubleCardMenu");
	menu->setVisible(false);
	_pMainScene->addChild(menu, 1);
	
	menuItems.clear();
	callbacks.clear();
	
	return true;
}
bool MDoubleCardMenu::reset(cocos2d::SpriteFrame* card1SpriteFrame, cocos2d::SpriteFrame* card2SpriteFrame, cocos2d::SpriteFrame* selectedSpriteFrame, bool card1Enabled, bool card2Enabled) {
	if (!card1SpriteFrame) return false;
	if (!card2SpriteFrame) return false;
	if (!selectedSpriteFrame) return false;

	//remove old sprites
	if (sprites[0][0]) pMainScene->removeChild(sprites[0][0]);
	if (sprites[1][0]) pMainScene->removeChild(sprites[1][0]);
	if (sprites[0][0]) pMainScene->removeChild(sprites[0][1]);
	if (sprites[1][0]) pMainScene->removeChild(sprites[1][1]);

	std::vector<cocos2d::SpriteFrame*> cardSprites;
	cardSprites.push_back(card1SpriteFrame);
	cardSprites.push_back(card2SpriteFrame);

	std::vector<bool> itemsEnabled;
	itemsEnabled.push_back(card1Enabled);
	itemsEnabled.push_back(card2Enabled);

	cocos2d::Node* node = menu;
	cocos2d::Vector<cocos2d::Node*> items = node->getChildren();
	cocos2d::MenuItemImage* menuItem;
	float offset;
	for (int i = 0; i < items.size(); i++) {
		//menuItem = (cocos2d::MenuItemImage*)items.at(i);
		menuItem = (MenuItemImageExt*)items.at(i);
		sprites[i][0] = cocos2d::Sprite::createWithSpriteFrame(cardSprites[i]);
		sprites[i][1] = combineSpritesFromFrames(cardSprites[i], selectedSpriteFrame);
		if (!sprites[i]) return false;
		menuItem->setNormalImage(sprites[i][0]);
		menuItem->setSelectedImage(sprites[i][1]);
		menuItem->setEnabled(itemsEnabled[i]);
		offset = position.x + (menuItem->getContentSize().width + 10) * items.size() / 4;
		menuItem->setPosition(offset + i * (menuItem->getContentSize().width + 10), position.y);
	}
	menu->setVisible(true);

	cardSprites.clear();
	itemsEnabled.clear();

	return true;
}
bool MDoubleCardMenu::reset(const std::string& card1Frame, const std::string& card2Frame, const std::string& selectedFrame, bool card1Enabled, bool card2Enabled) {
	if (card1Frame.empty() || card2Frame.empty() || selectedFrame.empty()) return false;
	if (cardFrames[0] == card1Frame && cardFrames[1] == card2Frame && cardFrames[2] == selectedFrame) {
		std::vector<bool> itemsEnabled;
		itemsEnabled.push_back(card1Enabled);
		itemsEnabled.push_back(card2Enabled);
		cocos2d::Node* node = menu;
		cocos2d::Vector<cocos2d::Node*> items = node->getChildren();
		cocos2d::MenuItemImage* menuItem;
		for (int i = 0; i < items.size(); i++) {
			menuItem = (MenuItemImageExt*)items.at(i);
			menuItem->setEnabled(itemsEnabled[i]);
		}
		menu->setVisible(true);
		return true;
	}

	cardFrames[0] = card1Frame;
	cardFrames[1] = card2Frame;
	cardFrames[2] = selectedFrame;

	cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
	if (!cache) return false;

	return reset(cache->getSpriteFrameByName(card1Frame), cache->getSpriteFrameByName(card2Frame), cache->getSpriteFrameByName(selectedFrame), card1Enabled, card2Enabled);
}
void MDoubleCardMenu::hide() {
	if (menu) menu->setVisible(false);
}
void MDoubleCardMenu::show() {
	if (menu) menu->setVisible(true);
}