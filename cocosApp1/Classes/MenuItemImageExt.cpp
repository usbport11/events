#include "MenuItemImageExt.h"

MenuItemImageExt::~MenuItemImageExt() {
    _eventDispatcher->removeEventListener(mouseListener);
}

MenuItemImageExt* MenuItemImageExt::create(const std::string &normalImage, const std::string &selectedImage, const std::string &disabledImage, const ccMenuCallback &callback) {
    MenuItemImageExt *ret = new (std::nothrow) MenuItemImageExt();
    if(ret && ret->initWithNormalImage(normalImage, selectedImage, disabledImage, callback)) {
        ret->setMouseListener();
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

MenuItemImageExt* MenuItemImageExt::create(cocos2d::Sprite* normalSprite, cocos2d::Sprite* seletedSprite, cocos2d::Sprite* disabledSprite, const ccMenuCallback& callback) {
    MenuItemImageExt* ret = new (std::nothrow) MenuItemImageExt();
    if (ret && ret->initWithNormalSprite(normalSprite, seletedSprite, disabledSprite, callback)) {
        ret->setMouseListener();
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void MenuItemImageExt::onMouseMove(Event *event) {
    if (!isEnabled()) {
        return;
    }
    //need think about combine hover and select
    EventMouse* e = (EventMouse*)event;
    Size size = getContentSize();
    Vec2 pos = getPosition();
    Vec2 mousePos = Vec2(e->getCursorX(), e->getCursorY());
    Rect r = Rect(pos.x - size.width/2, pos.y - size.height/2, size.width - 1, size.height - 1);
    if(r.containsPoint(mousePos)) {
        this->selected();
    }
    else {
        this->unselected();
    }   
}

void MenuItemImageExt::setMouseListener() {
    mouseListener = EventListenerMouse::create();
    mouseListener->onMouseMove = CC_CALLBACK_1(MenuItemImageExt::onMouseMove, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}