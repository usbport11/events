#include "gridmap.h"

MGridMap::MGridMap() {
    offset = cocos2d::Size(300, 300);
    cellSize = cocos2d::Size(64, 64);
    halfSize = cocos2d::Size(32, 32);
    cellsCount = cocos2d::Vec2(0, 0);
    currentCell = cocos2d::Vec2(0, 0);
    gridRect = cocos2d::Rect(0, 0, 0, 0);
}

bool MGridMap::createCells(cocos2d::Scene* scene, int countX, int countY) {
    if (countX < 0 || countY < 0) {
        return false;
    }
    cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
    if (!cache) {
        return false;
    }
    cache->addSpriteFramesWithFile("anim/cell.plist");

    cellsCount = cocos2d::Vec2(countX, countY);
    gridRect = cocos2d::Rect(offset.width, offset.height, cellSize.width * cellsCount.x, cellSize.height * cellsCount.y);

    std::string key;
    std::string cellName;
    char buffer[16];
    for (int i = 0; i < countX; i++) {
        for (int j = 0; j < countY; j++) {
            memset(buffer, 0, 16);
            snprintf(buffer, 16, "cell_%d_%d", i, j);
            key = buffer;
            memset(buffer, 0, 16);
            snprintf(buffer, 16, "cell%d", i* countX  + j);
            cellName = buffer;
            cocos2d::Sprite* sp = cocos2d::Sprite::createWithSpriteFrame(cache->getSpriteFrameByName(cellName));
            if (!sp) {
                return false;
            }
            sp->setScale(1.0);
            sp->getTexture()->setAliasTexParameters();
            sp->setPosition(cocos2d::Vec2(offset.width + i * cellSize.width + halfSize.width, offset.height + j * cellSize.height + halfSize.height));
            scene->addChild(sp, 0, key);
        }
    }
    return true;
}

cocos2d::Vec2 MGridMap::getCellUnderMouse(cocos2d::Event* event) {
    cocos2d::EventMouse* e = (cocos2d::EventMouse*)event;
    cocos2d::Vec2 cell = cocos2d::Vec2(e->getCursorX(), e->getCursorY());
    if (gridRect.containsPoint(cell)) {
        cell = cocos2d::Vec2((int)((cell.x - offset.width) / cellSize.width), (int)((cell.y - offset.height) / cellSize.height));
    }
    else {
        cell = cocos2d::Vec2(-1, -1);
    }
    return cell;
}

cocos2d::Vec2 MGridMap::getCoordsByCell(cocos2d::Vec2 cell) {
    return cocos2d::Vec2(offset.width + cell.x * cellSize.width + halfSize.width, offset.height + cell.y * cellSize.height + halfSize.height);
}

cocos2d::Vec2 MGridMap::sign(cocos2d::Vec2 vec) {
    return cocos2d::Vec2((0 < vec.x) - (vec.x < 0), (0 < vec.y) - (vec.y < 0));
}

bool MGridMap::cellCheck(cocos2d::Vec2 cell) {
    return (cell.x >= 0 && cell.y >= 0 && cell.x < cellsCount.x && cell.y < cellsCount.y);
}

cocos2d::Vec2 MGridMap::getCellsCount() {
    return cellsCount;
}

cocos2d::Vec2 MGridMap::getNewPoint(int x, int y) {
    return cocos2d::Vec2(offset.width + x * cellSize.width + halfSize.width, offset.height + y * cellSize.height + halfSize.height);
}

void MGridMap::setCurrentCell(cocos2d::Vec2 cell) {
    currentCell = cell;
}

cocos2d::Vec2 MGridMap::getCurrentCell() {
    return currentCell;
}