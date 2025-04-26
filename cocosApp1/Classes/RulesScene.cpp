#include "RulesScene.h"

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
	
	const std::string rules = "Main target of game is to collect all four artifacts, reach extract area by all adventurers\nand use helicoper card for escape.\n\n"
		"Number of adventurers varies form one to four.\n"
		"Each adventurer can execute one of four main actions : move, abfluss, get artifact and hand over.\n"
		"Also each adventurer has special abilities.\n"
		"Each action spend action points, one action - one action point. On each turn adventurers has three action points.\n"
		"Abluss - remove flood level from one area. Only direct connected area, not diagonal.\n"
		"Move - change adventurer area. Same area rules like abfluss.\n"
		"Hand over - pass artifact card to another player.\n"
		"Get artifact - need to stay on artifact area and has same four arifacts cards in hand.\n\n"
		"Special abilities.\n"
		"Explorer. Can diag move or abfluss.\n"
		"Pilot. One time at turn can move anywhere.\n"
		"Engineer. Can abluss one or two areas.\n"
		"Liaison. Can handover on any distance.\n"
		"Navigator. Can move other adventurer on one or two areas.\n"
		"Diver. Can move over any number abfluss or none areas.\n\n"
		"On start game each adventurer has get two cards.\n"
		"Maximum number of card in hand is five. If hand has more the five cards, extend cards must be discarded.\n"
		"Cards may be three types : item cards, flood cards, artifact cards.\n"
		"Items cards are sandbag (abfluss any area) and helicopter (move any adventurer anywhere or extract).\n"
		"Flood cards are same. They increase wather level.\n"
		"Artifacts cards need to collect artifacts.\n\n"
		"Lose conditions.\n"
		"Two area of same artifact totaly floodand this artifact not collected.\n"
		"Extraction aread flood.\n"
		"Water level is reach maximum.";
	
	cocos2d::Label* label;
	label = Label::createWithTTF(rules, "fonts/Marker Felt.ttf", 20);
    if (!label) return false;
	label->setTextColor(cocos2d::Color4B(0,0,0,255));
    label->setName("lblAdventurerDescrition");
    label->setAnchorPoint(cocos2d::Vec2(0, 0));
    label->setPosition(30, 50);
    this->addChild(label, 1);

	cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
	if (!cache) return false;

	cocos2d::Sprite* background = cocos2d::Sprite::create("rules.png");
	if (!background) return false;
	background->setPosition(cocos2d::Vec2(visibleSize.width/2, visibleSize.height/2));
	background->setContentSize(cocos2d::Size(visibleSize.width, visibleSize.height));
	this->addChild(background, 0);

	cocos2d::Sprite* extractionPointArea = cocos2d::Sprite::createWithSpriteFrame(cache->getSpriteFrameByName("cell7"));
	if (!extractionPointArea) return false;
	extractionPointArea->setScale(1.5);
	extractionPointArea->setPosition(cocos2d::Vec2(950, 700));
	this->addChild(extractionPointArea, 1);

	cocos2d::Sprite* card;
	const std::string itemCards[3] = {"card6", "card7", "card8"};
	for (int i = 0; i < 3; i++) {
		card = cocos2d::Sprite::createWithSpriteFrame(cache->getSpriteFrameByName(itemCards[i]));
		if (!card) return false;
		card->setPosition(cocos2d::Vec2(700 + i * 80, 350));
		this->addChild(card, 1);
	}

	char buffer[32];
	cocos2d::Sprite* artSprite;
	for (int i = 0; i < 4; i++) {
		memset(buffer, 0, 32);
		snprintf(buffer, 32, "artifact%d", i);
		artSprite = cocos2d::Sprite::createWithSpriteFrame(cache->getSpriteFrameByName(buffer));
		if (!artSprite) return false;
		artSprite->setPosition(cocos2d::Vec2(700 + i * 80, 150));
		this->addChild(artSprite, 1);
	}
	
	return true;
}

void MRulesScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
		Director::getInstance()->popScene();
	}
}