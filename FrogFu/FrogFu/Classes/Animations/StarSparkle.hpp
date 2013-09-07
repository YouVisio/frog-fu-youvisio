//
//  StarSparkle.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 13-01-20.
//  Copyright (c) 2013 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_StarSparkle_hpp
#define FrogFu_StarSparkle_hpp

#import <iostream>
#import "BaseAnimation.hpp"
#import "Image.hpp"


using namespace std;

class StarSparkle : public BaseAnimation {
public:
    StarSparkle(simlist<Drawable>* drawables)
    : BaseAnimation(sysRandomOfPeriodAsDouble(1.0, 3.0)){
        _drawables = drawables;
        
        _maxScale = sysRandomOfPeriodAsDouble(0.5, 0.9);
        
        _star =
            (new Image(_factory->TextureAtlas03, 741, 148, 57, 56))
            ->Center(57/2.0, 56/2.0)
            ->Func(ImgCenter_Centered_WithRotation);
    }
    ~StarSparkle(){
        _star->IsExpired(true);
    }
    void Initialize(double time){
        BaseAnimation::Initialize(time);
        if(!_drawables->push_after(_star, AfterTheBackground)){
            cout << "CANNOT FIND BACKGROUND to insert star sparkle animation!" << endl;
            Finish();
            return;
        }
        
        if(Factory::Instance->jumpPosition() < 0.01){
            _position = vec2(sysRandomOfPeriodAsInt(40, 600), sysRandomOfPeriodAsInt(790, 1100));
        } else {
            _position = vec2(sysRandomOfPeriodAsInt(40, 600), sysRandomOfPeriodAsInt(790, 1750));
        }
        
       
        if(vec2(340,1015).Distance(_position) < 100){
            _position.y = _position.y + 300;
        }
    }
    void Update(double time){
        if(_isFinished){ return; }
        
        _star->Pos(_factory->bgPlainToScreen(_position));
        
        double x = GetProgress(time);
        
        x = -((x-0.5)*(x-0.5))*(2*2) + 1.0;
        
        _star->Scale(x*_maxScale);
        
    }
    int Type(){ return ANI_StarSparkle; }
    
private:
    static bool AfterTheBackground(Drawable* img){
        return img->mark == MARK_BACKGROUND;
    }
    
    simlist<Drawable>* _drawables;
    Image* _star;
    double _maxScale;
    vec2 _position;
};

#endif
