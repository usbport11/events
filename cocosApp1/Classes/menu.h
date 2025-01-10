#ifndef menuH
#define menuH

#include "cocos2d.h"

class MGridMap;

class MMenu {
private:
	cocos2d::Scene* pScene;
	MGridMap* pGridMap;
	const std::string mask;
	const std::string name;
	void menuAbflussCallback(cocos2d::Ref* pSender);
	void menuEndTurnCallback(cocos2d::Ref* pSender);
	void menuMoveCallback(cocos2d::Ref* pSender);
	void menuHandOverCallback(cocos2d::Ref* pSender);
	void menuGetArtifactCallback(cocos2d::Ref* pSender);
	void unselectMenuAll();
	void selectMenuItem(cocos2d::Ref* pSender);
public:
	MMenu();
	bool create(cocos2d::Scene* _pScene, MGridMap* _pGrid);
	~MMenu();
};

#endif