#ifndef assetH
#define assetH

#include "object.h"

//adjustable wrench======================
//event: attack
//conditions: AST_CONDITION_ANY
//one hand +1 strength
//two hand +3 strength

class MHero;

class MAsset: MObject {
private:
    int eventId;
    bool used;
    std::map<int, int> conditionResult;
public:
    MAsset();
    MAsset(int _id);
    void use();
    bool canUse(MHero* hero);
    void setUsed(bool _used);
    void setConditionResult(int condition, int result);
    int getConditionResult(int condition);
};

#endif
