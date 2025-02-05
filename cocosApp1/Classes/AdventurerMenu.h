#ifndef adventurerMenuH
#define adventurerMenuH

#include "cocos2d.h"

class MMainScene;

class MAdventurerMenu {
private:
	MMainScene* pMainScene;
	std::map<int, std::string> numberAdventurer;
	std::map<std::string, std::string> advNameFrame;
	std::map<std::string, cocos2d::Sprite*> adventurerSprite; 
	void menuAdv0Callback(cocos2d::Ref* pSender);
	void menuAdv1Callback(cocos2d::Ref* pSender);
	void menuAdv2Callback(cocos2d::Ref* pSender);
	void menuAdv3Callback(cocos2d::Ref* pSender);
	void menuAdv4Callback(cocos2d::Ref* pSender);
	void menuAdv5Callback(cocos2d::Ref* pSender);
	void unselectMenuAll();
	void selectMenuItem(cocos2d::Ref* pSender);
public:
	MAdventurerMenu();
	~MAdventurerMenu();
	bool create(MMainScene* _pMainScene, const std::string& pListFile, std::vector<std::string> adventurers);
	bool init(std::vector<std::string> activeAdventurers);
	void selectByName(const std::string& name);
	std::map<std::string, cocos2d::Sprite*> getAdventurerSprite();
};

#endif