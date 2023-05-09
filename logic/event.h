#ifndef eventH
#define eventH

#include "object.h"

class MEvent {
private:
    int id;
    int type;
    MObject* object;
    MEvent* parent;
public:
    MEvent(int _type, MObject* _object, MEvent* _parent);
    ~MEvent();
    void setId(int _id);
    int getType();
    void execute();
    MObject* getObject();
};

#endif
