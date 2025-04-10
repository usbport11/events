#include "DoubleCardMenu.h"

void MDoubleCardMenu::card1Callback(cocos2d::Ref* pSender) {
	if (menu) menu->setVisible(false);
	//call main scene func
}
void MDoubleCardMenu::card2Callback(cocos2d::Ref* pSender) {
	if (menu) menu->setVisible(false);
	//call main scene func
}
MDoubleCardMenu::MDoubleCardMenu() {
	pScene = nullptr;
	menu = nullptr;
}
MDoubleCardMenu::~MDoubleCardMenu() {
	if (sprites[0]) pScene->removeChild(sprites[0]);
	if (sprites[1]) pScene->removeChild(sprites[1]);
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
	int i = 0;
	for(int i=0; i < callbacks.size(); i++) {
		menuItem = cocos2d::MenuItemImage::create("no_image.png", "", callbacks[i]);
		if (!menuItem) return false;
		menuItems.pushBack(menuItem);
		i ++;
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
bool MDoubleCardMenu::reset(cocos2d::SpriteFrame* card1SpriteFrame, cocos2d::SpriteFrame* card2SpriteFrame) {
	if (!card1SpriteFrame) return false;
	if (!card2SpriteFrame) return false;

	//remove old sprites
	if (sprites[0]) pScene->removeChild(sprites[0]);
	if (sprites[1]) pScene->removeChild(sprites[1]);

	std::vector<cocos2d::SpriteFrame*> cardSprites;
	cardSprites.push_back(card1SpriteFrame);
	cardSprites.push_back(card2SpriteFrame);

	cocos2d::Node* node = menu;
	cocos2d::Vector<cocos2d::Node*> items = node->getChildren();
	cocos2d::MenuItemImage* menuItem;
	float offset;
	for (int i = 0; i < items.size(); i++) {
		menuItem = (cocos2d::MenuItemImage*)items.at(i);
		sprites[i] = cocos2d::Sprite::createWithSpriteFrame(cardSprites[i]);
		if(!sprites[i]) return false;
		menuItem->setNormalImage(sprites[i]);
		menuItem->setSelectedImage(sprites[i]);
		offset = position.x + menuItem->getContentSize().width * items.size() / 4;
		menuItem->setPosition(offset + i * menuItem->getContentSize().width, position.y);
	}
	menu->setVisible(true);

	return true;
}