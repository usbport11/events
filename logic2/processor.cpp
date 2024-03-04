#include "processor.h"

#include "area.h"
#include "adventurer.h"
#include "card.h"
#include "artifact.h"
#include "ui.h"

#include <ctime>
#include <iostream>
#include <cstring>
#include <algorithm>

#define ACT_MOVE 101
#define ACT_ABFLUSS 102
#define ACT_SKIP 103
#define ACT_HANDOVER 104
#define ACT_GETARTIFACT 105
#define ACT_FLY 106
#define ACT_MOVEOTHER 107
#define ACT_SWIM 108
#define ACT_EXTRACT 109

bool MProcessor::argsLessLimit(int num) {
  return (vargs.size() < num);
}
MObject* MProcessor::findObject(std::map<std::string, MObject*>& objects, const std::string& name) {
  moi moit = objects.find(name);
  if(moit == objects.end()) {
    std::cout<<"Object ["<<name<<"] not found!"<<std::endl;
    return nullptr;
  }
  return objects[name];
}
MArea* MProcessor::findArea(const std::string& name) {
  return (MArea*)findObject(areas, name);
}
MAdventurer* MProcessor::findAdventurer(const std::string& name) {
  return (MAdventurer*)findObject(adventurers, name);
}
MCard* MProcessor::findItemCard(const std::string& name) {
  return (MCard*)findObject(itemCards, name);
}
MCard* MProcessor::findFloodCard(const std::string& name) {
  return (MCard*)findObject(floodCards, name);
}
MArtifact* MProcessor::findActifact(const std::string& name) {
  return (MArtifact*)findObject(artifacts, name);
}
void MProcessor::randStartDeck(std::map<std::string, MObject*> cards, std::deque<std::string>& deck) {
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
}
void MProcessor::randDeck(std::deque<std::string>& deck) {
  std::deque<std::string> temp;
  int rnd;
  while(!deck.empty()) {
    distribute = std::uniform_int_distribution<int>(0, deck.size()-1);
    rnd = distribute(rng);
    temp.push_back(deck.at(rnd));
    deck.erase(deck.begin() + rnd);
  }
  deck = temp;
}
void MProcessor::moveDeck(std::deque<std::string>& src, std::deque<std::string>& dest) {
  while(!src.empty()) {
    dest.push_back(src.front());
    src.pop_front();
  }
}
void MProcessor::start() {
  floodLevel = 0;
  adventurerNumber = 2;
  lastItemCard = nullptr;

  int startFloods = 6;
  int rnd, num = 0;

  if(areas.size() != floodCards.size()) {
    std::cout<<"Flood cards not equal areas"<<std::endl;
    return;
  }

  std::cout<<"Clear collected artifacts"<<std::endl;
  collectedArtifacts.clear();

  std::cout<<"Clear adventurer items, artifacts, positions"<<std::endl;
  MAdventurer* adv;
  for(moi moit = adventurers.begin(); moit != adventurers.end(); moit++) {
    adv = (MAdventurer*)moit->second;
	adv->removeAllCards();
	adv->removeAllArtifacts();
	adv->setArea(nullptr);
  }

  std::cout<<"Random place areas, remove flood on areas"<<std::endl;
  initAreas();

  std::cout<<"Chose random adventurers, set them on their start areas"<<std::endl;
  activeAdventurers.clear();
  std::vector<std::string> rndBaseStr;
  std::vector<int> rndBaseInt;
  for(moi moit=adventurers.begin(); moit!=adventurers.end(); moit++) {
    rndBaseStr.push_back(moit->first);
  }
  while(!rndBaseStr.empty() && num < adventurerNumber) {
    distribute = std::uniform_int_distribution<int>(0, rndBaseStr.size()-1);
    rnd = distribute(rng);
    adv = findAdventurer(rndBaseStr[rnd]);
    if(!adv) return;
    adv->setArea((MArea*)areas[adv->getStartArea()]);
	activeAdventurers.push_back(adv->getName());
	rndBaseStr.erase(rndBaseStr.begin() + rnd);
	num ++;
  }

  std::cout<<"Random item deck and flood deck"<<std::endl;
  itemDropDeck.clear();
  floodDropDeck.clear();
  floodOutDeck.clear();
  randStartDeck(itemCards, itemDeck);
  randStartDeck(floodCards, floodDeck);

  std::cout<<"Flood areas by get flood cards"<<std::endl;
  for(int i=0; i<startFloods; i++) {
    execFunction("getfloodcard", "");
  }

  std::cout<<"Get two cards by each adventurer"<<std::endl;
  for(int i=0; i<adventurerNumber; i++) {
    execFunction("getitemcard", activeAdventurers[i]);
    execFunction("getitemcard", activeAdventurers[i]);
  }
}
void MProcessor::move() {
  if(argsLessLimit(2)) return;
  MAdventurer* adventurer = findAdventurer(vargs[0]);
  MArea* area = findArea(vargs[1]);
  if(!adventurer) return;
  if(!area) return;
  adventurer->setArea(area);
  std::cout<<"Move: "<<vargs[0]<<" to the "<<vargs[1]<<std::endl;
}
void MProcessor::abfluss() {
  if(argsLessLimit(2)) return;
  MAdventurer* adventurer = findAdventurer(vargs[0]);
  MArea* area1 = findArea(vargs[1]);
  MArea* area2;
  if(!adventurer) return;
  if(!area1) return;
  area1->abfluss();
  std::cout<<"Abluss: "<<vargs[1]<<std::endl;
  if(vargs.size() == 3) {
    area2 = findArea(vargs[2]);
    if(!area2) return;
    std::cout<<"Abluss: "<<vargs[2]<<std::endl;
  }
}
void MProcessor::flood() {
  if(argsLessLimit(1)) return;
  MArea* area = findArea(vargs[0]);
  MAdventurer* adventurer;
  std::list<MArea*> neighbors;
  if(!area) return;
  area->flood();
  if(area->getFloodLevel() >= 2) {
    for(int i=0; i<activeAdventurers.size(); i++) {
      adventurer = findAdventurer(activeAdventurers[i]);
      if(!adventurer) return;
      if(adventurer->getArea() == area) {
        std::cout<<" "<<adventurer->getName()<<" stay on drown area"<<std::endl;
        neighbors = area->getAllActiveNeighbors();
		if(neighbors.empty()) {
          std::cout<<" "<<adventurer->getName()<<" drown"<<std::endl;
		  activeAdventurers.erase(activeAdventurers.begin() + i);
		}
        execFunction("move", adventurer->getName() + " " + (*neighbors.begin())->getName());
      }
    }
    floodOutDeck.push_back(floodDeck.front());
  }
  else {
    floodDropDeck.push_back(floodDeck.front());
  }
  floodDeck.pop_front();
  std::cout<<"Flood: "<<vargs[0]<<std::endl;
}
void MProcessor::skip() {
  if(argsLessLimit(1)) return;
  std::cout<<"Skip: "<<vargs[0]<<std::endl;
}
void MProcessor::handOver() {
  if(argsLessLimit(3)) return;
  MAdventurer* srcAdventurer = findAdventurer(vargs[0]);
  MAdventurer* dstAdventurer = findAdventurer(vargs[1]);
  MCard* card = findItemCard(vargs[2]);
  if(!srcAdventurer || !dstAdventurer) return;
  if(!card) return;
  dstAdventurer->addCard(card);
  std::cout<<"Handover "<<vargs[2]<<" from "<<vargs[0]<<" to "<<vargs[1]<<std::endl;
}
void MProcessor::getItemCard() {
  int rnd;
  if(argsLessLimit(1)) return;
  MAdventurer* adventurer = findAdventurer(vargs[0]);
  if(!adventurer) return;

  if(itemDeck.empty()) {
    randDeck(itemDropDeck);
    moveDeck(itemDropDeck, itemDeck);
  }

  MCard* card = findItemCard(itemDeck.front());
  if(!card) return;
  std::cout<<"Get item card "<<card->getName()<<" by "<<vargs[0]<<std::endl;
  if(card->getType() == "item" || card->getType() == "artifact") {
    adventurer->addCard(card);
  }

  if(card->getType() == "flood") {
    floodLevel += 0.5;
    std::cout<<"  Flood level increase: "<<floodLevel<<std::endl;
    if(lastItemCard) {
      if(lastItemCard->getType() != "flood") {
        randDeck(floodDropDeck);
        moveDeck(floodDropDeck, floodDeck);
      }
    }
    else {
      randDeck(floodDropDeck);
      moveDeck(floodDropDeck, floodDeck);
    }
  }
  itemDropDeck.push_front(itemDeck.front());
  itemDeck.pop_front();

  lastItemCard = card;
}
void MProcessor::getFloodCard() {
  if(floodDeck.empty()) {
    randDeck(floodDropDeck);
    moveDeck(floodDropDeck, floodDeck);
  }
  std::cout<<"Get flood card"<<std::endl;
  execFunction("flood", floodDeck.front());
}
void MProcessor::discard() {
  if(argsLessLimit(2)) return;
  MAdventurer* adventurer = findAdventurer(vargs[0]);
  MCard* card = findItemCard(vargs[1]);
  if(!adventurer) return;
  if(!card) return;
  itemDropDeck.push_front(card->getName());
  adventurer->removeCard(card);
  std::cout<<"Discard: "<<vargs[0]<<" card "<<vargs[1]<<std::endl;
}
void MProcessor::useCard() {
  if(argsLessLimit(2)) return;
  std::cout<<"Use card: "<<vargs[1]<<" by "<<vargs[0]<<std::endl;

  std::vector<std::string> names;
  names.push_back("helicopter");
  names.push_back("sandbag");
  MAdventurer* adventurer = findAdventurer(vargs[0]);
  MCard* card = findItemCard(vargs[1]);
  if(!adventurer) return;
  if(!card) return;
  if(card->getType() == "item") {
    for(int i=0; i<names.size(); i++) {
      if(names[i] == card->getName().substr(0, names[i].length())) {
        if(names[i] == "helicopter") {
          if(argsLessLimit(4)) return;
          execFunction("move", vargs[2] + " " + vargs[3]);
          break;
        }
        if(names[i] == "sandbag") {
          if(argsLessLimit(3)) return;
          execFunction("abfluss", vargs[2]);
          break;
        }
      }
    }
  }
  else return;
  itemDropDeck.push_front(card->getName());
  adventurer->removeCard(card);
}
void MProcessor::getArtifact() {
  if(argsLessLimit(2)) return;
  MAdventurer* adventurer = findAdventurer(vargs[0]);
  MArtifact* artifact = findActifact(vargs[1]);
  if(!adventurer) return;
  if(!artifact) return;
  adventurer->addArtifact(artifact);
  collectedArtifacts.push_back(vargs[1]);
  std::vector<MCard*> cards = adventurer->getArtifactCards(vargs[1]);
  for(int i=0; i<4 ;i++) {
    execFunction("discard", adventurer->getName() + " " + cards[i]->getName());
  }
  std::cout<<"Get artifact: "<<vargs[0]<<" get "<<vargs[1]<<std::endl;
}
void MProcessor::fly() {
  if(argsLessLimit(2)) return;
  MAdventurer* adventurer = findAdventurer(vargs[0]);
  MArea* area = findArea(vargs[1]);
  if(!adventurer) return;
  if(!area) return;
  adventurer->setArea(area);
  std::cout<<"Fly: "<<vargs[0]<<" to the "<<vargs[1]<<std::endl;
}
void MProcessor::moveOther() {
  if(argsLessLimit(3)) return;
  std::cout<<"Move "<<vargs[1]<<" by "<<vargs[0]<<" to the "<<vargs[2]<<std::endl;
  execFunction("move", vargs[1] + " " + vargs[2]);
}
void MProcessor::swim() {
  if(argsLessLimit(2)) return;
  MAdventurer* adventurer = findAdventurer(vargs[0]);
  MArea* area = findArea(vargs[1]);
  if(!adventurer) return;
  if(!area) return;
  adventurer->setArea(area);
  std::cout<<"Swim: "<<vargs[0]<<" to the "<<vargs[1]<<std::endl;
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
    arg = sargs.substr(beg, end - beg);
    vargs.push_back(arg);
    beg = end + 1;
  }
  arg = sargs.substr(beg, sargs.size() - beg);
  vargs.push_back(arg);
}
void MProcessor::call(const std::string& name) {
  (*this.*m[name])();
}
void MProcessor::createItemCards(const std::string& _name, const std::string& type, int number) {
  std::string name;
  char buff[4];
  for(int i=0; i<number; i++) {
    name = _name;
    std::memset(buff, 0, 4);
    itoa(i, buff, 10);
    name += buff;
    itemCards[name] = new MCard(name, type);
  }
}
void MProcessor::intitMaps() {
  m["start"] = MProcessor::start;
  m["move"] = MProcessor::move;
  m["abfluss"] = MProcessor::abfluss;
  m["flood"] = MProcessor::flood;
  m["skip"] = MProcessor::skip;
  m["handover"] = MProcessor::handOver;
  m["getitemcard"] = MProcessor::getItemCard;
  m["getfloodcard"] = MProcessor::getFloodCard;
  m["discard"] = MProcessor::discard;
  m["usecard"] = MProcessor::useCard;
  m["getartifact"] = MProcessor::getArtifact;
  m["fly"] = MProcessor::fly;
  m["moveother"] = MProcessor::moveOther;
  m["swim"] = MProcessor::swim;
  m["extract"] = MProcessor::extract;

  areas["gate"] = new MArea("gate");
  areas["castle"] = new MArea("castle");
  areas["house"] = new MArea("house");
  areas["swamp"] = new MArea("swamp");
  areas["monastary"] = new MArea("monastary");
  areas["factory"] = new MArea("factory");
  areas["river"] = new MArea("river");
  areas["hole"] = new MArea("hole");
  areas["lake"] = new MArea("lake");
  areas["port"] = new MArea("port");
  areas["cavern"] = new MArea("cavern");
  areas["libary"] = new MArea("libary");
  areas["swimpool"] = new MArea("swimpool");
  areas["shop"] = new MArea("shop");
  areas["crossroad"] = new MArea("crossroad");
  areas["plato"] = new MArea("plato");

  adventurers["explorer"] = new MAdventurer("explorer", "gate");//can diag move or abfluss
  adventurers["pilot"] = new MAdventurer("pilot", "plato");//one time can move anywhere
  adventurers["engineer"] = new MAdventurer("engineer", "factory");//can abluss one or two areas
  adventurers["liaison"] = new MAdventurer("liaison", "shop");//can handover on any distance
  adventurers["navigator"] = new MAdventurer("navigator", "castle");//can move other adventurer on one or two areas
  adventurers["diver"] = new MAdventurer("diver", "lake");//can move on any number abfluss or none areas

  artifacts["crystall"] = new MArtifact("crystall", "gate", "house");
  artifacts["sphere"] = new MArtifact("sphere", "monastary", "cavern");
  artifacts["cube"] = new MArtifact("cube", "plato", "port");
  artifacts["key"] = new MArtifact("key", "hole", "factory");

  //artifact cards
  for(moi moit = artifacts.begin(); moit != artifacts.end(); moit++) {
    createItemCards(moit->first, "artifact", 5);
  }
  //ext cards
  createItemCards("helicopter", "item", 2);
  createItemCards("sandbag", "item", 2);
  createItemCards("flood", "flood", 3);

  //equal to areas number and name
  for(moi moit=areas.begin(); moit != areas.end(); moit++) {
    floodCards[moit->first] = new MCard(moit->first);
  }

  actionsSwitches["move"] = ACT_MOVE;
  actionsSwitches["abfluss"] = ACT_ABFLUSS;
  actionsSwitches["skip"] = ACT_SKIP;
  actionsSwitches["handover"] = ACT_HANDOVER;
  actionsSwitches["getartifact"] = ACT_GETARTIFACT;
  actionsSwitches["fly"] = ACT_FLY;
  actionsSwitches["moveother"] = ACT_MOVEOTHER;
  actionsSwitches["swim"] = ACT_SWIM;
  actionsSwitches["extract"] = ACT_EXTRACT;
}
void MProcessor::initAreas() {
  int num;

  //simple type
  int rows = 4, cols = 4;
  bool areaMap[rows][cols] = {
    {1,1,1,1},
    {1,1,1,1},
    {1,1,1,1},
    {1,1,1,1}
  };
  /*
  //more complex
  int rows = 6, cols = 6;
  bool areaMap[6][6] = {
    {0,0,1,1,0,0},
    {0,1,1,1,1,0},
    {1,1,1,1,1,1},
    {1,1,1,1,1,1},
    {0,1,1,1,1,0},
    {0,0,1,1,0,0}
  };
  */

  num = 0;
  for(int i=0; i<rows; i++) {
    for(int j=0; j<cols; j++) {
      if(areaMap[i][j]) num ++;
    }
  }
  if(areas.size() < num) {
    std::cout<<"Error: areas size < row * cols"<<std::endl;
    return;
  }

  int inds[8][2] = {{-1,-1}, {0,-1}, {1,-1}, {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}};
  int* ind;
  MArea* area;
  std::map<int, MArea*> temp;
  std::vector<std::string> rndBase;

  int rnd;
  int x, y;
  for(moi moit=areas.begin(); moit!=areas.end(); moit++) {
    rndBase.push_back(moit->first);
  }

  num = 0;
  while(!rndBase.empty()) {
    x = int(num / rows);
    y = num - (int(num / rows)) * rows;
    if(!areaMap[x][y]) continue;
    distribute = std::uniform_int_distribution<int>(0, rndBase.size()-1);
    rnd = distribute(rng);
    area = (MArea*)areas[rndBase[rnd]];
    area->setIndex(x, y);
	area->setFloodLevel(0);
    rndBase.erase(rndBase.begin() + rnd);
    temp[num] = area;
    num ++;
  }

  for(int i=0; i<rows; i++) {
    for(int j=0; j<cols; j++) {
      ind = temp[i * rows + j]->getIndex();
      std::cout<<" "<<temp[i * rows + j]->getName();
      for(int k=0; k<8; k++) {
        x = ind[0] + inds[k][0];
        y = ind[1] + inds[k][1];
        if((x < 0 || y < 0) || (x >= rows || y >= cols)) continue;
        temp[i * rows + j]->addNeighbor(temp[x * rows + y]);
	  }
	}
	std::cout<<std::endl;
  }
}
MProcessor::MProcessor():extractionArea("plato") {
  struct timespec tm;
  clock_gettime(CLOCK_REALTIME, &tm);
  rng.seed(tm.tv_nsec);
  intitMaps();
  ui = new MUI;
}
bool MProcessor::looseCheck() {
  int num;
  MArtifact* artifact;
  MArea* area;

  for(moi moit = artifacts.begin(); moit != moit; moit++) {
    num = 0;
    if(find(collectedArtifacts.begin(), collectedArtifacts.end(), moit->first) != collectedArtifacts.end()) continue;
    artifact = (MArtifact*)moit->second;
    for(int i=0; i<2; i++) {
      area = (MArea*)areas[artifact->getMainAreas()[i]];
	  if(area->getFloodLevel() >= 2) num ++;
	}
	if(num >= 2) {
      std::cout<<"Some artifact flooded"<<std::endl;
      return true;
	}
  }

  area = findArea(extractionArea);
  if(!area) {
    std::cout<<"Extraction area not found"<<std::endl;
    return true;
  }
  if(area->getFloodLevel() >= 2) {
    std::cout<<"Extraction area flooded"<<std::endl;
    return true;
  }
  //max flood level reach
  if(floodLevel > 5) {
    std::cout<<"Total flooded"<<std::endl;
    return true;
  }

  return false;
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

  if(ui) delete ui;

  itemDeck.clear();
  itemDropDeck.clear();
  floodDeck.clear();
  floodDropDeck.clear();
  floodOutDeck.clear();
  activeAdventurers.clear();
  collectedArtifacts.clear();
  usedActions.clear();
  actionsSwitches.clear();
}
bool MProcessor::execFunction(const std::string& name, const std::string& _sargs) {
  parseArgs(_sargs);
  std::map<std::string, pt2>::iterator mit = m.find(name);
  if(mit == m.end()) {
    std::cout<<"Function ["<<name<<"] not found!"<<std::endl;
    return false;
  }
  call(name);
  return true;
}
bool MProcessor::tryMomentCard(MAdventurer* adventurer) {
  MCard* card;
  std::vector<MCard*> cards = adventurer->getMomentCards();
  if(!cards.empty()) {
    while(ui->askQuestion("Whould you like to use moment card?") == true) {
	  card = ui->selectCard(cards);
      execFunction("usecard", adventurer->getName() + " " + card->getName());
      return true;
    }
  }
  return false;
}
void MProcessor::getSwimAreas(MArea* area, std::vector<std::string>& result, int level) {
  level ++;
  std::list<MArea*> neighbors = area->getDirectActiveNeighbors();
  for(std::list<MArea*>::iterator it=neighbors.begin(); it != neighbors.end(); it ++) {
    if((*it)->getFloodLevel() >= 1 && std::find(result.begin(), result.end(), (*it)->getName()) == result.end()) {
	  result.push_back((*it)->getName());
	  getSwimAreas(*it, result, level);
	}
  }
  if(level == 1) {
	std::vector<std::string>::iterator it = std::find(result.begin(), result.end(), area->getName());
	if(it != result.end()) {
	  result.erase(it);
	}
  }
}
std::vector<std::string> MProcessor::getAvailableActions(MAdventurer* adventurer) {
  std::vector<std::string> actions;
  std::list<MArea*> neighbors;

  actions.push_back("skip");

  if(adventurer->getName() == "explorer") {
	neighbors = adventurer->getArea()->getAllActiveNeighbors();
  }
  else {
	neighbors = adventurer->getArea()->getDirectActiveNeighbors();
  }
  if(neighbors.size() > 0) actions.push_back("move");

  if(adventurer->getArea()->getFloodLevel() == 1) {
    actions.push_back("abfluss");
  }
  else {
    for(std::list<MArea*>::iterator it=neighbors.begin(); it != neighbors.end(); it++) {
	  if((*it)->getFloodLevel() == 1) {
	    actions.push_back("abfluss");
	     if(adventurer->getName() == "diver") {
           actions.push_back("swim");
        }
	    break;
	  }
	}
  }

  if(adventurer->getName() == "liaison") actions.push_back("handover");
  else {
    for(int i=0; i<activeAdventurers.size(); i++) {
      if(adventurers[activeAdventurers[i]] == adventurer) continue;
	  if(((MAdventurer*)adventurers[activeAdventurers[i]])->getArea() == adventurer->getArea()) {
	    actions.push_back("handover");
	    break;
	  }
    }
  }

  for(moi moit = artifacts.begin(); moit != artifacts.end(); moit ++) {
    if(adventurer->getArtifactCards(moit->first).size() >= 4) {
	  actions.push_back("getartifact");
	  break;
	}
  }

  if(adventurer->getName() == "navigator") actions.push_back("moveother");
  if(adventurer->getName() == "pilot") {
    if(std::find(usedActions.begin(), usedActions.end(), "fly") == usedActions.end()) {
      actions.push_back("fly");
	}
  }

  return actions;
}
std::string MProcessor::getActionParams(MAdventurer* adventurer, std::string action) {
  std::vector<std::string> params;
  std::list<MArea*> neighbors;
  std::string result = "";
  std::vector<MCard*> cards;

  switch(actionsSwitches[action]) {
  case ACT_MOVE:
	if(adventurer->getName() == "explorer") {
	  neighbors = adventurer->getArea()->getAllActiveNeighbors();
	}
	else {
	  neighbors = adventurer->getArea()->getDirectActiveNeighbors();
	}
	for(std::list<MArea*>::iterator it = neighbors.begin(); it != neighbors.end(); it++) {
	  params.push_back((*it)->getName());
	}
	break;
  case ACT_ABFLUSS:
    if(adventurer->getArea()->getFloodLevel() == 1) {
      params.push_back(adventurer->getArea()->getName());
    }
    else {
	  if(adventurer->getName() == "explorer") {
	    neighbors = adventurer->getArea()->getAllActiveNeighbors();
	  }
	  else {
	    neighbors = adventurer->getArea()->getDirectActiveNeighbors();
	  }
      for(std::list<MArea*>::iterator it=neighbors.begin(); it != neighbors.end(); it++) {
	    if((*it)->getFloodLevel() == 1) {
	      params.push_back((*it)->getName());
		}
	  }
	}
	if(adventurer->getName() == "engineer" && params.size() > 1) {
      result = ui->select(params);
      if(ui->askQuestion("Select more?")) {
        params.erase(std::find(params.begin(), params.end(), result));
        result += " " + ui->select(params);
      }
      return result;
    }
	break;
  case ACT_SKIP:
  case ACT_EXTRACT:
    return "";
  case ACT_HANDOVER:
    if(adventurer->getName() == "liaison") {
	  for(int i=0; i<activeAdventurers.size(); i++) {
	    if(adventurers[activeAdventurers[i]] == adventurer) continue;
		params.push_back(activeAdventurers[i]);
	  }
	}
	else {
	  for(int i=0; i<activeAdventurers.size(); i++) {
        if(adventurers[activeAdventurers[i]] == adventurer) continue;
	    if(((MAdventurer*)adventurers[activeAdventurers[i]])->getArea() == adventurer->getArea()) {
	      params.push_back(activeAdventurers[i]);
	    }
      }
	}
	result = ui->select(params);
	params.clear();
    cards = adventurer->getAllCards();
    for(int i=0; i<cards.size(); i++) {
      params.push_back(cards[i]->getName());
    }
    result += " " + ui->select(params);
    return result;
  case ACT_GETARTIFACT:
    for(moi moit = artifacts.begin(); moit != artifacts.end(); moit ++) {
      if(adventurer->getArtifactCards(moit->first).size() >= 4) {
	    params.push_back(moit->first);
	    break;
	  }
    }
	break;
  case ACT_FLY:
    for(moi moit = areas.begin(); moit != areas.end(); moit ++) {
	  if(moit->first != adventurer->getArea()->getName() && ((MArea*)moit->second)->getFloodLevel() < 2) {
	    params.push_back(moit->first);
	  }
	}
    break;
  case ACT_MOVEOTHER:
    for(int i=0; i<activeAdventurers.size(); i++) {
	  if(adventurers[activeAdventurers[i]] == adventurer) continue;
	  params.push_back(activeAdventurers[i]);
	}
	result = ui->select(params);
	params.clear();
	neighbors = ((MAdventurer*)adventurers[result])->getArea()->getDirectActiveNeighbors();
	for(std::list<MArea*>::iterator it = neighbors.begin(); it != neighbors.end(); it++) {
	  params.push_back((*it)->getName());
	}
	result += " " + ui->select(params);
	return result;
  case ACT_SWIM:
    getSwimAreas(adventurer->getArea(), params);
    break;
  default:
    std::cout<<"  Unknown action"<<std::endl;
    return "";
  }

  return ui->select(params);
}
void MProcessor::run() {
  MAdventurer* adventurer;
  MCard* card;
  std::vector<MCard*> cards;
  std::string action;
  std::string param;

  int i = 0;
  int steps = 2;

  std::cout<<"Run====================================="<<std::endl;

  while(!looseCheck() && i < steps) {
    for(int i=0; i<activeAdventurers.size(); i++) {
      adventurer = (MAdventurer*)adventurers[activeAdventurers[i]];
	  usedActions.clear();
	  std::cout<<"[Adventurer] "<<adventurer->getName()<<" 3 actions"<<std::endl;
	  std::cout<<"  "<<adventurer->getArea()->getName()<<std::endl;
	  for(int j=0; j < 3; j++) {
        std::cout<<"Select action:"<<std::endl;
	    action = ui->select(getAvailableActions(adventurer));
	    param = getActionParams(adventurer, action);
	    execFunction(action, adventurer->getName() + " " + param);
	    usedActions.push_back(action);
	  }
	  std::cout<<"[Adventurer] "<<adventurer->getName()<<" get 2 item cards"<<std::endl;
	  for(int j=0; j<2; j++) {
        execFunction("getitemcard", adventurers[activeAdventurers[j]]->getName());
		while(adventurer->getCardsNumber() > CARDS_LIMIT) {
		  if(tryMomentCard(adventurer)) continue;
		  std::cout<<"Select card to discard:"<<std::endl;
		  card = ui->selectCard(adventurer->getAllCards());
		  execFunction("discard", adventurer->getName() + " " + card->getName());
		}
	  }
	  std::cout<<"[Adventurer] "<<adventurer->getName()<<" get 2 flood cards"<<std::endl;
	  for(int j=0; j<2; j++) {
	    execFunction("getfloodcard", adventurer->getName());
	  }
    }

    i ++;
  }
}
