#include "adventurer.h"

#include <algorithm>

#include "area.h"
#include "card.h"
#include "artifact.h"

MAdventurer::MAdventurer():MObject() {
  cardsLimit = 5;
}
MAdventurer::MAdventurer(const std::string& _name):MObject(_name) {
  cardsLimit = 5;
}
MAdventurer::MAdventurer(const std::string& _name, MArea* _area):MObject(_name) {
  cardsLimit = 5;
  area = _area;
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
  //if cards more than 5 need draw card
}
void MAdventurer::removeCard(MCard* card) {
  std::vector<MCard*>::iterator it = std::find(cards.begin(), cards.end(), card);
  if(it == cards.end()) return;
  cards.erase(it);
}
void MAdventurer::move(MArea* _area) {
  area = _area;
}
void MAdventurer::handOver(MCard* card, MAdventurer* adventurer) {
  removeCard(card);
  adventurer->addCard(card);
}
