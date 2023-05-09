#include "mui.h"

#include "hero.h"

#include <vector>
#include <iostream>

void MMui::selectEvent() {
	MHero* hero = (MHero*)getExecParamObject("hero");
	std::vector availableActions = hero->getAvailableActions();
	std::cout<<"Availble hero actions:"<<std::endl;
	int sel = -1;
	for(int i=0; i<availableActions.size(); i++) {
		std::cout<<i<<" - "<<actionNames[availableActions[i]]<<std::endl;
	}
	std::cin>>sel;
	sel += 201;
	availableActions.clear();
	execResults.insert(std::pair<std::string, MVariant*>("selected", new MVariant(sel)));
}
void MMui::selectMonster() {
	//here must be menu select
	execResults.insert(std::pair<std::string, MVariant*>("selected", new MVariant(int(MSTR_DOG))));
}
MMui::MMui():MObject() {
	id = OBJ_MUI;
	actionNames.insert(std::pair<int, std::string>(EVNT_HERO_ATTACK, "attack"));
	actionNames.insert(std::pair<int, std::string>(EVNT_HERO_RESEARCH, "research"));
	actionNames.insert(std::pair<int, std::string>(EVNT_HERO_IMPROVE, "improve"));
	actionNames.insert(std::pair<int, std::string>(EVNT_HERO_MOVE, "move"));
	actionNames.insert(std::pair<int, std::string>(EVNT_HERO_DEFEND, "defend"));
	actionNames.insert(std::pair<int, std::string>(EVNT_HERO_FUNDRAISE, "fundrise"));
	actionNames.insert(std::pair<int, std::string>(EVNT_HERO_TRADE, "trade"));
	actionNames.insert(std::pair<int, std::string>(EVNT_HERO_EVADE, "evade"));
}
MMui::~MMui() {
	actionNames.clear();
}
void MMui::execute(int eventId) {
	std::cout<<"mui execute: "<<eventId<<std::endl;
	switch(eventId) {
	case EVNT_MUI_SELECT_ACTION:
		selectEvent();
		break;
	case EVNT_MUI_SELECT_MONSTER:
		selectMonster();
		break;
	}
}
