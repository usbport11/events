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

class MProcessor {
private:
  typedef void (MProcessor::*pt2)();
  typedef std::map<std::string, pt2>::iterator mi;
  typedef std::map<std::string, MObject*>::iterator moi;

  float totalFloodLevel;

  std::mt19937 rng;
  std::uniform_int_distribution<int> distribute;

  std::map<std::string, pt2> m;
  std::vector<std::string> vargs;
  std::string sargs;

  std::map<std::string, MObject*> areas;
  std::map<std::string, MObject*> adventurers;
  std::map<std::string, MObject*> itemCards;
  std::map<std::string, MObject*> floodCards;
  std::map<std::string, MObject*> artifacts;
  std::deque<std::string> itemDeck;
  std::deque<std::string> itemDropDeck;
  std::deque<std::string> floodDeck;
  std::deque<std::string> floodDropDeck;
  std::vector<std::string> activeAdventurers;

  bool argsLessLimit(int num);
  MArea* findArea(const std::string& name);
  MAdventurer* findAdventurer(const std::string& name);
  MCard* findItemCard(const std::string& name);
  MCard* findFloodCard(const std::string& name);
  void randDeck(std::map<std::string, MObject*> cards, std::deque<std::string>& deck);
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
  void extract();
  void parseArgs(const std::string& _sargs);
  void call(const std::string& name);
  void fillItemCards(const std::string& _name, const std::string& type, int number);
  void intitMaps();
  void initAreas();
  void initDecks();
public:
  MProcessor();
  ~MProcessor();
  bool execFunction(const std::string& name, const std::string& _sargs="");
};

#endif
