#include "artifact.h"

MArtifact::MArtifact():MObject() {
  mainAreas[0] = mainAreas[2] = "";
}
MArtifact::MArtifact(const std::string& _name):MObject(_name) {
  mainAreas[0] = mainAreas[2] = "";
}
MArtifact::MArtifact(const std::string& _name, const std::string& mainArea1, const std::string& mainArea2):MObject(_name) {
  mainAreas[0] = mainArea1;
  mainAreas[1] = mainArea2;
}
std::string* MArtifact::getMainAreas() {
  return (std::string*)mainAreas;
}
