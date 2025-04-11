#ifndef menuitemimageextH
#define menuitemimageextH

#include "cocos2d.h"

USING_NS_CC;

class MenuItemImageExt : public MenuItemImage {
public:
    ~MenuItemImageExt();
    static MenuItemImageExt* create(const std::string &normalImage, const std::string &selectedImage, const std::string &disabledImage, const ccMenuCallback& callback);
    static MenuItemImageExt* create(cocos2d::Sprite* normalSprite, cocos2d::Sprite* seletedSprite, cocos2d::Sprite* disabledSprite, const ccMenuCallback& callback);
protected:
    EventListenerMouse* mouseListener;
private:
    void onMouseMove(Event *event);
    void setMouseListener();
};

#endif