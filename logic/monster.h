#ifndef monsterH
#define monsterH

#include "object.h"

class MMonster: public MObject {
private:
    int health;
    void takeDamage(int damage);
public:
    MMonster();
    MMonster(int _id);
    bool execute(MEvent* _event);
};

#endif
