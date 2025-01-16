#ifndef __END_SCENE_H__
#define __END_SCENE_H__

#include "cocos2d.h"

class MEndScene: public cocos2d::Scene {
private:
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void setMessage(const std::string& message);
    void menuCloseCallback(cocos2d::Ref* pSender);
    CREATE_FUNC(MEndScene);
};

#endif
