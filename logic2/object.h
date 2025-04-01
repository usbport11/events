#ifndef objectH
#define objectH

#include <string>

class MObject {
private:
  std::string name;
public:
  MObject();
  MObject(const std::string& _name);
  std::string getName();
  std::string getNameU();
};

#endif
