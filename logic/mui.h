#ifndef muiH
#define muiH

#include "object.h"

#include <map>

class MMui: public MObject {
private:
    std::map<int, std::string> actionNames;
    void selectEvent();
    void selectMonster();
public:
    MMui();
    ~MMui();
    void execute(int eventId);
};

#endif
