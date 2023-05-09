#include "object.h"

#include <iostream>

MObject::MObject() {
	id = OBJ_OBJ;
}
MObject::~MObject() {
	clearResults();
	clearParams();
}
void MObject::execute(int eventId) {
	std::cout<<"base execute!"<<std::endl;
}
int MObject::getId() {
	return id;
}
int MObject::getExecResultInt(std::string name) {
	return execResults[name]->getValue<int>();
}
void MObject::setExecParamObject(std::string name, MObject* object) {
	std::cout<<"set param: "<<name<<" = "<<object<<std::endl;
	execParams.insert(std::pair<std::string, MVariant*>(name, new MVariant(object)));
}
MObject* MObject::getExecParamObject(std::string name) {
	std::map<std::string, MVariant*>::iterator it = execParams.find(name);
	if(it == execParams.end()) {
		std::cout<<"can't find param: "<<name<<std::endl;
		return nullptr;
	}
	return execParams[name]->getValue<MObject*>();
}
void MObject::clearResults() {
	for(std::map<std::string, MVariant*>::iterator it = execResults.begin(); it != execResults.end(); it++) {
		if(it->second) delete it->second;
	}
	execResults.clear();
}
void MObject::clearParams() {
	for(std::map<std::string, MVariant*>::iterator it = execParams.begin(); it != execParams.end(); it++) {
		if(it->second) delete it->second;
	}
	execParams.clear();
}
stTokenValue MObject::getAttribute(int id) {
    return attributes[id];
}
void MObject::setAttribute(int id, stTokenValue value) {
    attributes[id] = value;
}
