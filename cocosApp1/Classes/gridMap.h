#ifndef gridmapH
#define gridmapH

#include "cocos2d.h"

class MGridMap {
private:
    cocos2d::Size offset;
    cocos2d::Rect gridRect;
    cocos2d::Vec2 cellsCount;
    cocos2d::Size cellSize;
    cocos2d::Size halfSize;
    cocos2d::Vec2 currentCell;
public:
    MGridMap();
    cocos2d::Vec2 getCellUnderMouse(cocos2d::Event* event);
    cocos2d::Vec2 getCoordsByCell(cocos2d::Vec2 cell);
    cocos2d::Vec2 sign(cocos2d::Vec2 vec);
    bool createCells(cocos2d::Scene* scene, int countX, int countY);
    bool cellCheck(cocos2d::Vec2 cell);
    cocos2d::Vec2 getCellsCount();
    cocos2d::Vec2 getNewPoint(int x, int y);
    void setCurrentCell(cocos2d::Vec2 cell);
    cocos2d::Vec2 getCurrentCell();
};

#endif