#include "adventurer.h"

#include "area.h"
#include "card.h"
#include "artifact.h"
#include <algorithm>

MAdventurer::MAdventurer():MObject() {
  startArea = "";
}
MAdventurer::MAdventurer(const std::string& _name):MObject(_name) {
  startArea = "";
}
MAdventurer::MAdventurer(const std::string& _name, const std::string& _startArea):MObject(_name) {
  startArea = _startArea;
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
  if(std::find(cards.begin(), cards.end(), card) != cards.end()) return;
  cards.push_back(card);
}
void MAdventurer::removeCard(MCard* card) {
  if(cards.empty()) return;
  std::vector<MCard*>::iterator it = std::find(cards.begin(), cards.end(), card);
  if(it == cards.end()) return;
  cards.erase(it);
}
void MAdventurer::removeArtifactCards(std::string name) {
  int i = 0;
  for(std::vector<MCard*>::iterator it=cards.begin(); it != cards.end() && i < 4; it++) {
    if((*it)->getType() == "artifact") {
      if(name == (*it)->getName().substr(0, name.length())) {
        cards.erase(it);
        i ++;
      }
    }
  }
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
