#include "waterLevel.h"

MWaterLevel::MWaterLevel() {
    current = 2.1;
    limit = 6;
    step = 0.5;
    position = cocos2d::Vec2(0, 0);
    floodValueMask = "Flood: %.1f";
}
bool MWaterLevel::create(cocos2d::Scene* _pScene, const std::string& _animatedName, const std::string& _staticName, cocos2d::Vec2 _position, cocos2d::Size _spriteSize) {
	if(!_pScene) return false;
    pScene = _pScene;
	if(_animatedName.empty() || _staticName.empty()) return false;
	animatedName = _animatedName;
	staticName = _staticName;
	position = _position;
	spriteSize = _spriteSize;
	halfSize = spriteSize / 2;

    //place labels
    char buffer[12];
    for (int i = 1; i <= limit; i++) {
        memset(buffer, 0, 12);
        snprintf(buffer, 12, "Level %d", i);
        cocos2d::Label* labelLevel = cocos2d::Label::createWithTTF(buffer, "fonts/Marker Felt.ttf", 24);
        if (!labelLevel) return false;
        labelLevel->setPosition(cocos2d::Vec2(position.x - spriteSize.width, (position.y - spriteSize.height - halfSize.height) + spriteSize.height * i));
        pScene->addChild(labelLevel);
    }

    memset(buffer, 0, 12);
    snprintf(buffer, 12, floodValueMask.c_str(), current);
    cocos2d::Label* labelFlood = cocos2d::Label::createWithTTF(buffer, "fonts/Marker Felt.ttf", 24);
    if (!labelFlood) return false;
    labelFlood->setPosition(cocos2d::Vec2(position.x - halfSize.width, (position.y - spriteSize.height - halfSize.height) + spriteSize.height * 0 + 16));
    labelFlood->setName("lblFloodLevel");
    pScene->addChild(labelFlood);
    update();
	
	return true;
}
void MWaterLevel::update() {
    //head - fixed size
    pScene->getChildByName(animatedName)->setPosition(cocos2d::Vec2(position.x, position.y + spriteSize.height * (current - 2)));
    pScene->getChildByName(animatedName)->setVisible(true);
    //body - scaled
    pScene->getChildByName(staticName)->setContentSize(cocos2d::Size(spriteSize.width, spriteSize.height + spriteSize.height * (current - 2)));
    pScene->getChildByName(staticName)->setPosition(cocos2d::Vec2(pScene->getChildByName(animatedName)->getPosition().x, position.y + spriteSize.height * ((current - 2) / 2)));
    pScene->getChildByName(staticName)->setVisible(true);
    //level label
    char buffer[12] = { 0 };
    memset(buffer, 0, 12);
    snprintf(buffer, 12, floodValueMask.c_str(), current);
    cocos2d::Label* labelFlood = (cocos2d::Label*)pScene->getChildByName("lblFloodLevel");
    if(labelFlood) labelFlood->setString(buffer);
}
void MWaterLevel::increase() {
	if (!fullFilled()) {
        current += step;
        update();
    }
}
bool MWaterLevel::fullFilled() {
	return (current >= limit);
}
void MWaterLevel::reset() {
	current = 2.1;
    update();
}

void MWaterLevel::setCurrent(float _current) {
    current = _current;
    update();
}