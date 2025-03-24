#include "processor.h"

#include "area.h"
#include "adventurer.h"
#include "card.h"
#include "artifact.h"
#include "ui.h"

#include <chrono>
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
#define ACT_ENDTURN 110

bool MProcessor::argsLessLimit(int num) {
  return (vargs.size() < num);
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
bool MProcessor::call(const std::string& name) {
  return (*this.*functions[name])();
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
    //dest.push_back(src.front());
    dest.push_front(src.front());
    src.pop_front();
  }
}
bool MProcessor::start() {
  adventureStarted = false;
  floodLevel = 2.1;
  lastItemCard = nullptr;
  currentActionNumber = 0;

  int startFloods = 6;
  int rnd, num = 0;

  if(areas.size() != floodCards.size()) {
    std::cout<<"Flood cards not equal areas"<<std::endl;
    return false;
  }

  std::cout<<"Clear collected artifacts"<<std::endl;
  collectedArtifacts.clear();

  std::cout<<"Clear adventurer items, artifacts, positions"<<std::endl;
  MAdventurer* adv;
  MArea* area;
  for(moi moit = adventurers.begin(); moit != adventurers.end(); moit++) {
    adv = (MAdventurer*)moit->second;
	adv->removeAllCards();
	adv->removeAllArtifacts();
	adv->setArea(nullptr);
  }

  std::cout<<"Random place areas, remove flood on areas"<<std::endl;
  if(!initAreas()) return false;

  std::cout<<"Chose random adventurers, set them on their start areas"<<std::endl;
  activeAdventurers.clear();
  std::vector<std::string> rndBaseStr;
  std::vector<int> rndBaseInt;
  for(moi moit=adventurers.begin(); moit!=adventurers.end(); moit++) {
    rndBaseStr.push_back(moit->first);
  }
  while(!rndBaseStr.empty() && num < adventurersNumber) {
    distribute = std::uniform_int_distribution<int>(0, rndBaseStr.size()-1);
    rnd = distribute(rng);
    adv = findAdventurer(rndBaseStr[rnd]);
    if(!adv) {
      std::cout<<"Adventurer not found"<<std::endl;
      return false;
    }
    area = findArea(adv->getStartArea());
    if(!area) {
      std::cout<<"Base area for adventurer not found"<<std::endl;
      return false;
    }
    if(!execFunction("move", adv->getName() + " " + area->getName())) return false;
    //adv->setArea(area);
    //std::cout << " Adventurer added: " << adv->getName() << " Main area: " << adv->getStartArea() << std::endl;
    activeAdventurers.push_back(adv->getName());
    std::cout << " Adventurer " << adv->getName() << " added to active adventurers" << std::endl;
	rndBaseStr.erase(rndBaseStr.begin() + rnd);
	num ++;
  }
  currentAdventurer = findAdventurer(activeAdventurers[0]);
  if (!currentAdventurer) return false;

  std::cout<<"Random item deck and flood deck"<<std::endl;
  itemDropDeck.clear();
  floodDropDeck.clear();
  floodOutDeck.clear();
  randStartDeck(itemCards, itemDeck);
  randStartDeck(floodCards, floodDeck);

  std::cout<<"Flood areas by get flood cards"<<std::endl;
  for(int i=0; i<startFloods; i++) {
    if(!execFunction("getfloodcard", "")) return false;
  }

  //at start there is peculiarity: flood card returened to deck
  std::cout<<"Get two cards by each adventurer"<<std::endl;
  for(int i=0; i< adventurersNumber; i++) {
    if(!execFunction("getitemcard", activeAdventurers[i])) return false;
    if(!execFunction("getitemcard", activeAdventurers[i])) return false;
  }

  usedActions.clear();
  adventureStarted = true;

  return true;
}

