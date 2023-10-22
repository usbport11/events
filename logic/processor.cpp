#include "processor.h"

#include "dice.h"
#include "mui.h"
#include "hero.h"
#include "monster.h"
#include "event.h"

MEvent* MProcessor::addEvent(int eventType) {
	MObject* object;
	switch(eventObjectType[eventType]) {
	case OBJ_HERO:
		object = hero;
		break;
	case OBJ_MUI:
		object = mui;
		break;
	case OBJ_DICE:
		object = dice;
		break;
	}
	std::string evtName = getEventName(eventType);
	if(evtName == "") {
		std::cout<<"addEvent error: can't determine event: "<<eventType<<std::endl;
		return nullptr;
	}
	std::string objName = getObjectName(object->getTypeId());
	if(objName == "") {
		std::cout<<"addEvent error: can't determine object: "<<object->getTypeId()<<std::endl;
		return nullptr;
	}
	std::cout<<"Event add. Event type: "<<evtName<<". Object: "<<objName<<std::endl;
	MEvent* nextEvent = new MEvent(eventType, object);
	nextEvent->setId(eventNumber ++);
    events.push_front(nextEvent);
	switch(eventType) {
	case EVNT_HERO_ATTACK:
	    addEvent(EVNT_HERO_CHECK_STRENGTH);
		addEvent(EVNT_MUI_SELECT_MONSTER);
		break;
	case EVNT_HERO_CHECK_STRENGTH:
		addEvent(EVNT_DICE_CAST);
		break;
	case EVNT_MUI_SELECT_ACTION:
		nextEvent->setExecParamObject("hero", hero);
		break;
    case EVNT_MUI_SELECT_MONSTER:
		nextEvent->setExecParamObject("hero", hero);
		break;
	}

	return nextEvent;
}
void MProcessor::resoveNextEvent() {
    int result = -1;

	event = events.front();
	std::string evtName = getEventName(event->getType());
	if(evtName == "") {
		std::cout<<"resoveNextEvent error: can't determine event"<<std::endl;
		return;
	}

	std::cout<<"start resolve event: "<<evtName<<std::endl;
	if(!event->execute()) {
        std::cout<<"terminate process"<<std::endl;
        terminate = true;
        return;
	}

	std::cout<<"add to history: "<<evtName<<std::endl;
	history.push_back(event);
	events.pop_front();

	std::cout<<"post processing"<<std::endl;
	switch(event->getType()) {
	case EVNT_MUI_SELECT_ACTION:
		result = event->getExecResultInt("selected");
		std::cout<<"mui action selected: "<<result<<std::endl;
		addEvent(result);
		break;
	case EVNT_MUI_SELECT_MONSTER:
		result = event->getExecResultInt("selected");
		std::cout<<"mui monster selected: "<<result<<std::endl;
		break;
	}

	std::cout<<"end resolve event: "<<evtName<<std::endl;
    event->clearParams();
    event->clearResults();
}
std::string MProcessor::getEventName(int id) {
	std::map<int, std::string>::iterator it = eventName.find(id);
	if(it == eventName.end()) {
		return "";
	}
	return eventName[id];
}
std::string MProcessor::getObjectName(int id) {
	std::map<int, std::string>::iterator it = objectName.find(id);
	if(it == objectName.end()) {
		return "";
	}
	return objectName[id];
}
MProcessor::MProcessor() {
    terminate = false;
	eventNumber = 0;
	dice = new MDice;
	mui = new MMui;
	monster = new MMonster(MSTR_DOG);
	monster->setAttribute(ATR_HEALTH, stTokenValue(1, 0));
	hero = new MHero;
	hero->addMonster(monster);
	hero->setAttribute(ATR_HEALTH, stTokenValue(6, 0));
	hero->setAttribute(ATR_MIND, stTokenValue(6, 0));
	eventObjectType.insert(std::pair<int, int>(EVNT_HERO_ATTACK, OBJ_HERO));
	eventObjectType.insert(std::pair<int, int>(EVNT_DICE_CAST, OBJ_DICE));
	eventObjectType.insert(std::pair<int, int>(EVNT_HERO_CHECK_STRENGTH, OBJ_HERO));
	eventObjectType.insert(std::pair<int, int>(EVNT_MUI_SELECT_ACTION, OBJ_MUI));
	eventObjectType.insert(std::pair<int, int>(EVNT_MUI_SELECT_MONSTER, OBJ_MUI));
	eventName.insert(std::pair<int, std::string>(EVNT_HERO_ATTACK, "hero attack"));
	eventName.insert(std::pair<int, std::string>(EVNT_DICE_CAST, "dice cast"));
	eventName.insert(std::pair<int, std::string>(EVNT_HERO_CHECK_STRENGTH, "hero check strength"));
	eventName.insert(std::pair<int, std::string>(EVNT_MUI_SELECT_ACTION, "mui select action"));
	eventName.insert(std::pair<int, std::string>(EVNT_MUI_SELECT_MONSTER, "mui select monster"));
	objectName.insert(std::pair<int, std::string>(OBJ_HERO, "hero"));
	objectName.insert(std::pair<int, std::string>(OBJ_MONSTER, "monster"));
	objectName.insert(std::pair<int, std::string>(OBJ_DICE, "dice"));
	objectName.insert(std::pair<int, std::string>(OBJ_ASSET, "asset"));
	objectName.insert(std::pair<int, std::string>(OBJ_MUI, "mui"));
}
MProcessor::~MProcessor() {
	for(std::vector<MEvent*>::iterator it = history.begin(); it != history.end(); it ++) {
		delete *it;
	}
	for(std::deque<MEvent*>::iterator it = events.begin(); it != events.end(); it ++) {
		delete *it;
	}
	history.clear();
	events.clear();
	eventObjectType.clear();
	eventName.clear();
	objectName.clear();
	if(dice) delete dice;
	if(mui) delete mui;
	if(hero) delete hero;
	if(monster) delete monster;
}
void MProcessor::start() {
	addEvent(EVNT_MUI_SELECT_ACTION);
}
void MProcessor::resolveEvents() {
	while(!events.empty() && !terminate) {
		resoveNextEvent();
	}
}

