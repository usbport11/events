#ifndef processorH
#define processorH

#include <map>
#include <vector>
#include <deque>
#include <string>
#include <random>

#define CARDS_LIMIT 5

class MObject;
class MArea;
class MAdventurer;
class MCard;
class MArtifact;
class MUI;

class MProcessor {
private:
  typedef void (MProcessor::*pt2)();
  typedef std::map<std::string, MObject*>::iterator moi;

  float floodLevel;
  int adventurerNumber;
  MCard* lastItemCard;

  std::mt19937 rng;
  std::uniform_int_distribution<int> distribute;

  std::map<std::string, pt2> m;
  std::vector<std::string> vargs;
  std::string sargs;

  MUI* ui;

  const std::string extractionArea;
  std::map<std::string, MObject*> areas;
  std::map<std::string, MObject*> adventurers;
  std::map<std::string, MObject*> itemCards;
  std::map<std::string, MObject*> floodCards;
  std::map<std::string, MObject*> artifacts;
  std::deque<std::string> itemDeck;
  std::deque<std::string> itemDropDeck;
  std::deque<std::string> floodDeck;
  std::deque<std::string> floodDropDeck;
  std::deque<std::string> floodOutDeck;
  std::vector<std::string> activeAdventurers;
  std::vector<std::string> collectedArtifacts;
  std::vector<std::string> usedActions;
  std::map<std::string, int> actionsSwitches;

  bool argsLessLimit(int num);
  MObject* findObject(std::map<std::string, MObject*>& objects, const std::string& name);
  MArea* findArea(const std::string& name);
  MAdventurer* findAdventurer(const std::string& name);
  MCard* findItemCard(const std::string& name);
  MCard* findFloodCard(const std::string& name);
  MArtifact* findActifact(const std::string& name);
  void randStartDeck(std::map<std::string, MObject*> cards, std::deque<std::string>& deck);
  void randDeck(std::deque<std::string>& deck);
  void moveDeck(std::deque<std::string>& src, std::deque<std::string>& dest);
  void start();
  void move();
  void abfluss();
  void flood();
  void skip();
  void handOver();
  void getItemCard();
  void getFloodCard();
  void discard();
  void useCard();
  void getArtifact();
  void fly();
  void moveOther();
  void swim();
  void extract();
  void parseArgs(const std::string& _sargs);
  void call(const std::string& name);
  void createItemCards(const std::string& _name, const std::string& type, int number);
  void intitMaps();
  void initAreas();
  void initDecks();
  bool looseCheck();
  bool tryMomentCard(MAdventurer* adventurer);
  std::vector<std::string> getAvailableActions(MAdventurer* adventurer);
  std::string getActionParams(MAdventurer* adventurer, std::string action);
  void getSwimAreas(MArea* area, std::vector<std::string>& result, int level=0);
public:
  MProcessor();
  ~MProcessor();
  bool execFunction(const std::string& name, const std::string& _sargs="");
  void run();
};

#endif
