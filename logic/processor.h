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

class MProcessor {
private:
    std::deque<MEvent*> events;
    std::vector<MEvent*> history;
    int eventNumber;
    MDice* dice;
    MMui* mui;
    MHero* hero; //link to current hero
    MEvent* event; //link to current event
    std::map<int, int> eventObjectType;
    std::map<int, std::string> eventName;
    std::map<int, std::string> objectName;
    void addEvent(int eventType, MEvent* parent);
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
