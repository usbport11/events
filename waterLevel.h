#ifndef waterlevelH
#define waterlevelH

#include "cocos2d.h"

class MWaterLevel {
private:
	cocos2d::Scene* pScene;
	float current;
    float limit;
	float step;
    cocos2d::Vec2 position;
	std::string animatedName;
	std::string staticName;
	cocos2d::Size spriteSize;
	cocos2d::Size halfSize;
	std::string floodValueMask;
	void update();
public:
	MWaterLevel();
	bool create(cocos2d::Scene* _pScene, const std::string& _animatedName, const std::string& _staticName, cocos2d::Vec2 _position, cocos2d::Size _spriteSize);
	void increase();
	bool fullFilled();
	void reset();
	void setCurrent(float _current);
};

#endif
