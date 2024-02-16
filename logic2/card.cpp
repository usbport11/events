#include "card.h"

MCard::MCard():MObject() {
}
MCard::MCard(const std::string& _name):MObject(_name) {
}
MCard::MCard(const std::string& _name, const std::string& _type):MObject(_name) {
  type = _type;
}
std::string MCard::getType() {
  return type;
}
