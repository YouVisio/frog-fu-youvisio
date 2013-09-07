//
//  ISceneEngine.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-11.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_ISceneEngine_hpp
#define FrogFu_ISceneEngine_hpp

struct ISceneEngine {
    virtual void Initialize() = 0;
    virtual void Construct() = 0;
    virtual void TakeOffScreen() = 0;
    virtual void Update(double step) = 0;
    virtual void TouchBegan(float x, float y) = 0;
    virtual void TouchMoved(float x, float y, float prevX, float prevY) = 0;
    virtual void TouchEnded(float x, float y) = 0;
    virtual void InitFromStartScene(ISceneEngine* startScene) = 0;
    virtual vec2 GetTouch() = 0;
    virtual void* GetMapManager() = 0;
    virtual void* GetInsectManager() = 0;
    virtual void* GetTouchTrailManager() = 0;
    virtual void* GetFrogManager() = 0;
    virtual void* GetButtonsBar() = 0;
    virtual void* GetStatusBar() = 0;
    virtual void* GetEndOfGame() = 0;
    virtual void* GetPauseManager() = 0;
    virtual int TransitionIndex() = 0;
    virtual void TransitionIndex(int val) = 0;
    virtual void applicationWillResignActive() = 0;
    virtual void applicationDidEnterBackground() = 0;
    virtual void applicationWillEnterForeground() = 0;
    virtual void applicationDidBecomeActive() = 0;
    virtual void applicationWillTerminate() = 0;
    virtual ~ISceneEngine() {}
    
    
};

#endif
