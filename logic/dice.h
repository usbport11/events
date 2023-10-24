#ifndef diceH
#define diceH

#include "object.h"

#include <vector>
#include <random>
#include <ctime>

class MDiceSet: public MObject {
private:
    std::mt19937 rng;
    std::uniform_int_distribution<int> distribute;
    std::vector<int> rollResults;
    bool roll(int diceNumber);
    void reRollAll();
    void reRoll(int number);
    void reRollLessThan(int value);
public:
    MDiceSet();
    ~MDiceSet();
    bool execute(MEvent* _event);
    void nullAll();
    void removeAll();
    int getSuccessRolls();
};

#endif
