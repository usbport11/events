#ifndef utilsH
#define utilsH

#include "cocos2d.h"

bool createAnimSpriteFromPlist(cocos2d::Scene* scene, const std::string& fileName, const std::string& spriteName, const std::string& prefix, int count, float step);
cocos2d::Sprite* combineSpritesFromFrames(cocos2d::SpriteFrame* spfA, cocos2d::SpriteFrame* spfB);
cocos2d::Sprite* combineSprites(cocos2d::Sprite* spfA, cocos2d::Sprite* spfB);

#endif