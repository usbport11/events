#ifndef assetH
#define assetH

#include "object.h"

class MAsset: MObject {
private:
    int id;
    int eventId;
    void use();
public:
    MAsset();
    bool canUse(int _eventId);
    void execute(int eventId);
};

#endif
