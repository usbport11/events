#include "hero.h"

#include <iostream>
#include <algorithm>
#include "monster.h"
#include "asset.h"

MHero::MHero():MObject() {
	typeId = OBJ_HERO;
	actionsLimit = 2;
	actions[0] = EVNT_HERO_ATTACK;
	actions[1] = EVNT_HERO_RESEARCH;
	actions[2] = EVNT_HERO_IMPROVE;
	actions[3] = EVNT_HERO_MOVE;
	actions[4] = EVNT_HERO_DEFEND;
	actions[5] = EVNT_HERO_FUNDRAISE;
	actions[6] = EVNT_HERO_TRADE;
	actions[7] = EVNT_HERO_EVADE;
	tokenPerSkillLimit = 1;
    tokenLimitType = TKN_USUAL;
    tokensLimit = 2;
    usedHands = 0;
	skills.insert(std::pair<int, stTokenValue>(SKL_KNOWLEDGE, stTokenValue(3, 0)));
    skills.insert(std::pair<int, stTokenValue>(SKL_COMMUNICATION, stTokenValue(3, 0)));
    skills.insert(std::pair<int, stTokenValue>(SKL_ATTENTION, stTokenValue(3, 0)));
    skills.insert(std::pair<int, stTokenValue>(SKL_STRENGTH, stTokenValue(3, 0)));
    skills.insert(std::pair<int, stTokenValue>(SKL_WILL, stTokenValue(1, 0)));
    attributes.insert(std::pair<int, stTokenValue>(ATR_DOLLAR, stTokenValue()));
    attributes.insert(std::pair<int, stTokenValue>(ATR_SPEED, stTokenValue()));
    attributes.insert(std::pair<int, stTokenValue>(ATR_TROPHEY, stTokenValue()));
    attributes.insert(std::pair<int, stTokenValue>(ATR_EVIDENCE, stTokenValue()));
    assets.insert(std::pair<int, MAsset*>(AST_ADJUSTABLE_WRENCH, new MAsset(AST_ADJUSTABLE_WRENCH)));
    assets.insert(std::pair<int, MAsset*>(AST_TOMMY_GUN, new MAsset(AST_TOMMY_GUN)));
}
MHero::~MHero() {
	usedActions.clear();
	skills.clear();
	monsters.clear();
	for(std::map<int, MAsset*>::iterator it = assets.begin(); it != assets.end(); it++) {
		if(it->second) delete it->second;
	}
	assets.clear();
}
bool MHero::execute(MEvent* _event) {
    if(!MObject::execute(_event)) return false;
	switch(_event->getType()) {
	case EVNT_HERO_ATTACK:
		break;
    case EVNT_HERO_RESEARCH:
        break;
    case EVNT_HERO_IMPROVE:
        break;
    case EVNT_HERO_MOVE:
        break;
    case EVNT_HERO_DEFEND:
        break;
    case EVNT_HERO_FUNDRAISE:
        break;
    case EVNT_HERO_TRADE:
        break;
    case EVNT_HERO_EVADE:
        break;
	}
	usedActions.push_back(event->getType());
	return true;
}
bool MHero::actionWasUsed(int action) {
	return (std::find(usedActions.begin(), usedActions.end(), action) != usedActions.end());
}
std::vector<int> MHero::getAvailableActions() {
	std::vector<int> result;
	for(int i=0; i<ACTIONS_SIZE; i++) {
		if(!actionWasUsed(actions[i])) {
			result.push_back(actions[i]);
		}
	}
	return result;
}
void MHero::clearUsedActions() {
    usedActions.clear();
}
void MHero::setSkill(int id, stTokenValue value) {
    skills[id] = value;
}
stTokenValue MHero::getSkill(int id) {
    return skills[id];
}
void MHero::addMonster(MMonster* monster) {
    monsters.push_back(monster);
}
void MHero::removeMonster(MMonster* monster) {
    monsters.erase(std::remove(monsters.begin(), monsters.end(), monster), monsters.end());
}
std::vector<MMonster*> MHero::getMonsters() {
    return monsters;
}
