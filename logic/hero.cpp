#include "hero.h"

#include <algorithm>

MHero::MHero():MObject() {
	id = OBJ_HERO;
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
	skills.insert(std::pair<int, stTokenValue>(SKL_KNOWLEDGE, stTokenValue()));
    skills.insert(std::pair<int, stTokenValue>(SKL_COMMUNICATION, stTokenValue()));
    skills.insert(std::pair<int, stTokenValue>(SKL_ATTENTION, stTokenValue()));
    skills.insert(std::pair<int, stTokenValue>(SKL_STRENGTH, stTokenValue()));
    skills.insert(std::pair<int, stTokenValue>(SKL_WILL, stTokenValue()));
    attributes.insert(std::pair<int, stTokenValue>(ATR_HEALTH, stTokenValue()));
    attributes.insert(std::pair<int, stTokenValue>(ATR_MIND, stTokenValue()));
    attributes.insert(std::pair<int, stTokenValue>(ATR_DOLLAR, stTokenValue()));
    attributes.insert(std::pair<int, stTokenValue>(ATR_SPEED, stTokenValue()));
    attributes.insert(std::pair<int, stTokenValue>(ATR_TROPHEY, stTokenValue()));
    attributes.insert(std::pair<int, stTokenValue>(ATR_EVIDENCE, stTokenValue()));
}
MHero::~MHero() {
	usedActions.clear();
}
void MHero::execute(int eventId) {
	switch(eventId) {
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
	usedActions.push_back(eventId);
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
