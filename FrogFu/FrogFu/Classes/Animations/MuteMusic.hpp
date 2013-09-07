//
//  MuteMusic.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-12-25.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_MuteMusic_hpp
#define FrogFu_MuteMusic_hpp

#import "Factory.hpp"
#import "BaseAnimation.hpp"
#import "Interfaces.hpp"

class MuteMusic : public BaseAnimation{
public:
    MuteMusic() : BaseAnimation(1.5){
        _factory = Factory::Instance;
    }
    ~MuteMusic(){
        sysSoundVolume("M1", 0.0);
    }
    void Initialize(double time){
        BaseAnimation::Initialize(time);
    }
    void Update(double time){
        if(_isFinished) return;
        
        if(!_factory->Settings->Music()) return;
        
        double progress = GetProgress(time);
        
        sysSoundVolume("M1", 1.0 - progress);
    }
private:
    Factory* _factory;
};
#endif
