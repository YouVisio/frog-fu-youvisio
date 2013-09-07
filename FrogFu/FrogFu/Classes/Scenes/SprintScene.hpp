//
//  SprintScene.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-12-29.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_SprintScene_hpp
#define FrogFu_SprintScene_hpp

#import "ClassicScene.hpp"

class SprintScene : public ClassicScene {
public:
    SprintScene(IRenderingEngine* renderingEngine)
    : ClassicScene(renderingEngine){}
    virtual void Construct(){
        
        ClassicScene::Construct();
        
        _bgImages->BgBack->RedefineTextureCoords(1280, 438, 640, 1612);
        
        _bgImages->BgFront->Texture(_factory->TextureAtlas09);
        
    }
    virtual void Initialize(){
        ClassicScene::Initialize();
        
        _insectManager->SetEngine(this);
        
        _factory->sceneID = SCENE_SPRINT; 
    }
    virtual void TransitionToStart(){
        _factory->transitionSprintToStart();
    }
    virtual void InitializeStatusBar(){
        _statusBar->Initialize(_time, _factory->UserState->SprintS(),
                               60, new ActionAndState(OnStaticTimeOver, this),
                               -1, NULL);
    }
    static void OnStaticTimeOver(void* caller, long id){ ((SprintScene*)caller)->OnTimeOver(id);  }
    void OnTimeOver(long id){
        _mapManager->arrow->IsVisible(false);
        float all = (float)(_factory->numberHit + _factory->numberMissed);
        float perc = (all < 100) ? 0 : round((_factory->numberHit / all) * 100.0);
        
        _touchTrailManager->IsEnabled(false);
        _endOfGame->Show(END_OF_GAME_TIME,_factory->numberHit, _factory->UserState->SprintS(), perc, _factory->UserState->SprintP(),
                         new ActionAndState(OnStaticEndOfGameBack, this),
                         new ActionAndState(OnStaticEndOfGameReplay, this));
        
        _factory->UserState->SprintScene(_factory->numberHit, perc);
        
        analyticsEvent("evt_sprintEnd", "score", _factory->numberHit);
        analyticsScreen("sprintEnd");
        submitScore(_factory->numberHit, "lb_sprint_score");
        if(perc >= 50) submitAchievement("ach_50_sprint");
        if(perc >= 80) submitAchievement("ach_80_sprint");
        if(perc >= 99) submitAchievement("ach_80_sprint");
    }
    virtual void UpdateInsectManager(){
        double p = 1.0;
        double limit = 60*60*0.8;// 60 frames x 60 seconds x 0.8 minute
        if(_sessionFrame < limit){
            p = _sessionFrame / limit;
        }
        _insectManager->nextCreateWaspTimeRange.Set(CalculateCurr(6.0, 0.2, p),CalculateCurr(6.1, 1.0, p));
    }
    virtual void OnStungAnimationEnds(){
         _factory->enableInsectCreation = true;
        _frogManager->StartStaticPose();
        _factory->numberHit -= 10;
        if(_factory->numberHit < 0) _factory->numberHit = 0;
    }
    
};

ISceneEngine* CreateSprintScene(IRenderingEngine* renderer){
    return new SprintScene(renderer);
}
#endif
