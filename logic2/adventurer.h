#ifndef adventurerH
#define adventurerH

#include <vector>
#include "object.h"

class MCard;
class MArtifact;
class MArea;

class MAdventurer: public MObject {
private:
  std::string specification;
  std::vector<MArtifact*> artifacts;
  std::vector<MCard*> cards;
  MArea* area;
public:
  MAdventurer();
  MAdventurer(const std::string& _name);
  MAdventurer(const std::string& _name, MArea* _area);
  ~MAdventurer();
  void addArtifact(MArtifact* artifact);
  void addCard(MCard* card);
  void removeCard(MCard* card);
  void move(MArea* _area);
  void handOver(MCard* card, MAdventurer* adventurer);
  int getCardsNumber();
};

#endif
