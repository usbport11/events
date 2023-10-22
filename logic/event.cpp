#include "event.h"
#include "object.h"
#include <iostream>

MEvent::MEvent(int _type, MObject* _object) {
	id = -1;
	type = _type;
	object = _object;
}
MEvent::~MEvent() {
	id = -1;
	type = -1;
	object = NULL;
	clearResults();
	clearParams();
}
void MEvent::setId(int _id) {
	id = _id;
}
int MEvent::getType() {
	return type;
}
bool MEvent::execute() {
	return object->execute(this);
}
MObject* MEvent::getObject() {
	return object;
}
void MEvent::setExecResultInt(std::string name, int value) {
    std::map<std::string, MVariant*>::iterator it = execResults.find(name);
	if(it == execResults.end()) {
	    execResults.insert(std::pair<std::string, MVariant*>(name, new MVariant(value)));
	}
	else {
        execResults[name]->setValue(value);
	}
}
int MEvent::getExecResultInt(std::string name) {
	return execResults[name]->getValue<int>();
}
void MEvent::setExecParamObject(std::string name, MObject* object) {
	std::map<std::string, MVariant*>::iterator it = execParams.find(name);
	if(it == execParams.end()) {
	    execParams.insert(std::pair<std::string, MVariant*>(name, new MVariant(object)));
	}
	else {
        execParams[name]->setValue(object);
	}
}
MObject* MEvent::getExecParamObject(std::string name) {
	std::map<std::string, MVariant*>::iterator it = execParams.find(name);
	if(it == execParams.end()) {
		std::cout<<"can't find object param: "<<name<<std::endl;
		return nullptr;
	}
	return execParams[name]->getValue<MObject*>();
}
void MEvent::setExecParamInt(std::string name, int value) {
    std::map<std::string, MVariant*>::iterator it = execParams.find(name);
    if(it == execParams.end()) {
	    execParams.insert(std::pair<std::string, MVariant*>(name, new MVariant(value)));
	}
	else {
        execParams[name]->setValue(value);
	}
}
int MEvent::getExecParamInt(std::string name) {
    std::map<std::string, MVariant*>::iterator it = execParams.find(name);
	if(it == execParams.end()) {
		std::cout<<"can't find int param: "<<name<<std::endl;
		return -1;
	}
	return execParams[name]->getValue<int>();
}
void MEvent::clearResults() {
	for(std::map<std::string, MVariant*>::iterator it = execResults.begin(); it != execResults.end(); it++) {
		if(it->second) delete it->second;
	}
	execResults.clear();
}
void MEvent::clearParams() {
	for(std::map<std::string, MVariant*>::iterator it = execParams.begin(); it != execParams.end(); it++) {
		if(it->second) delete it->second;
	}
	execParams.clear();
}
