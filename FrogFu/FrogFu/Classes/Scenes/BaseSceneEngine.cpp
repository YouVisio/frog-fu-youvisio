//
//  BaseSceneManager.cpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-05.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#import "BaseSceneEngine.hpp"
#import "Factory.hpp"

BaseSceneEngine::BaseSceneEngine(IRenderingEngine* renderingEngine) :
_touch(0,0),
_time(0),
_nextTimeFrame(0),
_framesPerSecond(0),
_statusText(NULL)
{
    _renderingEngine = renderingEngine;
    _drawables = _renderingEngine->GetDrawables();
    _touchTrails = _renderingEngine->GetTouchTrails();
    _mapDots = _renderingEngine->GetMapDots();
    _animations = _renderingEngine->GetAnimations();//new list<IAnimation*>();
}
void* BaseSceneEngine::GetMapManager(){ return _mapManager; }
void* BaseSceneEngine::GetInsectManager(){ return _insectManager; }
void* BaseSceneEngine::GetTouchTrailManager(){ return _touchTrailManager; }
void* BaseSceneEngine::GetFrogManager(){ return _frogManager; }
void* BaseSceneEngine::GetButtonsBar(){ return _buttonBar; }
void* BaseSceneEngine::GetStatusBar(){ return _statusBar; }
void* BaseSceneEngine::GetEndOfGame(){ return _endOfGame; }
void* BaseSceneEngine::GetPauseManager(){ return _pauseManager; }
bool BaseSceneEngine::StartAnimation(IAnimation* animation){
    if(!animation->CanStart(_animations)){
        return false;
    }
    _animations->push_back(animation);
    return true;
}
void BaseSceneEngine::Construct() {}
void BaseSceneEngine::Initialize()
{
    _time = 0;
#if ISDEBUG
    if(_statusText != NULL) delete _statusText;
    
    _drawables->push_back(
                          _statusText = new TextBlock(Factory::Instance->TextureAtlas01, " ", 200.0f, 200.0f, Factory::Instance->getInitialFontDescription()));
    _statusText->Position(250, 2)->Color(1, 0, 0, 0)->Depth(99889);
    //_statusText->sfactor = GL_SRC_ALPHA;
    //_statusText->dfactor = GL_ONE_MINUS_SRC_ALPHA;
#endif
}
void BaseSceneEngine::applicationWillResignActive(){ }
void BaseSceneEngine::applicationDidEnterBackground(){ }
void BaseSceneEngine::applicationWillEnterForeground(){ }
void BaseSceneEngine::applicationDidBecomeActive(){ }
void BaseSceneEngine::applicationWillTerminate(){ }
void BaseSceneEngine::TakeOffScreen(){ }
int BaseSceneEngine::TransitionIndex(){ return _transitionIndex; }
void BaseSceneEngine::TransitionIndex(int val){ _transitionIndex = val; }
IAnimation* BaseSceneEngine::FindFirstAnimationOfType(int type){
    simnode<IAnimation>* node = _animations->head;
    while (node != NULL) {
        IAnimation * a = node->data;
        if(a->Type() == type){
            return a;
        }
        node = node->next;
    }
    return NULL;
}
void BaseSceneEngine::ClearAllAnimations(){
    _animations->clear(true);
}
void BaseSceneEngine::RemoveAnimationsByType(int type){
    simnode<IAnimation>* node = _animations->head;
    while (node != NULL) {
        IAnimation * a = node->data;
        if(a->Type() == type){
            node = _animations->removeAndGetNext(node, true);
        }else{
            node = node->next;
        }
    }
}
void BaseSceneEngine::Update(double step)
{
    _time += step;
    simnode<IAnimation>* node = _animations->head;
    while (node != NULL) {
        IAnimation * a = node->data;
        if(a->IsFinished()){
            node = _animations->removeAndGetNext(node, true);
        }else{
            if(!a->IsInitialized()){
                a->Initialize(_time);
            }
            a->Update(_time);
            node = node->next;
        }
    }
    
#if ISDEBUG
    // the following code manages calculation of number frames per second - only for DEBUG
    if(_time >= _nextTimeFrame && _statusText != NULL){
        char ch[30];
        sprintf( ch,"FPS:%d FM:%d MB", _framesPerSecond, getFreeMemory());
        _statusText->SetText(ch);
        
        _framesPerSecond = 0;
        _nextTimeFrame = _time + 1.0;
    }else ++_framesPerSecond;
#endif
}
vec2 BaseSceneEngine::GetTouch() { return _touch; }
void BaseSceneEngine::InitFromStartScene(ISceneEngine* startScene){ };