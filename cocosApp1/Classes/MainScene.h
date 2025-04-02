#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include "gridMap.h"
#include "deck.h"
#include "hand2.h"
#include "waterLevel.h"
#include "menu.h"
#include "AdventurerMenu.h"
#include "logic/processor.h"
#include <fstream>

class MMainScene: public cocos2d::Scene {
private:
    std::ofstream logStream;

    MProcessor processor;
    std::string currentAction;

    MGridMap gridMap;
    MDeck itemDeck;
    MDeck floodDeck;
    MWaterLevel waterLevel;
    MMenu menu;
    MAdventurerMenu advMenu;
    MAdventurer* advMenuAdventurer;
    
    std::string moveAdventurer;
    int abflussNumber;
    int gridSize;
    std::map<std::string, std::string> floodSprite;
    std::map<std::string, std::string> artifactSprite;
    std::vector<MHand2*> hands2;
    std::map<MAdventurer*, MHand2*> adventurerHand2;
    std::map<std::string, cocos2d::Sprite*> adventurerSprite;

    //cocos2d::Vec2 speed;
    //void moveSprite(cocos2d::Sprite* sprite, cocos2d::Vec2 destination);

    bool initVisual();
    bool updateAreas();
    bool updateActionNumber();
    bool initAdventurers();
    bool initHand();
public:
    MProcessor* getProcessor();
    ~MMainScene();
    static cocos2d::Scene* createScene();
    virtual bool init();
    void update(float delta);

    bool endTurn();
    bool startMove();//start move instead move because it complex
    bool startAbfluss();//start abluss instead abluss because it complex
    bool extract();
    bool getArtifact();
    bool discard(MAdventurer* adventurer, std::list<int> cards);
    bool startHandover();//start handover instead handover because it complex
    bool sumbitHandover(MAdventurer* adventurer, int cardNumber);
    bool startFly();
    bool startSwim();
    bool startMoveOther();

    bool reset();
    void adventurerClicked(MAdventurer* adventurer);
    void adventurerClicked(const std::string& name);
    int getAdventurerNumber(const std::string& name);
    MAdventurer* getCurrentAdventurer();
    MHand2* getAdventurerHand2(MAdventurer* adventurer);
    MAdventurerMenu* getAdventurerMenu();

    void lbmGridProcess(cocos2d::Event* event);
    void rbmGridProcess(cocos2d::Event* event);

    void onMouseMove(cocos2d::Event* event);
    void onMouseDown(cocos2d::Event* event);
    void onMouseUp(cocos2d::Event* event);
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    CREATE_FUNC(MMainScene);
};

#endif
