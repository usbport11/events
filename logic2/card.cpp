#include "card.h"

#include <iostream>

MCard::MCard():MObject() {
  momentUse = false;
}
MCard::MCard(const std::string& _name):MObject(_name) {
  momentUse = false;
}
MCard::MCard(const std::string& _name, const std::string& _type):MObject(_name) {
  type = _type;

  std::string momentCards[2] = {"helicopter", "sandbag"};
  for(int i=0; i<2; i++) {
    if(_type != "item") continue;
    if(_name.find(momentCards[i]) == std::string::npos) continue;
	momentUse = true;
	break;
  }
}
std::string MCard::getType() {
  return type;
}
bool MCard::getMomentUse() {
  return momentUse;
}
