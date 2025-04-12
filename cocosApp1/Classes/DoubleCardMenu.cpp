#include "DoubleCardMenu.h"
#include "MenuItemImageExt.h"
#include "utils.h"

void MDoubleCardMenu::card1Callback(cocos2d::Ref* pSender) {
	//if (menu) menu->setVisible(false);
	//call main scene func
}
void MDoubleCardMenu::card2Callback(cocos2d::Ref* pSender) {
	//if (menu) menu->setVisible(false);
	//call main scene func
}
MDoubleCardMenu::MDoubleCardMenu() {
	pScene = nullptr;
	menu = nullptr;
	sprites[0][0] = nullptr;
	sprites[1][0] = nullptr;
	sprites[0][1] = nullptr;
	sprites[1][1] = nullptr;
	prevFrames[0] = prevFrames[1] = prevFrames[2] = nullptr;
}
MDoubleCardMenu::~MDoubleCardMenu() {
	if (sprites[0][0]) pScene->removeChild(sprites[0][0]);
	if (sprites[1][0]) pScene->removeChild(sprites[1][0]);
	if (sprites[0][1]) pScene->removeChild(sprites[0][1]);
	if (sprites[1][1]) pScene->removeChild(sprites[1][1]);
}
bool MDoubleCardMenu::create(cocos2d::Scene* _pScene, cocos2d::Vec2 _position) {
	if(!_pScene) return false;
	pScene = _pScene;
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
	_pScene->addChild(menu, 1);
	
	menuItems.clear();
	callbacks.clear();
	
	return true;
}
bool MDoubleCardMenu::reset(const std::string& card1Frame, const std::string& card2Frame, const std::string& selectedFrame, bool card1Enabled, bool card2Enabled) {
	if (card1Frame.empty() || card2Frame.empty() || selectedFrame.empty()) return false;

	cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
	if (!cache) return false;

	return reset(cache->getSpriteFrameByName(card1Frame), cache->getSpriteFrameByName(card2Frame), cache->getSpriteFrameByName(selectedFrame), card1Enabled, card2Enabled);
}
bool MDoubleCardMenu::reset(cocos2d::SpriteFrame* card1SpriteFrame, cocos2d::SpriteFrame* card2SpriteFrame, cocos2d::SpriteFrame* selectedSpriteFrame, bool card1Enabled, bool card2Enabled) {
	if (!card1SpriteFrame) return false;
	if (!card2SpriteFrame) return false;
	if (!selectedSpriteFrame) return false;

	if (prevFrames[0] == card1SpriteFrame && prevFrames[1] == card2SpriteFrame && prevFrames[2] == selectedSpriteFrame) {
		menu->setVisible(true);
		return true;
	}
	prevFrames[0] = card1SpriteFrame;
	prevFrames[1] = card2SpriteFrame;
	prevFrames[2] = selectedSpriteFrame;

	//remove old sprites
	if (sprites[0][0]) pScene->removeChild(sprites[0][0]);
	if (sprites[1][0]) pScene->removeChild(sprites[1][0]);
	if (sprites[0][0]) pScene->removeChild(sprites[0][1]);
	if (sprites[1][0]) pScene->removeChild(sprites[1][1]);

	std::vector<cocos2d::SpriteFrame*> cardSprites;
	cardSprites.push_back(card1SpriteFrame);
	cardSprites.push_back(card2SpriteFrame);

	cocos2d::Node* node = menu;
	cocos2d::Vector<cocos2d::Node*> items = node->getChildren();
	cocos2d::MenuItemImage* menuItem;
	float offset;
	for (int i = 0; i < items.size(); i++) {
		//menuItem = (cocos2d::MenuItemImage*)items.at(i);
		menuItem = (MenuItemImageExt*)items.at(i);
		sprites[i][0] = cocos2d::Sprite::createWithSpriteFrame(cardSprites[i]);
		sprites[i][1] = combineSpritesFromFrames(cardSprites[i], selectedSpriteFrame);
		if(!sprites[i]) return false;
		menuItem->setNormalImage(sprites[i][0]);
		menuItem->setSelectedImage(sprites[i][1]);
		offset = position.x + (menuItem->getContentSize().width + 10) * items.size() / 4;
		menuItem->setPosition(offset + i * (menuItem->getContentSize().width + 10), position.y);
	}
	menu->setVisible(true);

	return true;
}
void MDoubleCardMenu::hide() {
	if (menu) menu->setVisible(false);
}
void MDoubleCardMenu::show() {
	if (menu) menu->setVisible(true);
}