#ifndef processorH
#define processorH

#include <deque>
#include <vector>
#include <map>
#include <string>
#include <iostream>

class MDice;
class MMui;
class MHero;
class MEvent;
class MMonster;

class MProcessor {
private:
    bool terminate;
    std::deque<MEvent*> events;
    std::vector<MEvent*> history;
    int eventNumber;
    MDice* dice;
    MMui* mui;
    MHero* hero;
    MMonster* monster;
    MEvent* event;
    std::map<int, int> eventObjectType;
    std::map<int, std::string> eventName;
    std::map<int, std::string> objectName;
    MEvent* addEvent(int eventType);
    void resoveNextEvent();
    std::string getEventName(int id);
    std::string getObjectName(int id);
public:
    MProcessor();
    ~MProcessor();
    void start();
    void resolveEvents();
};

#endif
