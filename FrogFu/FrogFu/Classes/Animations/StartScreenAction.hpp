//
//  PlayPressed.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-11-28.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_StartScreenAction_hpp
#define FrogFu_StartScreenAction_hpp

#import "Image.hpp"
#import "Factory.hpp"
#import "BaseAnimation.hpp"
#import "Interfaces.hpp"
#import "StartScreenState.hpp"

#define PI2 2.0*Pi
#define CIRC_PART PI2/3.0

class StartScreenAction : public BaseAnimation{
public:
    StartScreenAction(bool isInitial, StartScreenState* state) : BaseAnimation(BILLION){
        _isInitial = isInitial;
        _state = state;
        _factory = Factory::Instance;
    }
    void Initialize(double time){
        BaseAnimation::Initialize(time);
        _isInitialized = true;
        _rotationSpeed = 0.0;
        _rotation = 0.0;
        _iconCircleRadius = 0.0;
        _frame = 0;
        
        _introActionStartTime = time - 0.000001;
        _introActionEndTime = time + 1.2;
        
        double degrees45 = Pi/4.0;
        
        _framesWithNoRotation = -1;
        
        RotateImg(_state->BtnClassic, degrees45);
        RotateImg(_state->BtnSprint, degrees45 * 2.0);
        RotateImg(_state->BtnEasyPlay, degrees45 * 3.0);
    }
    virtual void Update(double time){
        
        StartScreenState* s = _state;

        if(_introActionEndTime > 0){
            double progress = CalculateProgress(_introActionStartTime, time, _introActionEndTime);
            

            //_rotationSpeed = sysMax(0.05, -(sign(progress - 0.5) * pow(abs(progress - 0.5) / (0.5 / pow(0.5, 2.0)), 1.0 / 2.0) - 0.5));
            _rotationSpeed = max(0.05, (-pow(progress*1.5,2.0)+1)*1.2);
            
            
            //_iconCircleRadius = exp(-0.5 * (pow(progress,(1.0/3.0)) - 0.5) * (pow(progress,(1.0/3.0)) - 0.5) / pow(0.399, 4)) / (0.399 * sqrt(pow(2.0, 3.14159265359))) + 0.6;
            _iconCircleRadius = 0.6;
            
            if(_isInitial){
                double x = -pow(progress - 1, 3.0) - 1;
                double y = -((10.0*progress*progress) -pow(progress,(1.0/3.0)))*2.0;
                
                s->BtnPlay->Position(s->FrogPos.x - x * 300.0,s->FrogPos.y + y * 300.0);
            }
            
            double p = sysMin(pow(progress, 3)*6, 1.0);
            
            vec2 offScreen = vec2(-60,60);
            
            s->BtnMusic->Pos(offScreen + (s->BtnMusicPos - offScreen) * pow(p,3.0));
            s->BtnEffects->Pos(offScreen + (s->BtnEffectsPos - offScreen) * pow(p,2.5));
            s->BtnAchievements->Pos(offScreen + (s->BtnAchievementsPos - offScreen) * pow(p,2.0));
            s->BtnLeaderboards->Pos(offScreen + (s->BtnLeaderboardsPos - offScreen) * pow(p,1.5));
            s->BtnInfo->Pos(offScreen + (s->BtnInfoPos - offScreen) * p);
            
            // ease out
            p = (33*(progress * (progress * progress))*(progress * progress) + -106*(progress * progress)*(progress * progress) + 126*(progress * (progress * progress)) + -67*(progress * progress) + 15*progress) - pow(progress,1.0/8.0);
            
            s->Title->Pos(s->TitlePos + vec2(0, 200*p));
            
            if(progress > 1.0){
                s->BtnInfo->Pos(s->BtnInfoPos);
                s->BtnMusic->Pos(s->BtnMusicPos);
                s->BtnEffects->Pos(s->BtnEffectsPos);
                s->BtnAchievements->Pos(s->BtnAchievementsPos);
                s->BtnLeaderboards->Pos(s->BtnLeaderboardsPos);
                
                _introActionEndTime = -1;
            }
        }

        
        vec2 center = s->FrogPos;
        center.y -= 30;
        double radius = 100 + _iconCircleRadius*200;
            
            
        vec2 initialPos = center + vec2(0,radius);
        
        --_framesWithNoRotation;
        if(_framesWithNoRotation < 0){
            _rotation += _rotationSpeed*10*DTR;
        }
        
        double rotClassic = sysMod(_rotation, PI2);
        double rotSprint = sysMod(CIRC_PART+_rotation, PI2);
        double rotEasyPlay = sysMod(-CIRC_PART+_rotation, PI2);
        if(rotEasyPlay < 0) rotEasyPlay += PI2;
        
        if(_introActionEndTime < 0 && _framesWithNoRotation < -5 &&
           (IsNearPi(rotClassic) || IsNearPi(rotSprint) || IsNearPi(rotEasyPlay))){
            _framesWithNoRotation = 300;
        }

        rotClassic = UpdateRotation(rotClassic);
        rotSprint = UpdateRotation(rotSprint);
        rotEasyPlay = UpdateRotation(rotEasyPlay);

        s->BtnClassic->Pos(vec2(initialPos).Rotate(center, rotClassic));
        s->BtnSprint->Pos(vec2(initialPos).Rotate(center, rotSprint)); 
        s->BtnEasyPlay->Pos(vec2(initialPos).Rotate(center,rotEasyPlay));
//cout << sysMod(_rotation, 2*Pi) << "|" << CircLocation(sysMod(_rotation, 2*Pi)) << endl;
 
        
        Ellipsify(s->IsClassicHit, s->BtnClassic, s->InsectClassic, center, radius, rotClassic);
        Ellipsify(s->IsSprintHit, s->BtnSprint, s->InsectSprint, center, radius, rotSprint);
        Ellipsify(s->IsEasyPlayHit, s->BtnEasyPlay, s->InsectEasyPlay, center, radius, rotEasyPlay);
        
        
        if(_frame % 5 == 0){
            s->Drawables->sort(SortDrawableFunc, s->Drawables->head->next, s->Drawables->tail);
        }
   

        if(!s->IsClassicHit) s->InsectClassic->Pos(s->BtnClassic->Pos());
        if(!s->IsSprintHit) s->InsectSprint->Pos(s->BtnSprint->Pos());
        if(!s->IsEasyPlayHit) s->InsectEasyPlay->Pos(s->BtnEasyPlay->Pos());
        
        double rotateIconSpeed = -(_rotationSpeed/10.0);
        RotateImg(s->BtnClassic, rotateIconSpeed);
        RotateImg(s->BtnSprint, rotateIconSpeed);
        RotateImg(s->BtnEasyPlay, rotateIconSpeed);
    }
private:
    double IsNearPi(double angle){
        return angle > (Pi - 0.02) && angle < (Pi + 0.02);
    }
    
