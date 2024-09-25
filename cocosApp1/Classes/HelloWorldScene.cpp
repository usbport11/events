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
    char buffer[10];
    for (int i = 0; i < count; i++) {
        memset(buffer, 0, 10);
        snprintf(buffer, 10, "%d", i);
        key = prefix + buffer;
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

    auto listener = EventListenerMouse::create(); 
    listener->onMouseMove = CC_CALLBACK_1(HelloWorld::onMouseMove, this);
    listener->onMouseDown = CC_CALLBACK_1(HelloWorld::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    createAnimSpriteFromPlist("anim/out.plist", "selection", "pt", 4, 0.2);
    this->getChildByName("selection")->setPosition(0, 0);
    this->getChildByName("selection")->setScale(1.0);
    this->getChildByName("selection")->setVisible(false);
    createAnimSpriteFromPlist("anim/fox.plist", "anim_fox", "fox_pt", 4, 0.1);
    this->getChildByName("anim_fox")->setPosition(32, 32);
    this->getChildByName("anim_fox")->setScale(2.0);
    this->getChildByName("anim_fox")->setVisible(true);

    createCells(cellsCount.x, cellsCount.y);
    pg.setWorldSize(NVector2(cellsCount.x, cellsCount.y));
    pg.setDiagonalMovement(false);

    this->scheduleUpdate();

    return true;
}

void HelloWorld::createCells(int x, int y) {
    if (x < 0 || y < 0) {
        return;
    }
    auto cache = SpriteFrameCache::getInstance();
    if (!cache) {
        return;
    }
    cache->addSpriteFramesWithFile("anim/grass.plist");

    std::string key;
    char buffer[16];
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            memset(buffer, 0, 16);
            snprintf(buffer, 10, "cell_%d_%d", i, j);
            key = buffer;
            auto sp = Sprite::createWithSpriteFrame(cache->getSpriteFrameByName("gr0"));
            if (!sp) {
                return;
            }
            sp->setScale(4.0);
            sp->getTexture()->setAliasTexParameters();
            sp->setPosition(Vec2(offset.x + i * cellSize.x + halfSize.x, offset.y + j * cellSize.y + halfSize.y));
            this->addChild(sp, 0, key);
        }
    }
}

void HelloWorld::update(float delta) {
    if (moving) {
        if (path.empty()) {
            moving = false;
        }
        else {
            NVector2 nv2 = path.front();
            currentCell = cocos2d::Vec2(nv2.x, nv2.y);
            cocos2d::Vec2 destination = cocos2d::Vec2(offset.x + nv2.x * cellSize.x + halfSize.x, offset.y + nv2.y * cellSize.y + halfSize.y);
            if (this->getChildByName("anim_fox")->getPosition() == destination) {
                path.erase(path.begin());
            }
            else {
                moveSprite((cocos2d::Sprite*)this->getChildByName("anim_fox"), destination);
            }
        }
    }
}

void HelloWorld::menuCloseCallback(Ref* pSender) {
    Director::getInstance()->end();
}

cocos2d::Vec2 HelloWorld::getCellUnderMouse(cocos2d::Event* event) {
    cocos2d::EventMouse* e = (EventMouse*)event;
    cocos2d::Vec2 cell = cocos2d::Vec2(e->getCursorX(), e->getCursorY());
    if (gridRect.containsPoint(cell)) {
        cell = cocos2d::Vec2((int)(cell.x / cellSize.x), (int)(cell.y / cellSize.y));
    }
    else {
        cell = cocos2d::Vec2(-1, -1);
    }
    return cell;
}

void HelloWorld::selectCell(cocos2d::Vec2 cell, std::string name) {
    if (cell.x < 0 || cell.y < 0 || cell.x >= cellsCount.x || cell.y >= cellsCount.y) {
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

void HelloWorld::moveSprite(cocos2d::Sprite* sprite, cocos2d::Vec2 destination) {
    cocos2d::Vec2 direction = sign(destination - sprite->getPosition());
    cocos2d::Vec2 step = cocos2d::Vec2(speed.x * direction.x, speed.y * direction.y);
    sprite->setPosition(sprite->getPosition() + step);
}

void HelloWorld::onMouseMove(cocos2d::Event* event) {
    selectCell(getCellUnderMouse(event), "selection");
}

void HelloWorld::onMouseDown(Event* event) {
    if (moving) {
        return;
    }
    cocos2d::Vec2 cell = getCellUnderMouse(event);

    if (cell.x < 0 || cell.y < 0 || cell.x >= cellsCount.x || cell.y >= cellsCount.y) {
        return;
    }
    if (cell == currentCell) {
        return;
    }

    //momental move
    //currentCell = cell;
    //this->getChildByName("anim_fox")->setPosition(getCoordsByCell(cell));
    
    //flow move
    path.clear(); 
    path = pg.findPath(NVector2(currentCell.x, currentCell.y), NVector2(cell.x, cell.y));
    moving = true;
}