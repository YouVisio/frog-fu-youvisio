//
//  BaseSceneManager.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-05.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_BaseSceneEngine_hpp
#define FrogFu_BaseSceneEngine_hpp

#import <iostream>
#import "Interfaces.hpp"
#import <vector>
#import "IAnimation.hpp"
#import "TextBlock.hpp"
#import "Particle.hpp"
#import "ISceneEngine.hpp"
#import "IRenderingEngine.hpp"
#import "MapManager.hpp"
#import "FrogManager.hpp"
#import "InsectManager.hpp"
#import "TouchTrailManager.hpp"
#import "ButtonBar.hpp"
#import "StatusBar.hpp"
#import "EndOfGame.hpp"
#import "PauseManager.hpp"

using namespace std;

class BaseSceneEngine : public ISceneEngine {
public:
    BaseSceneEngine(IRenderingEngine* renderingEngine);
    virtual void Update(double step);
    virtual void Initialize();
    virtual void Construct();
    virtual void TakeOffScreen();
    virtual bool StartAnimation(IAnimation* animation);
    virtual vec2 GetTouch();
    virtual void RemoveAnimationsByType(int type);
    virtual IAnimation* FindFirstAnimationOfType(int type);
    virtual void InitFromStartScene(ISceneEngine* startScene) = 0;
    virtual void applicationWillResignActive();
    virtual void applicationDidEnterBackground();
    virtual void applicationWillEnterForeground();
    virtual void applicationDidBecomeActive();
    virtual void applicationWillTerminate();
    void* GetMapManager();
    void* GetInsectManager();
    void* GetTouchTrailManager();
    void* GetFrogManager();
    void* GetButtonsBar();
    void* GetStatusBar();
    void* GetEndOfGame();
    void* GetPauseManager();
    int TransitionIndex();
    void TransitionIndex(int val);
    
protected:
    IRenderingEngine* _renderingEngine;
    TextBlock* _statusText;
    simlist<Drawable>* _drawables;
    ParticleList* _touchTrails;
    simlist<Particle>* _mapDots;
    simlist<IAnimation>* _animations;
    vec2 _touch;
    double _time;
    int _transitionIndex;
    
    MapManager* _mapManager;
    InsectManager* _insectManager;
    TouchTrailManager* _touchTrailManager;
    FrogManager* _frogManager;
    ButtonBar* _buttonBar;
    StatusBar* _statusBar;
    EndOfGame* _endOfGame;
    PauseManager* _pauseManager;
    
    virtual void ClearAllAnimations();
    
private:
    double _nextTimeFrame;
    int _framesPerSecond;
};


#endif
