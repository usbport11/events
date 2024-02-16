#include "processor.h"

#include "area.h"
#include "adventurer.h"
#include "card.h"
#include "artifact.h"
#include <ctime>
#include <iostream>
#include <cstring>

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
void MProcessor::randDeck(std::map<std::string, MObject*> cards, std::deque<std::string>& deck) {
  std::vector<std::string> rndBase;
  int rnd;
  deck.clear();
  for(moi moit=cards.begin(); moit!=cards.end(); moit++) {
    rndBase.push_back(moit->first);
  }
  while(!rndBase.empty()) {
    distribute = std::uniform_int_distribution<int>(0, rndBase.size()-1);
    rnd = distribute(rng);
	deck.push_front(rndBase[rnd]);
	rndBase.erase(rndBase.begin() + rnd);
  }
  rndBase.clear();
}
void MProcessor::start() {
  totalFloodLevel = 0;

  int adventurerNumber = 2;
  int startFloods = 6;
  int rnd, num = 0;

  if(areas.size() != floodCards.size()) {
    std::cout<<"Flood cards not equal areas"<<std::endl;
    return;
  }

  std::cout<<"clear adventurer items, positions"<<std::endl;
  MAdventurer* adv;
  for(moi moit = adventurers.begin(); moit != adventurers.end(); moit++) {
    adv = (MAdventurer*)moit->second;
	adv->removeAllCards();
	adv->setArea((MArea*)areas[adv->getStartArea()]);
  }

  std::cout<<"random place areas, remove flood on areas"<<std::endl;
  initAreas();

  std::cout<<"chose random adventurers"<<std::endl;
  activeAdventurers.clear();
  std::vector<std::string> rndBaseStr;
  std::vector<int> rndBaseInt;
  for(moi moit=adventurers.begin(); moit!=adventurers.end(); moit++) {
    rndBaseStr.push_back(moit->first);
  }
  while(!rndBaseStr.empty() && num < adventurerNumber) {
    distribute = std::uniform_int_distribution<int>(0, rndBaseStr.size()-1);
    rnd = distribute(rng);
	activeAdventurers.push_back(rndBaseStr[rnd]);
	rndBaseStr.erase(rndBaseStr.begin() + rnd);
	num ++;
  }
  rndBaseStr.clear();

  std::cout<<"random item deck and flood deck"<<std::endl;
  initDecks();

  std::cout<<"flood areas by get flood cards"<<std::endl;
  for(int i=0; i<startFloods; i++) {
    execFunction("getfloodcard", "");
  }

  std::cout<<"get two cards by each adventurer"<<std::endl;
  for(int i=0; i<adventurerNumber; i++) {
    execFunction("getitemcard", activeAdventurers[i]);
  }
}
void MProcessor::move() {
  if(argsLessLimit(2)) return;
  std::cout<<"Move: "<<vargs[0]<<" to the "<<vargs[1]<<std::endl;
}
void MProcessor::abfluss() {
  if(argsLessLimit(1)) return;
  MArea* area = findArea(vargs[0]);
  if(!area) return;
  area->abfluss();
  std::cout<<"Abluss: "<<vargs[0]<<std::endl;
}
void MProcessor::flood() {
  if(argsLessLimit(1)) return;
  MArea* area = findArea(vargs[0]);
  if(!area) return;
  area->flood();
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
  if(argsLessLimit(1)) return;
  std::cout<<"Get item card by "<<vargs[0]<<std::endl;
  MAdventurer* adventurer = findAdventurer(vargs[0]);
  if(!adventurer) return;
  adventurer->addCard((MCard*)itemCards[itemDeck.front()]);
  itemDropDeck.push_front(itemDeck.front());
  itemDeck.pop_front();
}
void MProcessor::getFloodCard() {
  std::cout<<"Get flood card"<<std::endl;
  execFunction("flood", floodDeck.front());
  floodDropDeck.push_back(floodDeck.front());
  floodDeck.pop_front();
}
void MProcessor::discard() {
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
  //draw four artifacts cards
  //for(int i=0; i<4; i++) {
  //  execFunction("drawcard", vargs[0] + " artifact_card_name")
  //}
  std::cout<<"Get artifact: "<<vargs[0]<<" get "<<vargs[1]<<std::endl;
}
void MProcessor::extract() {
  std::cout<<"extract!"<<std::endl;
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
void MProcessor::fillItemCards(const std::string& _name, const std::string& type, int number) {
  std::string name = _name;
  char buff[4];
  for(int i=0; i<number; i++) {
    std::memset(buff, 0, 4);
    itoa(i, buff, 10);
    name += buff;
    itemCards.insert(std::pair<std::string, MObject*>(name, new MCard(name, type)));
  }
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
  m.insert(std::pair<std::string, pt2>("discard", &MProcessor::discard));
  m.insert(std::pair<std::string, pt2>("usecard", &MProcessor::useCard));
  m.insert(std::pair<std::string, pt2>("getartifact", &MProcessor::getArtifact));
  m.insert(std::pair<std::string, pt2>("extract", &MProcessor::extract));

  areas.insert(std::pair<std::string, MObject*>("gate", new MArea("gate")));
  areas.insert(std::pair<std::string, MObject*>("castle", new MArea("castle")));
  areas.insert(std::pair<std::string, MObject*>("house", new MArea("house")));
  areas.insert(std::pair<std::string, MObject*>("swamp", new MArea("swamp")));
  areas.insert(std::pair<std::string, MObject*>("monastary", new MArea("monastary")));
  areas.insert(std::pair<std::string, MObject*>("factory", new MArea("factory")));
  areas.insert(std::pair<std::string, MObject*>("river", new MArea("river")));
  areas.insert(std::pair<std::string, MObject*>("hole", new MArea("hole")));
  areas.insert(std::pair<std::string, MObject*>("lake", new MArea("lake")));
  areas.insert(std::pair<std::string, MObject*>("port", new MArea("port")));
  areas.insert(std::pair<std::string, MObject*>("cavern", new MArea("cavern")));
  areas.insert(std::pair<std::string, MObject*>("plato", new MArea("plato")));

  adventurers.insert(std::pair<std::string, MObject*>("trader", new MAdventurer("trader", "gate")));
  adventurers.insert(std::pair<std::string, MObject*>("pilot", new MAdventurer("pilot", "plato")));
  adventurers.insert(std::pair<std::string, MObject*>("engineer", new MAdventurer("engineer", "factory")));
  adventurers.insert(std::pair<std::string, MObject*>("traveler", new MAdventurer("traveler", "plato")));

  artifacts.insert(std::pair<std::string, MObject*>("crystall", new MArtifact("crystall")));
  artifacts.insert(std::pair<std::string, MObject*>("sphere", new MArtifact("sphere")));
  artifacts.insert(std::pair<std::string, MObject*>("cube", new MArtifact("cube")));
  artifacts.insert(std::pair<std::string, MObject*>("key", new MArtifact("key")));

  //artifact cards
  for(moi moit = artifacts.begin(); moit != artifacts.end(); moit++) {
    fillItemCards(moit->first, "artifact", 5);
  }
  //ext cards
  fillItemCards("helicopter", "item", 2);
  fillItemCards("sandbag", "item", 2);
  fillItemCards("flood", "flood", 3);

  //equal to areas number and name
  for(moi moit=areas.begin(); moit != areas.end(); moit++) {
    floodCards.insert(std::pair<std::string, MObject*>(moit->first, new MCard(moit->first)));
  }
}
void MProcessor::initAreas() {
  int rows = 4, cols = 4; // move to global?
  if(areas.size() < rows * cols) return;

  int inds[8][2] = {{-1,-1}, {0,-1}, {1,-1}, {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}};
  int* ind;
  MArea* area;
  std::map<int, MArea*> temp;
  std::vector<std::string> rndBase;

  int rnd, num = 0;
  int x, y;
  for(moi moit=areas.begin(); moit!=areas.end(); moit++) {
    rndBase.push_back(moit->first);
  }
  while(!rndBase.empty()) {
    distribute = std::uniform_int_distribution<int>(0, rndBase.size()-1);
    rnd = distribute(rng);
    area = (MArea*)areas[rndBase[rnd]];
    area->setIndex(int(num / rows), num - (int(num / rows)) * rows);
	area->setFloodLevel(0);
    rndBase.erase(rndBase.begin() + rnd);
    temp.insert(std::pair<int, MArea*>(num, area));
    num ++;
  }
  for(int i=0; i<rows; i++) {
    for(int j=0; j<cols; j++) {
      ind = temp[i * rows + j]->getIndex();
      for(int k=0; k<8; k++) {
        x = ind[0] + inds[k][0];
        y = ind[1] + inds[k][1];
        if((x < 0 || y < 0) || (x >= rows || y >= cols)) continue;
        temp[i * rows + j]->addNeighbor(temp[x * rows + y]);
	  }
	}
  }
  temp.clear();
  rndBase.clear();
}
void MProcessor::initDecks() {
  randDeck(itemCards, itemDeck);
  randDeck(floodCards, floodDeck);
}
MProcessor::MProcessor() {
  struct timespec tm;
  clock_gettime(CLOCK_REALTIME, &tm);
  rng.seed(tm.tv_nsec);

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
  for(moi moit = artifacts.begin(); moit != artifacts.end(); moit++) {
    if(moit->second) delete moit->second;
  }
  artifacts.clear();

  itemDeck.clear();
  itemDropDeck.clear();
  floodDeck.clear();
  floodDropDeck.clear();
  activeAdventurers.clear();
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
