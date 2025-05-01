#include "SettingsScene.h"
#include "MenuItemImageExt.h"
#include <iostream>

USING_NS_CC;

cocos2d::Scene* MSettingsScene::createScene() {
    return MSettingsScene::create();
}

bool MSettingsScene::init() {
	if(!Scene::init()) return false;
	
	cocos2d::EventListenerKeyboard* keybordListener = EventListenerKeyboard::create();
	keybordListener->onKeyPressed = CC_CALLBACK_2(MSettingsScene::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keybordListener, this);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	cocos2d::Label* itemLabel;
	cocos2d::ui::Slider* slider;

	itemLabel = Label::createWithTTF("Advenurers number: 1", "fonts/Marker Felt.ttf", 24);
	if (!itemLabel) return false;
	itemLabel->setName("lbl_slider_adv");
	itemLabel->setPosition(cocos2d::Vec2(visibleSize.width / 2, 600));
	this->addChild(itemLabel);

	slider = cocos2d::ui::Slider::create();
	slider->loadBarTexture("test/Slider_Back_1.png");
	slider->loadSlidBallTextures("test/SliderNode_Normal.png", "test/SliderNode_Press.png", "test/SliderNode_Disable.png");
	slider->loadProgressBarTexture("test/Slider_Normal.png");
	slider->setPosition(cocos2d::Vec2(visibleSize.width / 2, 550));
	slider->setName("slider_adv_num");
	slider->addTouchEventListener(CC_CALLBACK_2(MSettingsScene::adventurersNumberTouchCallback, this));
	this->addChild(slider);

	itemLabel = Label::createWithTTF("Water level: 1", "fonts/Marker Felt.ttf", 24);
	if (!itemLabel) return false;
	itemLabel->setName("lbl_slider_wtr");
	itemLabel->setPosition(cocos2d::Vec2(visibleSize.width / 2, 500));
	this->addChild(itemLabel);

	slider = cocos2d::ui::Slider::create();
	slider->loadBarTexture("test/Slider_Back_1.png");
	slider->loadSlidBallTextures("test/SliderNode_Normal.png", "test/SliderNode_Press.png", "test/SliderNode_Disable.png");
	slider->loadProgressBarTexture("test/Slider_Normal.png");
	slider->setPosition(cocos2d::Vec2(visibleSize.width / 2, 450));
	slider->setName("slider_wat_lev");
	slider->addTouchEventListener(CC_CALLBACK_2(MSettingsScene::waterLevelTouchCallback, this));
	this->addChild(slider);

	/*
	itemLabel = Label::createWithTTF("Music", "fonts/Marker Felt.ttf", 24);
	if (!itemLabel) return false;
	itemLabel->setPosition(cocos2d::Vec2(visibleSize.width / 2 - 25, 400));
	this->addChild(itemLabel);
	cocos2d::ui::CheckBox* checkbox = cocos2d::ui::CheckBox::create("test/CheckBox_Normal.png",
		"test/CheckBox_Press.png",
		"test/CheckBoxNode_Normal.png",
		"test/CheckBox_Disable.png",
		"test/CheckBoxNode_Disable.png");
	checkbox->addTouchEventListener(CC_CALLBACK_2(MSettingsScene::musicTouchCallback, this));
	checkbox->setPosition(cocos2d::Vec2(visibleSize.width / 2 + 50, 400));
	this->addChild(checkbox);
	*/

	cocos2d::Vector<cocos2d::MenuItem*> menuItems;
	MenuItemImage* menuItem;
	cocos2d::Vec2 itemPosition;
	float offset;

	int i = 0;
	std::map<std::string, ccMenuCallback> menuCallback;
	menuCallback["Submit"] = CC_CALLBACK_1(MSettingsScene::sumbitCallback, this);
	menuCallback["Cancel"] = CC_CALLBACK_1(MSettingsScene::cancelCallback, this);
	for (std::map<std::string, ccMenuCallback>::iterator it = menuCallback.begin();  it != menuCallback.end(); it ++) {
		menuItem = MenuItemImageExt::create("back_off.png", "back_on.png", "back_dis.png", it->second);
		if (!menuItem) return false;
		offset = (visibleSize.width - (menuItem->getContentSize().width + 20) * 2) / 2 + (menuItem->getContentSize().width + 20) / 2;
		itemPosition = Vec2(offset + i * (menuItem->getContentSize().width + 20), 150);
		menuItem->setName("settings_" + it->first);
		menuItem->setPosition(itemPosition);
		menuItems.pushBack(menuItem);

		itemLabel = Label::createWithTTF(it->first, "fonts/Marker Felt.ttf", 24);
		if (!itemLabel) return false;
		itemLabel->setPosition(itemPosition);
		this->addChild(itemLabel, 2);
		i++;
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

void MSettingsScene::adventurersNumberTouchCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
	auto slider = dynamic_cast<ui::Slider*>(sender);
	char buffer[32];
	cocos2d::Label* itemLabel;
	switch (type) {
	case ui::Widget::TouchEventType::ENDED:
		slider->setPercent(floorPercent(slider->getPercent()));
		itemLabel = (cocos2d::Label*)this->getChildByName("lbl_slider_adv");
		memset(buffer, 0, 32);
		snprintf(buffer, 32, "Advenurers number: %d", slider->getPercent()/33 + 1);
		itemLabel->setString(buffer);
		break;
	default:
		break;
	}
}

void MSettingsScene::waterLevelTouchCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
	auto slider = dynamic_cast<ui::Slider*>(sender);
	char buffer[32];
	cocos2d::Label* itemLabel;
	switch (type) {
	case ui::Widget::TouchEventType::ENDED:
		slider->setPercent(floorPercent(slider->getPercent()));
		itemLabel = (cocos2d::Label*)this->getChildByName("lbl_slider_wtr");
		memset(buffer, 0, 32);
		snprintf(buffer, 32, "Water level: %d", slider->getPercent() / 33 + 1);
		itemLabel->setString(buffer);
		break;
	default:
		break;
	}
}

void MSettingsScene::musicTouchCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
	switch (type) {
	case ui::Widget::TouchEventType::BEGAN:
		break;
	case ui::Widget::TouchEventType::ENDED:
		break;
	default:
		break;
	}
}

void MSettingsScene::sumbitCallback(cocos2d::Ref* pSender) {
	Director::getInstance()->popScene();
}

void MSettingsScene::cancelCallback(cocos2d::Ref* pSender) {
	Director::getInstance()->popScene();
}

void MSettingsScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
		Director::getInstance()->popScene();
	}
}

int MSettingsScene::floorPercent(int percent) {
	if (percent <= 16) return 0;
	if (percent >= 16 && percent <= 46) return 33;
	if (percent >= 46 && percent <= 86) return 66;
	if (percent > 86) return 100;
}