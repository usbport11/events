#include "area.h"

#include <math.h>
#include <vector>
#include <algorithm>

#include <iostream>

MArea::MArea():MObject() {
  floodLevel = 0;
  index[0] = -1;
  index[1] = -1;
}
MArea::MArea(const std::string& _name):MObject(_name) {
  floodLevel = 0;
  index[0] = -1;
  index[1] = -1;
}
MArea::MArea(const std::string& _name, int x, int y):MArea(_name) {
  floodLevel = 0;
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
void MArea::setIndex(int x, int y) {
  index[0] = x;
  index[1] = y;
}
std::list<MArea*> MArea::getAllNeighbors() {
  return neighbors;
}
std::list<MArea*> MArea::getAllActiveNeighbors() {
  std::list<MArea*> dn;
  for(std::list<MArea*>::iterator it = neighbors.begin(); it != neighbors.end(); it++) {
    if((*it)->getFloodLevel() < 2) dn.push_back(*it);
  }
  return dn;
}
std::list<MArea*> MArea::getDirectActiveNeighbors() {
  std::list<MArea*> dn;
  for(std::list<MArea*>::iterator it = neighbors.begin(); it != neighbors.end(); it++) {
    if(!isDiagonal(*it) && (*it)->getFloodLevel() < 2) dn.push_back(*it);
  }
  return dn;
}
std::list<MArea*> MArea::getDirectActiveNeighbors2() {
  std::list<MArea*> dn, middle;

  for (std::list<MArea*>::iterator it = neighbors.begin(); it != neighbors.end(); it++) {
	if (!isDiagonal(*it) && (*it)->getFloodLevel() < 2) middle.push_back(*it);
  }
  
  for (std::list<MArea*>::iterator it = middle.begin(); it != middle.end(); it++) {
	std::list<MArea*> subDn = (*it)->getDirectActiveNeighbors();
	for (std::list<MArea*>::iterator it2 = subDn.begin(); it2 != subDn.end(); it2++) {
	  if (std::find(dn.begin(), dn.end(), *it2) == dn.end()) {
		dn.push_back(*it2);
	  }
	}
  }
  
  for (std::list<MArea*>::iterator it = middle.begin(); it != middle.end(); it++) {
    if (std::find(dn.begin(), dn.end(), *it) == dn.end()) {
      if (!isDiagonal(*it) && (*it)->getFloodLevel() < 2) dn.push_back(*it);
    }
  }
  return dn;
}
std::list<MArea*> MArea::getAllFloodedNeighbors() {
	std::list<MArea*> dn;
	for (std::list<MArea*>::iterator it = neighbors.begin(); it != neighbors.end(); it++) {
		if ((*it)->getFloodLevel() == 1) dn.push_back(*it);
	}
	//dn.push_back(this);//what is it for?
	return dn;
}
std::list<MArea*> MArea::getDirectFloodedNeighbors() {
	std::list<MArea*> dn;
	for (std::list<MArea*>::iterator it = neighbors.begin(); it != neighbors.end(); it++) {
		if (!isDiagonal(*it) && (*it)->getFloodLevel() == 1) dn.push_back(*it);
	}
	//dn.push_back(this);//what is it for?
	return dn;
}
void MArea::addNeighbor(MArea* area) {
  if(!area) return;
  std::list<MArea*>::iterator it = std::find(neighbors.begin(), neighbors.end(), area);
  if(it != neighbors.end()) return;
  neighbors.push_back(area);
  area->addNeighbor(this);
}
void MArea::removeAllNeighbors() {
  neighbors.clear();
}
void MArea::removeNeighbor(MArea* area) {
  std::list<MArea*>::iterator it = std::find(neighbors.begin(), neighbors.end(), area);
  if(it == neighbors.end()) return;
  neighbors.erase(it);
  area->removeNeighbor(this);
}
void MArea::removeNeighbor(std::string name) {
  for(std::list<MArea*>::iterator it = neighbors.begin(); it != neighbors.end(); it++) {
    if((*it)->getName() == name) {
	  neighbors.erase(it);
	  (*it)->removeNeighbor(this);
	  break;
	}
  }
}
int MArea::flood() {
  floodLevel ++;
  return floodLevel;
}
int MArea::abfluss() {
  floodLevel --;
  return floodLevel;
}
void MArea::setFloodLevel(int _floodLevel) {
  floodLevel = _floodLevel;
}
int MArea::getFloodLevel() {
  return floodLevel;
}
