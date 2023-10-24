#include "mui.h"

#include "hero.h"
#include "monster.h"

#include <vector>
#include <iostream>

bool MMui::selectEvent() {
	std::vector availableActions = hero->getAvailableActions();
	int sel = 0;
	std::cout<<"Availble hero actions:"<<std::endl;
	for(int i=0; i<availableActions.size(); i++) {
		std::cout<<i<<" - "<<actionNames[availableActions[i]]<<std::endl;
	}
	std::cin>>sel;
	sel += 201;
	availableActions.clear();
	event->setExecResultInt("selected", sel);
	return true;
}
bool MMui::selectMonster() {
	std::vector<MMonster*> monsters = hero->getMonsters();
	int sel = 0;
	std::cout<<"Availble monsters:"<<std::endl;
	for(int i=0; i<monsters.size(); i++) {
		std::cout<<i<<" - "<<monsters[i]->getId()<<std::endl;
	}
	std::cin>>sel;
	sel = 1000 + monsters[sel]->getId();
	monsters.clear();
	event->setExecResultInt("selected", sel);
	return true;
}
MMui::MMui():MObject() {
	typeId = OBJ_MUI;
	actionNames.insert(std::pair<int, std::string>(EVNT_HERO_ATTACK, "attack"));
	actionNames.insert(std::pair<int, std::string>(EVNT_HERO_RESEARCH, "research"));
	actionNames.insert(std::pair<int, std::string>(EVNT_HERO_IMPROVE, "improve"));
	actionNames.insert(std::pair<int, std::string>(EVNT_HERO_MOVE, "move"));
	actionNames.insert(std::pair<int, std::string>(EVNT_HERO_DEFEND, "defend"));
	actionNames.insert(std::pair<int, std::string>(EVNT_HERO_FUNDRAISE, "fundrise"));
	actionNames.insert(std::pair<int, std::string>(EVNT_HERO_TRADE, "trade"));
	actionNames.insert(std::pair<int, std::string>(EVNT_HERO_EVADE, "evade"));
	hero = nullptr;
    event = nullptr;
}
MMui::~MMui() {
	actionNames.clear();
}
bool MMui::execute(MEvent* _event) {
    if(!MObject::execute(_event)) return false;
	hero = (MHero*)event->getExecParamObject("hero");
	if(!hero) {
        std::cout<<"mui: can't find hero"<<std::endl;
        return false;
	}
	switch(event->getType()) {
	case EVNT_MUI_SELECT_ACTION:
		return selectEvent();
	case EVNT_MUI_SELECT_MONSTER:
		return selectMonster();
	}
	std::cout<<"mui: can't find event processing type"<<std::endl;
	return false;
}
