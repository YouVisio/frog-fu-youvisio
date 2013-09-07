//
//  JumpingPose.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-09-06.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_JumpUpPose_hpp
#define FrogFu_JumpUpPose_hpp

#import "Poses.hpp"
#import "Particle.hpp"
#import "Image.hpp"
#import "Factory.hpp"
#import "BaseAnimation.hpp"
#import "Interfaces.hpp"
#import "ISceneEngine.hpp"

using namespace std;

class JumpUpPose : public BaseAnimation{
public:
    JumpUpPose(Image* body, Image* head, double strength, FuncAndState<int>* onStartJump, FuncAndState<int>* onJumpPicks)
    : BaseAnimation(BILLION){
        _factory = Factory::Instance;
        _body = body;
        _head = head;
        _strength = strength;
        _onStartJump = onStartJump;
        _onJumpPicks = onJumpPicks;
    }
    ~JumpUpPose(){
        if(_onStartJump != NULL){
            delete _onStartJump;
            _onStartJump = NULL;
        }
        if(_onJumpPicks != NULL){
            delete _onJumpPicks;
            _onJumpPicks = NULL;
        }
    }
    void Initialize(double time){
//cout << "JumpUpPose " << endl;
        _body->Texture(_factory->TextureAtlas05);
        _head->Texture(_factory->TextureAtlas05);
        _factory->bodyPos = _factory->staticBodyPos;// - vec2(0,-100);//vec2(320 - 73, 300);
        _body->poseID = -1;
        _head->poseID = -1;
        poseBodyPreJumpUp(_body, 0.0);
        poseHeadDucking(_head, 1.0);
        _jumpStarted = _jumpPicked = false;
        _factory->jumpRotation = DTR * sysRandomOfPeriodAsDouble(0, 50*_strength) * (sysRandom()>0.5 ? 1 : -1);
        
        BaseAnimation::Initialize(time);
        _timeJumpStarts = _initialTime + 0.3;
    }
    void Update(double time){
        
        if(IsPaused(time)){ return; }
        
        if(_jumpPicked) return;
        if(time < _timeJumpStarts){
            double progress = (time - _initialTime) / (_timeJumpStarts - _initialTime);
            poseBodyPreJumpUp(_body, progress);
            poseHeadDucking(_head, 1.0 - progress);
            return;
        }
        
        if(!_jumpStarted){
            _factory->bodyPos = _factory->staticBodyPos - vec2(0,-100);//vec2(320 - 73, 300);
            _jumpStarted = true;
            _onStartJump->Call();
            _factory->isWithinJump = true;
            _factory->bodyPos = _factory->jumpInitBodyPos;
            _head->Texture(_factory->TextureAtlas06);
            poseHeadEatingInsects(_head, -1, 0, 0);
        }
        
        _factory->bodyPos = vec2(_factory->jumpInitBodyPos.x, _factory->jumpInitBodyPos.y + _factory->frogPlainShiftY);
        
        poseBodyJumpUp(_body, _factory->jumpPosition());
        
        double rotation = _factory->jumpRotation * _factory->jumpPosition();
        _body->Rotation(rotation);
        _head->Rotation(rotation / 1.3);
        
        if(!_jumpPicked && _factory->jumpProgress() >= 0.5){
            _jumpPicked = _isFinished = true;
            _onJumpPicks->Call();
        }
    }
    int Type(){ return ANI_JumpUpPose; }
private:
    Image* _body;
    Image* _head;
    Factory* _factory;
    double _strength;
    double _timeJumpStarts;
    FuncAndState<int>* _onStartJump;
    FuncAndState<int>* _onJumpPicks;
    bool _jumpStarted;
    bool _jumpPicked;
};

#endif
