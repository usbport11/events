#include "monster.h"

void MMonster::takeDamage(int damage) {
	health -= damage;
}
MMonster::MMonster(): MObject() {
	id = OBJ_MONSTER;
	health = 3;
}
void MMonster::execute(int eventId) {
}
