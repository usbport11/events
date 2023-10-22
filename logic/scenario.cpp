#include "scenario.h"

#include "hero.h"
#include "monster.h"

MScenario::MScenario() {
	heroes.push_back(new MHero);
	monsters.push_back(new MMonster);
	monsters.push_back(new MMonster);
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
