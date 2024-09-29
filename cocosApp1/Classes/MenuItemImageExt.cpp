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

void MenuItemImageExt::onMouseMove(Event *event) {
    //need think about combine hover and select
    EventMouse* e = (EventMouse*)event;
    Size size = getContentSize();
    Vec2 pos = getPosition();
    Vec2 mousePos = Vec2(e->getCursorX(), e->getCursorY());
    Rect r = Rect(pos.x - size.width/2, pos.y - size.height/2, size.width, size.height);
    if(r.containsPoint(mousePos)) {
        selected();
    }
    else {
        unselected();
    }   
}

void MenuItemImageExt::setMouseListener() {
    mouseListener = EventListenerMouse::create();
    mouseListener->onMouseMove = CC_CALLBACK_1(MenuItemImageExt::onMouseMove, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}