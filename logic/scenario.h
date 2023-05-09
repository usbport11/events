#ifndef scenarioH
#define scenarioH

#include <vector>

class MHero;
class MMonster;

class MScenario {
private:
    std::vector<MHero*> heroes;
    std::vector<MMonster*> monsters;
public:
    MScenario();
    ~MScenario();
};

#endif