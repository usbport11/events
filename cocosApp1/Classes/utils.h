#ifndef utilsH
#define utilsH

#include "cocos2d.h"

bool createAnimSpriteFromPlist(cocos2d::Scene* scene, const std::string& fileName, const std::string& spriteName, const std::string& prefix, int count, float step);

#endif