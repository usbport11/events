#ifndef waterlevelH
#define waterlevelH

#include "cocos2d.h"

class MWaterLevel {
private:
	cocos2d::Scene* scene;
	int current;
    int limit;
    cocos2d::Vec2 position;
	std::string animatedName;
	std::string staticName;
	cocos2d::Size spriteSize;
	cocos2d::Size halfSize;
public:
	MWaterLevel();
	bool create(cocos2d::Scene* _scene, const std::string& _animatedName, const std::string& _staticName, cocos2d::Vec2 _position, cocos2d::Size _spriteSize, int _limit);
	void increase();
	bool fullFilled();
	void reset();
};

#endif
