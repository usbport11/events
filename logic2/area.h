#ifndef areaH
#define areaH

#include "object.h"
#include <list>

class MArea: public MObject {
private:
  int index[2];
  std::list<MArea*> neighbors;
  bool isDiagonal(MArea* area);
public:
  MArea();
  MArea(const std::string& _name);
  MArea(const std::string& _name, int x, int y);
  ~MArea();
  int* getIndex();
  std::list<MArea*> getNeighbors(bool all);
  void addNeighbor(MArea* area);
  void removeAllNeighbors();
  void removeNeighbor(MArea* area);
  void removeNeighbor(std::string name);
};

#endif
