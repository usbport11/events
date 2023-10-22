#include "monster.h"

void MMonster::takeDamage(int damage) {
	health -= damage;
}
MMonster::MMonster(): MObject() {
	typeId = OBJ_MONSTER;
	health = 3;
}
MMonster::MMonster(int _id) {
    MMonster();
    id = _id;
}
bool MMonster::execute(MEvent* _event) {
    return true;
}
