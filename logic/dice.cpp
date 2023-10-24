#include "dice.h"

#include <iostream>

bool MDiceSet::roll(int diceNumber) {
    for(int i=0; i<diceNumber; i++) {
        rollResults.push_back(distribute(rng));
        std::cout<<rollResults[i]<<" ";
    }
    std::cout<<std::endl;
    return true;
}
void MDiceSet::reRollAll() {
    for(int i=0; i<rollResults.size(); i++) {
        rollResults[i] = distribute(rng);
    }
}
void MDiceSet::reRoll(int number) {
    rollResults[number] = distribute(rng);
}
void MDiceSet::reRollLessThan(int value) {
    for(int i=0; i<rollResults.size(); i++) {
        if(rollResults[i] < value) reRoll(i);
    }
}
void MDiceSet::nullAll() {
    for(int i=0; i<rollResults.size(); i++) {
        rollResults[i] = 0;
    }
}
void MDiceSet::removeAll() {
    rollResults.clear();
}
MDiceSet::MDiceSet() {
    typeId = OBJ_DICESET;
    struct timespec tm;
    clock_gettime(CLOCK_REALTIME, &tm);
	rng.seed(tm.tv_nsec);
	distribute = std::uniform_int_distribution<int>(1, 6);
}
MDiceSet::~MDiceSet() {
    removeAll();
}
bool MDiceSet::execute(MEvent* _event) {
    if(!MObject::execute(_event)) return false;
    removeAll();
    switch(event->getType()) {
	case EVNT_DICESET_ROLL:
	    std::cout<<"dice_number: "<<event->getExecParamInt("dice_number")<<std::endl;
	    roll(event->getExecParamInt("dice_number"));
	    event->setExecResultInt("roll_result", getSuccessRolls());
	    return true;
    }
    return false;
}
int MDiceSet::getSuccessRolls() {
    int good = 0;
    for(int i=0; i<rollResults.size(); i++) {
        if(rollResults[i] >= 5) good ++;
    }
    return good;
}
