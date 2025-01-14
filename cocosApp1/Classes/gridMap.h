#ifndef gridmapH
#define gridmapH

#include "cocos2d.h"

class MMainScene;

class MGridMap {
private:
    float scale;
    MMainScene* pMainScene;
    cocos2d::Size offset;
    cocos2d::Rect gridRect;
    int gridSize;
    int spritesNumber;
    cocos2d::Size cellSize;
    cocos2d::Size halfSize;
    cocos2d::Vec2 currentCell;
    std::map<std::string, std::string> areaSprite;
    std::map<int, cocos2d::Sprite*> cellSprite;
    std::vector<int> areaLimit;
public:
    MGridMap();
    ~MGridMap();
    cocos2d::Vec2 getCellUnderMouse(cocos2d::Event* event);
    cocos2d::Vec2 getCellByCoordinates(cocos2d::Vec2 coordinates);
    cocos2d::Vec2 getCoordsByCell(cocos2d::Vec2 cell);
    cocos2d::Vec2 sign(cocos2d::Vec2 vec);
    bool create(MMainScene* _pMainScene, const std::string& plistFile, int gridSize, int spritesNumber, cocos2d::Size _offset, cocos2d::Size _cellSize);
    bool init();
    bool cellCheck(cocos2d::Vec2 cell);
    int getGridSize();
    cocos2d::Vec2 getNewPoint(int x, int y);
    void setCurrentCell(cocos2d::Vec2 cell);
    cocos2d::Vec2 getCurrentCell();
    cocos2d::Sprite* getSpriteByCell(int x, int y);
    cocos2d::Sprite* getSpriteByAreaName(const std::string& areaName);
    void addAreaLimit(int pos);
    void clearAreaLimit();
};

#endif