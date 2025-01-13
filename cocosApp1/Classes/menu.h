#ifndef menuH
#define menuH

#include "cocos2d.h"

class MMainScene;
class MGridMap;

class MMenu {
private:
	MMainScene* pMainScene;
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
	bool create(MMainScene* _pMainScene, MGridMap* _pGrid);
	~MMenu();
};

#endif