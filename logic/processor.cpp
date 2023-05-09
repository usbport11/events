#include "processor.h"

#include "dice.h"
#include "mui.h"
#include "hero.h"
#include "event.h"

void MProcessor::addEvent(int eventType, MEvent* parent) {
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
		return;
	}
	std::string objName = getObjectName(object->getId());
	if(objName == "") {
		std::cout<<"addEvent error: can't determine object: "<<object->getId()<<std::endl;
		return;
	}
	std::cout<<"Event add. Event type: "<<evtName<<". Object: "<<objName<<std::endl;
	MEvent* nextEvent = new MEvent(eventType, object, parent);
	nextEvent->setId(eventNumber ++);
	events.push_front(nextEvent);
	switch(eventType) {
	case EVNT_HERO_ATTACK:
		addEvent(EVNT_MUI_SELECT_MONSTER, events.back());
		resoveNextEvent();
		addEvent(EVNT_HERO_CHECK_STRENGTH, events.back());
		break;
	case EVNT_HERO_CHECK_STRENGTH:
		addEvent(EVNT_DICE_CAST, events.back());
		break;
	case EVNT_MUI_SELECT_ACTION:
		object->setExecParamObject("hero", hero);
		break;
	}
}
void MProcessor::resoveNextEvent() {
	event = events.front();
	std::string evtName = getEventName(event->getType());
	if(evtName == "") {
		std::cout<<"resoveNextEvent error: can't determine event"<<std::endl;
		return;
	}
	std::cout<<"trying resolve: "<<evtName<<std::endl;
	event->execute();
	//save execute event in history and remove from stack
	history.push_back(event);
	events.pop_front();
	//post execute actions
	switch(event->getType()) {
	case EVNT_MUI_SELECT_ACTION: {
		int result = event->getObject()->getExecResultInt("selected");
		std::cout<<"mui action selected: "<<result<<std::endl;
		addEvent(result, NULL);
		break;
	}
	}
	event->getObject()->clearParams();
	event->getObject()->clearResults();
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
		std::cout<<"Alrama!"<<std::endl;
		return "";
	}
	return objectName[id];
}
MProcessor::MProcessor() {
	eventNumber = 0;
	dice = new MDice;
	hero = new MHero;
	mui = new MMui;
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
}
void MProcessor::start() {
	addEvent(EVNT_MUI_SELECT_ACTION, NULL);
}
void MProcessor::resolveEvents() {
	while(!events.empty()) {
		resoveNextEvent();
	}
}

