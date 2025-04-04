#ifndef cardH
#define cardH

#include "object.h"

class MCard: public MObject {
private:
  std::string type;//item, artifact, flood
public:
  MCard();
  MCard(const std::string& _name);
  MCard(const std::string& _name, const std::string& _type);
  std::string getType();
};

#endif
