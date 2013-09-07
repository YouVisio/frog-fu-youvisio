//
//  JumpDownPose.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-09-08.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_JumpDownPose_hpp
#define FrogFu_JumpDownPose_hpp

#import "Poses.hpp"
#import "Particle.hpp"
#import "Image.hpp"
#import "Factory.hpp"
#import "BaseAnimation.hpp"
#import "Interfaces.hpp"
#import "ISceneEngine.hpp"

using namespace std;

class JumpDownPose : public BaseAnimation{
public:
    JumpDownPose(Image* body, Image* head, FuncAndState<int>* onJumpEnds, FuncAndState<int>* onTouchGround) : BaseAnimation(BILLION){
        _body = body;
        _head = head;
        _factory = Factory::Instance;
        _onJumpEnds = onJumpEnds;
        _onTouchGround = onTouchGround;
    }
    ~JumpDownPose(){
        _head->Rotation(0);
        _body->Rotation(0);
        if(_onJumpEnds != NULL){
            delete _onJumpEnds;
            _onJumpEnds = NULL;
        }
        if(_onTouchGround != NULL){
            delete _onTouchGround;
            _onTouchGround = NULL;
        }
    }
    void Initialize(double time){
        BaseAnimation::Initialize(time);
        _body->Texture(_factory->TextureAtlas05);
        _head->Texture(_factory->TextureAtlas06);
        _body->poseID = -1;
        _head->poseID = -1;
        poseBodyJumpDown(_body, 0.0);
        poseHeadEatingInsects(_head, -1, 0, 0);
        _timePostJumpDownStarts = -1;
        _factory->isWithinJump = true;
    }
    void Update(double time){
        
        if(IsPaused(time)){ return; }
//cout << "JDP:" << _factory->jumpPosition() << endl;
        if(_isFinished) return;
//if(_body->poseID == 806) return;
        if(_timePostJumpDownStarts < 0){
            double progress = pow(1.0 - _factory->jumpPosition(), 1.9);
            
            poseBodyJumpDown(_body, progress);
            
            double rotation = _factory->jumpRotation * _factory->jumpPosition();
            _body->Rotation(rotation);
            _head->Rotation(rotation / 1.3);
            
            _factory->bodyPos = vec2(_factory->jumpInitBodyPos.x, _factory->jumpInitBodyPos.y + _factory->frogPlainShiftY);
            if(_factory->jumpPosition() <= 0){
                _timePostJumpDownStarts = time;
                _timePostJumpDownEnds = time + 0.6;
                poseBodyPostJumpDown(_body, 0.0);
                
                _onTouchGround->Call();
                
                if(_body->poseID == 806){
                    _factory->bodyPos = _factory->staticBodyPos;
                    //_body->Y(_factory->bodyPos.y);
                }
            }
        } else {
            if(_factory->isWithinJump){
                _factory->isWithinJump = false;
                _body->Texture(_factory->TextureAtlas05);
                _head->Texture(_factory->TextureAtlas05);
            }
            
            double allDiff = _timePostJumpDownEnds - _timePostJumpDownStarts;
            double currDiff = time - _timePostJumpDownStarts;
            double progress = currDiff / allDiff;
            if(progress > 1.0){
                _isFinished = true;
                progress = 1.0;
            }
            poseBodyPostJumpDown(_body, progress);
            double headProgress = -4.0 * pow(abs(progress - 0.5), 2) + 1.0;
            poseHeadDucking(_head, headProgress);
            if(_isFinished){
                _onJumpEnds->Call();
            }
        }
    }
    int Type(){ return ANI_JumpDownPose; }
private:
    Image* _body;
    Image* _head;
    Factory* _factory;
    FuncAndState<int>* _onJumpEnds;
    FuncAndState<int>* _onTouchGround;
    double _timePostJumpDownStarts;
    double _timePostJumpDownEnds;
};
#endif
