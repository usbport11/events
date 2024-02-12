#include "processor.h"

#include <iostream>
#include "area.h"
#include "adventurer.h"

bool MProcessor::argsLessLimit(int num) {
  return (vargs.size() < num);
}
MArea* MProcessor::getArea(const std::string& name) {
  moi moit = areas.find(name);
  if(moit == areas.end()) {
    std::cout<<"Area ["<<name<<"] not found!"<<std::endl;
  }
  return (MArea*)areas[name];
}
MAdventurer* MProcessor::getAdventurer(const std::string& name) {
  moi moit = adventurers.find(name);
  if(moit == adventurers.end()) {
    std::cout<<"Adventurer ["<<name<<"] not found!"<<std::endl;
  }
  return (MAdventurer*)adventurers[name];
}
void MProcessor::print() {
  std::cout<<sargs<<std::endl;
}
void MProcessor::move() {
  if(argsLessLimit(2)) return;
  MObject* adv = getAdventurer(vargs[0]);
  MObject* area = getArea(vargs[1]);
  if(!adv) return;
  if(!area) return;
  std::cout<<"Move: "<<adv->getName()<<" to the "<<area->getName()<<std::endl;
}
void MProcessor::abfluss() {
  if(argsLessLimit(2)) return;
  MObject* adv = getAdventurer(vargs[0]);
  MObject* area = getArea(vargs[1]);
  if(!adv) return;
  if(!area) return;
    std::cout<<"Abfluss: "<<adv->getName()<<" the "<<area->getName()<<std::endl;
}
void MProcessor::flood() {
  if(argsLessLimit(2)) return;
  std::cout<<"Flood: "<<vargs[0]<<std::endl;
}
void MProcessor::skip() {
  if(argsLessLimit(1)) return;
  MObject* adv = getAdventurer(vargs[0]);
  if(!adv) return;
  std::cout<<"Skip: "<<adv->getName()<<std::endl;
}
void MProcessor::handOver() {
  if(argsLessLimit(3)) return;
  std::cout<<"Hand over: "<<vargs[0]<<" card "<<vargs[1]<<" to "<<vargs[3]<<std::endl;
}
void MProcessor::getCard() {
  if(argsLessLimit(2)) return;
  std::cout<<"Get card: "<<vargs[0]<<" card "<<vargs[1]<<std::endl;
}
void MProcessor::drawCard() {
  if(argsLessLimit(2)) return;
  std::cout<<"Draw card: "<<vargs[0]<<" card "<<vargs[1]<<std::endl;
}
void MProcessor::useCard() {
  if(argsLessLimit(2)) return;
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
  m.insert(std::pair<std::string, pt2>("print", &MProcessor::print));
  m.insert(std::pair<std::string, pt2>("move", &MProcessor::move));
  m.insert(std::pair<std::string, pt2>("abfluss", &MProcessor::abfluss));
  m.insert(std::pair<std::string, pt2>("flood", &MProcessor::flood));
  m.insert(std::pair<std::string, pt2>("skip", &MProcessor::skip));
  m.insert(std::pair<std::string, pt2>("handover", &MProcessor::handOver));
  m.insert(std::pair<std::string, pt2>("getcard", &MProcessor::getCard));
  m.insert(std::pair<std::string, pt2>("drawcard", &MProcessor::drawCard));
  m.insert(std::pair<std::string, pt2>("usecard", &MProcessor::useCard));
  m.insert(std::pair<std::string, pt2>("getartifact", &MProcessor::getArtifact));

  initAreas();

  adventurers.insert(std::pair<std::string, MObject*>("adven1", new MAdventurer("adven1")));
  adventurers.insert(std::pair<std::string, MObject*>("adven2", new MAdventurer("adven2")));
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
  std::map<std::string, MObject*> areas;
}
MProcessor::MProcessor() {
  intitMaps();
}
MProcessor::~MProcessor() {
  m.clear();
  vargs.clear();
  sargs.clear();
  areas.clear();
  adventurers.clear();
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
