#include "waterLevel.h"

MWaterLevel::MWaterLevel() {
    current = 2.1;
    limit = 6;
    step = 0.5;
    position = cocos2d::Vec2(0, 0);
}
bool MWaterLevel::create(cocos2d::Scene* _scene, const std::string& _animatedName, const std::string& _staticName, cocos2d::Vec2 _position, cocos2d::Size _spriteSize) {
	if(!_scene) return false;
	scene = _scene;
	if(_animatedName.empty() || _staticName.empty()) return false;
	animatedName = _animatedName;
	staticName = _staticName;
	position = _position;
	spriteSize = _spriteSize;
	halfSize = spriteSize / 2;

    //place labels
    char buffer[10];
    for (int i = 1; i <= limit; i++) {
        memset(buffer, 0, 10);
        snprintf(buffer, 10, "Level %d", i);
        cocos2d::Label* labelLevel = cocos2d::Label::createWithTTF(buffer, "fonts/Marker Felt.ttf", 24);
        if (!labelLevel) return false;
        labelLevel->setPosition(cocos2d::Vec2(position.x - spriteSize.width, (position.y - spriteSize.height - halfSize.height) + spriteSize.height * i));
        scene->addChild(labelLevel);
    }
    update();
	
	return true;
}
void MWaterLevel::update() {
    //head - fixed size
    scene->getChildByName(animatedName)->setPosition(cocos2d::Vec2(position.x, position.y + spriteSize.height * (current - 2)));
    scene->getChildByName(animatedName)->setVisible(true);
    //body - scaled
    scene->getChildByName(staticName)->setContentSize(cocos2d::Size(spriteSize.width, spriteSize.height + spriteSize.height * (current - 2)));
    scene->getChildByName(staticName)->setPosition(cocos2d::Vec2(scene->getChildByName(animatedName)->getPosition().x, position.y + spriteSize.height * ((current - 2) / 2)));
    scene->getChildByName(staticName)->setVisible(true);
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