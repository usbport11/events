#ifndef __END_SCENE_H__
#define __END_SCENE_H__

#include "cocos2d.h"

class MMainScene;

class MEndScene: public cocos2d::Scene {
private:
    MMainScene* pMainScene;
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void setMainScene(MMainScene* _pMainScene);
    void setMessage(const std::string& message);
    void menuCloseCallback(cocos2d::Ref* pSender);
    CREATE_FUNC(MEndScene);
};

#endif
