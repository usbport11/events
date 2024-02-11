#include "object.h"

MObject::MObject() {
  name = "";
}
MObject::MObject(const std::string& _name) {
  name = _name;
}
std::string MObject::getName() {
  return name;
}
