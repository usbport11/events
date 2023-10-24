#ifndef muiH
#define muiH

#include "object.h"
#include <map>

class MHero;
class MEvent;

class MMui: public MObject {
private:
    MHero* hero;
    std::map<int, std::string> actionNames;
    bool selectEvent();
    bool selectMonster();
public:
    MMui();
    ~MMui();
    bool execute(MEvent* _event);
};

#endif
