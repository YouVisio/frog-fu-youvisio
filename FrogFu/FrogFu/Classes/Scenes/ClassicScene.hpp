//
//  ClassicScene.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-10-07.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_ClassicScene_hpp
#define FrogFu_ClassicScene_hpp


#import <iostream>
#import "Interfaces.hpp"
#import "BaseSceneEngine.hpp"
#import "IRenderingEngine.hpp"
#import "BackgroundInJump.hpp"
#import "Factory.hpp"
#import "SplashAction.hpp"
#import "StartScreenState.hpp"
#import "MuteMusic.hpp"
#import <vector>

using namespace std;



class ClassicScene : public BaseSceneEngine {
public:
    ClassicScene(IRenderingEngine* renderingEngine)
    : BaseSceneEngine(renderingEngine),
    _firstTouchY(-1),
    _bgImages(NULL),
    _branchesOriginalY(0),
    _bgOriginalY(0),
    _strength(0){ }
    virtual void Construct(){
        Factory* f = Factory::Instance;
        
        _bgImages = new BackgroundImages();
        
        _bgImages->BgBack =
        (new Image(f->TextureAtlas01, 0, 439, 640, 1609))
        ->Position(0, 250)->Func(ImgCenter_BottomLeft_NoRotation);
        _bgImages->BgBack->mark = MARK_BACKGROUND;
        
        _bgImages->BgFarBg =
        (new Image(f->TextureAtlas02, 0, 657, 640, 366))
        ->Position(0, 290)->Func(ImgCenter_BottomLeft_NoRotation);
        _bgImages->BgNearBg =
        (new Image(f->TextureAtlas02, 0, 392, 640, 264))
        ->Position(0, 250)->Func(ImgCenter_BottomLeft_NoRotation);
        _bgImages->BgMain =
        (new Image(f->TextureAtlas02, 0, 0, 640, 391))
        ->Position(0, 30)->Func(ImgCenter_BottomLeft_NoRotation);
        _bgImages->BgWave1 =
        (new Image(f->TextureAtlas02, 641, 75, 323, 64))
        ->Position(320, 175)->Center(323/2.0, 64/2.0)->Func(ImgCenter_Centered_NoRotation)->IsVisible(false);
        _bgImages->BgWave2 =
        (new Image(f->TextureAtlas02, 641, 75, 323, 64))
        ->Position(320, 175)->Center(323/2.0, 64/2.0)->Func(ImgCenter_Centered_NoRotation)->IsVisible(false);
        _bgImages->BgPlatform =
        (new Image(f->TextureAtlas02, 641, 0, 329, 73))
        ->Position(300, 185)->Center(307/2.0, 65/2.0)->Func(ImgCenter_Centered_NoRotation);
        _bgImages->BgSideTwoLeft =
        (new Image(f->TextureAtlas02, 822, 140, 117, 721))
        ->Position(0, 500)->Func(ImgCenter_BottomLeft_NoRotation);
        _bgImages->BgSideTwoRight =
        (new Image(f->TextureAtlas02, 940, 140, 80, 587))
        ->Position(640-80, 500)->Func(ImgCenter_BottomLeft_NoRotation);
        _bgImages->BgSideOneLeft =
        (new Image(f->TextureAtlas02, 641, 141, 82, 872))
        ->Position(0, 800)->Func(ImgCenter_BottomLeft_NoRotation);
        _bgImages->BgSideOneRight =
        (new Image(f->TextureAtlas02, 724, 140, 97, 748))
        ->Position(640-97, 800)->Func(ImgCenter_BottomLeft_NoRotation);
        _bgImages->BgFront =
        (new Image(f->TextureAtlas03, 0, 800, 640, 223))
        ->Position(0, 0)->Func(ImgCenter_BottomLeft_NoRotation);
        
        _sessionFrame = 0;
        
        /*_bgImages->BgBack->textureGroup = 1;
         _bgImages->BgBack->sfactor = GL_ONE;
         _bgImages->BgBack->dfactor = GL_ONE_MINUS_SRC_ALPHA;
         _bgImages->BgFarBg->textureGroup = 1;
         _bgImages->BgFarBg->sfactor = GL_ONE;
         _bgImages->BgFarBg->dfactor = GL_ONE_MINUS_SRC_ALPHA;
         _bgImages->BgNearBg->textureGroup = 1;
         _bgImages->BgNearBg->sfactor = GL_ONE;
         _bgImages->BgNearBg->dfactor = GL_ONE_MINUS_SRC_ALPHA;
         _bgImages->BgMain->textureGroup = 1;
         _bgImages->BgMain->sfactor = GL_ONE;
         _bgImages->BgMain->dfactor = GL_ONE_MINUS_SRC_ALPHA;*/
        //        _bgImages->BgWave1->textureGroup = 1;
        //        _bgImages->BgWave1->sfactor = GL_ONE;
        //        _bgImages->BgWave1->dfactor = GL_ONE_MINUS_SRC_ALPHA;
        //        _bgImages->BgWave2->textureGroup = 1;
        //        _bgImages->BgWave2->sfactor = GL_ONE;
        //        _bgImages->BgWave2->dfactor = GL_ONE_MINUS_SRC_ALPHA;
        /*_bgImages->BgPlatform->textureGroup = 2;
         _bgImages->BgPlatform->sfactor = GL_ONE;
         _bgImages->BgPlatform->dfactor = GL_ONE_MINUS_SRC_ALPHA;
         _bgImages->BgSideTwoLeft->textureGroup = 2;
         _bgImages->BgSideTwoLeft->sfactor = GL_ONE;
         _bgImages->BgSideTwoLeft->dfactor = GL_ONE_MINUS_SRC_ALPHA;
         _bgImages->BgSideTwoRight->textureGroup = 2;
         _bgImages->BgSideTwoRight->sfactor = GL_ONE;
         _bgImages->BgSideTwoRight->dfactor = GL_ONE_MINUS_SRC_ALPHA;
         _bgImages->BgSideOneLeft->textureGroup = 2;
         _bgImages->BgSideOneLeft->sfactor = GL_ONE;
         _bgImages->BgSideOneLeft->dfactor = GL_ONE_MINUS_SRC_ALPHA;
         _bgImages->BgSideOneRight->textureGroup = 2;
         _bgImages->BgSideOneRight->sfactor = GL_ONE;
         _bgImages->BgSideOneRight->dfactor = GL_ONE_MINUS_SRC_ALPHA;
         _bgImages->BgFront->textureGroup = 2;
         _bgImages->BgFront->sfactor = GL_ONE;
         _bgImages->BgFront->dfactor = GL_ONE_MINUS_SRC_ALPHA;*/
        
        _bgImages->BgWave1->textureGroup = 1;
        _bgImages->BgWave1->sfactor = GL_SRC_ALPHA;
        _bgImages->BgWave1->dfactor = GL_ONE_MINUS_SRC_ALPHA;
        _bgImages->BgWave2->textureGroup = 1;
        _bgImages->BgWave2->sfactor = GL_SRC_ALPHA;
        _bgImages->BgWave2->dfactor = GL_ONE_MINUS_SRC_ALPHA;
        
        _branchesOriginalY = _bgImages->BgFront->Y();
        _bgOriginalY = _bgImages->BgBack->Y();
        
        _factory = f;
    }
    void InitFromStartScene(ISceneEngine* startScene){
        _mapManager = (MapManager*)startScene->GetMapManager();
        _insectManager = (InsectManager*)startScene->GetInsectManager();
        _insectManager->SetEngine(this);
        _touchTrailManager = (TouchTrailManager*)startScene->GetTouchTrailManager();
        _frogManager = (FrogManager*)startScene->GetFrogManager();
        _buttonBar = (ButtonBar*)startScene->GetButtonsBar();
        _statusBar = (StatusBar*)startScene->GetStatusBar();
        _endOfGame = (EndOfGame*)startScene->GetEndOfGame();
        _pauseManager = (PauseManager*)startScene->GetPauseManager();
    }
    virtual void Initialize(){
        
        _time = 0;
        
        _factory->sceneID = SCENE_CLASSIC;
        
        _sessionFrame = 0;
        
        _insectManager->SetEngine(this);
        
        sysPlaySound("M2");
        sysSoundVolume("M2", 0.3);
        
        
        _bgImages->BgFront->Texture(_factory->TextureAtlas03);
        
        _factory->enableInsectCreation = true;
        _factory->setEnableInput(true);

        _factory->isWithinJump = false;
        
        _factory->frogPlainShiftY = 0;
        _factory->bgPlainShiftY = 0;
        
        _drawables->clear();
        
        if(TransitionIndex() == 1){
            _renderingEngine->UnloadTextureById(7);
            _renderingEngine->LoadTextureById(4);
            _renderingEngine->LoadTextureById(5);
            TransitionIndex(0);
        }
        
        _frogManager->OnStungAnimationEndsCaller = this;
        _frogManager->OnStungAnimationEnds = OnStaticStungAnimationEnds;
        
        
        _drawables->push_back(_bgImages->BgBack);
        _drawables->push_back(_bgImages->BgFarBg);
        _drawables->push_back(_bgImages->BgNearBg);
        _drawables->push_back(_bgImages->BgMain);
        _drawables->push_back(_bgImages->BgWave1);
        _drawables->push_back(_bgImages->BgWave2);
        _drawables->push_back(_bgImages->BgPlatform);
        _drawables->push_back(_bgImages->BgSideTwoLeft);
        _drawables->push_back(_bgImages->BgSideTwoRight);
        _drawables->push_back(_bgImages->BgSideOneLeft);
        _drawables->push_back(_bgImages->BgSideOneRight);
        _drawables->push_back(_bgImages->BgFront);
        
        
        _factory->numberMissed = 0;
        _factory->numberHit = 0;
        
        
        _endOfGame->Initialize();
        _pauseManager->Initialize();
        
        _buttonBar->Initialize(new ActionAndState(OnStaticBackBtnTap, this),
                               new ActionAndState(OnStaticPauseBtnTap, this));
        InitializeStatusBar();
        _mapManager->Initialize();
        _frogManager->Initialize();
        _touchTrailManager->Initialize();
        _insectManager->Initialize(0);
        
        _factory->bodyPos = _factory->staticBodyPos;
        
        BaseSceneEngine::Initialize();
        
        _animations->push_back(new MuteMusic());
        
        _touchTrailManager->IsEnabled(false);
        
        
        
        
        //sysLoadSound("music", "caf", true);
        //sysLoadSound("triumph", "caf", false);
        
        
        //sysPlaySound("music");

    }
    virtual void InitializeStatusBar(){
        _statusBar->Initialize(_time, _factory->UserState->ClassicS(),
                               -1/*90*/, NULL,
                               60.0, new ActionAndState(OnStaticMaxMissed, this));
    }
    static void OnStaticStungAnimationEnds(void* caller){ ((ClassicScene*)caller)->OnStungAnimationEnds(); }
    virtual void OnStungAnimationEnds(){
        
        _factory->enableInsectCreation = false;
        _mapManager->arrow->IsVisible(false);
        float all = (float)(_factory->numberHit + _factory->numberMissed);
        float perc = (all < 100) ? 0 : round((_factory->numberHit / all) * 100.0);
        
        _touchTrailManager->IsEnabled(false);
        _endOfGame->Show(END_OF_GAME_WASP,_factory->numberHit, _factory->UserState->ClassicS(), perc, _factory->UserState->ClassicP(),
                         new ActionAndState(OnStaticEndOfGameBack, this),
                         new ActionAndState(OnStaticEndOfGameReplay, this));
        
        _factory->UserState->ClassicScene(_factory->numberHit, perc);
        
        analyticsEvent("evt_classicEnd", "score", _factory->numberHit);
        analyticsScreen("classicEnd");
        submitScore(_factory->numberHit, "lb_classic_score");
        if(perc >= 50) submitAchievement("ach_50_classic");
        if(perc >= 80) submitAchievement("ach_80_classic");
        if(perc >= 99) submitAchievement("ach_80_classic");
    }
    static void OnStaticMaxMissed(void* caller, long id){ ((ClassicScene*)caller)->OnMaxMissed(id);  }

