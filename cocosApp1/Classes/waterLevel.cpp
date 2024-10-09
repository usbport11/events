#include "waterLevel.h"

MWaterLevel::MWaterLevel() {
	current = 0;
    limit = 0;
    position = cocos2d::Vec2(0, 0);
}
bool MWaterLevel::create(cocos2d::Scene* _scene, const std::string& _animatedName, const std::string& _staticName, cocos2d::Vec2 _position, cocos2d::Size _spriteSize, int _limit) {
	if(!_scene) return false;
	scene = _scene;
	if(_animatedName.empty() || _staticName.empty()) return false;
	animatedName = _animatedName;
	staticName = _staticName;
	if(_limit < 0) return false;
	limit = _limit;
	position = _position;
	spriteSize = _spriteSize;
	halfSize = spriteSize / 2;

    current = 0;
    char buffer[10];
    for (int i = 1; i <= limit; i++) {
        memset(buffer, 0, 10);
        snprintf(buffer, 10, "Level %d", i);
        cocos2d::Label* labelLevel = cocos2d::Label::createWithTTF(buffer, "fonts/Marker Felt.ttf", 24);
        if (!labelLevel) return false;
        labelLevel->setPosition(cocos2d::Vec2(position.x - spriteSize.width, (position.y - spriteSize.height) + spriteSize.height * i - 1));//70
        scene->addChild(labelLevel);
    }
	
	return true;
}
void MWaterLevel::increase() {
	if (!fullFilled()) {
        current ++;
        scene->getChildByName(animatedName)->setPosition(cocos2d::Vec2(position.x, position.y + spriteSize.height * (current - 1)));
        scene->getChildByName(animatedName)->setVisible(true);
        if (current > 1) {
            scene->getChildByName(staticName)->setPosition(cocos2d::Vec2(position.x, position.y + halfSize.height * (current - 2)));
            scene->getChildByName(staticName)->setScaleY(current - 1);
            scene->getChildByName(staticName)->setVisible(true);
        }
    }
}
bool MWaterLevel::fullFilled() {
	return (current >= limit);
}

void MWaterLevel::reset() {
	current = 0;
	scene->getChildByName(animatedName)->setVisible(false);
	scene->getChildByName(staticName)->setVisible(false);
}