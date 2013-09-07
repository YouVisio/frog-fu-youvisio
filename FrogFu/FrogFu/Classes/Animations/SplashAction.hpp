//
//  SplashAction.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-09-30.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_SplashAction_hpp
#define FrogFu_SplashAction_hpp

#import <iostream>
#import "BaseAnimation.hpp"
#import <cmath>
#import "Image.hpp"

using namespace std;

class SplashAction : public BaseAnimation {
public:
    SplashAction(bool moveLeaf, Image* leaf, Image* wave1, Image* wave2)
    : BaseAnimation(1.2){
        _moveLeaf = moveLeaf;
        _leaf = leaf;
        _leafOriginalY = 185;
        _wave1 = wave1;
        _wave2 = wave2;
    }
    ~SplashAction(){
        _wave1->Scale(1);
        _wave2->Scale(1);
        _wave1->IsVisible(false);
        _wave2->IsVisible(false);
        if(_moveLeaf){
            _leaf->Y(_leafOriginalY);
        }
        _isFinished = true;
    }
    void Initialize(double time){
        BaseAnimation::Initialize(time);
        
        sysPlaySound("Splash");
        sysSoundVolume("Splash", _moveLeaf ? 0.16 : 0.07);
        
        _wave1->Scale(1);
        _wave2->Scale(1);
        
        _wave1->IsVisible(true);
        _wave2->IsVisible(true);
    }
    void Update(double time){
        if(_isFinished) return;
        
        if(IsPaused(time)){ return; }
        
        double progress = GetProgress(time);
        
        if(_moveLeaf){
            const double spread = 30;
            const double curveture = 1.7;
            const double shiftLeafBy = -10;
            double leafProgress = sysMax(-pow(abs(progress - pow(1/spread, 1/curveture)), curveture) * spread + 1, 0);
            _leaf->Y(_leafOriginalY + leafProgress * shiftLeafBy);
        }
        double change1 = sysMin(sysMax(progress * 1.5, 0), 1);
        _wave1->IsVisible(change1 > 0);
        _wave1->Scale(1 + change1);
        double change2 = sysMin(sysMax((progress - 1 + 1/1.5) * 1.5, 0), 1) ;
        _wave2->IsVisible(change2 > 0);
        _wave2->Scale(1 + change2);
        
        _wave1->Alpha(1.0 - change1);
        _wave2->Alpha(1.0 - change2);
        
    }
    int Type(){ return ANI_SplashAction; }
private:
    bool _moveLeaf;
    double _leafOriginalY;
    Image* _leaf;
    Image* _wave1;
    Image* _wave2;
};
#endif
