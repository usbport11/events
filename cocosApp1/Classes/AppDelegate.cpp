#include "AppDelegate.h"
#include "MainScene.h"
#include "IntroScene.h"
#include "MainMenuScene.h"
#include "TestScene.h"
#include <iostream>

//#define USE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate() {
    if (!AllocConsole()) {
        return;
    }
    FILE* fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONIN$", "r", stdin);
    std::cout.clear();
    std::clog.clear();
    std::cerr.clear();
    std::cin.clear();
}

AppDelegate::~AppDelegate() {
    FreeConsole();
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#endif
}

void AppDelegate::initGLContextAttrs() {
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};
    GLView::setGLContextAttrs(glContextAttrs);
}

static int register_all_packages() {
    return 0;
}

bool AppDelegate::applicationDidFinishLaunching() {
    designResolutionSize = mediumResolutionSize;

    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::createWithRect("cocosApp1", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
        director->setOpenGLView(glview);
    }

    director->setDisplayStats(false);//true
    director->setAnimationInterval(1.0f / 60);

    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    auto frameSize = glview->getFrameSize();
    if (frameSize.height > mediumResolutionSize.height) {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }
    else if (frameSize.height > smallResolutionSize.height) {        
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }
    else {        
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }

    register_all_packages();

    //MTestScene* testScene = (MTestScene*)MTestScene::createScene();
    //if (!testScene) return false;
    //director->runWithScene(testScene);
    //return true;

    MMainScene* mainScene = (MMainScene*)MMainScene::createScene();
    if (!mainScene) return false;
    MMainMenuScene* mainMenuScene = (MMainMenuScene*)MMainMenuScene::createScene();
    if (!mainMenuScene) return false;
    if (!mainMenuScene->create(mainScene, true)) return false;
    MIntroScene* introScene = (MIntroScene*)MIntroScene::createScene();
    if (!introScene) return false;

    director->pushScene(mainScene);
    director->pushScene(mainMenuScene);
    director->pushScene(introScene);
    //director->runWithScene(mainScene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#endif
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#endif
}
