//
//  PointingPose.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-30.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_PointingPose_hpp
#define FrogFu_PointingPose_hpp
#import "Poses.hpp"
#import "Particle.hpp"
#import "Image.hpp"
#import "Factory.hpp"
#import "BaseAnimation.hpp"
#import "Interfaces.hpp"
#import "ISceneEngine.hpp"


class PointingPose : public BaseAnimation{
public:
    PointingPose(Image* body, Image* head) : BaseAnimation(BILLION){
        _body = body;
        _head = head;
        _body->poseID = -1;
        _head->poseID = -1;
        _head->Rotation(0);
//_head->IsVisible(false);
        //_body->Center(86, 131);
        _isBodyMoveReversed = false;
        _isHeadMoveReversed = false;
        Factory* factory = Factory::Instance;
        _body->Texture(factory->TextureAtlas04);
        _head->Texture(factory->TextureAtlas04);
        _tapRepetitions = 0;
    }
    ~PointingPose(){
    }
    void* Caller;
    void (*OnEnd)(void* caller);
    void Initialize(double time){
        _body->Rotation(0);
        _head->Rotation(0);
        InitiateBodyMove(time, 0.5, 0.5,
                         PointingPose::OnBodyTransitionPointRightUp,
                         PointingPose::OnBodyTransitionPointRightUpEnds);
        InitiateHeadMove(time, 0.5, 0.5,
                         PointingPose::OnHeadTransitionLookRightUp,
                         PointingPose::OnHeadTransitionLookRightUpEnds);
        _isInitialized = true;
    }
    void Update(double time){
        if(time >= _bodyNextKeyPointTime){
            _onProcessBodyEnds(this, time);
        }
        else{
            double progress = (time - (_bodyNextKeyPointTime - _bodyMoveLength)) / _bodyMoveLength;
            if(_isBodyMoveReversed) progress = 1.0 - progress;
            _processBody(this, progress);
        }
        if(time >= _headNextKeyPointTime){
            _onProcessHeadEnds(this, time);
        }
        else{
            double progress = (time - (_headNextKeyPointTime - _headMoveLength)) / _headMoveLength;
            if(_isHeadMoveReversed) progress = 1.0 - progress;
            _processHead(this, progress);
        }

        
    }
    int Type(){ return ANI_PointingPose; }
private:
    void InitiateBodyMove(double time, double minLen, double maxLen, void (*f1)(PointingPose* pose, double progress), void (*f2)(PointingPose* pose, double time)){
        _bodyMoveLength = minLen == maxLen ? maxLen : sysRandomOfPeriodAsDouble(minLen, maxLen);
        _bodyNextKeyPointTime = time + _bodyMoveLength;
        _processBody = f1;
        _onProcessBodyEnds = f2;
    }
    void InitiateHeadMove(double time, double minLen, double maxLen, void (*f1)(PointingPose* pose, double progress), void (*f2)(PointingPose* pose, double time)){
        _headMoveLength = minLen == maxLen ? maxLen : sysRandomOfPeriodAsDouble(minLen, maxLen);
        _headNextKeyPointTime = time + _headMoveLength;
        _processHead = f1;
        _onProcessHeadEnds = f2;
    }
    
    Image* _body;
    Image* _head;
    void (*_processBody)(PointingPose* pose, double progress);
    void (*_onProcessBodyEnds)(PointingPose* pose, double time);
    void (*_processHead)(PointingPose* pose, double progress);
    void (*_onProcessHeadEnds)(PointingPose* pose, double time);
    double _bodyNextKeyPointTime;
    double _bodyMoveLength;
    double _headNextKeyPointTime;
    double _headMoveLength;
    bool _isBodyMoveReversed;
    bool _isHeadMoveReversed;
    int _tapRepetitions;
    
    static void OnWait(PointingPose* pose, double progress){}
    static void OnBodyTransitionPointRightUp(PointingPose* pose, double progress){
        poseBodyPointRightUp(pose->_body, progress);
    }
    static void OnBodyTransitionPointRightUpEnds(PointingPose* pose, double time){
        if(pose->_isBodyMoveReversed){
            if(pose->OnEnd != NULL){
                pose->OnEnd(pose->Caller);
            }
        } else{
            pose->InitiateBodyMove(time, 0.2, 0.2,
                                   PointingPose::OnWait,
                                   PointingPose::OnBodyWaitAfterPointRightUpEnds);
        }
    }
    static void OnBodyWaitAfterPointRightUpEnds(PointingPose* pose, double time){
        ++pose->_tapRepetitions;
        pose->_isBodyMoveReversed = false;
        pose->InitiateBodyMove(time, 0.2, 0.2,
                               PointingPose::OnBodyTransitionTapPointing,
                               PointingPose::OnBodyTransitionTapPointingEnds);
    }
    static void OnBodyTransitionTapPointing(PointingPose* pose, double progress){
        poseBodyTapPointing(pose->_body, progress);
    }
    static void OnBodyTransitionTapPointingEnds(PointingPose* pose, double time){
        if(pose->_tapRepetitions >= 8){
            pose->_isBodyMoveReversed = true;
            pose->_isHeadMoveReversed = true; 
            pose->InitiateBodyMove(time, 0.5, 0.5,
                             PointingPose::OnBodyTransitionPointRightUp,
                             PointingPose::OnBodyTransitionPointRightUpEnds);
            pose->InitiateHeadMove(time, 0.5, 0.5,
                             PointingPose::OnHeadTransitionLookRightUp,
                             PointingPose::OnHeadTransitionLookRightUpEnds);
        } else {
            pose->_isBodyMoveReversed = !pose->_isBodyMoveReversed;
            ++pose->_tapRepetitions;
            pose->InitiateBodyMove(time, 0.2, 0.2,
                                   PointingPose::OnBodyTransitionTapPointing,
                                   PointingPose::OnBodyTransitionTapPointingEnds);
        }
    }
    static void OnHeadTransitionLookRightUp(PointingPose* pose, double progress){
        poseHeadLookRightUp(pose->_head, progress);
    }
    static void OnHeadTransitionLookRightUpEnds(PointingPose* pose, double time){
        
    }

};
#endif
