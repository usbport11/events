#include "HelloWorldScene.h"
#include "MenuItemImageExt.h"

USING_NS_CC;

Scene* HelloWorld::createScene() {
    return HelloWorld::create();
}

static void problemLoading(const char* filename) {
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool HelloWorld::createAnimSpriteFromPlist(const std::string& fileName, const std::string& spriteName, const std::string& prefix, int count, float step) {
    if (fileName.empty() || spriteName.empty() || prefix.empty() || count <= 0) {
        return false;
    }

    auto spritecache = SpriteFrameCache::getInstance();
    if (!spritecache) {
        return false;
    }

    Vector<SpriteFrame*> animFrames;
    spritecache->addSpriteFramesWithFile(fileName);
    std::string key;
    char buf[3];
    for (int i = 0; i < count; i++) {
        memset(buf, 0, 3);
        itoa(i, buf, 10);
        key = prefix + buf;
        animFrames.pushBack(spritecache->getSpriteFrameByName(key));
    }

    cocos2d::Sprite* animSprite_fox = Sprite::createWithSpriteFrame(animFrames.front());
    animSprite_fox->setName(spriteName);
    animSprite_fox->getTexture()->setAliasTexParameters();
    cocos2d::Animation* animation_fox = Animation::createWithSpriteFrames(animFrames, step);
    cocos2d::Animate* animate_fox = Animate::create(animation_fox);
    animSprite_fox->runAction(RepeatForever::create(animate_fox));
    this->addChild(animSprite_fox, 1);
    animFrames.clear();

    return true;
}

bool HelloWorld::init() {
    if(!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    offset = cocos2d::Vec2(0, 0);
    cellSize = cocos2d::Vec2(64, 64);
    halfSize = cocos2d::Vec2(32, 32);
    cellsCount = cocos2d::Vec2(4, 4);
    speed = cocos2d::Vec2(1, 1);
    currentCell = cocos2d::Vec2(0, 0);
    gridRect = cocos2d::Rect(0, 0, cellSize.x * cellsCount.x, cellSize.y * cellsCount.y);
    moving = false;
    //pg.setWorldSize(N2Vector(cellsCount.x, cellsCount.y));
    //pg.setDiagonalMovement(false);

    auto listener = EventListenerMouse::create(); 
    listener->onMouseMove = CC_CALLBACK_1(HelloWorld::onMouseMove, this);
    listener->onMouseDown = CC_CALLBACK_1(HelloWorld::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    auto cache = SpriteFrameCache::getInstance();
    if (!cache) {
        return false;
    }
    cache->addSpriteFramesWithFile("anim/grass.plist");
    auto sprite = Sprite::createWithSpriteFrame(cache->getSpriteFrameByName("gr0"));
    sprite->setPosition(300, 300);
    sprite->setScale(4.0);
    sprite->getTexture()->setAliasTexParameters();
    this->addChild(sprite);

    auto sprite2 = Sprite::createWithSpriteFrame(cache->getSpriteFrameByName("gr1"));
    sprite2->setPosition(370, 300);
    sprite2->setScale(4.0);
    sprite2->getTexture()->setAliasTexParameters();
    this->addChild(sprite2);

    createAnimSpriteFromPlist("anim/out.plist", "selection", "pt", 4, 0.2);
    this->getChildByName("selection")->setPosition(100, 600);
    this->getChildByName("selection")->setScale(1.0);
    this->getChildByName("selection")->setVisible(false);
    createAnimSpriteFromPlist("anim/fox.plist", "anim_fox", "fox_pt", 4, 0.1);
    this->getChildByName("anim_fox")->setPosition(700, 80);
    this->getChildByName("anim_fox")->setScale(4.0);
    this->getChildByName("anim_fox")->setVisible(true);

    return true;
}

void HelloWorld::update(float delta) {
    /*
    if (moving) {
        if (path.empty()) {
            moving = false;
        }
        else {
            N2Vector n2v = path.front();
            currentCell = cocos2d::vec2(n2v.x, n2v.y);
            cocos2d::vec2 destination = cocos2d::vec2(offset.x + n2v.x * size.x + halfSize.x, offset.y + n2v.y * size.y + halfSize.y);
            if (sprite.pos == destination) {
                path.erase(path.begin());
            }
            else {
                moveSprite(sprite, destination);
            }
        }
    }
    */
}

void HelloWorld::menuCloseCallback(Ref* pSender) {
    Director::getInstance()->end();
}

cocos2d::Vec2 HelloWorld::getCellUnderMouse(cocos2d::Event* event) {
    cocos2d::EventMouse* e = (EventMouse*)event;
    cocos2d::Vec2 cell = cocos2d::Vec2(e->getCursorX(), e->getCursorY());
    if (gridRect.containsPoint(cell)) {
        cell = cocos2d::Vec2((int)(offset.x + cell.x / gridRect.size.width), (int)(offset.y + cell.y / gridRect.size.height));
    }
    else {
        cell = cocos2d::Vec2(-1, -1);
    }
    return cell;
}

void HelloWorld::selectCell(cocos2d::Vec2 cell, std::string name) {
    if (cell.x < 0 || cell.y < 0) {
        this->getChildByName(name)->setVisible(false);
        return;
    }
    this->getChildByName(name)->setPosition(getCoordsByCell(cell));
    this->getChildByName(name)->setVisible(true);
}

cocos2d::Vec2 HelloWorld::getCoordsByCell(cocos2d::Vec2 cell) {
    return cocos2d::Vec2(offset.x + cell.x * cellSize.x + halfSize.x, offset.y + cell.y * cellSize.y + halfSize.y);
}

cocos2d::Vec2 HelloWorld::sign(cocos2d::Vec2 vec) {
    return cocos2d::Vec2((0 < vec.x) - (vec.x < 0), (0 < vec.y) - (vec.y < 0));
}

void HelloWorld::moveSprite(cocos2d::Sprite& sprite, cocos2d::Vec2 destination) {
    cocos2d::Vec2 direction = sign(destination - sprite.getPosition());
    cocos2d::Vec2 step = cocos2d::Vec2(speed.x * direction.x, speed.y * direction.y);
    sprite.setPosition(sprite.getPosition() + step);
}

void HelloWorld::onMouseMove(cocos2d::Event* event) {
    selectCell(getCellUnderMouse(event), "selection");
}

void HelloWorld::onMouseDown(Event* event) {
    if (moving) {
        return;
    }
    cocos2d::Vec2 cell = getCellUnderMouse(event);
    if (cell.x < 0 || cell.y < 0) {
        return;
    }
    if (cell == currentCell) {
        return;
    }
    //std::vector<N2Vector> path = pg.findPath(N2Vector(cell.x, cell.y), N2Vector(sprite->getPositionX(), sprite->getPositionY()));
    //moving = true;
}