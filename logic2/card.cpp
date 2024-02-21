#include "card.h"

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
    if(_type == "item" && momentCards[i] == _name.substr(0, momentCards[i].length())) {
	  momentUse = true;
	  continue;
	}
  }
}
std::string MCard::getType() {
  return type;
}
bool MCard::getMomentUse() {
  return momentUse;
}
