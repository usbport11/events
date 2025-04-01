#include "utils.h"

bool createAnimSpriteFromPlist(cocos2d::Scene* scene, const std::string& fileName, const std::string& spriteName, const std::string& prefix, int count, float step) {
    if (fileName.empty() || spriteName.empty() || prefix.empty() || count <= 0) {
        return false;
    }
    cocos2d::SpriteFrameCache* spritecache = cocos2d::SpriteFrameCache::getInstance();
    if (!spritecache) {
        return false;
    }

    cocos2d::Vector<cocos2d::SpriteFrame*> animFrames;
    spritecache->addSpriteFramesWithFile(fileName);
    std::string key;
    char buffer[32];
    for (int i = 0; i < count; i++) {
        memset(buffer, 0, 32);
        snprintf(buffer, 32, "%d", i);
        key = prefix + buffer;
        animFrames.pushBack(spritecache->getSpriteFrameByName(key));
    }

    cocos2d::Sprite* animSprite = cocos2d::Sprite::createWithSpriteFrame(animFrames.front());
    animSprite->setName(spriteName);
    animSprite->getTexture()->setAliasTexParameters();
    cocos2d::Animation* animation = cocos2d::Animation::createWithSpriteFrames(animFrames, step);
    cocos2d::Animate* animate = cocos2d::Animate::create(animation);
    animSprite->runAction(cocos2d::RepeatForever::create(animate));
    scene->addChild(animSprite, 1);
    animFrames.clear();

    return true;
}