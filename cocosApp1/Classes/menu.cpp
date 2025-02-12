#include "menu.h"
#include "MainScene.h"
#include "logic/adventurer.h"
#include "gridMap.h"
#include "MenuItemImageExt.h"
#include <iostream>

MMenu::MMenu():mask("cell%d"),name("MainMenu") {
	pMainScene = nullptr;
}

MMenu::~MMenu() {
}

void MMenu::menuAbflussCallback(cocos2d::Ref* pSender) {
    selectMenuItem(pSender);
    std::cout << " [Menu] Item 'abflus' selected" << std::endl;
    if (!pMainScene->startAbfluss()) {
        std::cout << " [Menu] Item 'abluss' failed!" << std::endl;
    }
}

void MMenu::menuEndTurnCallback(cocos2d::Ref* pSender) {
    selectMenuItem(pSender);
    std::cout << " [Menu] Item 'endTurn' selected" << std::endl;
    if (!pMainScene->endTurn()) {
        std::cout << " [Menu] Item 'endTurn' failed!" << std::endl;
    }
}

void MMenu::menuMoveCallback(cocos2d::Ref* pSender) {
    selectMenuItem(pSender);
    std::cout << " [Menu] Item 'move' selected" << std::endl;
    if (!pMainScene->startMove()) {
        std::cout << " [Menu] Item 'move' failed!" << std::endl;
    }
}

void MMenu::menuHandOverCallback(cocos2d::Ref* pSender) {
    selectMenuItem(pSender);
    std::cout << " [Menu] Item 'handOver' selected" << std::endl;
    if (!pMainScene->startHandover()) {
        std::cout << " [Menu] item 'handOver' failed!" << std::endl;
    }
}

void MMenu::menuGetArtifactCallback(cocos2d::Ref* pSender) {
    selectMenuItem(pSender);
    std::cout << " [Menu] Item 'getArtifact' selected" << std::endl;
    if (!pMainScene->getArtifact()) {
        std::cout << " [Menu] item 'getArtifact' failed!" << std::endl;
    }
}

void MMenu::menuExtractCallback(cocos2d::Ref* pSender) {
    selectMenuItem(pSender);
    std::cout << " [Menu] Item 'extract' selected" << std::endl;
    if (!pMainScene->extract()) {
        std::cout << " [Menu] Item 'extract' failed!" << std::endl;
    }
}

void MMenu::menuFlyCallback(cocos2d::Ref* pSender) {
    selectMenuItem(pSender);
    std::cout << " [Menu] Item 'fly' selected" << std::endl;
    if (!pMainScene->startFly()) {
        std::cout << " [Menu] Item 'fly' failed!" << std::endl;
    }
}

void MMenu::menuSwimCallback(cocos2d::Ref* pSender) {
    selectMenuItem(pSender);
    std::cout << " [Menu] Item 'swim' selected" << std::endl;
    if (!pMainScene->startSwim()) {
        std::cout << " [Menu] Item 'swim' failed!" << std::endl;
    }
}

/*
void MMenu::menuSkipCallback(cocos2d::Ref* pSender) {
    selectMenuItem(pSender);
    std::cout << " [Menu] Menu item 'skip' selected" << std::endl;
    if (!pMainScene->skip()) {
        std::cout << " [Menu] item 'skip' failed!" << std::endl;
    }
}
*/

bool MMenu::create(MMainScene* _pMainScene) {
    if (!_pMainScene) {
        return false;
    }

    pMainScene = _pMainScene;

    const std::string nameList[] = {"EndTurn", "Move", "Abfluss", "HandOver", "GetArtifact", "Extract", "Fly", "Swim"};//, "Skip"
    std::map<std::string, ccMenuCallback> menuCallback;
    menuCallback["EndTurn"] = CC_CALLBACK_1(MMenu::menuEndTurnCallback, this);
    menuCallback["Move"] = CC_CALLBACK_1(MMenu::menuMoveCallback, this);
    menuCallback["Abfluss"] = CC_CALLBACK_1(MMenu::menuAbflussCallback, this);
    menuCallback["HandOver"] = CC_CALLBACK_1(MMenu::menuHandOverCallback, this);
    menuCallback["GetArtifact"] = CC_CALLBACK_1(MMenu::menuGetArtifactCallback, this);
    menuCallback["Extract"] = CC_CALLBACK_1(MMenu::menuExtractCallback, this);
    menuCallback["Fly"] = CC_CALLBACK_1(MMenu::menuFlyCallback, this);
    menuCallback["Swim"] = CC_CALLBACK_1(MMenu::menuSwimCallback, this);
    //menuCallback["Skip"] = CC_CALLBACK_1(MMenu::menuSkipCallback, this);

    float topOffset = pMainScene->getContentSize().height - 30;
    const std::string btnBackPng[2] = { "back_off_2.png", "back_on_2.png" };

    //create menu
    int num = 0;
    cocos2d::Vector<cocos2d::MenuItem*> menuItems;
    for (int i = 0; i < sizeof(nameList) / sizeof(std::string); i++) {
        if (menuCallback.find(nameList[i]) == menuCallback.end()) {
            std::cout << " [Menu] Can't find item with name " << nameList[i] << std::endl;
        }
        MenuItemImageExt* menuItem = MenuItemImageExt::create(btnBackPng[0], btnBackPng[1], "", menuCallback[nameList[i]]);
        if(!menuItem) {
            return false;
        }
        cocos2d::Vec2 itemPosition = Vec2(menuItem->getContentSize().width / 2, topOffset - num * menuItem->getContentSize().height);
        menuItem->setPosition(itemPosition);
        menuItem->setName(nameList[i]);
        menuItems.pushBack(menuItem);
        cocos2d::Label* itemLabel = Label::createWithTTF(nameList[i], "fonts/Marker Felt.ttf", 24);
        if (!itemLabel) {
            return false;
        }
        itemLabel->setPosition(itemPosition);
        pMainScene->addChild(itemLabel, 2);
        num ++;
    }

    cocos2d::Menu* menu = Menu::createWithArray(menuItems);
    if(!menu) {
        return false;
    }
    menu->setPosition(Vec2::ZERO);
    menu->setName(name);
    pMainScene->addChild(menu, 1);
    menuCallback.clear();

    return true;
}

void MMenu::unselectMenuAll() {
    cocos2d::Node* node = pMainScene->getChildByName(name);
    cocos2d::Vector<cocos2d::Node*> items = node->getChildren();
    for (int i = 0; i < items.size(); i++) {
        items.at(i)->setColor(cocos2d::Color3B(255, 255, 255));
    }
}

void MMenu::selectMenuItem(cocos2d::Ref* pSender) {
    unselectMenuAll();
    MenuItemImageExt* menuItem = (MenuItemImageExt*)pSender;
    menuItem->setColor(cocos2d::Color3B(255, 0, 0));
    menuItem->selected();//?
}