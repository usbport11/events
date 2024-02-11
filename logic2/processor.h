#ifndef processorH
#define processorH

#include <map>
#include <vector>
#include <string>

class MObject;
class MArea;
class MAdventurer;

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

  bool argsLessLimit(int num);
  MArea* getArea(const std::string& name);
  MAdventurer* getAdventurer(const std::string& name);
  void print();
  void move();
  void abfluss();
  void flood();
  void skip();
  void handOver();
  void getCard();
  void drawCard();
  void useCard();
  void getArtifact();
  void parseArgs(const std::string& _sargs);
  void call(const std::string& name);
  void intitMaps();
public:
  MProcessor();
  ~MProcessor();
  bool execFunction(const std::string& name, const std::string& _sargs="");
};

#endif
