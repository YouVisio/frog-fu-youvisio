//
//  CatchFlash.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-11-18.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_CatchFlash_hpp
#define FrogFu_CatchFlash_hpp

#import "Image.hpp"
#import "Factory.hpp"
#import "BaseAnimation.hpp"
#import "Interfaces.hpp"

class CatchFlash : public BaseAnimation{
public:
    CatchFlash(simlist<Drawable>* drawables, Image* insect) : BaseAnimation(0.3){
        _factory = Factory::Instance;
        _splash =
            (new Image(_factory->TextureAtlas06, 897, 121, 127, 127))
            ->Position(0, 0)->Func(ImgCenter_Centered_NoRotation)
            ->IsVisible(false)
            ->Center(127/2, 127/2)
            /*->Color(1.0, 0.0, 0.0)*/;
        _insect = insect;
        
        drawables->push_back(_splash);
    }
    ~CatchFlash(){
        _splash->IsVisible(false);
        _splash->IsExpired(true);
    }
    void Initialize(double time){
        BaseAnimation::Initialize(time);
        _isInitialized = true;
        vec2 insectPos = _insect->Pos();
        _target = _factory->screenToFrogPlain(insectPos);
        _splash->Pos(insectPos);
        _splash->Scale(0.3);
        //_splash->Alpha(0.2);
        _splash->IsVisible(true);
    }
    void Update(double time){
        if(_isFinished) return;
        
        double progress = GetProgress(time);
        
        _splash->Scale(_splash->Drawable::Scale() + 0.08);
        double a = -(sign(progress - 0.5) * pow(abs(progress - 0.5) / (0.5 / pow(0.5, 2)), 1 / 2) - 0.5);
        
        
        _splash->Pos(_factory->frogPlainToScreen(_target));
        if(a < 0) a = 0;
        _splash->Alpha(a);
    }
private:
    Factory* _factory;
    Image* _insect;
    Image* _splash;
    vec2 _target;
};

#endif
