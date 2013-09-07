//
//  EasyPlayScene.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-12-29.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_EasyPlayScene_hpp
#define FrogFu_EasyPlayScene_hpp

#import "ClassicScene.hpp"
#import "StarSparkle.hpp"

class EasyPlayScene : public ClassicScene {
public:
    EasyPlayScene(IRenderingEngine* renderingEngine)
    : ClassicScene(renderingEngine){
        _nextStarSparkleTime = BILLION;
    }
    virtual void Construct(){
        
        ClassicScene::Construct();
        
        _bgImages->BgBack->RedefineTextureCoords(0, 438, 640, 1610);
        _bgImages->BgBack->Texture(_factory->TextureAtlas10);
        
        _bgImages->BgFront->Texture(_factory->TextureAtlas03);

        
    }
    virtual void Initialize(){
        ClassicScene::Initialize();
        
        _insectManager->SetEngine(this);
        
        _factory->sceneID = SCENE_EASY_PLAY;
        
        _insectManager->SetNextWaspCreationTime(BILLION);
        
        _nextStarSparkleTime = _time + sysRandomOfPeriodAsDouble(0.1, 1.0);
//_drawables->push_back(_factory->redDot);
//_factory->redDot->Position(340,1015);
    }
    virtual void Update(double step){
        ClassicScene::Update(step);
        
        if(_time > _nextStarSparkleTime){
            StartAnimation(new StarSparkle(_drawables));
            
            if(_factory->jumpPosition() < 0.01){
                _nextStarSparkleTime = _time + sysRandomOfPeriodAsDouble(2.5, 5.0);
            } else {
                _nextStarSparkleTime = _time + sysRandomOfPeriodAsDouble(0.5, 2.0);
            }
            // TODO: plan new sparkle
        }
        
    }
    virtual void TransitionToStart(){
        _factory->transitionEasyPlayToStart();
    }
    virtual void InitializeStatusBar(){   
        _statusBar->Initialize(_time, _factory->UserState->EasyPlayS(),
                               90, new ActionAndState(OnStaticTimeOver, this),
                               -1, NULL);
    }
    static void OnStaticTimeOver(void* caller, long id){ ((EasyPlayScene*)caller)->OnTimeOver(id);  }
    void OnTimeOver(long id){
        _mapManager->arrow->IsVisible(false);
        float all = (float)(_factory->numberHit + _factory->numberMissed);
        float perc = (all < 100) ? 0 : round((_factory->numberHit / all) * 100.0);
        
        _touchTrailManager->IsEnabled(false);
        _endOfGame->Show(END_OF_GAME_TIME,_factory->numberHit, _factory->UserState->EasyPlayS(), perc, _factory->UserState->EasyPlayP(),
                         new ActionAndState(OnStaticEndOfGameBack, this),
                         new ActionAndState(OnStaticEndOfGameReplay, this));
        
        _factory->UserState->EasyPlayScene(_factory->numberHit, perc);
        
        analyticsEvent("evt_easyPlayEnd", "score", _factory->numberHit);
        analyticsScreen("easyPlayEnd");
        submitScore(_factory->numberHit, "lb_easy_play_score");
        if(perc >= 50) submitAchievement("ach_50_easy_play");
        if(perc >= 80) submitAchievement("ach_80_easy_play");
        if(perc >= 99) submitAchievement("ach_80_easy_play");
    }
    virtual void UpdateInsectManager(){
        
    }
private:
    int _nextStarSparkleTime;
};

ISceneEngine* CreateEasyPlayScene(IRenderingEngine* renderer){
    return new EasyPlayScene(renderer);
}


#endif
