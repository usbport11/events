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

cocos2d::Sprite* combineSpritesFromFrames(cocos2d::SpriteFrame* spfA, cocos2d::SpriteFrame* spfB) {
    cocos2d::Sprite* spC = nullptr;

    cocos2d::Texture2D* textureA = spfA->getTexture();
    cocos2d::Rect rectA = spfA->getRect();
    cocos2d::Sprite* spriteA = cocos2d::Sprite::createWithTexture(textureA, rectA);
    spriteA->setPosition(rectA.size.width / 2, rectA.size.height / 2);
    spriteA->setFlippedY(true);
    cocos2d::Texture2D* textureB = spfB->getTexture();
    cocos2d::Rect rectB = spfB->getRect();
    cocos2d::Sprite* spriteB = cocos2d::Sprite::createWithTexture(textureB, rectB);
    spriteB->setPosition(rectB.size.width / 2, rectB.size.height / 2);
    spriteB->setFlippedY(true);

    cocos2d::RenderTexture* texture = cocos2d::RenderTexture::create(rectA.size.width, rectA.size.height, cocos2d::PixelFormat::RGBA8888);
    texture->begin();
    spriteA->visit();
    spriteB->visit();
    texture->end();
    spC = cocos2d::Sprite::createWithTexture(texture->getSprite()->getTexture(), cocos2d::Rect(0, 0, rectA.size.width, rectA.size.height));

    return spC;
}

cocos2d::Sprite* combineSprites(cocos2d::Sprite* spA, cocos2d::Sprite* spB) {
    cocos2d::Sprite* spC = nullptr;

    cocos2d::Vec2 pos[2] = { spA->getPosition(), spB->getPosition() };
    spA->setPosition(spA->getTextureRect().size.width / 2, spA->getTextureRect().size.height / 2);
    spB->setPosition(spB->getTextureRect().size.width / 2, spB->getTextureRect().size.height / 2);

    cocos2d::RenderTexture* texture = cocos2d::RenderTexture::create(spA->getTextureRect().size.width, spA->getTextureRect().size.height, cocos2d::PixelFormat::RGBA8888);
    texture->begin();
    spA->visit();
    spB->visit();
    texture->end();
    spC = cocos2d::Sprite::createWithTexture(texture->getSprite()->getTexture(), cocos2d::Rect(0, 0, spA->getTextureRect().size.width, spA->getTextureRect().size.height));

    spA->setPosition(pos[0]);
    spB->setPosition(pos[1]);

    return spC;
}