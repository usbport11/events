#ifndef processorH
#define processorH

#include <map>
#include <vector>
#include <string>

#define CARDS_LIMIT 5

class MObject;
class MArea;
class MAdventurer;
class MCard;

class MProcessor {
private:
  typedef void (MProcessor::*pt2)();
  typedef std::map<std::string, pt2>::iterator mi;
  typedef std::map<std::string, MObject*>::iterator moi;

  std::map<std::string, pt2> m;
  std::vector<std::string> vargs;
  std::string sargs;

  std::map<std::string, MObject*> areas;
  std::map<std::string, MObject*> adventurers;
  std::map<std::string, MObject*> itemCards;
  std::map<std::string, MObject*> floodCards;
  std::vector<MObject*> itemDeck;
  std::vector<MObject*> floodDeck;

  bool argsLessLimit(int num);
  MArea* findArea(const std::string& name);
  MAdventurer* findAdventurer(const std::string& name);
  MCard* findItemCard(const std::string& name);
  MCard* findFloodCard(const std::string& name);
  void start();
  void move();
  void abfluss();
  void flood();
  void skip();
  void handOver();
  void getItemCard();
  void getFloodCard();
  void drawCard();
  void useCard();
  void getArtifact();
  void parseArgs(const std::string& _sargs);
  void call(const std::string& name);
  void intitMaps();
  void initAreas();
public:
  MProcessor();
  ~MProcessor();
  bool execFunction(const std::string& name, const std::string& _sargs="");
};

#endif
