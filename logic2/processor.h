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
  typedef bool (MProcessor::*bptr)();
  typedef std::map<std::string, MObject*>::iterator moi;

  float floodLevel;
  int adventurerNumber;
  MCard* lastItemCard;

  std::mt19937 rng;
  std::uniform_int_distribution<int> distribute;

  std::map<std::string, bptr> functions;
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
  void parseArgs(const std::string& _sargs);
  bool call(const std::string& name);

  MObject* findObject(std::map<std::string, MObject*>& objects, const std::string& name);
  MArea* findArea(const std::string& name);
  MAdventurer* findAdventurer(const std::string& name);
  MCard* findItemCard(const std::string& name);
  MCard* findFloodCard(const std::string& name);
  MArtifact* findActifact(const std::string& name);

  void randStartDeck(std::map<std::string, MObject*> cards, std::deque<std::string>& deck);
  void randDeck(std::deque<std::string>& deck);
  void moveDeck(std::deque<std::string>& src, std::deque<std::string>& dest);

  bool start();
  bool move();
  bool abfluss();
  bool flood();
  bool skip();
  bool handOver();
  bool getItemCard();
  bool getFloodCard();
  bool discard();
  bool useCard();
  bool getArtifact();
  bool fly();
  bool moveOther();
  bool swim();
  bool extract();

  void createItemCards(const std::string& _name, const std::string& type, int number);
  bool initAreas();
  bool looseCheck();
  bool tryMomentCard(MAdventurer* adventurer);
  std::vector<std::string> getAvailableActions(MAdventurer* adventurer);
  std::string getActionParams(MAdventurer* adventurer, std::string action);
  void getSwimAreas(MArea* area, std::vector<std::string>& result, int level=0);

  void createConsole();

public:
  MProcessor();
  ~MProcessor();
  bool execFunction(const std::string& name, const std::string& _sargs="");
  bool run();
};

#endif
