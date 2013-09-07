//
//  Stung.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-11-09.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_Stung_hpp
#define FrogFu_Stung_hpp

#import "Poses.hpp"
#import "Image.hpp"
#import "Factory.hpp"
#import "BaseAnimation.hpp"
#import "Interfaces.hpp"

#define STUNG_LEN 2.8

class Stung : public BaseAnimation{
public:
    Stung(Image* body, Image* head, Image* background, Image* wasp, Image* minusPoints, bool animateMinusPoints) : BaseAnimation(STUNG_LEN){
        _factory = Factory::Instance;
        _body = body;
        _head = head;
        _background = background;
        _wasp = wasp;
        _minusPoints = minusPoints;
        _animateMinusPoints = animateMinusPoints;
    }
    ~Stung(){
        if(OnEnd != NULL) OnEnd(OnEndState);
// cout << "~Stung()" << endl;
        _background->IsVisible(false);
        _body->IsVisible(false);
        _head->IsVisible(false);
        _wasp->IsVisible(false);
        _factory->setEnableInput(true); 
        _minusPoints->IsVisible(false);
        _factory->isPaused = false;
    }
    void Initialize(double time){
        
        sysPlaySound("Laughter");
        sysSoundVolume("Laughter", 0.2);
        
        
        _factory->isPaused = true;
        _factory->enableInsectCreation = false;
        _factory->setEnableInput(false);
        _factory->bodyPos = _factory->jumpInitBodyPos;
        BaseAnimation::Initialize(time);
        poseBodyStung(_body, 0.0);
        poseHeadStung(_head, 0.0);
        _body->Alpha(1); 
        _head->Alpha(1);  
        _wasp->Alpha(1);
        _background->IsVisible(true);
        _background->Alpha(1);
        _wasp->IsVisible(true);
        _body->Texture(_factory->TextureAtlas05);
        _head->Texture(_factory->TextureAtlas06);
        _wasp->TextureX(0);
        _timeEnds = time + STUNG_LEN;
        _timeStartsFadeIn = _timeEnds - 1.8;
        
        _frame = 0;
        _isWingDown = true;
        
        //_factory->isPaused = true;
    }
    void Update(double time){
//_factory->printIfDiff(_body->Pos().y);
//_factory->printIfDiff2(Factory::Instance->bodyPos.y);
        
        //if(IsPaused(time)){ return; }
        
        ++_frame;
        
        
        poseBodyStung(_body, 0.0);
        poseHeadStung(_head, 0.0);
        
        if(_frame % 3 == 0){
            if(_isWingDown) _wasp->TextureX(_wasp->TextureX() + _wasp->TextureWidth());
            else _wasp->TextureX(_wasp->TextureX() - _wasp->TextureWidth());
            _isWingDown = !_isWingDown;
        }
        
        double progress = GetProgress(time);
        
        if(_animateMinusPoints){
            if(progress > 0.3 && progress <= 0.5){
                _minusPoints->IsVisible(true);
                
                double x = (progress - 0.3) / 0.2;
                
                double s = pow(pow(5, -(pow((x - 0.77), 2) / pow((2 * 0.334197), 2))) * 1.2, 3.0);

                _minusPoints->Scale(s*s);
            }
            if(progress > 0.7 && progress <= 0.9){
                double x = (progress - 0.7) / 0.2;
                
                x = 1.0 - x;
                
                double s = pow(pow(5, -(pow((x - 0.77), 2) / pow((2 * 0.334197), 2))) * 1.2, 3.0);

                _minusPoints->Scale(s*s);
                
            }
        }
        
        
        double shift = sin(progress*60) * 15;
        vec2 bodyPos = _body->Pos();
        vec2 p = vec2(bodyPos.x, bodyPos.y + 250);
        p = p.Rotate(bodyPos, -_factory->waspPositionAngle);
        _wasp->Position(p.x, p.y + shift);
        
        double scale = 1 + sin(progress*25) * 0.3;
        _body->Scale(scale);
        _head->Scale(scale);
        
         double rotation = sin(progress*12) * 0.3;
        _body->Rotation(rotation);
        _head->Rotation(rotation);
        
        if(_timeStartsFadeIn < time){
            double fade = 1.0 - (time - _timeStartsFadeIn) / (_timeEnds - _timeStartsFadeIn);
            _body->Alpha(fade/2.5);
            _head->Alpha(fade);
            _wasp->Alpha(fade);
            _background->Alpha(fade);
            _background->Color(1.0, 1.0, 0.2);
        }else{
            double color = sin(progress*20) * 0.5;
            _background->Color(1.0, 1.0, 0.5+color);
        }
    }
    int Type(){ return ANI_Stung; }
    void (*OnEnd)(void*);
    void* OnEndState;
private:
    Factory* _factory;
    Image* _wasp;
    Image* _body;
    Image* _head;
    Image* _background;
    Image* _minusPoints;
    
    long _frame;
    bool _isWingDown;
    
    double _timeStartsFadeIn;
    double _timeEnds;
    bool _animateMinusPoints;
};
#endif
