#include "area.h"

#include <math.h>
#include <vector>

MArea::MArea():MObject() {
}
MArea::MArea(const std::string& _name):MObject(_name) {
}
MArea::MArea(const std::string& _name, int x, int y):MArea(_name) {
  index[0] = x;
  index[1] = y;
}
MArea::~MArea() {
  neighbors.clear();
}
bool MArea::isDiagonal(MArea* area) {
  return abs(index[0] - area->getIndex()[0]) == abs(index[1] - area->getIndex()[1]);
}
int* MArea::getIndex() {
  return (int*)index;
}
std::list<MArea*> MArea::getNeighbors(bool all=false) {
  if(all) {
    return neighbors;
  }
  else {
    std::list<MArea*> dn = neighbors;
    for(std::list<MArea*>::iterator it = neighbors.begin(); it != neighbors.end(); it++) {
      if(!isDiagonal(*it)) dn.push_back(*it);
    }
    return dn;
  }
}
void MArea::addNeighbor(MArea* area) {
  neighbors.push_back(area);
}
void MArea::removeAllNeighbors() {
  neighbors.clear();
}
void MArea::removeNeighbor(MArea* area) {
  for(std::list<MArea*>::iterator it = neighbors.begin(); it != neighbors.end(); it++) {
    if(area == *it) neighbors.erase(it);
  }
}
void MArea::removeNeighbor(std::string name) {
  for(std::list<MArea*>::iterator it = neighbors.begin(); it != neighbors.end(); it++) {
    if((*it)->getName() == name) neighbors.erase(it);
  }
}
