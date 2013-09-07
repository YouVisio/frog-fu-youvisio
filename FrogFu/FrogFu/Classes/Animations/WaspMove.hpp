//
//  WaspMove.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-11-08.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_WaspMove_hpp
#define FrogFu_WaspMove_hpp

#import "Particle.hpp"
#import "Image.hpp"
#import "Factory.hpp"
#import "BaseAnimation.hpp"
#import "Interfaces.hpp"
#import "ISceneEngine.hpp"
#import "FrogManager.hpp"

class WaspMove : public InsectMove{
public:
    WaspMove(ISceneEngine* engine, FrogManager* frogManager, vec2 pos, Particle* particle, Image* image) :
    InsectMove(engine, frogManager, pos, particle, image){
        _originalY = _pos.y;
        _periodShift = sysRandomOfPeriodAsDouble(0, 10);
    }
    void Initialize(double time){
        InsectMove::Initialize(time);
        
        _speed = sysRandomOfPeriodAsDouble(2.0, 6.5);
        
        _periodWidth = sysRandomOfPeriodAsDouble(1.0, 3.0);
    }
    void Update(double time){
        if(_isFinished) return;
        
        if(IsPaused(time)){ return; }
        
        _pos.y = _originalY + sin(time+_periodShift * _periodWidth)*200;
        
        InsectMove::Update(time);        
    }
    
    
    bool IsWasp(){ return true; }
private:
    double _originalY;
    double _periodShift;
    double _periodWidth;
};
#endif