bool MProcessor::flood() {
  if(argsLessLimit(1)) return false;
  MArea* area = findArea(vargs[0]);
  MAdventurer* adventurer;
  std::list<MArea*> neighbors;
  if(!area) return false;
  area->flood();
  if(area->getFloodLevel() >= 2) {
    for(int i=0; i<activeAdventurers.size(); i++) {
      adventurer = findAdventurer(activeAdventurers[i]);
      if(!adventurer) return false;
      if(adventurer->getArea() == area) {
        std::cout<<" "<<adventurer->getName()<<" stay on drown area"<<std::endl;
        neighbors = area->getAllActiveNeighbors();
		if(neighbors.empty()) {
          std::cout<<" "<<adventurer->getName()<<" drown"<<std::endl;
		  activeAdventurers.erase(activeAdventurers.begin() + i);
		}
        if(!execFunction("move", adventurer->getName() + " " + (*neighbors.begin())->getName())) return false;
      }
    }
    std::cout << "  Area " << area->getName() << " fully flooded. Moved to flood out deck" << std::endl;
    floodOutDeck.push_front(floodDeck.front());
  }
  else {
    std::cout << "  Area " << area->getName() << " partly flooded. Moved to flood drop deck" << std::endl;
    floodDropDeck.push_front(floodDeck.front());
  }
  floodDeck.pop_front();
  std::cout<<"Flood: "<<vargs[0]<<std::endl;
  return true;
}
/*
bool MProcessor::skip() {
  if(argsLessLimit(1)) return false;
  std::cout<<"Skip: "<<vargs[0]<<std::endl;
  return true;
}
*/
bool MProcessor::getItemCard() {
  int rnd;
  if(argsLessLimit(1)) return false;
  MAdventurer* adventurer = findAdventurer(vargs[0]);
  if(!adventurer) return false;

  if(itemDeck.empty()) {
    randDeck(itemDropDeck);
    moveDeck(itemDropDeck, itemDeck);
  }

  //get card
  MCard* card = findItemCard(itemDeck.front());
  if(!card) return false;
  //pop card
  itemDeck.pop_front();
  std::cout<<"Get item card "<<card->getName()<<" by "<<vargs[0]<<std::endl;

  if(card->getType() == "item" || card->getType() == "artifact") {
    adventurer->addCard(card);
  }

  if(card->getType() == "flood") {
      if (adventureStarted) {
          floodLevel += 0.5;
          std::cout << "  Flood level increase: " << floodLevel << std::endl;
          if (lastItemCard) {
              if (lastItemCard->getType() != "flood") {
                  randDeck(floodDropDeck);
                  moveDeck(floodDropDeck, floodDeck);
              }
          }
          else {
              randDeck(floodDropDeck);
              moveDeck(floodDropDeck, floodDeck);
          }
          itemDropDeck.push_front(itemDeck.front());
      }
      else {
          std::cout << "  Found flood card at start game"<< std::endl;
          std::cout << "  Get next card" << std::endl;
          if (!execFunction("getitemcard", vargs[0])) return false;
          std::cout << "  Return flood card (" << card->getName() << ") randomly back to items card" << std::endl;
          itemDeck.push_back(card->getName());
          randDeck(itemDeck);
      }
  }

  lastItemCard = card;
  return true;
}
bool MProcessor::getFloodCard() {
  if(floodDeck.empty()) {
    randDeck(floodDropDeck);
    moveDeck(floodDropDeck, floodDeck);
  }
  std::cout<<"Get flood card"<<std::endl;
  return(execFunction("flood", floodDeck.front()));
}
bool MProcessor::discard() {
  if(argsLessLimit(2)) return false;
  MAdventurer* adventurer = findAdventurer(vargs[0]);
  MCard* card = findItemCard(vargs[1]);
  if(!adventurer) return false;
  if(!card) return false;
  itemDropDeck.push_front(card->getName());
  adventurer->removeCard(card);
  std::cout<<"Discard: "<<vargs[0]<<" card "<<vargs[1]<<std::endl;
  return true;
}
bool MProcessor::useCard() {
  if(argsLessLimit(2)) return false;
  std::cout<<"Use card: "<<vargs[1]<<" by "<<vargs[0]<<std::endl;

  MAdventurer* adventurer = findAdventurer(vargs[0]);
  MCard* card = findItemCard(vargs[1]);
  if(!adventurer) return false;
  if(!card) return false;
  if(card->getType() == "item") {
    if(card->getName().find("helicopter") != std::string::npos) {
      if (argsLessLimit(4)) return false;
      if (!execFunction("move", vargs[2] + " " + vargs[3])) return false;
    }
    if (card->getName().find("sandbag") != std::string::npos) {
      if (argsLessLimit(3)) return false;
      if (!execFunction("abfluss", vargs[2])) return false;
    }
  }
  else {
    return false;
  }
  itemDropDeck.push_front(card->getName());
  adventurer->removeCard(card);
  return true;
}
//basic actions================================================================
bool MProcessor::handOver() {
    if (argsLessLimit(3)) return false;
    MAdventurer* srcAdventurer = findAdventurer(vargs[0]);
    MAdventurer* dstAdventurer = findAdventurer(vargs[1]);
    MCard* card = findItemCard(vargs[2]);
    if (!srcAdventurer || !dstAdventurer) return false;
    if (!card) return false;
    dstAdventurer->addCard(card);
    std::cout << "Handover " << vargs[2] << " from " << vargs[0] << " to " << vargs[1] << std::endl;
    usedActions.push_back("handover");
    return true;
}
bool MProcessor::move() {
    if (argsLessLimit(2)) return false;
    MAdventurer* adventurer = findAdventurer(vargs[0]);
    MArea* area = findArea(vargs[1]);
    if (!adventurer) return false;
    if (!area) return false;
    adventurer->setArea(area);
    std::cout << "Move: " << vargs[0] << " to the " << vargs[1] << std::endl;
    usedActions.push_back("move");
    return true;
}
bool MProcessor::abfluss() {
    if (argsLessLimit(2)) return false;
    MAdventurer* adventurer = findAdventurer(vargs[0]);
    MArea* area1 = findArea(vargs[1]);
    MArea* area2;
    if (!adventurer) return false;
    if (!area1) return false;
    area1->abfluss();
    std::cout << "Abluss: " << vargs[1] << std::endl;
    if (vargs.size() == 3) {
        area2 = findArea(vargs[2]);
        if (!area2) return false;
        std::cout << "Abluss: " << vargs[2] << std::endl;
    }
    usedActions.push_back("abfluss");
    return true;
}
bool MProcessor::getArtifact() {
  if(argsLessLimit(2)) return false;
  MAdventurer* adventurer = findAdventurer(vargs[0]);
  MArtifact* artifact = findActifact(vargs[1]);
  if(!adventurer) return false;
  if(!artifact) return false;
  adventurer->addArtifact(artifact);
  collectedArtifacts.push_back(vargs[1]);
  std::vector<MCard*> cards = adventurer->getArtifactCards(vargs[1]);
  for(int i=0; i<4 ;i++) {
    if(!execFunction("discard", adventurer->getName() + " " + cards[i]->getName())) return false;
  }
  std::cout<<"Get artifact: "<<vargs[0]<<" get "<<vargs[1]<<std::endl;
  usedActions.push_back("getartifact");
  return true;
}
bool MProcessor::fly() {
  if(argsLessLimit(2)) return false;
  MAdventurer* adventurer = findAdventurer(vargs[0]);
  MArea* area = findArea(vargs[1]);
  if(!adventurer) return false;
  if(!area) return false;
  adventurer->setArea(area);
  std::cout<<"Fly: "<<vargs[0]<<" to the "<<vargs[1]<<std::endl;
  usedActions.push_back("fly");
  return true;
}
bool MProcessor::moveOther() {
  if(argsLessLimit(3)) return false;
  std::cout<<"Move "<<vargs[1]<<" by "<<vargs[0]<<" to the "<<vargs[2]<<std::endl;
  usedActions.push_back("moveother");
  MAdventurer* adventurer = findAdventurer(vargs[1]);
  MArea* area = findArea(vargs[2]);
  if (!adventurer) return false;
  if (!area) return false;
  adventurer->setArea(area);
  //return execFunction("move", vargs[1] + " " + vargs[2]); //this calculated as additional action - it's wrong
}
bool MProcessor::swim() {
  if(argsLessLimit(2)) return false;
  MAdventurer* adventurer = findAdventurer(vargs[0]);
  MArea* area = findArea(vargs[1]);
  if(!adventurer) return false;
  if(!area) return false;
  adventurer->setArea(area);
  std::cout<<"Swim: "<<vargs[0]<<" to the "<<vargs[1]<<std::endl;
  usedActions.push_back("swim");
  return true;
}
//================================================================
bool MProcessor::extract() {
  std::cout<<"extract!"<<std::endl;
  return true;
}
bool MProcessor::endTurn() {
    if (argsLessLimit(1)) return false;
    MAdventurer* adventurer = findAdventurer(vargs[0]);
    if (!adventurer) return false;
    for (int j = 0; j < 2; j++) {
        if (!execFunction("getitemcard", vargs[0])) return false;
    }
    for (int j = 0; j < (int)floodLevel; j++) {
        if (!execFunction("getfloodcard", vargs[0])) return false;
    }
    std::cout << "End turn by: " << vargs[0] << std::endl;
    changeCurrentAdventurer();
    currentActionNumber = 0;
    std::cout << "Next adventurer: " << currentAdventurer->getName() << " [clear action number]" << std::endl;
    usedActions.clear();
    return true;
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
bool MProcessor::initAreas() {
  const int rows = 6, cols = 6;
  bool areaMap[rows][cols] = {
    {0,0,1,1,0,0},
    {0,1,1,1,1,0},
    {1,1,1,1,1,1},
    {1,1,1,1,1,1},
    {0,1,1,1,1,0},
    {0,0,1,1,0,0}
  };

  int num = 0;
  for(int i=0; i<rows; i++) {
    for(int j=0; j<cols; j++) {
      if(areaMap[i][j]) num ++;
    }
  }
  if(areas.size() < num) {
    std::cout<<"Error: areas size < row * cols"<<std::endl;
    return false;
  }

  int inds[8][2] = {{-1,-1}, {0,-1}, {1,-1}, {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}};
  int* ind;
  MArea* area;
  std::map<int, MArea*> temp;
  std::vector<std::string> rndBase;

  //fill rand base
  //check areas on extraction(1) and artifacts(4 * 2)
  int areaChecks = 0;
  std::vector<std::string> artefactAreas;
  for(moi moit=artifacts.begin(); moit!=artifacts.end(); moit++) {
    artefactAreas.push_back(((MArtifact*)moit->second)->getMainAreas()[0]);
    artefactAreas.push_back(((MArtifact*)moit->second)->getMainAreas()[1]);
  }
  for(moi moit=areas.begin(); moit!=areas.end(); moit++) {
    if(moit->first == extractionArea) {
      areaChecks ++;
    }
    if(std::find(artefactAreas.begin(), artefactAreas.end(), moit->first) != artefactAreas.end()) {
      areaChecks ++;
    }
    rndBase.push_back(moit->first);
  }
  if(areaChecks < 1 + artifacts.size() * 2) {
    return false;
  }

  int rnd;
  int x, y;
  num = 0;
  //rand and init areas
  while(!rndBase.empty()) {
    x = int(num / rows);
    y = num - (int(num / rows)) * rows;
    temp[num] = nullptr;
    if(areaMap[x][y]) {
      distribute = std::uniform_int_distribution<int>(0, rndBase.size()-1);
      rnd = distribute(rng);
      area = (MArea*)areas[rndBase[rnd]];
      area->setIndex(x, y);
	  area->setFloodLevel(0);
      area->removeAllNeighbors();
      rndBase.erase(rndBase.begin() + rnd);
      temp[num] = area;
    }
    num ++;
  }

  //set neighbors
  for(int i=0; i<rows; i++) {
    for(int j=0; j<cols; j++) {
      if(areaMap[i][j]) {
        ind = temp[i * rows  + j]->getIndex();
        std::cout<<" "<<temp[i * rows + j]->getName();
        for(int k=0; k<8; k++) {
          x = ind[0] + inds[k][0];
          y = ind[1] + inds[k][1];
          if((x < 0 || y < 0) || (x >= rows || y >= cols)) continue;
          temp[i * rows + j]->addNeighbor(temp[x * rows + y]);
	    }
      }
	}
	std::cout<<std::endl;
  }

  return true;
}
MProcessor::MProcessor():extractionArea("adventurers_circle") {
  std::chrono::steady_clock::time_point tp = std::chrono::steady_clock::now();
  long nsec = std::chrono::time_point_cast<std::chrono::nanoseconds>(tp).time_since_epoch().count();
  rng.seed(nsec);

  ui = new MUI;

  functions = {{"start", &MProcessor::start},
    {"move", &MProcessor::move},
    {"abfluss", &MProcessor::abfluss},
    {"flood", &MProcessor::flood},
    //{"skip", &MProcessor::skip},
    {"handover", &MProcessor::handOver},
    {"getitemcard", &MProcessor::getItemCard},
    {"getfloodcard", &MProcessor::getFloodCard},
    {"discard", &MProcessor::discard},
    {"usecard", &MProcessor::useCard},
    {"getartifact", &MProcessor::getArtifact},
    {"fly", &MProcessor::fly},
    {"moveother", &MProcessor::moveOther},
    {"swim", &MProcessor::swim},
    {"extract", &MProcessor::extract},
    {"endturn", &MProcessor::endTurn}};

  areas = {{"tidal_castle", new MArea("tidal_castle")},
    {"coral_castle", new MArea("coral_castle")},
    {"treacherous_dunes", new MArea("treacherous_dunes")},
    {"watchtower", new MArea("watchtower")},
    {"scarlet_forest", new MArea("scarlet_forest")},
    {"temple_of_the_sun", new MArea("temple_of_the_sun")},
    {"observatory", new MArea("observatory")},
    {"temple_of_the_moon", new MArea("temple_of_the_moon")},
    {"adventurers_circle", new MArea("adventurers_circle")},
    {"golden_gate", new MArea("golden_gate")},
    {"bronze_gate", new MArea("bronze_gate")},
    {"ghost_rock", new MArea("ghost_rock")},
    {"foggy_marshes", new MArea("foggy_marshes")},
    {"whispering_garden", new MArea("whispering_garden")},
    {"silver_gate", new MArea("silver_gate")},
    {"iron_gate", new MArea("iron_gate")},
    {"bridge_of_the_brave", new MArea("bridge_of_the_brave")},
    {"twilight_hollow", new MArea("twilight_hollow")}, 
    {"abandoned_cliffs", new MArea("abandoned_cliffs")}, 
    {"lost_lagoon", new MArea("lost_lagoon")}, 
    {"fire_cave", new MArea("fire_cave")},
    {"cave_of_shadows", new MArea("cave_of_shadows")},
    {"howling_garden", new MArea("howling_garden")},
    {"copper_gate", new MArea("copper_gate")}};

  adventurers = {{"explorer", new MAdventurer("explorer", "copper_gate")},//can diag move or abfluss - green
    {"pilot", new MAdventurer("pilot", "adventurers_circle")},//one time can move anywhere - blue
    {"engineer", new MAdventurer("engineer", "bronze_gate")},//can abluss one or two areas - red
    {"liaison", new MAdventurer("liaison", "silver_gate")},//can handover on any distance - grey
    {"navigator", new MAdventurer("navigator", "golden_gate")},//can move other adventurer on one or two areas - yellow
    {"diver", new MAdventurer("diver", "iron_gate")}};//can move on any number abfluss or none areas - black

  artifacts = {{"crystal", new MArtifact("crystal", "fire_cave", "cave_of_shadows")},
    {"sphere", new MArtifact("sphere", "temple_of_the_sun", "temple_of_the_moon")},
    {"lion", new MArtifact("lion", "whispering_garden", "howling_garden")},
    {"bowl", new MArtifact("bowl", "tidal_castle", "coral_castle")}};

  //artifact cards
  for(moi moit = artifacts.begin(); moit != artifacts.end(); moit++) {
    createItemCards(moit->first, "artifact", 5);
  }
  //ext cards
  createItemCards("helicopter", "item", 2);//3?
  createItemCards("sandbag", "item", 2);
  createItemCards("flood", "flood", 3);

  //equal to areas number and name
  for(moi moit=areas.begin(); moit != areas.end(); moit++) {
    floodCards[moit->first] = new MCard(moit->first);
  }

  actionsSwitches = {{"move", ACT_MOVE},
    {"abfluss", ACT_ABFLUSS},
    {"skip", ACT_SKIP},
    {"handover", ACT_HANDOVER},
    {"getartifact", ACT_GETARTIFACT},
    {"fly", ACT_FLY},
    {"moveother", ACT_MOVEOTHER},
    {"swim", ACT_SWIM},
    {"extract", ACT_EXTRACT}}; //"endturn", ACT_ENDTURN

  actionsLimit = 3;
  adventurersNumber = 1;
}
MProcessor::~MProcessor() {
    functions.clear();
    vargs.clear();
    sargs.clear();
    for (moi moit = areas.begin(); moit != areas.end(); moit++) {
        if (moit->second) delete moit->second;
    }
    areas.clear();
    for (moi moit = adventurers.begin(); moit != adventurers.end(); moit++) {
        if (moit->second) delete moit->second;
    }
    adventurers.clear();
    for (moi moit = itemCards.begin(); moit != itemCards.end(); moit++) {
        if (moit->second) delete moit->second;
    }
    itemCards.clear();
    for (moi moit = floodCards.begin(); moit != floodCards.end(); moit++) {
        if (moit->second) delete moit->second;
    }
    floodCards.clear();
    for (moi moit = artifacts.begin(); moit != artifacts.end(); moit++) {
        if (moit->second) delete moit->second;
    }
    artifacts.clear();

    if (ui) delete ui;

    itemDeck.clear();
    itemDropDeck.clear();
    floodDeck.clear();
    floodDropDeck.clear();
    floodOutDeck.clear();
    activeAdventurers.clear();
    collectedArtifacts.clear();
    //usedActions.clear();
    actionsSwitches.clear();
}
bool MProcessor::adventureFailed() {
  int num;
  MArtifact* artifact;
  MArea* area;

  if (activeAdventurers.empty()) {
      std::cout << "All adventurer flooded" << std::endl;
      return true;
  }

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
  
  if(floodLevel > 6) {
    std::cout<<"Island totaly flooded"<<std::endl;
    return true;
  }

  return false;
}

void MProcessor::setAdventurersNumber(int number) {
    if (number <= 0 || number > ADVENTURES_LIMIT) adventurersNumber = 1;
    else adventurersNumber = number;
}

bool MProcessor::execFunction(const std::string& name, const std::string& _sargs, bool increase) {
  parseArgs(_sargs);
  std::map<std::string, bptr>::iterator mit = functions.find(name);
  if(mit == functions.end()) {
    std::cout<<"Function ["<<name<<"] not found!"<<std::endl;
    return false;
  }
  if(!call(name)) return false;
  if (adventureStarted && actionsSwitches.find(name) != actionsSwitches.end()) {
      if (increase) {
          currentActionNumber++;
          std::cout << "Increase action number" << std::endl;
      }
      else {
          std::cout << "Increase action number skiped" << std::endl;
      }
  }
  return true;
}
bool MProcessor::tryMomentCard(MAdventurer* adventurer) {
  MCard* card;
  bool used = false;
  std::vector<MCard*> cards = adventurer->getMomentCards();
  if(cards.empty()) return false;
  while(ui->askQuestion("Whould you like to use moment card?") == true) {
    used = true;
	card = ui->selectCard(cards);
    execFunction("usecard", adventurer->getName() + " " + card->getName());
  }
  return used;
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
    //remove self
	std::vector<std::string>::iterator it = std::find(result.begin(), result.end(), area->getName());
	if(it != result.end()) {
	  result.erase(it);
	}
    //remove drown areas
    for(std::vector<std::string>::iterator it=result.begin(); it!=result.end(); it++) {
      if(((MArea*)areas[*it])->getFloodLevel() == 2) {
        result.erase(it);
      }
    }
  }
}

