#ifndef diceH
#define diceH

#include "object.h"

#include <vector>

class MDice: public MObject {
private:
    std::vector<int> res;
    void cast();
public:
    MDice();
    void execute(int eventId);
};

#endif
