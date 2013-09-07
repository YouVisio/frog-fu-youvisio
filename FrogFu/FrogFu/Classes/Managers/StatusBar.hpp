//
//  StatusBar.h
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-11-22.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_StatusBar_hpp
#define FrogFu_StatusBar_hpp

#import <iostream>
#import "BaseManager.hpp"
#import "Interfaces.hpp"
#import "Factory.hpp"
#import "Image.hpp"
#import "TextBlock.hpp"
#import "IAnimation.hpp"

using namespace std;

#define NUM_TO_SWITCH_TO_PERCENTAGE 100

class StatusBar : BaseManager {
public:
    StatusBar(simlist<Drawable>* drawables){
        _factory = Factory::Instance;
        _drawables = drawables;
        _barBg =
            (new Image(_factory->TextureAtlas03, 335, 650, 461, 50))
            ->Position(_factory->HALF_SCREEN_WIDTH, _factory->SCREEN_HEIGHT - 30)
            ->Func(ImgCenter_Centered_NoRotation)
            ->Center(461/2.0, 50/2.0);
        _misHead =
            (new Image(_factory->TextureAtlas03, 741, 561, 7, 23))
            ->Position(_factory->HALF_SCREEN_WIDTH, _factory->SCREEN_HEIGHT - 30)
            ->Func(ImgCenter_Centered_NoRotation)
            ->Center(7, 23/2.0);
        _misBody =
            (new Image(_factory->TextureAtlas03, 750, 561, 10, 23))
            ->Position(_factory->HALF_SCREEN_WIDTH, _factory->SCREEN_HEIGHT - 30)
            ->Func(ImgCenter_Centered_NoRotation)
            ->Center(10, 23/2.0);
        _hitHead =
            (new Image(_factory->TextureAtlas03, 774, 561, 7, 23))
            ->Position(_factory->HALF_SCREEN_WIDTH, _factory->SCREEN_HEIGHT - 30)
            ->Func(ImgCenter_Centered_NoRotation)
            ->Center(0, 23/2.0);
        _hitBody =
            (new Image(_factory->TextureAtlas03, 762, 561, 10, 23))
            ->Position(_factory->HALF_SCREEN_WIDTH, _factory->SCREEN_HEIGHT - 30)
            ->Func(ImgCenter_Centered_NoRotation)
            ->Center(0, 23/2.0);
        
        _score =
            (new Image(_factory->TextureAtlas03, 743, 586, 78, 19))
            ->Position(_factory->SCREEN_WIDTH - 50, _factory->SCREEN_HEIGHT - 38)
            ->Func(ImgCenter_Centered_NoRotation)
            ->Center(78/2.0, 19/2.0);
        
        _best =
            (new Image(_factory->TextureAtlas03, 743, 607, 63, 20))
            ->Position(_score->X(), _score->Y() - 80)
            ->Func(ImgCenter_Centered_NoRotation)
            ->Center(43/2.0, 14/2.0);
        
        _numScore =
            (new TextBlock(_factory->TextureAtlas03, "0", _factory->SCREEN_WIDTH - 10, _score->Y() - 35,
                          _factory->getScoreNumFontDescription()))
                          ->IsRightAligned(true);
        
        _numBest =
            (new TextBlock(_factory->TextureAtlas03, "0", _factory->SCREEN_WIDTH - 10, _best->Y() - 35,
                          _factory->getScoreBestNumFontDescription()))
                          ->IsRightAligned(true);
        
        _numTime =
            (new TextBlock(_factory->TextureAtlas03, "1:30", 0, _factory->SCREEN_HEIGHT - 45,
                          _factory->getScoreTimeNumFontDescription()));
        
//        _hitLabel =
//            new TextBlock(_factory->TextureAtlas01, " ", 100, 100,
//                          _factory->getInitialFontDescription());
//        _hitLabel->Position(_factory->HALF_SCREEN_WIDTH+115, _factory->SCREEN_HEIGHT - 42)
//                 ->Color(1, 0.3, 0.1, 0);
//        _misLabel =
//            new TextBlock(_factory->TextureAtlas01, " ", 100, 100,
//                          _factory->getInitialFontDescription());
//        _misLabel->Position(_factory->HALF_SCREEN_WIDTH-170, _factory->SCREEN_HEIGHT - 42)
//                 ->Color(1, 0.3, 0.1, 0);
        
//        _star =
//            (new Image(_factory->TextureAtlas03, 861, 579, 27, 26))
//            ->Position(_barBg->X(), _barBg->Y() + 18)
//            ->Func(ImgCenter_Centered_NoRotation)
//            ->Center(27/2.0, 26/2.0);
        _lightning =
            (new Image(_factory->TextureAtlas03, 836, 578, 18, 29))
            ->Position(_barBg->X(), _barBg->Y() - 23)
            ->Func(ImgCenter_Centered_NoRotation)
            ->Center(18/2.0, 29/2.0);
        
        _maxHit = 0;
        _pauseTime = 0;
        _numToSwitchToPerc = NUM_TO_SWITCH_TO_PERCENTAGE;
        _onTimeOver = NULL;
        _onMaxMissed = NULL;
        ResetState();
    }
    void Initialize(double time, double bestScore, int secondsRun, ActionAndState* onTimeOver, double percMissedLimit, ActionAndState* onMaxMissed){
        
        _lastTime = time;
        _secondsLeft = secondsRun;
        _percMissedLimit = percMissedLimit;
        _onTimeOver = onTimeOver;
        _onMaxMissed = onMaxMissed;
        _maxHit = 0;
        _pauseTime = 0;
        
        _drawables->push_back(_barBg->IsExpired(false));
        _drawables->push_back(_misHead->IsExpired(false));
        _drawables->push_back(_misBody->IsExpired(false));
        _drawables->push_back(_hitHead->IsExpired(false));
        _drawables->push_back(_hitBody->IsExpired(false));
//        _drawables->push_back(_hitLabel->IsExpired(false));
//        _drawables->push_back(_misLabel->IsExpired(false));
        _drawables->push_back(_score->IsExpired(false));
        _drawables->push_back(_numScore->IsExpired(false));
//        _drawables->push_back(_star->IsExpired(false));
//        _star->IsVisible(false);
        if(percMissedLimit > 0){
            _drawables->push_back(_lightning->IsExpired(false));
            _lightning->X(_factory->HALF_SCREEN_WIDTH - 12 - 164.0 * (percMissedLimit / 100.0));
        }
        
        if(_secondsLeft >= 0){
            _drawables->push_back(_numTime->IsExpired(false));
        }
        _drawables->push_back(_best->IsExpired(false));
        _drawables->push_back(_numBest->IsExpired(false));
        
        if(bestScore > 0){
            char ch[10];
            sprintf( ch,"%d", (int)bestScore);
            _numBest->SetText(ch);
            _numBest->IsVisible(true);
            _best->IsVisible(true);
        } else {
            _numBest->IsVisible(false);
            _best->IsVisible(false);
        }
        
        
        
        _misBody->IsVisible(false);
        _misHead->IsVisible(false);
        _hitBody->IsVisible(false);
        _hitHead->IsVisible(false);
    }
    void TakeOffScreen(){
        ResetState();
        
        _barBg->IsExpired(true)->PreserveExpired(true);
        _misHead->IsExpired(true)->PreserveExpired(true);
        _misBody->IsExpired(true)->PreserveExpired(true);
        _hitHead->IsExpired(true)->PreserveExpired(true);
        _hitBody->IsExpired(true)->PreserveExpired(true);
//        _hitLabel->IsExpired(true)->PreserveExpired(true);
//        _misLabel->IsExpired(true)->PreserveExpired(true);
        _score->IsExpired(true)->PreserveExpired(true);
        _numScore->IsExpired(true)->PreserveExpired(true);
        _numTime->IsExpired(true)->PreserveExpired(true);
        _best->IsExpired(true)->PreserveExpired(true);
        _numBest->IsExpired(true)->PreserveExpired(true);
//        _star->IsExpired(true)->PreserveExpired(true);
        _lightning->IsExpired(true)->PreserveExpired(true);
    }
    bool IsPaused(double time){
        if(_pauseTime == 0){
            if(_factory->isPaused){
                _pauseTime = time;
                return true;
            }
            return false;
        }else{
            if(!_factory->isPaused){
                if(_pauseTime != 0){
                    double diff = _pauseTime - _lastTime;
                    _lastTime = time - diff;
                }
                _pauseTime = 0;
                return false;
            }
            return true;
        }
    }
    void Update(double time){
        
        if(IsPaused(time)) return;
        
        ProcessTime(time);
        
        char ch2[10];
        sprintf( ch2,"%d", _factory->numberHit);
        _numScore->SetText(ch2);
        
        
        int all =  _factory->numberHit + _factory->numberMissed;
        if(all <= 0) all = 1;
        else if(all < _numToSwitchToPerc) all = _numToSwitchToPerc;
        double progressHit = 0.0;
        double progressMissed = 0.0;
        
        
        progressHit = sysMin((_factory->numberHit / (double)all), 0.99);
        progressMissed = sysMin((_factory->numberMissed / (double)all),0.99);
        
        if(progressHit > _maxHit) _maxHit = progressHit;
        
//        int hit = (int)round(progressHit*100, 0);
        int miss = (int)round(progressMissed*100, 0);
        
        if(miss >= _percMissedLimit && _onMaxMissed != NULL){
            _onMaxMissed->Call(0); 
        }
        
//        if(hit > 0){
//            if(!_star->Drawable::IsVisible()) _star->IsVisible(true);
//            _star->X(_factory->HALF_SCREEN_WIDTH + 164.0 * _maxHit);
//        }
        
//        if(miss < 10){
//            if(_misLabel->X() != _factory->HALF_SCREEN_WIDTH-160) _misLabel->X(_factory->HALF_SCREEN_WIDTH-160);
//        }else{
//            if(_misLabel->X()  != _factory->HALF_SCREEN_WIDTH-170) _misLabel->X(_factory->HALF_SCREEN_WIDTH-170);
//        }
//        
//        if(hit < 10){
//            if(_hitLabel->X() != _factory->HALF_SCREEN_WIDTH+125) _hitLabel->X(_factory->HALF_SCREEN_WIDTH+125);
//        }else{
//            if(_hitLabel->X() != _factory->HALF_SCREEN_WIDTH+115) _hitLabel->X(_factory->HALF_SCREEN_WIDTH+115);
//        }
        
        
//        if(all == _numToSwitchToPerc || all == 1){
//            char ch3[10];
//            sprintf( ch3,"%d", hit);
//            _hitLabel->SetText(ch3);
//            char ch4[10];
//            sprintf( ch4,"%d", miss);
//            _misLabel->SetText(ch4);
//        } else {
//            char ch3[10];
//            sprintf( ch3,"%d%%", hit);
//            _hitLabel->SetText(ch3);
//            char ch4[10];
//            sprintf( ch4,"%d%%", miss);
//            _misLabel->SetText(ch4);
//        }
        
        
        if(progressHit > 0){
            // show only hit head
            if(progressHit <= 0.01){
                if(!_hitHead->Drawable::IsVisible()) _hitHead->IsVisible(true);
                if( _hitBody->Drawable::IsVisible()) _hitBody->IsVisible(false);
                _hitHead->Position(_factory->HALF_SCREEN_WIDTH - 6, _factory->SCREEN_HEIGHT - 31);
            }else{
                if(!_hitHead->Drawable::IsVisible()) _hitHead->IsVisible(true);
                if(!_hitBody->Drawable::IsVisible()) _hitBody->IsVisible(true);
                _hitBody->Position(_factory->HALF_SCREEN_WIDTH - 6, _factory->SCREEN_HEIGHT - 31);
                _hitBody->ScaleX(16.4 * progressHit);
                _hitHead->Position(_factory->HALF_SCREEN_WIDTH - 6 + (164 * progressHit)-2, _factory->SCREEN_HEIGHT - 31);
            }
        }
        if(progressMissed > 0){
            // show only hit head
            if(progressMissed <= 0.01){
                if(!_misHead->Drawable::IsVisible()) _misHead->IsVisible(true);
                if( _misBody->Drawable::IsVisible()) _misBody->IsVisible(false);
                _misHead->Position(_factory->HALF_SCREEN_WIDTH - 6, _factory->SCREEN_HEIGHT - 31);
            }else{
                if(!_misHead->Drawable::IsVisible()) _misHead->IsVisible(true);
                if(!_misBody->Drawable::IsVisible()) _misBody->IsVisible(true);
                _misBody->Position(_factory->HALF_SCREEN_WIDTH - 6, _factory->SCREEN_HEIGHT - 31);
                _misBody->ScaleX(16.4 * progressMissed);
                _misHead->Position(_factory->HALF_SCREEN_WIDTH - 6 - (164 * progressMissed - 3.0*progressMissed)+2, _factory->SCREEN_HEIGHT - 31);
            }
        }
        
        
    }
private:
    bool ProcessTime(double time){
        if(_secondsLeft > 0){
            double diff = time - _lastTime;
            if(diff > 0.9999 && _secondsLeft >= 0){
                --_secondsLeft;
                _lastTime = time;
                if(_secondsLeft == 0 && _onTimeOver != NULL){
                    _onTimeOver->Call(0);
                }
            }
            int minutes = _secondsLeft >= 60 ? floor(_secondsLeft / 60.0) : 0;
            int seconds = _secondsLeft - minutes * 60;
            
            char ch1[10];
            if(seconds < 10){
                sprintf( ch1,"%d:0%d", minutes, seconds);
            } else {
                sprintf( ch1,"%d:%d", minutes, seconds);
            }
            _numTime->SetText(ch1);
            
            if(_secondsLeft < 0){
                return false;
            }
            return true;
        }
        return false;
    }
    void ResetState(){
        _lastTime = 0;
        _percMissedLimit = -1;
        if(_onMaxMissed != NULL) delete _onMaxMissed;
        _onMaxMissed = NULL;
        _secondsLeft = -1;
        if(_onTimeOver != NULL) delete _onTimeOver;
        _onTimeOver = NULL;
    }
    
    Factory* _factory;
    simlist<Drawable>* _drawables;
    Image* _barBg;
    Image* _hitHead;
    Image* _hitBody;
    Image* _misHead;
    Image* _misBody;
//    Image* _star;
    Image* _lightning;
//    TextBlock* _hitLabel;
//    TextBlock* _misLabel;
    int _secondsLeft;
    ActionAndState* _onTimeOver;
    double _lastTime;
    ActionAndState* _onMaxMissed;
    double _percMissedLimit;
    double _pauseTime;
    double _maxHit;
    
    int _numToSwitchToPerc;
    Image* _score;
    Image* _best;
    TextBlock* _numScore;
    TextBlock* _numBest;
    TextBlock* _numTime;
};

#endif