std::map<std::string, MObject*> MProcessor::getArtifacts() {
    return artifacts;
}

std::map<std::string, MObject*> MProcessor::getAreas() {
    return areas;
}

std::map<std::string, MObject*> MProcessor::getAdventurers() {
    return adventurers;
}

std::vector<std::string> MProcessor::getActiveAdventurers() {
    return activeAdventurers;
}

std::deque<std::string> MProcessor::getItemDropDeck() {
    return itemDropDeck;
}

std::deque<std::string> MProcessor::getFloodDropDeck() {
    return floodDropDeck;
}

MAdventurer* MProcessor::getCurrentAdventurer() {
    return currentAdventurer;
}

float MProcessor::getFloodLevel() {
    return floodLevel;
}

void MProcessor::changeCurrentAdventurer() {
    int pos = 0;
    for (int i = 0; i < activeAdventurers.size(); i++) {
        if (activeAdventurers[i] == currentAdventurer->getName()) {
            pos = i + 1;
            break;
        }
    }
    if (pos >= activeAdventurers.size()) pos = 0;
    currentAdventurer = findAdventurer(activeAdventurers[pos]);
    std::cout << "Change current adventurer to: " << currentAdventurer->getName() << std::endl;
}

MArea* MProcessor::getAreaByIndex(int x, int y) {
    MArea* area;
    for (std::map<std::string, MObject*>::iterator it = areas.begin(); it != areas.end(); it++) {
        area = (MArea*)it->second;
        if (area->getIndex()[0] == x && area->getIndex()[1] == y) return area;
    }
    return nullptr;
}

