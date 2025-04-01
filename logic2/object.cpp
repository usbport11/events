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
std::string MObject::getNameU() {
  if (name.empty()) return "";
  std::string uname = name;
  uname[0] = toupper(uname[0]);
  return uname;
}