    void OnMaxMissed(long id){
        _mapManager->arrow->IsVisible(false);
        float all = (float)(_factory->numberHit + _factory->numberMissed);
        float perc = (all < 100) ? 0 : round((_factory->numberHit / all) * 100.0);
        
        _touchTrailManager->IsEnabled(false);
        _endOfGame->Show(END_OF_GAME_MISSED,_factory->numberHit, _factory->UserState->ClassicS(), perc, _factory->UserState->ClassicP(),
                         new ActionAndState(OnStaticEndOfGameBack, this),
                         new ActionAndState(OnStaticEndOfGameReplay, this));
        
        _factory->UserState->ClassicScene(_factory->numberHit, perc);
    }
    static void OnStaticBackBtnTap(void* caller, long id){ ((ClassicScene*)caller)->OnBackBtnTap(id);  }
    static void OnStaticPauseBtnTap(void* caller, long id){ ((ClassicScene*)caller)->OnPauseBtnTap(id);  }
    void OnBackBtnTap(long id){
        _touchTrailManager->ClearAll();
        _insectManager->Disable();
        ClearAllAnimations();
        _renderingEngine->ClearExpired();
        TransitionToStart();
    }
    virtual void TransitionToStart(){
        _factory->transitionClassicToStart();
    }
    void OnPauseBtnTap(long id){
        
        _touchTrailManager->IsEnabled(!_factory->isPaused);
        _mapManager->arrow->IsVisible(false);
        _mapManager->ClearTicker();
        
        _pauseManager->Show(_time,
                            new ActionAndState(OnStaticPauseBack, this),
                            new ActionAndState(OnStaticPauseReplay, this));
        

    }
    void TakeOffScreen(){
        
        sysStopSound("M2");
        
        _factory->enableInsectCreation = false;
        ClearAllAnimations();
        _frogManager->TakeOffScreen();
        _factory->isWithinJump = false;
        _factory->setEnableInput(false);
        _mapManager->TakeOffScreen();
        _buttonBar->TakeOffScreen();
        _statusBar->TakeOffScreen();
    }
    virtual void Update(double step){
        ++_sessionFrame;
        BaseSceneEngine::Update(step);
        _mapManager->Update(_time);
        _touchTrailManager->Update(_time);
        _factory->frogPlainShiftY = _branchesOriginalY - _bgImages->BgFront->Y();
        _factory->bgPlainShiftY = _bgOriginalY - _bgImages->BgBack->Y();
        _insectManager->Update(_time);
        _frogManager->Update(_time);
        _statusBar->Update(_time);
        
        _endOfGame->Update(_time);
        _pauseManager->Update(_time);
        
        UpdateInsectManager();
    }
    virtual void UpdateInsectManager(){
        double p = 1.0;
        double limit = 60*60*1.5;// 60 frames x 60 seconds x 1.5 minute
        if(_sessionFrame < limit){
            p = _sessionFrame / limit;
        }
        _insectManager->nextCreateWaspTimeRange.Set(CalculateCurr(6.0, 0.2, p),CalculateCurr(6.1, 1.0, p));
    }
    void TouchBegan(float x, float y){
        _touch.Set(x,y);

        ++_factory->currentTouchId;
        
        if(_pauseManager->IsVisible() && _pauseManager->TouchBegan(_touch)){
            return;
        }
        
        if(_endOfGame->IsVisible() && _endOfGame->TouchBegan(_touch)){
            return;
        }
        
        if(!_factory->getEnableInput()) return;
        
        if(_buttonBar->TouchBegan(_touch)){
            return;
        }
        
        _firstTouchY = y;
        vec2 touch = vec2(x, y);
        _touchTrailManager->TouchBegins(touch);
        _frogManager->TouchBegins(touch);
        if(_factory->isWithinJump){ return; }
        _mapManager->TouchBegins();
        ChangeMapArrowHeight(0.1);
    }
    void TouchMoved(float x, float y, float prevX, float prevY){
        _touch.Set(x,y);
        
        if(_pauseManager->IsVisible() && _pauseManager->TouchMoved(_touch)){
            return;
        }
        
        if(_endOfGame->IsVisible() && _endOfGame->TouchMoved(_touch)){
            return;
        }
        
        if(!_factory->getEnableInput()) return;
        
        
        if(_buttonBar->TouchMoved(_touch)){
            return;
        }
        
        
        vec2 touch = vec2(x, y);
        _touchTrailManager->TouchMoved(touch);
        _frogManager->TouchMoved(touch);
        if(!_factory->isWithinJump){
            float progress = GetYTouchProgress(y);
            ChangeMapArrowHeight(progress);
        }
    }
    void TouchEnded(float x, float y){
        
        bool endOfGameBtnReleased = false;
        if(_endOfGame->IsVisible()){
            endOfGameBtnReleased = _endOfGame->TouchEnded(_touch);
        }
        
        bool pauseBtnReleased = false;
        if(_pauseManager->IsVisible()){
            pauseBtnReleased = _pauseManager->TouchEnded(_touch);
        }
        
        bool buttonBarBtnReleased = _buttonBar->TouchEnded(_touch);
        
        _touch.Set(-1,-1);
        if(!_factory->getEnableInput()) return;
        if(buttonBarBtnReleased || endOfGameBtnReleased || pauseBtnReleased){
            return;
        }
        
        vec2 touch = vec2(x, y);
        _touchTrailManager->TouchEnded(touch);
        _frogManager->TouchEnded(touch);
        if(!_factory->isWithinJump){
            _strength = GetYTouchProgress(y);
            if(_strength > 0.11){
                StartSplashAction(false);
                
                
                _frogManager->StartJump(
                                        new FuncAndState<int>(ClassicScene::OnInitiateJump, this),
                                        new FuncAndState<int>(ClassicScene::OnJumpPicks, this),
                                        _strength);
            } else if(_sessionFrame > 30 && !_factory->isPaused) { // if the screen is just touched
                _frogManager->StartPointingPoseAnimation();
                _mapManager->StartPreviewOneAnimation();
                _mapManager->SetFrogSit();
                _mapManager->arrow->IsVisible(false);
            }
        }
        _firstTouchY = -1;
    }
    static int OnInitiateJump(void* caller){
        ClassicScene* scene = (ClassicScene*) caller;
        scene->StartAnimation(new BackgroundInJump(scene->_strength,
                                                   scene->_mapManager,
                                                   scene->_bgImages));
        return 0;
    }
    static int OnJumpPicks(void* caller){
        ClassicScene* scene = (ClassicScene*) caller;
        scene->_frogManager->JumpPicks(new FuncAndState<int>(ClassicScene::OnStaticJumpEnds, scene),
                                       new FuncAndState<int>(ClassicScene::OnStaticTouchGround, scene));
        return 0;
    }
    static int OnStaticJumpEnds(void* caller){
        ((ClassicScene*) caller)->OnJumpEnd();
        return 0;
    }
    void OnJumpEnd(){
        _frogManager->OnJumpEnd();
    }
    static int OnStaticTouchGround(void* caller){
        ((ClassicScene*) caller)->OnTouchGround();
        return 0;
    }
    void OnTouchGround(){
        _frogManager->OnTouchGround();
        
        _insectManager->ContinueAll();
        
        _mapManager->JumpAnimationEnds();
        StartSplashAction(true);
        
        
        _touchTrailManager->OnJumpEnd();
        
        if(_touch.y >= 0){
            _firstTouchY = _touch.y;
            _mapManager->TouchBegins();
        }
    }
protected:
    double CalculateCurr(double from, double to, double progress){
        return from + (to - from)*progress;
    }
    static void OnStaticPauseBack(void* caller, long id){ ((ClassicScene*)caller)->OnPauseBack(id); }
    static void OnStaticPauseReplay(void* caller, long id){ ((ClassicScene*)caller)->OnPauseReplay(id); }
    void OnPauseBack(long id){
        OnBackBtnTap(id);
    }
    void OnPauseReplay(long id){
        TakeOffScreen();
        Initialize();
    }
    static void OnStaticEndOfGameBack(void* caller, long id){ ((ClassicScene*)caller)->OnEndOfGameBack(id); }
    static void OnStaticEndOfGameReplay(void* caller, long id){ ((ClassicScene*)caller)->OnEndOfGameReplay(id); }
    void OnEndOfGameBack(long id){
        _endOfGame->Hide();
        
        OnBackBtnTap(id);
    }
    void OnEndOfGameReplay(long id){
        _endOfGame->Hide();
        
        TakeOffScreen();
        Initialize();
    }
    float GetYTouchProgress(float y){
        float diff = _firstTouchY - y;
        diff = diff < 0
        ? 0
        : diff > MAX_SPRING
        ? MAX_SPRING
        : diff;
        return ensureRange(diff / MAX_SPRING, 0.1, 1.0);
    }
    void ChangeMapArrowHeight(float progress){
        if(progress == Factory::Instance->springDegree) return;
        
        Factory::Instance->springDegree = progress;
        
        _mapManager->ChangeMapArrowHeight(progress);
    }
    void StartSplashAction(bool moveLeaf){
        IAnimation* prevSplash = FindFirstAnimationOfType(ANI_SplashAction);
        if(prevSplash != NULL){
            prevSplash->Update(1.0);
            RemoveAnimationsByType(ANI_SplashAction);
        }
        
        BackgroundImages* bi = _bgImages;
        StartAnimation(new SplashAction(moveLeaf, bi->BgPlatform, bi->BgWave1, bi->BgWave2));
    }
    
    BackgroundImages* _bgImages;
    
    float _firstTouchY;
    float _branchesOriginalY;
    float _bgOriginalY;
    
    
    
    Factory* _factory;
    double _strength;
    long _sessionFrame;
    
};

ISceneEngine* CreateClassicScene(IRenderingEngine* renderer){
    return new ClassicScene(renderer);
}
#endif
