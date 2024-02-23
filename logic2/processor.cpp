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

  std::cout<<"clear collected artifacts"<<std::endl;
  collectedArtifacts.clear();

  std::cout<<"clear adventurer items, artifacts, positions"<<std::endl;
  MAdventurer* adv;
  for(moi moit = adventurers.begin(); moit != adventurers.end(); moit++) {
    adv = (MAdventurer*)moit->second;
	adv->removeAllCards();
	adv->removeAllArtifacts();
	adv->setArea(nullptr);
  }

  std::cout<<"random place areas, remove flood on areas"<<std::endl;
  initAreas();

  std::cout<<"chose random adventurers, set them on their start areas"<<std::endl;
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

  std::cout<<"random item deck and flood deck"<<std::endl;
  initDecks();

  std::cout<<"flood areas by get flood cards"<<std::endl;
  for(int i=0; i<startFloods; i++) {
    execFunction("getfloodcard", "");
  }

  std::cout<<"get two cards by each adventurer"<<std::endl;
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
  if(argsLessLimit(1)) return;
  MArea* area = findArea(vargs[0]);
  if(!area) return;
  area->abfluss();
  std::cout<<"Abluss: "<<vargs[0]<<std::endl;
}
void MProcessor::flood() {
  if(argsLessLimit(1)) return;
  MArea* area = findArea(vargs[0]);
  MAdventurer* adventurer;
  if(!area) return;
  area->flood();
  if(area->getFloodLevel() >= 2) {
    for(int i=0; i<activeAdventurers.size(); i++) {
      adventurer = findAdventurer(activeAdventurers[i]);
      if(!adventurer) return;
      if(adventurer->getArea() == area) {
        std::list<MArea*> near = area->getNeighbors();
        execFunction("move", adventurer->getName() + " " + (*near.begin())->getName());
      }
    }
    area->removeAllNeighbors();
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
  std::cout<<"Handover: from"<<vargs[0]<<" to "<<vargs[1]<<" card "<<vargs[2]<<std::endl;
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
  std::string name;
  char buff[4];
  for(int i=0; i<number; i++) {
    name = _name;
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

  adventurers.insert(std::pair<std::string, MObject*>("explorer", new MAdventurer("explorer", "gate")));//can diag move or abfluss
  adventurers.insert(std::pair<std::string, MObject*>("pilot", new MAdventurer("pilot", "plato")));//one time can move anywhere
  adventurers.insert(std::pair<std::string, MObject*>("engineer", new MAdventurer("engineer", "factory")));//can abluss one or two areas
  adventurers.insert(std::pair<std::string, MObject*>("liaison", new MAdventurer("liaison", "plato")));//can handover on any distance
  adventurers.insert(std::pair<std::string, MObject*>("navigator", new MAdventurer("navigator", "castle")));//can move other adventurer on one or two areas
  adventurers.insert(std::pair<std::string, MObject*>("diver", new MAdventurer("diver", "lake")));//can move on any number abfluss or none areas

  artifacts.insert(std::pair<std::string, MObject*>("crystall", new MArtifact("crystall", "gate", "house")));
  artifacts.insert(std::pair<std::string, MObject*>("sphere", new MArtifact("sphere", "monastary", "cavern")));
  artifacts.insert(std::pair<std::string, MObject*>("cube", new MArtifact("cube", "plato", "port")));
  artifacts.insert(std::pair<std::string, MObject*>("key", new MArtifact("key", "hole", "factory")));

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
    x = int(num / rows);
    y = num - (int(num / rows)) * rows;
    area->setIndex(x, y);
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
}
void MProcessor::initDecks() {
  randStartDeck(itemCards, itemDeck);
  randStartDeck(floodCards, floodDeck);
}
MProcessor::MProcessor():extractionArea("plato") {
  struct timespec tm;
  clock_gettime(CLOCK_REALTIME, &tm);
  rng.seed(tm.tv_nsec);
  intitMaps();
  ui = new MUI;
}
bool MProcessor::looseCheck() {
  //some artifact areas flood but artifact not get
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
	if(num >= 2) return true;
  }
  //extraction area flood
  area = (MArea*)areas[extractionArea];
  if(area->getFloodLevel() >= 2) return true;
  //max flood level reach
  if(floodLevel > 5) return true;
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
bool MProcessor::useMomentCard(MAdventurer* adventurer) {
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
void MProcessor::run() {
  MAdventurer* adventurer;
  MCard* card;
  std::vector<MCard*> cards;
  while(!looseCheck()) {
    for(int i=0; i<activeAdventurers.size(); i++) {
      adventurer = (MAdventurer*)adventurers[activeAdventurers[i]];
	  //three actions by turn
	  //for(int j=0; j < 3; j++) {
	  //  action = selectAction(adventurer->getAvailableActions());
	  //  param = selectActionParams(adventurer, action);
	  //  execFunction(action, adventurer->getName() + " " + action + " " + param);
	  //}
	  //get 2 item cards
	  for(int j=0; j<2; j++) {
        execFunction("getitemcard", adventurers[activeAdventurers[j]]->getName());
		while(adventurer->getCardsNumber() > CARDS_LIMIT) {
		  if(useMomentCard(adventurer)) continue;
		  card = ui->selectCard(adventurer->getAllCards());
		  execFunction("discard", adventurer->getName() + " " + card->getName());
		}
	  }
	  //get 2 flood cards
	  for(int j=0; j<2; j++) {
	    execFunction("getfloodcard", adventurer->getName());
	  }
    }
  }
}
