//
//  Pause.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-12-02.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_PauseManager_hpp
#define FrogFu_PauseManager_hpp

#import <iostream>
#import "BaseManager.hpp"
#import "Interfaces.hpp"
#import "Factory.hpp"
#import "Image.hpp"
#import "TextBlock.hpp"
#import "IAnimation.hpp"

using namespace std;

#define PAUSE_ACTION_BACK 1
#define PAUSE_ACTION_PLAY 2
#define PAUSE_ACTION_REPLAY 3

class PauseManager : BaseManager {
public:
    PauseManager(simlist<Drawable>* drawables, simlist<IAnimation>* animations){
        _factory = Factory::Instance;
        _drawables = drawables;
        _animations = animations;
        
        _btnBackIsOn = false;
        _btnPlayIsOn = false;
        _btnReplayIsOn = false;
        _isVisible = false;
        
        _onBack = NULL;
        _onReplay = NULL;
        
        _startInTime = 0;
        _endInTime = 0;
        _startOutTime = 0;
        _endOutTime = 0;
        _action = 0;
        
        _background =
            (new Image(_factory->TextureAtlas03, 746, 453, 60, 106))
            ->Position(-5, -5)->Func(ImgCenter_BottomLeft_NoRotation)
            ->Scale(1150.0 / 106.0)->SetInit();
        _screenPad =
            (new Image(_factory->TextureAtlas01, 1, 239, 602, 198))
            ->Position((640 - 602)/2.0, 0)
            ->Func(ImgCenter_BottomLeft_NoRotation)->SetInit();
        _title =
            (new Image(_factory->TextureAtlas05, 618, 331, 254, 52))
            ->Position(640/2.0, 40)->Center(254/2.0, 52/2.0)
            ->Func(ImgCenter_Centered_NoRotation)->SetInit();
        
        _background->textureGroup = 2;
        _background->sfactor = GL_SRC_ALPHA;
        _background->dfactor = GL_ONE_MINUS_SRC_ALPHA;
        _screenPad->textureGroup = 3;
        _screenPad->sfactor = GL_SRC_ALPHA;
        _screenPad->dfactor = GL_ONE_MINUS_SRC_ALPHA;
        
        _btnBack =
            (new Image(_factory->TextureAtlas03, 335, 703, 95, 95))->Position(0, 0)
            ->Func(ImgCenter_Centered_NoRotation)
            ->Center(95/2.0, 95/2.0)->Position(150+1, 120)->SetInit();
        _btnPlay =
            (new Image(_factory->TextureAtlas06, 439, 575, 120, 120))->Position(0, 0)
            ->Func(ImgCenter_Centered_NoRotation)
            ->Center(120/2.0, 120/2.0)->Position(320, 120+14)->SetInit();
        _btnReplay =
            (new Image(_factory->TextureAtlas03, 833, 822, 95, 95))->Position(0, 0)
            ->Func(ImgCenter_Centered_NoRotation)
            ->Center(95/2.0, 95/2.0)->Position(640-150+1, 120)->SetInit();
    }
    void Initialize(){}
    void Update(double time){
        
        if(_action != 0){
            if(_startOutTime == 0){
                _startInTime = 0;
                _endInTime = 0;
                _startOutTime = time;
                _endOutTime = time + 0.25;
            }
        }
        
        if(_startInTime > 0){
            double len = _endInTime - _startInTime;
            double p = (time - _startInTime) / len;
            
            if(p > 1.0){
                p = 1.0;
                _startInTime = 0;
                _endInTime = 0;
            }
            
            _background->Alpha(p*0.7);
            double shift = 200.0;
            p = 1.0 - p;
            _title->Y(_title->initY - shift*p);
            _screenPad->Y(_screenPad->initY - shift*p);
            _btnBack->Y(_btnBack->initY - shift*p);
            _btnPlay->Y(_btnPlay->initY - shift*p);
            _btnReplay->Y(_btnReplay->initY - shift*p);
            
        } else if(_startOutTime > 0) {
            double len = _endOutTime - _startOutTime;
            double p = (time - _startOutTime) / len;
            
            if(p > 1.0){
                p = 1.0;
                _startOutTime = 0;
                _endOutTime = 0;
                
                OnActionTime();
            }
            
            _background->Alpha((1.0 - p)*0.7);
            double shift = 200.0;
            _title->Y(_title->initY - shift*p);
            _screenPad->Y(_screenPad->initY - shift*p);
            _btnBack->Y(_btnBack->initY - shift*p);
            _btnPlay->Y(_btnPlay->initY - shift*p);
            _btnReplay->Y(_btnReplay->initY - shift*p);
        }
        
    }
    void Show(double time, ActionAndState* onBack, ActionAndState* onReplay){
        
        if(_isVisible){
            if(onBack != NULL) delete onBack;
            if(onReplay != NULL) delete onReplay;
            return;
        }
        
        analyticsScreen("pauseInit");
        
        _factory->enableInsectCreation = false;
        
        _onBack = onBack;
        _onReplay = onReplay;
        
        _action = 0;
        _startInTime = time;
        _endInTime = time + 0.25;
        _startOutTime = 0;
        _endOutTime = 0;
        
        _factory->isPaused = true;
        _factory->setEnableInput(false);
        _isVisible = true;
        
        _btnBackIsOn = false;
        _btnPlayIsOn = false;
        _btnReplayIsOn = false;
        
        _drawables->push_back(_background->IsExpired(false));
        _drawables->push_back(_screenPad->IsExpired(false));
        _drawables->push_back(_title->IsExpired(false));
        _drawables->push_back(_btnBack->IsExpired(false));
        _drawables->push_back(_btnPlay->IsExpired(false));
        _drawables->push_back(_btnReplay->IsExpired(false));
        
    }
    /*void Hide(double time){
        _factory->isPaused = false;
        _factory->setEnableInput(true);
        _isVisible = false;
        
        _startInTime = 0;
        _endInTime = 0;
        _startOutTime = time;
        _endOutTime = time + 0.30;
    }*/
    bool IsVisible() { return _isVisible; }
    bool TouchBegan(vec2 touch){
        if(_startInTime > 0 || _endOutTime > 0) return false;
        if(touch.Distance(_btnPlay->Pos()) < BTN_RADIUS){
            PlayButton(true);
            return true;
        }
        if(touch.Distance(_btnBack->Pos()) < BTN_RADIUS){
            BackButton(true);
            return true;
        }
        if(touch.Distance(_btnReplay->Pos()) < BTN_RADIUS){
            ReplayButton(true);
            return true;
        }
        return false;
    }
    bool TouchMoved(vec2 touch){
        if(_startInTime > 0 || _endOutTime > 0) return false;
        if(_btnPlayIsOn && touch.Distance(_btnPlay->Pos()) > BTN_RADIUS){
            PlayButton(false);
            return true;
        }
        if(_btnBackIsOn && touch.Distance(_btnBack->Pos()) > BTN_RADIUS){
            BackButton(false);
            return true;
        }
        if(_btnReplayIsOn && touch.Distance(_btnReplay->Pos()) > BTN_RADIUS){
            ReplayButton(false);
            return true;
        }
        return false;
    }
    bool TouchEnded(vec2 touch){
        if(_startInTime > 0 || _endOutTime > 0) return false;
        if(_btnPlayIsOn && touch.Distance(_btnPlay->Pos()) < BTN_RADIUS*1.9){
            _action = PAUSE_ACTION_PLAY;
            
            PlayButton(false);
            return true;
        }
        if(_btnBackIsOn && touch.Distance(_btnBack->Pos()) < BTN_RADIUS*1.9){
            _action = PAUSE_ACTION_BACK;
            
            BackButton(false);
            return true;
        }
        if(_btnReplayIsOn && touch.Distance(_btnReplay->Pos()) < BTN_RADIUS*1.9){
            _action = PAUSE_ACTION_REPLAY;
            
            ReplayButton(false);
            return true;
        }
        return false;
    }
private:
    void ClearEvents(){
        if(_onBack != NULL) {
            delete _onBack;
            _onBack = NULL;
        }
        if(_onReplay != NULL) {
            delete _onReplay;
            _onReplay = NULL;
        }
    }
    void OnActionTime(){
        
        _factory->enableInsectCreation = true;
        
        _isVisible = false;
        _factory->forceClearExpired = true; 
        _factory->isPaused = false;
        _factory->setEnableInput(true);
        
        if(_action == PAUSE_ACTION_BACK){
            _onBack->Call(0L);
        } else if (_action == PAUSE_ACTION_REPLAY){
            _onReplay->Call(0L);
        }
        
        _action = 0;
        
        _background->IsExpired(true)->PreserveExpired(true);
        _screenPad->IsExpired(true)->PreserveExpired(true);
        _title->IsExpired(true)->PreserveExpired(true);
        _btnBack->IsExpired(true)->PreserveExpired(true);
        _btnPlay->IsExpired(true)->PreserveExpired(true);
        _btnReplay->IsExpired(true)->PreserveExpired(true);
    }
    
