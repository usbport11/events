#include "scenario.h"

#include "hero.h"
#include "monster.h"

MScenario::MScenario() {
	MHero* hero = new MHero;
	MMonster* monster = new MMonster;
	heroes.push_back(hero);
	monsters.push_back(monster);
}
MScenario::~MScenario() {
	for(std::vector<MHero*>::iterator it = heroes.begin(); it != heroes.end(); it ++) {
		delete *it;
	}
	for(std::vector<MMonster*>::iterator it = monsters.begin(); it != monsters.end(); it ++) {
		delete *it;
	}
	heroes.clear();
	monsters.clear();
}