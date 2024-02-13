#ifndef cardH
#define cardH

#include "object.h"

class MCard: public MObject {
public:
  MCard();
  MCard(const std::string& _name);
};

#endif
