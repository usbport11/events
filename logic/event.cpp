#include "event.h"

MEvent::MEvent(int _type, MObject* _object, MEvent* _parent) {
	id = -1;
	type = _type;
	object = _object;
	parent = _parent;
}
MEvent::~MEvent() {
	id = -1;
	type = -1;
	object = NULL;
	parent = NULL;
}
void MEvent::setId(int _id) {
	id = _id;
}
int MEvent::getType() {
	return type;
}
void MEvent::execute() {
	object->execute(type);
}
MObject* MEvent::getObject() {
	return object;
}