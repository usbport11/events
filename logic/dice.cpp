#include "dice.h"

void MDice::cast() {
}

MDice::MDice():MObject() {
	typeId = OBJ_DICE;
}
bool MDice::execute(MEvent* _event) {
	switch(_event->getType()) {
	case EVNT_DICE_CAST:
		cast();
		break;
	}
	return true;
}
