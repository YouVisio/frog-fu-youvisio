//
//  InsectMove.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-19.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_InsectMove_hpp
#define FrogFu_InsectMove_hpp

#import "Particle.hpp"
#import "Image.hpp"
#import "Factory.hpp"
#import "BaseAnimation.hpp"
#import "Interfaces.hpp"
#import "ISceneEngine.hpp"
#import "FrogManager.hpp"

class InsectMove : public BaseAnimation{
public:
    InsectMove(ISceneEngine* engine, FrogManager* frogManager, vec2 pos, Particle* particle, Image* image) : BaseAnimation(BILLION){
        _engine = engine;
        _frogManager = frogManager;
        _pos = pos;
        _particle = particle;
        _image = image;
//registerFinalizer(_image->UniqueId(), StaticImageDeleted, this);
        _isWingUp = true;
        _isEyeClosed = false;
        _isTouched = false;
        _initialTextureX = 0;
        _image->TextureX(0);
        _flapSpeed = particle == NULL ? 5 : 3;
        _isCounted = false;
    }
    ~InsectMove(){
        if(_particle != NULL){
            _particle->Age(1.1);
        }
        if(_image != NULL){
            _image->IsExpired(true);
        }
    }
    static void StaticImageDeleted(void* caller, long id){
        ((InsectMove*)caller)->ImageDeleted();
    }
    void ImageDeleted(){
        _image = NULL;
    }
    void Hide(){
        if(_particle != NULL) _particle->Age(1.1);
        if(_image != NULL) _image->IsVisible(false);
    }
    virtual bool IsWasp(){ return false; }
    virtual void Initialize(double time){
        
        _frame = 0;
        
        _isCounted = false;
        
        _timeShift = sysRandom()*5;
        
        _nextBlinkTime = time + sysRandomOfPeriodAsDouble(1, 3);
        
        // in 15% give higher speed
        _speed = sysRandom() > 0.15 ? sysRandomOfPeriodAsDouble(1.8, 2.5) : sysRandomOfPeriodAsDouble(3.5, 5.5);
        
        _isInitialized = true;
    }
    void SetIsTouched(bool b) { _isTouched = b; }
    double InitialTextureX() { return _initialTextureX; }
    virtual void Update(double time){
        if(_isFinished) return;
        
        if(IsPaused(time)){ return; }        
        
        Factory* f = Factory::Instance;
        
        ++_frame;
        
        time += _timeShift;
        
        if(!_isTouched && _particle != NULL){
            _pos.x -= _speed;
        }
        
        vec2 screen = f->frogPlainToScreen(_pos);
        
        if(_image == NULL){
            _isFinished = true;
            return;
        }
        
        if(!_image->isOutOfScreen){
            if(!_isTouched){
                vec2 touch = _engine->GetTouch();
                if(touch.x >= 0){
                    float distanceToTouch = screen.Distance(touch);
                    double correction = IsWasp() ? 1.0 : 1.15;
                    double s = _image->Drawable::Scale();
                    if(distanceToTouch <= (sysMax(_image->Width()*s, _image->Height()*s) / 2.0)*correction){
                        if(f->isWithinJump){
                            _isWingUp = true;
                            _image->TextureX(_initialTextureX + _image->TextureWidth()*4);
                            _image->Color(0.8,1,0.7,0.0);
                            _isTouched = true;
                            _frogManager->SelectedInsect(this, IsWasp(), true);
                        }else{
                            _frogManager->InsectTouchedWithoutJump(this);
                        }
                    }
                }
            }
        
        
//cout.precision(15);
//cout << time << "|" << sysMod(time, 1.0) << "|" << sin(time * Pi * 8) << "|" << sin(sysMod(time, 1.0) * Pi * 8) << endl;
            if(_frame % _flapSpeed == 0){
                if(_isWingUp) _image->TextureX(_image->TextureX() + _image->TextureWidth());
                else _image->TextureX(_image->TextureX() - _image->TextureWidth());
                _isWingUp = !_isWingUp;
            }
            
            if(!_isTouched && time >= _nextBlinkTime){
                if(_isEyeClosed) _image->TextureX(_image->TextureX() - _image->TextureWidth()*2);
                else _image->TextureX(_image->TextureX() + _image->TextureWidth()*2);
                _isEyeClosed = !_isEyeClosed;
                _nextBlinkTime = time +
                    (_isEyeClosed
                         ? sysRandomOfPeriodAsDouble(0.05, 0.3)
                         : sysRandomOfPeriodAsDouble(0.5, 3));
                
                
            }
        }
        
        if(!_isCounted && !IsWasp() && _pos.x < -20){
            _isCounted = true;
            ++f->numberMissed;
        }
        
        if(_pos.x < -100){
            BaseAnimation::Finish();
            return;
        }
        if(!IsFinished() && _particle != NULL){
            /*if(_particle != NULL)*/ 
            if(_image->mark != 9898){
                _particle->Pos(f->frogPlainToMap(_pos));
                _image->Pos(screen);
            }
        }
    }
    void Continue(){ 
        _isTouched = false;
        _image->Color(1,1,1,1);
        _isWingUp = true;
        _isEyeClosed = false;
        _image->TextureX(_initialTextureX);
        _image->mark = 88;
    }
    void Finish(){
        if(_image->mark == 88){
            cout << "now" << endl;
        }
        BaseAnimation::Finish();
        if(_particle != NULL) _particle->Age(1.1);
        _image->IsVisible(false);
    }
    bool IsTouched(){ return _isTouched; }
    int Type(){ return ANI_InsectMove; }
    void* State(){ return _image; }
    void State(void* s){ _image = (Image*)s; }
protected:
    vec2 _pos;
    double _speed;
    Image* _image;
private:
    ISceneEngine* _engine;
    FrogManager* _frogManager;
    Particle* _particle;
    bool _isWingUp;
    bool _isEyeClosed;
    double _nextBlinkTime;
    double _timeShift;
    long _frame;
    bool _isTouched;
    double _initialTextureX;
    int _flapSpeed;
    bool _isCounted;
};

#endif
