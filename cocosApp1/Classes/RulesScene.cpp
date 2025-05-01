#include "RulesScene.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

cocos2d::Scene* MRulesScene::createScene() {
    return MRulesScene::create();
}

bool MRulesScene::init() {
	if(!Scene::init()) return false;
	
	cocos2d::EventListenerKeyboard* keybordListener = EventListenerKeyboard::create();
	keybordListener->onKeyPressed = CC_CALLBACK_2(MRulesScene::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keybordListener, this);
	
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    cocos2d::ui::ScrollView* scrollView = cocos2d::ui::ScrollView::create();
    scrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    scrollView->setContentSize(cocos2d::Size(visibleSize.width + origin.x, visibleSize.height + origin.y));
    scrollView->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
    scrollView->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 2));

    cocos2d::Label* label;
    cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
    if (!cache) return false;

    cocos2d::Sprite* backgound = cocos2d::Sprite::create("rules.png");//lake.png
    if (!backgound) return false;
    backgound->setScale(2.2);
    backgound->setPosition(cocos2d::Vec2(backgound->getBoundingBox().size.width / 2, backgound->getBoundingBox().size.height / 2));

    const std::string rules = "Main target of game is to collect all four artifacts, reach extract area by all adventurers\nand use helicoper card for escape.\n\n"
        "Number of adventurers varies form one to four.\n"
        "Each adventurer can execute one of four main actions: move, abfluss, get artifact and hand over.\n"
        "Also each adventurer has special abilities.\n"
        "Each action spend action points, one action - one action point. On each turn adventurers has three action points.\n"
        "Abluss - remove flood level from one area. Only direct connected area and current area, not diagonal.\n"
        "Move - change adventurer area. Only direct connected area.\n"
        "Hand over - pass artifact card to another player. One card one action point.\n"
        "Get artifact - need to stay on artifact area and has same four arifacts cards in hand.\n\n"
        "Maximum number of card in hand is five. If hand has more the five cards, extend cards must be discarded.\n"
        "Cards may be three types: item cards, 'water come' cards and artifact cards.\n"
        "Items cards are sandbag (abfluss any area) and helicopter (move any adventurer anywhere or extract).\n"
        "'Water come' cards are same. They increase wather level and move areas cards back to flood deck\n"
        "Artifacts cards need to collect artifacts.\n\n"
        "Adventures special abilities.\n"
        "Explorer. Can diag move or abfluss.\n"
        "Pilot. One time at turn can move anywhere.\n"
        "Engineer. Can abluss one or two areas.\n"
        "Liaison. Can handover on any distance.\n"
        "Navigator. Can move other adventurer on one or two areas.\n"
        "Diver. Can move over any number abfluss or none areas.\n\n"
        "At game start all areas are placed randomly, six random areas flood by half.\n"
        "Adventurers also selected randomly. Each adventurer get two cards.\n\n"
        "Each area has one of three flood level: not flood, half flood, totaly flood. Area can be used to move only if not totaly flood.\n"
        "Area can abluss only if it at half flood level\n\n"
        "Game area.\n"
        "Left top corner - actions menu and number of action points used.\n"
        "Right top corner - water level\n"
        "Bottom part ow window - current adventurer hand, flood deck, item deck. Also there is adventurers menu.\n"
        "Adventurer menu show all adventurers. Current adventurer is colored by green. Selected adventurer by blinking frame.\n"
        "By click on adventurer icon changing help text that hold name and description.\n\n"
        "Lose conditions.\n"
        "Two area of same artifact totaly floodand this artifact not collected.\n"
        "Extraction aread flood.\n"
        "Water level is reach maximum.";

    cocos2d:Layer* layer = cocos2d::Layer::create();

        label = Label::createWithTTF(rules, "fonts/Marker Felt.ttf", 20);
        if (!label) return false;
        label->setTextColor(cocos2d::Color4B(0, 0, 0, 255));
        label->setAnchorPoint(cocos2d::Vec2(0, 0));
        label->setPosition(30, 60);
        layer->addChild(label, 1);

        cocos2d::Sprite* extractionPointArea = cocos2d::Sprite::createWithSpriteFrame(cache->getSpriteFrameByName("cell7"));
        if (!extractionPointArea) return false;
        extractionPointArea->setScale(1.5);
        extractionPointArea->setPosition(cocos2d::Vec2(950, 950));
        layer->addChild(extractionPointArea, 1);

        cocos2d::Sprite* card;
        const std::string itemCards[2] = {"card7", "card8" };
        for (int i = 0; i < 2; i++) {
            card = cocos2d::Sprite::createWithSpriteFrame(cache->getSpriteFrameByName(itemCards[i]));
            if (!card) return false;
            card->setPosition(cocos2d::Vec2(900 + i * 80, 700));
            layer->addChild(card, 1);
        }

        char buffer[32];
        cocos2d::Sprite* artSprite;
        for (int i = 0; i < 4; i++) {
            memset(buffer, 0, 32);
            snprintf(buffer, 32, "artifact%d", i);
            artSprite = cocos2d::Sprite::createWithSpriteFrame(cache->getSpriteFrameByName(buffer));
            if (!artSprite) return false;
            artSprite->setPosition(cocos2d::Vec2(735 + i * 80, 600));
            layer->addChild(artSprite, 1);
        }

        cocos2d::Sprite* cell;
        const std::string cellCards[2] = { "cell1", "cell2" };
        for (int i = 0; i < 2; i++) {
            cell = cocos2d::Sprite::createWithSpriteFrame(cache->getSpriteFrameByName(cellCards[i]));
            if (!cell) return false;
            if (i == 1) cell->setColor(cocos2d::Color3B(96, 96, 255));
            cell->setScale(1.5);
            cell->setPosition(cocos2d::Vec2(735 + i * 100, 450));
            layer->addChild(cell, 1);
        }

        layer->addChild(backgound);

    scrollView->setInnerContainerSize(Size(backgound->getBoundingBox().size.width, backgound->getBoundingBox().size.height));
    scrollView->addChild(layer);

    this->addChild(scrollView);
	
	return true;
}

void MRulesScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
		Director::getInstance()->popScene();
	}
}