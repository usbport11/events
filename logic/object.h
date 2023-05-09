#ifndef objectH
#define objectH

#include "variant.h"

#include <map>

#define OBJ_OBJ 0
#define OBJ_HERO 1
#define OBJ_MONSTER 2
#define OBJ_DICE 3
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

#define MSTR_DOG 1001
#define MSTR_CULTIST 1002
#define MSTR_SHADOW 1003

#define EVNT_DICE_CAST 301
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
    int id;
    std::map<std::string, MVariant*> execParams;
    std::map<std::string, MVariant*> execResults;
    std::map<int, stTokenValue> attributes;
public:
    MObject();
    ~MObject();
    virtual void execute(int eventId);
    int getId();
    int getExecResultInt(std::string name);
    void setExecParamObject(std::string name, MObject* object);
    MObject* getExecParamObject(std::string name);
    void clearResults();
    void clearParams();
    void setAttribute(int id, stTokenValue value);
    stTokenValue getAttribute(int id);
};

#endif
