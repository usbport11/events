#ifndef eventH
#define eventH

#include "variant.h"
#include <map>

class MObject;

class MEvent {
private:
    int id;
    int type;
    MObject* object;
    std::map<std::string, MVariant*> execParams;
    std::map<std::string, MVariant*> execResults;
public:
    MEvent(int _type, MObject* _object);
    ~MEvent();
    void setId(int _id);
    int getType();
    bool execute();
    MObject* getObject();
    void setExecResultInt(std::string name, int value);
    int getExecResultInt(std::string name);
    void setExecParamObject(std::string name, MObject* object);
    MObject* getExecParamObject(std::string name);
    void setExecParamInt(std::string name, int value);
    int getExecParamInt(std::string name);
    void clearResults();
    void clearParams();
};

#endif
