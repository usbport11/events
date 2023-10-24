#include "object.h"

#include "event.h"
#include <iostream>

MObject::MObject() {
	typeId = OBJ_OBJ;
	attributes.insert(std::pair<int, stTokenValue>(ATR_HEALTH, stTokenValue()));
    attributes.insert(std::pair<int, stTokenValue>(ATR_MIND, stTokenValue()));
}
MObject::~MObject() {
	attributes.clear();
}
bool MObject::execute(MEvent* _event) {
    event = _event;
    if(!event) {
        std::cout<<"null event"<<std::endl;
        return false;
    }
    return true;
}
int MObject::getId() {
    return id;
}
int MObject::getTypeId() {
	return typeId;
}
stTokenValue MObject::getAttribute(int id) {
    return attributes[id];
}
void MObject::setAttribute(int id, stTokenValue value) {
    attributes[id] = value;
}
void MObject::takeDamage(int health, int mind) {
    attributes[ATR_HEALTH].token += health;
    attributes[ATR_MIND].token += mind;
}
