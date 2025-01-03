#include "menu.h"
#include "gridMap.h"
#include "MenuItemImageExt.h"

MMenu::MMenu() {
	pScene = nullptr;
	pGridMap = nullptr;
}

MMenu::~MMenu() {
}

void MMenu::menuAbflussCallback(cocos2d::Ref* pSender) {
    selectMenuItem(pSender);
}

void MMenu::menuEndTurnCallback(cocos2d::Ref* pSender) {
    selectMenuItem(pSender);
}

void MMenu::menuMoveCallback(cocos2d::Ref* pSender) {
    selectMenuItem(pSender);
    //highlight available cells
    char buffer[16] = {0};
    cocos2d::Vec2 curr = pGridMap->getCurrentCell();

    //clear alredy higlighted
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            snprintf(buffer, 16, "cell_%d_%d", i, j);
            cocos2d::Sprite* sp = (cocos2d::Sprite*)pScene->getChildByName(buffer);
            if (!sp) continue;
            sp->setColor(cocos2d::Color3B(255, 255, 255));
        }
    }

    //hightlight needed
    int ind[4][2] = { {-1,0},{1,0},{0,-1},{0, 1} };
    for (int i = 0; i<4; i++) {
        memset(buffer, 0, 16);
        cocos2d::Vec2 neigh = curr + cocos2d::Vec2(ind[i][0], ind[i][1]);
        if (neigh.x < 0 || neigh.y < 0 || neigh.x >= 4 || neigh.y >= 4) continue;
        snprintf(buffer, 16, "cell_%d_%d", (int)neigh.x, (int)neigh.y);
        cocos2d::Sprite* sp = (cocos2d::Sprite*) pScene->getChildByName(buffer);
        if (!sp) continue;
        sp->setColor(cocos2d::Color3B(0, 255, 0));
    }
}

void MMenu::menuHandOverCallback(cocos2d::Ref* pSender) {
    selectMenuItem(pSender);
}

void MMenu::menuGetArtifactCallback(cocos2d::Ref* pSender) {
    selectMenuItem(pSender);
}

bool MMenu::create(cocos2d::Scene* _pScene, MGridMap* _pGridMap) {
    if (!_pScene || !_pGridMap) {
        return false;
    }

    pScene = _pScene;
    pGridMap = _pGridMap;

    const std::string nameList[5] = {"EndTurn", "Move", "Abfluss", "HandOver", "GetArtifact"};
    std::map<std::string, ccMenuCallback> menuCallback;
    menuCallback["EndTurn"] = CC_CALLBACK_1(MMenu::menuEndTurnCallback, this);
    menuCallback["Move"] = CC_CALLBACK_1(MMenu::menuMoveCallback, this);
    menuCallback["Abfluss"] = CC_CALLBACK_1(MMenu::menuAbflussCallback, this);
    menuCallback["HandOver"] = CC_CALLBACK_1(MMenu::menuHandOverCallback, this);
    menuCallback["GetArtifact"] = CC_CALLBACK_1(MMenu::menuGetArtifactCallback, this);

    float topOffset = pScene->getContentSize().height - 30;
    const std::string btnBackPng[2] = { "back_off.png", "back_on.png" };
    const std::string fontTTF = "fonts/Marker Felt.ttf";

    //create menu
    int num = 0;
    cocos2d::Vector<cocos2d::MenuItem*> menuItems;
    for (int i = 0; i < sizeof(nameList) / sizeof(std::string); i++) {
        MenuItemImageExt* menuItem = MenuItemImageExt::create(btnBackPng[0], btnBackPng[1], "", menuCallback[nameList[i]]);
        if(!menuItem) {
            return false;
        }
        cocos2d::Vec2 itemPosition = Vec2(menuItem->getContentSize().width / 2, topOffset - num * menuItem->getContentSize().height);
        menuItem->setPosition(itemPosition);
        menuItem->setName(nameList[i]);
        menuItems.pushBack(menuItem);
        cocos2d::Label* itemLabel = Label::createWithTTF(nameList[i], fontTTF, 24);
        if (!itemLabel) {
            return false;
        }
        itemLabel->setPosition(itemPosition);
        pScene->addChild(itemLabel, 2);
        num ++;
    }
    cocos2d::Menu* menu = Menu::createWithArray(menuItems);
    if(!menu) {
        return false;
    }
    menu->setPosition(Vec2::ZERO);
    menu->setName("MainMenu");
    pScene->addChild(menu, 1);
    menuCallback.clear();

    return true;
}

void MMenu::unselectMenuAll() {
    cocos2d::Node* node = pScene->getChildByName("MainMenu");
    cocos2d::Vector<cocos2d::Node*> items = node->getChildren();
    for (int i = 0; i < items.size(); i++) {
        items.at(i)->setColor(cocos2d::Color3B(255, 255, 255));
    }
}

void MMenu::selectMenuItem(cocos2d::Ref* pSender) {
    unselectMenuAll();
    MenuItemImageExt* menuItem = (MenuItemImageExt*)pSender;
    menuItem->setColor(cocos2d::Color3B(255, 0, 0));
    menuItem->selected();
}

