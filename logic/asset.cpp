#include "asset.h"
#include "hero.h"

MAsset::MAsset():MObject() {
	typeId = OBJ_ASSET;
	eventId = AST_CONDITION_ANY;
	used = false;
}
MAsset::MAsset(int _id) {
    MAsset();
    id = _id;
    switch(id) {
    case AST_ADJUSTABLE_WRENCH:
        eventId = EVNT_HERO_ATTACK;
        setConditionResult(AST_CONDITION_ONEHAND, AST_RESULT_PLUS_ONE_STRENGTH);
        setConditionResult(AST_CONDITION_TWOHAND, AST_RESULT_PLUS_THREE_STRENGTH);
        break;
    case AST_TOMMY_GUN:
        eventId = EVNT_HERO_ATTACK;
        setConditionResult(AST_CONDITION_ONEHAND, AST_RESULT_PLUS_FOUR_STRENGTH);
        break;
    case AST_FLESH_DEFENSE:
        eventId = EVNT_HERO_TAKE_DAMAGE;
        setConditionResult(AST_CONDITION_ONEHAND, AST_RESULT_DEF_BY_KNOWLEDGE_CHECK);
    }
}
void MAsset::use() {
}
bool MAsset::canUse(MHero* hero) {
	//todo
}
void MAsset::setUsed(bool _used) {
    used = _used;
}
void MAsset::setConditionResult(int condition, int result) {
    std::map<int, int>::iterator it = conditionResult.find(condition);
	if(it == conditionResult.end()) {
		conditionResult.insert(std::pair<int, int>(condition, result));
	}
	else {
        it->second = result;
	}
}
int MAsset::getConditionResult(int condition) {
    std::map<int, int>::iterator it = conditionResult.find(condition);
	if(it == conditionResult.end()) {
		return -1;
	}
	return it->second;
}
