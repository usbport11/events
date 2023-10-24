#ifndef objectH
#define objectH

#include "event.h"
#include <map>

#define PRSR_EVNT_BAD 0
#define PRSR_EVNT_GOOD 1

#define OBJ_OBJ 0
#define OBJ_HERO 1
#define OBJ_MONSTER 2
#define OBJ_DICESET 3
#define OBJ_ASSET 4
#define OBJ_MUI 5

#define EVNT_HERO_ATTACK 201
#define EVNT_HERO_RESEARCH 202
#define EVNT_HERO_IMPROVE 203
#define EVNT_HERO_MOVE 204
#define EVNT_HERO_DEFEND 205
#define EVNT_HERO_FUNDRAISE 206
#define EVNT_HERO_TRADE 207
#define EVNT_HERO_EVADE 208
#define EVNT_HERO_CHECK_STRENGTH 209
#define EVNT_HERO_TAKE_DAMAGE 210

#define HERO_AGNES_BAKER 2001
#define HERO_VENDY_ADAMS 2002

#define MSTR_DOG 1001
#define MSTR_CULTIST 1002
#define MSTR_SHADOW 1003

#define EVNT_DICESET_ROLL 301

#define EVNT_ASSET_USE 401
#define EVNT_MUI_SELECT_ACTION 501
#define EVNT_MUI_SELECT_MONSTER 502
#define EVNT_MUI_SELECT_REGION 503

#define SKL_KNOWLEDGE 1
#define SKL_COMMUNICATION 2
#define SKL_ATTENTION 3
#define SKL_STRENGTH 4
#define SKL_WILL 5

#define ATR_HEALTH 1
#define ATR_MIND 2
#define ATR_DOLLAR 3
#define ATR_SPEED 4
#define ATR_TROPHEY 5
#define ATR_EVIDENCE 6

#define AST_ADJUSTABLE_WRENCH 3001
#define AST_TOMMY_GUN 3002
#define AST_FLESH_DEFENSE 3003

#define AST_CONDITION_ANY 1
#define AST_CONDITION_ONEHAND 2
#define AST_CONDITION_TWOHAND 3

#define AST_RESULT_PLUS_ONE_STRENGTH 1
#define AST_RESULT_PLUS_THREE_STRENGTH 2
#define AST_RESULT_PLUS_FOUR_STRENGTH 3
#define AST_RESULT_DEF_BY_KNOWLEDGE_CHECK 4

struct stTokenValue {
	int base;
    int token;
    stTokenValue() {
        base = token = -1;
    }
    stTokenValue(int _base, int _token) {
        base = _base;
        token = _token;
    }
    int sum() {
        return base + token;
    }
    int diff() {
        return base - token;
    }
    stTokenValue null() {
        base = token = 0;
        return *this;
    }
    stTokenValue addToken(int size) {
		token += size;
		return *this;
	}
	stTokenValue diffToken(int size) {
		token -= size;
		return *this;
	}
    stTokenValue operator+(stTokenValue const& val) {
        return stTokenValue(base + val.base, token + val.token);
    }
    stTokenValue operator-(stTokenValue const& val) {
        return stTokenValue(base - val.base, token - val.token);
    }
};

class MObject {
protected:
    MEvent* event;
    int id;//id of object
    int typeId; //id of object type
    std::map<int, stTokenValue> attributes;
public:
    MObject();
    ~MObject();
    virtual bool execute(MEvent* _event);
    int getId();
    int getTypeId();
    void setAttribute(int id, stTokenValue value);
    stTokenValue getAttribute(int id);
    void takeDamage(int health, int mind);
};

#endif
