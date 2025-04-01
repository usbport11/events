#include "adventurer.h"

#include "area.h"
#include "card.h"
#include "artifact.h"
#include <algorithm>
#include <iostream>

MAdventurer::MAdventurer():MObject() {
  startArea = "";
  description = "";
}
MAdventurer::MAdventurer(const std::string& _name):MObject(_name) {
  startArea = "";
  if (_name == "explorer") description = "can diag move or abfluss";
  if (_name == "pilot") description = "one time can move anywhere";
  if (_name == "engineer") description = "can abluss one or two areas";
  if (_name == "liaison") description = "can handover on any distance";
  if (_name == "navigator") description = "can move other adventurer on one or two areas";
  if (_name == "diver") description = "can move on any number abfluss or none areas";
}
MAdventurer::MAdventurer(const std::string& _name, const std::string& _startArea):MObject(_name) {
  startArea = _startArea;
  if (_name == "explorer") description = "can diag move or abfluss";
  if (_name == "pilot") description = "one time can move anywhere";
  if (_name == "engineer") description = "can abluss one or two areas";
  if (_name == "liaison") description = "can handover on any distance";
  if (_name == "navigator") description = "can move other adventurer on one or two areas";
  if (_name == "diver") description = "can move on any number abfluss or none areas";
}
MAdventurer::~MAdventurer() {
  artifacts.clear();
  cards.clear();
}
void MAdventurer::addArtifact(MArtifact* artifact) {
  if(std::find(artifacts.begin(), artifacts.end(), artifact) != artifacts.end()) return;
  artifacts.push_back(artifact);
}
void MAdventurer::addCard(MCard* card) {
  if (std::find(cards.begin(), cards.end(), card) != cards.end()) {
    std::cout << "Can't add card. It alredy added!" << std::endl;
    return;
  }
  cards.push_back(card);
}
void MAdventurer::removeCard(MCard* card) {
  if(cards.empty()) return;
  std::vector<MCard*>::iterator it = std::find(cards.begin(), cards.end(), card);
  if(it == cards.end()) return;
  cards.erase(it);
}
void MAdventurer::removeAllCards() {
  cards.clear();
}
void MAdventurer::removeAllArtifacts() {
  artifacts.clear();
}
void MAdventurer::handOver(MCard* card, MAdventurer* adventurer) {
  removeCard(card);
  adventurer->addCard(card);
}
int MAdventurer::getCardsNumber() {
  return cards.size();
}
std::string MAdventurer::getStartArea() {
  return startArea;
}
void MAdventurer::setStartArea(std::string _startArea){
  startArea = _startArea;
}
void MAdventurer::setArea(MArea* _area) {
  area = _area;
}
MArea* MAdventurer::getArea() {
  return area;
}
std::vector<MCard*> MAdventurer::getAllCards() {
  return cards;
}
MCard* MAdventurer::getCardByNumber(int number) {
  if (number < 0 || number >= cards.size()) nullptr;
  else return cards[number];
}
std::vector<MCard*> MAdventurer::getMomentCards() {
  std::vector<MCard*> resCards;
  for(int i=0; i<cards.size(); i++) {
    if(cards[i]->getMomentUse()) {
      resCards.push_back(cards[i]);
	}
  }
  return resCards;
}
std::vector<MCard*> MAdventurer::getArtifactCards(const std::string& name) {
  std::vector<MCard*> resCards;
  for(int i=0; i < cards.size() && i < 4; i++) {
    if(cards[i]->getType() == "artifact") {
      if(name != "") {
        if(name == cards[i]->getName().substr(0, name.length())) {
          resCards.push_back(cards[i]);
        }
      }
      else {
        resCards.push_back(cards[i]);
      }
    }
  }
  return resCards;
}
bool MAdventurer::canUseDiagonal() {
  return (getName() == "explorer");
}
bool MAdventurer::hasCard(const std::string& cardName) {
  for (int i = 0; i < cards.size(); i++) {
    if (cards[i]->getName() == cardName) return true;
  }
  return false;
}
std::string MAdventurer::getDescription() {
  return description;
}