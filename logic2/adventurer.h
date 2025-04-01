#ifndef adventurerH
#define adventurerH

#include <vector>
#include "object.h"

class MCard;
class MArtifact;
class MArea;

class MAdventurer: public MObject {
private:
  std::string description;
  std::string startArea;
  std::vector<MArtifact*> artifacts;
  std::vector<MCard*> cards;
  MArea* area;
  std::string color;
  void initDescription();
public:
  MAdventurer();
  MAdventurer(const std::string& _name);
  MAdventurer(const std::string& _name, const std::string& _startArea);
  ~MAdventurer();
  void addArtifact(MArtifact* artifact);
  void addCard(MCard* card);
  void removeCard(MCard* card);
  void removeAllCards();
  void removeAllArtifacts();
  void handOver(MCard* card, MAdventurer* adventurer);
  int getCardsNumber();
  std::string getStartArea();
  void setStartArea(std::string _startArea);
  void setArea(MArea* _area);
  MArea* getArea();
  std::vector<MCard*> getAllCards();
  MCard* getCardByNumber(int number);
  //std::vector<MCard*> getMomentCards();
  std::vector<MCard*> getArtifactCards(const std::string& name="");
  bool canUseDiagonal();
  bool hasCard(const std::string& cardName);
  std::string getDescription();
};

#endif
