#include "gridmap.h"
#include "MainScene.h"
#include "logic/area.h"
#include <iostream>

MGridMap::MGridMap() {
    offset = cocos2d::Size(300, 300);
    cellSize = cocos2d::Size(64, 64);
    halfSize = cocos2d::Size(32, 32);
    gridSize = 0;
    spritesNumber = 0;
    currentCell = cocos2d::Vec2(0, 0);
    gridRect = cocos2d::Rect(0, 0, 0, 0);
    areaSprite = {{"fire_cave", "cell0"},
        {"silver_gate", "cell1"},
        {"iron_gate", "cell2"},
        {"bridge_of_the_brave", "cell3"},
        {"twilight_hollow", "cell4"},
        {"howling_garden", "cell5"},
        {"whispering_garden", "cell6"},
        {"adventurers_circle", "cell7"},
        {"golden_gate", "cell8"},
        {"abandoned_cliffs", "cell9"},
        {"lost_lagoon", "cell10"},
        {"cave_of_shadows", "cell11"},
        {"coral_castle", "cell12"},
        {"bronze_gate", "cell13"},
        {"copper_gate", "cell14"},
        {"treacherous_dunes", "cell15"},
        {"watchtower", "cell16"},
        {"temple_of_the_sun", "cell17"},
        {"temple_of_the_moon", "cell18"},
        {"ghost_rock", "cell19"},
        {"foggy_marshes", "cell20"},
        {"scarlet_forest", "cell21"},
        {"observatory", "cell22"},
        {"tidal_castle", "cell23"}};
}

MGridMap::~MGridMap() {
    areaSprite.clear();
    cellSprite.clear();
}

bool MGridMap::create(MMainScene* _pMainScene, const std::string& plistFile, int _gridSize, int _spritesNumber, cocos2d::Size _offset, cocos2d::Size _cellSize) {
    if (!_pMainScene) {
        return false;
    }
    pMainScene = _pMainScene;

    cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
    if (!cache) {
        return false;
    }
    cache->addSpriteFramesWithFile(plistFile);
    
    if (_gridSize <= 0) {
        return false;
    }
    if (_spritesNumber <= 0) {
        return false;
    }
    gridSize = _gridSize;
    spritesNumber = _spritesNumber;

    cellSize = _cellSize;
    halfSize = _cellSize/2;
    offset = _offset;
    gridRect = cocos2d::Rect(offset.width, offset.height, cellSize.width * gridSize, cellSize.height * gridSize);

    std::string key;
    std::string cellName;
    char buffer[16];
    for (int i = 0; i < spritesNumber; i++) {
        memset(buffer, 0, 16);
        snprintf(buffer, 16, "cell%d", i);
        cellName = buffer;
        cocos2d::Sprite* sp = cocos2d::Sprite::createWithSpriteFrame(cache->getSpriteFrameByName(cellName));
        if (!sp) {
            return false;
        }
        sp->getTexture()->setAliasTexParameters();
        sp->setPosition(cocos2d::Vec2(0, 0));
        sp->setVisible(false);
        sp->setScale(1.5);
        sp->setName(cellName);
        pMainScene->addChild(sp);
    }
    return true;
}

bool MGridMap::init() {
    cellSprite.clear();

    std::map<std::string, MObject*> areas = pMainScene->getProcessor()->getAreas();
    int size = areas.size()/2;
    if (size <= 0) return false;
    int pos[2];
    cocos2d::Sprite* sp;
    int cellPos;
    MArea* area;
    for (std::map<std::string, MObject*>::iterator it = areas.begin(); it != areas.end(); it++) {
        area = (MArea*)it->second;
        if (!area) {
            return false;
        }
        pos[0] = area->getIndex()[0];
        pos[1] = area->getIndex()[1];
        if (areaSprite.find(area->getName()) == areaSprite.end()) {
            return false;
        }
        sp = (cocos2d::Sprite*)pMainScene->getChildByName(areaSprite[area->getName()]);
        if (!sp) {
            std::cout<<"can't find sprite with name: "<< areaSprite[area->getName()] <<std::endl;
            return false;
        }
        cellPos = pos[0] * gridSize + pos[1];
        cellSprite[cellPos] = sp;
        sp->setPosition(cocos2d::Vec2(offset.width + pos[0] * cellSize.width + halfSize.width, offset.height + pos[1] * cellSize.height + halfSize.height));
        sp->setVisible(true);
    }

    return true;
}

cocos2d::Vec2 MGridMap::getCellUnderMouse(cocos2d::Event* event) {
    cocos2d::EventMouse* e = (cocos2d::EventMouse*)event;
    return getCellByCoordinates(cocos2d::Vec2(e->getCursorX(), e->getCursorY()));
}

cocos2d::Vec2 MGridMap::getCellByCoordinates(cocos2d::Vec2 coordinates) {
    cocos2d::Vec2 cell = coordinates;
    int pos;
    cocos2d::Sprite* sp;
    if (gridRect.containsPoint(cell)) {
        cell = cocos2d::Vec2((int)((cell.x - offset.width) / cellSize.width), (int)((cell.y - offset.height) / cellSize.height));
        pos = cell.x * gridSize + cell.y;
        if (cellSprite.find(pos) == cellSprite.end()) {
            cell = cocos2d::Vec2(-1, -1);
        }
        else {
            if (!cellSprite.find(pos)->second->isVisible()) {
                cell = cocos2d::Vec2(-1, -1);
            }
        }
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
    return (cell.x >= 0 && cell.y >= 0 && cell.x < gridSize && cell.y < gridSize);
}

int MGridMap::getGridSize() {
    return gridSize;
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

cocos2d::Sprite* MGridMap::getSpriteByCell(int x, int y) {
    int pos = x * gridSize + y;
    if (cellSprite.find(pos) != cellSprite.end()) {
        return cellSprite[pos];
    }
    return nullptr;
}

cocos2d::Sprite* MGridMap::getSpriteByAreaName(const std::string& areaName) {
    if (areaSprite.find(areaName) != areaSprite.end()) {
       return (cocos2d::Sprite*)pMainScene->getChildByName(areaSprite[areaName]);
    }
    return nullptr;
}