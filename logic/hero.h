#ifndef heroH
#define heroH

#include "object.h"

#include <vector>

#define ACTIONS_SIZE 8

#define TKN_USUAL 1
#define TKN_SPELLS_NUMBER 2
#define TKN_UNLIMITED 3

class MMonster;
class MAsset;

class MHero: public MObject {
private:
    bool detained;
    int actionsLimit;
    int tokenPerSkillLimit;
    int tokenLimitType;
    int tokensLimit;
    int dollarsMoveLimt;
    int actions[ACTIONS_SIZE];
    std::vector<int> usedActions;
    std::map<int, stTokenValue> skills;
    //data
    std::vector<MMonster*> monsters;
    std::map<int, MAsset*> assets;
    //other
    int usedHands;
    //actions helpers
    bool actionWasUsed(int action);
    //actions
    void attack();
    void fundraise();
    void improve();
    void defend();
    void evade();
    bool research();
    void trade();
    void move();
public:
    MHero();
    ~MHero();
    bool execute(MEvent* _event);
    //actions helpers
    std::vector<int> getAvailableActions();
    void clearUsedActions();
    //get/set
    void setSkill(int id, stTokenValue value);
    stTokenValue getSkill(int id);
    //monsters
    void addMonster(MMonster* monster);
    void removeMonster(MMonster* monster);
    std::vector<MMonster*> getMonsters();
};

#endif
