#include "asset.h"

void MAsset::use() {
}
MAsset::MAsset():MObject() {
	id = OBJ_ASSET;
	eventId = EVNT_HERO_ATTACK;
}
bool MAsset::canUse(int _eventId) {
	return eventId == _eventId;
}
void MAsset::execute(int eventId) {
}
