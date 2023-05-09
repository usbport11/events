#include "dice.h"

void MDice::cast() {
}

MDice::MDice():MObject() {
	id = OBJ_DICE;
}
void MDice::execute(int eventId) {
	switch(eventId) {
	case EVNT_DICE_CAST:
		cast();
		break;
	}
}
