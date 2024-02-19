#ifndef artifactH
#define artifactH

#include "object.h"

class MArtifact: public MObject {
public:
  std::string  mainAreas[2];
  MArtifact();
  MArtifact(const std::string& _name);
  MArtifact(const std::string& _name, const std::string& mainArea1, const std::string& mainArea2);
  std::string* getMainAreas();
};

#endif
