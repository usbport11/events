#ifndef adventurerH
#define adventurerH

#include "object.h"

class MAdventurer: public MObject {
public:
  MAdventurer();
  MAdventurer(const std::string& _name);
};

#endif
