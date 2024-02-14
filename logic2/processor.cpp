#include "processor.h"

#include <iostream>
#include "area.h"
#include "adventurer.h"
#include "card.h"

bool MProcessor::argsLessLimit(int num) {
  return (vargs.size() < num);
}
MArea* MProcessor::findArea(const std::string& name) {
  moi moit = areas.find(name);
  if(moit == areas.end()) {
    std::cout<<"Area ["<<name<<"] not found!"<<std::endl;
  }
  return (MArea*)areas[name];
}
MAdventurer* MProcessor::findAdventurer(const std::string& name) {
  moi moit = adventurers.find(name);
  if(moit == adventurers.end()) {
    std::cout<<"Adventurer ["<<name<<"] not found!"<<std::endl;
  }
  return (MAdventurer*)adventurers[name];
}
MCard* MProcessor::findItemCard(const std::string& name) {
  moi moit = itemCards.find(name);
  if(moit == itemCards.end()) {
    std::cout<<"Item Card ["<<name<<"] not found!"<<std::endl;
  }
  return (MCard*)itemCards[name];
}
MCard* MProcessor::findFloodCard(const std::string& name) {
  moi moit = floodCards.find(name);
  if(moit == floodCards.end()) {
    std::cout<<"Flood Card ["<<name<<"] not found!"<<std::endl;
  }
  return (MCard*)floodCards[name];
}
void MProcessor::start() {
  //random areas
  //random adventurers
  //random item queue
  //random flood queue
}
void MProcessor::move() {
  if(argsLessLimit(2)) return;
  std::cout<<"Move: "<<vargs[0]<<" to the "<<vargs[1]<<std::endl;
}
void MProcessor::abfluss() {
  if(argsLessLimit(2)) return;
  std::cout<<"Abfluss: "<<vargs[0]<<" the "<<vargs[1]<<std::endl;
}
void MProcessor::flood() {
  if(argsLessLimit(2)) return;
  std::cout<<"Flood: "<<vargs[0]<<std::endl;
}
void MProcessor::skip() {
  if(argsLessLimit(1)) return;
  std::cout<<"Skip: "<<vargs[0]<<std::endl;
}
void MProcessor::handOver() {
  if(argsLessLimit(3)) return;
  std::cout<<"Hand over: "<<vargs[0]<<" card "<<vargs[1]<<" to "<<vargs[3]<<std::endl;
}
void MProcessor::getItemCard() {
  if(argsLessLimit(2)) return;
  MAdventurer* adventurer = findAdventurer(vargs[0]);
  MCard* card = findItemCard(vargs[1]);
  if(!adventurer) return;
  if(!card) return;
  adventurer->addCard(card);
  if(adventurer->getCardsNumber() > CARDS_LIMIT) {
    //draw cards
  }
  std::cout<<"Get: "<<vargs[0]<<" item card "<<vargs[1]<<std::endl;
}
void MProcessor::getFloodCard() {
  if(argsLessLimit(2)) return;
  MCard* card = findItemCard(vargs[1]);
  if(!card) return;
  std::string params = vargs[0] + " " + card->getName();
  execFunction("flood", params);
  std::cout<<"Get: "<<vargs[0]<<" flood card "<<vargs[1]<<std::endl;
}
void MProcessor::drawCard() {
  if(argsLessLimit(2)) return;
  MAdventurer* adventurer = findAdventurer(vargs[0]);
  MCard* card = findItemCard(vargs[1]);
  if(!adventurer) return;
  if(!card) return;
  adventurer->removeCard(card);
  std::cout<<"Draw card: "<<vargs[0]<<" card "<<vargs[1]<<std::endl;
}
void MProcessor::useCard() {
  if(argsLessLimit(2)) return;
  MAdventurer* adventurer = findAdventurer(vargs[0]);
  MCard* card = findItemCard(vargs[1]);
  if(!adventurer) return;
  if(!card) return;
  //get card type
  //use card
  //remove card
  adventurer->removeCard(card);
  std::cout<<"Use card: "<<vargs[0]<<" card "<<vargs[1]<<std::endl;
}
void MProcessor::getArtifact() {
  if(argsLessLimit(2)) return;
  std::cout<<"Get artifact: "<<vargs[0]<<" get "<<vargs[1]<<std::endl;
}
void MProcessor::parseArgs(const std::string& _sargs) {
  sargs = _sargs;
  vargs.clear();
  if(sargs.empty()) {
    return;
  }
  std::string delim = " ";
  std::string arg;
  size_t beg = 0;
  size_t end = 0;
  while((end = sargs.find(delim, beg)) != std::string::npos) {
    arg = sargs.substr(beg, end);
    vargs.push_back(arg);
    beg = end + 1;
  }
  arg = sargs.substr(beg, sargs.size() - beg);
  vargs.push_back(arg);
}
void MProcessor::call(const std::string& name) {
  (*this.*m[name])();
}
void MProcessor::intitMaps() {
  m.insert(std::pair<std::string, pt2>("start", &MProcessor::start));
  m.insert(std::pair<std::string, pt2>("move", &MProcessor::move));
  m.insert(std::pair<std::string, pt2>("abfluss", &MProcessor::abfluss));
  m.insert(std::pair<std::string, pt2>("flood", &MProcessor::flood));
  m.insert(std::pair<std::string, pt2>("skip", &MProcessor::skip));
  m.insert(std::pair<std::string, pt2>("handover", &MProcessor::handOver));
  m.insert(std::pair<std::string, pt2>("getitemcard", &MProcessor::getItemCard));
  m.insert(std::pair<std::string, pt2>("getfloodcard", &MProcessor::getFloodCard));
  m.insert(std::pair<std::string, pt2>("drawcard", &MProcessor::drawCard));
  m.insert(std::pair<std::string, pt2>("usecard", &MProcessor::useCard));
  m.insert(std::pair<std::string, pt2>("getartifact", &MProcessor::getArtifact));

  initAreas();

  adventurers.insert(std::pair<std::string, MObject*>("adven1", new MAdventurer("adven1")));
  adventurers.insert(std::pair<std::string, MObject*>("adven2", new MAdventurer("adven2")));

  itemCards.insert(std::pair<std::string, MObject*>("itemcard1", new MCard("itemcard1")));
  itemCards.insert(std::pair<std::string, MObject*>("itemcard2", new MCard("itemcard2")));
  itemCards.insert(std::pair<std::string, MObject*>("itemcard3", new MCard("itemcard3")));

  floodCards.insert(std::pair<std::string, MObject*>("floodcard1", new MCard("floodcard1")));
  floodCards.insert(std::pair<std::string, MObject*>("floodcard2", new MCard("floodcard2")));
  floodCards.insert(std::pair<std::string, MObject*>("floodcard3", new MCard("floodcard3")));
}
void MProcessor::initAreas() {
  int rows = 2;
  int cols = 2;
  char buff[4];
  std::string base;
  for(int i=0; i<rows; i++) {
    for(int j=0; j<cols; j++) {
      base = "area";
      base += itoa(i, buff, 10);
      base += itoa(j, buff, 10);
      areas.insert(std::pair<std::string, MObject*>(base, new MArea(base, i, j)));
    }
  }
  //some additional stuff
}
MProcessor::MProcessor() {
  intitMaps();
}
MProcessor::~MProcessor() {
  m.clear();
  vargs.clear();
  sargs.clear();
  for(moi moit = areas.begin(); moit != areas.end(); moit++) {
    if(moit->second) delete moit->second;
  }
  areas.clear();
  for(moi moit = adventurers.begin(); moit != adventurers.end(); moit++) {
    if(moit->second) delete moit->second;
  }
  adventurers.clear();
  for(moi moit = itemCards.begin(); moit != itemCards.end(); moit++) {
    if(moit->second) delete moit->second;
  }
  itemCards.clear();
  for(moi moit = floodCards.begin(); moit != floodCards.end(); moit++) {
    if(moit->second) delete moit->second;
  }
  floodCards.clear();
}
bool MProcessor::execFunction(const std::string& name, const std::string& _sargs) {
  parseArgs(_sargs);
  mi mit = m.find(name);
  if(mit == m.end()) {
    std::cout<<"Function ["<<name<<"] not found!"<<std::endl;
    return false;
  }
  call(name);
  return true;
}