int MProcessor::getCurrentActionNumber() {
    return currentActionNumber;
}

bool MProcessor::actionNumberLimitReached() {
    return (currentActionNumber >= actionsLimit);
}

bool MProcessor::allActiveAdventurersOnArea(const std::string& areaName) {
    for (int i = 0; i < activeAdventurers.size(); i++) {
        if (findAdventurer(activeAdventurers[i])->getArea()->getName() != areaName) {
            return false;
        }
    }
    return true;
}

bool MProcessor::allArifactsCollected() {
    return (collectedArtifacts.size() == 4);
}

std::vector<std::string> MProcessor::getAvailableActions(MAdventurer* adventurer) {
    std::vector<std::string> actions;
    std::list<MArea*> nearAreas;

    actions.push_back("endturn");

    if (adventurer->getName() == "explorer") {
        nearAreas = adventurer->getArea()->getAllActiveNeighbors();
    }
    else {
        nearAreas = adventurer->getArea()->getDirectActiveNeighbors();
    }
    if (nearAreas.size() > 0) actions.push_back("move");

    if (adventurer->getArea()->getFloodLevel() == 1) {
        actions.push_back("abfluss");
    }
    else {
        for (std::list<MArea*>::iterator it = nearAreas.begin(); it != nearAreas.end(); it++) {
            if ((*it)->getFloodLevel() == 1) {
                actions.push_back("abfluss");
                break;
            }
        }
    }

    if (adventurer->getName() == "diver") {
        for (std::list<MArea*>::iterator it = nearAreas.begin(); it != nearAreas.end(); it++) {
            if ((*it)->getFloodLevel() == 1) {
                actions.push_back("swim");
                break;
            }
        }
    }

    if (adventurer->getName() == "liaison") actions.push_back("handover");
    else {
        for (int i = 0; i < activeAdventurers.size(); i++) {
            if (adventurers[activeAdventurers[i]] == adventurer) continue;
            if (((MAdventurer*)adventurers[activeAdventurers[i]])->getArea() == adventurer->getArea()) {
                actions.push_back("handover");
                break;
            }
        }
    }

    for (moi moit = artifacts.begin(); moit != artifacts.end(); moit++) {
        if (adventurer->getArtifactCards(moit->first).size() >= 4) {
            actions.push_back("getartifact");
            break;
        }
    }

    if (adventurer->getName() == "navigator") actions.push_back("moveother");
    if (adventurer->getName() == "pilot") {
        if (std::find(usedActions.begin(), usedActions.end(), "fly") == usedActions.end()) {
            actions.push_back("fly");
        }
    }

    return actions;
}