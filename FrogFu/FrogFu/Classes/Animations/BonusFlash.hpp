//
//  BonusFlash.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-12-10.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_BonusFlash_hpp
#define FrogFu_BonusFlash_hpp

#import "Image.hpp"
#import "TextBlock.hpp"
#import "Factory.hpp"
#import "BaseAnimation.hpp"
#import "Interfaces.hpp"

class BonusFlash : public BaseAnimation{
public:
    BonusFlash(int bonus, simlist<Drawable>* drawables) : BaseAnimation(BILLION){
        _bonus = bonus;
        _paidBonus = 0;
        _progressPerScoreTick = bonus > 0 ? 1.0 / bonus : 0.0;
        
        _drawables = drawables;
        _comboBonus =
            (new Image(_factory->TextureAtlas06, 684, 579, 202, 100))
            ->Position(_bonusX = 500, 300)->Func(ImgCenter_Centered_NoRotation)
            ->Center(197/2.0, 107/2.0)
            ->SetInit();
        
        char ch[10];
        sprintf( ch,"+%d", abs(bonus));
        _number =
            (new TextBlock(_factory->TextureAtlas06, NULL, 0, 0, _factory->getBonusNumFontDescription()))
                ->IsCentered(true)
                ->SetText(ch)
                ->Position(_numberX = (500+30), 300-75)
                ->IsVisible(false);
        
        _inOneStartTime = 0;
        _inOneEndTime = 0;
        _inTwoStartTime = 0;
        _inTwoEndTime = 0;
        _outOneStartTime = 0;
        _outOneEndTime = 0;
        _outTwoStartTime = 0;
        _outTwoEndTime = 0;
    }
    ~BonusFlash(){
        _comboBonus->IsExpired(true);
        _number->IsExpired(true);
    }
    void Initialize(double time){
        BaseAnimation::Initialize(time);
        
        sysPlaySound("Bonus");
        sysSoundVolume("Bonus", 0.1);
        
        SetStartTime(time);
        
        if(sysRandom() > 0.5){
            _comboBonus->X(_bonusX - 375);
            _number->X(_numberX - 375);
        }
        
        _drawables->push_back(_comboBonus);
        _drawables->push_back(_number);
        
    }
    void SetStartTime(double time){
        double x = 1.0;
        _inOneStartTime = time;
        _inOneEndTime = _inOneStartTime + 0.6*x;
        
        _inTwoStartTime = _inOneEndTime - 0.3*x;
        _inTwoEndTime = _inTwoStartTime + 0.6*x;
        
        _outTwoStartTime = _inTwoEndTime + 1.0*x;
        _outTwoEndTime = _outTwoStartTime + 0.6*x;
        
        _outOneStartTime = _outTwoStartTime - 0.3*x;
        _outOneEndTime = _outOneStartTime + 0.6*x;
    }
    virtual bool IsPaused(double time){
        if(_pauseTime == 0){
            if(_factory->isPaused){
                _pauseTime = time;
                return true;
            }
            return false;
        }else{
            if(!_factory->isPaused){
                if(_pauseTime != 0){
                    double diff = _pauseTime - _initialTime;
                    SetStartTime(time - diff);
                }
                _pauseTime = 0;
                return false;
            }
            return true;
        }
    }
    void Update(double time){
        
        
        
        if(IsPaused(time)){ return; }
        if(time > _outTwoEndTime){
            _isFinished = true;
            return;
        }
        
        double x = 0;
        if(IsWithin(time, _inOneStartTime, _inOneEndTime)){
            
           x = CalcProgress(time, _inOneStartTime, _inOneEndTime);

            double s = pow(pow(5, -(pow((x - 0.77), 2) / pow((2 * 0.334197), 2))) * 1.2, 3.0);

            _comboBonus->Scale(s);
            
        } else if(IsWithin(time, _outOneStartTime, _outOneEndTime)){
            
            if(_progressPerScoreTick > 0 && _paidBonus < _bonus){
                double k = CalcProgress(time, _outOneStartTime, _outOneEndTime);
                int bonusPart = ceil(k / _progressPerScoreTick);
                if(bonusPart > _paidBonus){
                    ++_factory->numberHit;
                    ++_paidBonus;
                }
            } 
            
            x = 1.0 - CalcProgress(time, _outOneStartTime, _outOneEndTime);
            
            double s = pow(pow(5, -(pow((x - 0.77), 2) / pow((2 * 0.334197), 2))) * 1.2, 3.0);
            
            _comboBonus->Scale(s);
        }
        
        if(IsWithin(time, _inTwoStartTime, _inTwoEndTime)){
            
            x = CalcProgress(time, _inTwoStartTime, _inTwoEndTime);
            
            double s = pow(pow(5, -(pow((x - 0.77), 2) / pow((2 * 0.334197), 2))) * 1.2, 4.0);
            
            _number->Scale(s)->IsVisible(true);
            
        } else if(IsWithin(time, _outTwoStartTime, _outTwoEndTime)){
            
            x = 1.0 - CalcProgress(time, _outTwoStartTime, _outTwoEndTime);
            
            double s = pow(pow(5, -(pow((x - 0.77), 2) / pow((2 * 0.334197), 2))) * 1.2,4.0);
            
            _number->Scale(s)->IsVisible(true);
            
        }
        
    }
private:
    double CalcProgress(double time, double start, double end){
        return (time - start) / (end - start);
    }
    bool IsWithin(double time, double start, double end){
        return time >= start && time <= end;
    }
    
    simlist<Drawable>* _drawables;
    Image* _comboBonus;
    TextBlock* _number;
    int _bonus;
    int _paidBonus;
    double _inOneStartTime;
    double _inOneEndTime;
    double _inTwoStartTime;
    double _inTwoEndTime;
    double _outOneStartTime;
    double _outOneEndTime;
    double _outTwoStartTime;
    double _outTwoEndTime;
    int _bonusX;
    int _numberX;
    double _progressPerScoreTick;
};

#endif