    static int SortDrawableFunc(Drawable* one, Drawable* two){
        return one->depth > two->depth ? 1 : one->depth < two->depth ? -1 : 0;
    }
    double UpdateRotation(double rotation){
        double coef =  2.2;
        if(rotation <= Pi){
            double x = rotation / Pi;
            x = pow(x, 1.0/coef);
            return x * Pi;
        }else{
            double x = 1.0 - ((rotation - Pi) / Pi);
            x = pow(x, 1.0/coef);
            return (1.0 - x) * Pi + Pi;
        }
        return rotation;
    }
    void Ellipsify(bool isHit, Image* circ, Image* insect, vec2 center, double radius, double rotation){
        double circLoc = CircLocation(rotation);
        double s = 0.30 + 0.701*circLoc;
        
        circ->Scale(s);
        circ->Depth(circLoc * 1000 + 20);
        
        if(!isHit){
            insect->Depth(circ->depth-10);
            insect->Scale(s);
        }
        
        double diffFromCenter = circ->Y() - center.y;
        
        //double x = (pow(elipsificProgress, 0.05) + (1 - pow(1 - elipsificProgress, 1/0.05)))/2.0;
        

        
//cout << x << endl;
        
        circ->Y(center.y + diffFromCenter - diffFromCenter*0.23);
        
    }
    double CircLocation(double angle){
        return angle <= Pi ? angle / Pi : 1.0 - ((angle-Pi)/Pi);
    }
    double CalculateProgress(double start, double now, double end){
        double length = end - start;
        double passed = now - start;
        return passed / length;
    }
    void RotateImg(Image* image, double speed){
        image->Rotation(image->Rotation()+speed);
    }
    
    
    
    Factory* _factory;
    StartScreenState* _state;
    int _framesWithNoRotation;
    bool _isInitial;
    double _introActionStartTime;
    double _introActionEndTime;
    double _rotationSpeed;
    double _rotation;
    double _iconCircleRadius;
    long _frame;
};

#endif