    void TakeOffScreen(){
        
    }
    
    void BackButton(bool isOn){
        _btnBackIsOn = isOn;
        if(isOn) _btnBack->RedefineTextureCoords(431, 703, 95, 95);
        else _btnBack->RedefineTextureCoords(335, 703, 95, 95);
    }
    void PlayButton(bool isOn){
        _btnPlayIsOn = isOn;
        if(isOn) _btnPlay->RedefineTextureCoords(560, 575, 120, 120);
        else _btnPlay->RedefineTextureCoords(439, 575, 120, 120);
    }
    void ReplayButton(bool isOn){
        _btnReplayIsOn = isOn;
        if(isOn) _btnReplay->RedefineTextureCoords(929, 822, 95, 95);
        else _btnReplay->RedefineTextureCoords(833, 822, 95, 95);
    }
    
    simlist<Drawable>* _drawables;
    simlist<IAnimation>* _animations;
    Factory* _factory;
    
    Image* _background;
    Image* _screenPad;
    Image* _title;
    Image* _btnBack;
    Image* _btnPlay;
    Image* _btnReplay;
    bool _btnBackIsOn;
    bool _btnPlayIsOn;
    bool _btnReplayIsOn;
    bool _isVisible;
    
    double _startInTime;
    double _endInTime;
    double _startOutTime;
    double _endOutTime;
    
    int _action;
    
    ActionAndState* _onBack;
    ActionAndState* _onReplay;
    
};


#endif
