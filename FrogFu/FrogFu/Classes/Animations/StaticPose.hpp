//
//  StaticPose.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-28.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_StaticPose_hpp
#define FrogFu_StaticPose_hpp

#import "Poses.hpp"
#import "Particle.hpp"
#import "Image.hpp"
#import "Factory.hpp"
#import "BaseAnimation.hpp"
#import "Interfaces.hpp"
#import "ISceneEngine.hpp"

class StaticPose : public BaseAnimation{
public:
    double minTimeBodyMove;
    double maxTimeBodyMove;
    StaticPose(Image* body, Image* head) : BaseAnimation(BILLION){
        minTimeBodyMove = 1.2;
        maxTimeBodyMove = 3.5;
        _factory = Factory::Instance;
        _body = body;
        _head = head;
        
//cout << "StaticPose:" << time(NULL) << "|" << _body->poseID << endl;
        
    }
    ~StaticPose(){
//cout << "~StaticPose" << endl;
    }
    void Initialize(double time){
//cout << "Init StaticPose" << endl;
        BaseAnimation::Initialize(time);
        Factory::Instance->isWithinJump = false;
        _body->poseID = -1;
        _head->poseID = -1;
        _body->IsVisible(true);
        _head->IsVisible(true);
        _body->Alpha(1);
        _head->Alpha(1);
        _body->Scale(1);
        _head->Scale(1);
        _body->Texture(_factory->TextureAtlas03);
        _head->Texture(_factory->TextureAtlas03);
        _isHeadMoveReversed = false;
        _head->Rotation(0);
        _head->RedefineTextureCoords(0, 1936, 189, 112);
        //_head->Center(74, 110);
        //_body->Center(102 / 2.0, 130 - 10);
        _body->RedefineTextureCoords(0, 1576, 120, 136);
        poseStatBody(_body, 0.0);
        poseStatHeadTrans1(_head, 0.0);
        
        InitiateBodyMove(time, minTimeBodyMove, maxTimeBodyMove,
                               StaticPose::OnBodyTransitionForward,
                               StaticPose::OnBodyTransitionForwardEnds);
        InitiateHeadMove(time, 0.6, 0.9,
                               StaticPose::OnHeadTransition1,
                               StaticPose::OnHeadTransition1Ends);
        InitiateHeadRotation(time, 1.0, 3.0,
                               StaticPose::OnHeadRotateBack,
                               StaticPose::OnHeadRotateBackEnds);
        
    }
    void Update(double time){
        
        if(IsPaused(time)){ return; }

        if(time >= _bodyNextKeyPointTime){
            _onProcessBodyEnds(this, time);
        }
        else{
            double progress = (time - (_bodyNextKeyPointTime - _bodyMoveLength)) / _bodyMoveLength;
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
        
        if(time >= _headRotationNextKeyPointTime){
            _onRotateHeadEnds(this, time);
        }
        else{
            double progress = (time - (_headRotationNextKeyPointTime - _headRotationLength)) / _headRotationLength;
            _rotateHead(this, progress);
        }
        
    }
    int Type(){ return ANI_StaticPose; }
private:
    void (*_processBody)(StaticPose* pose, double progress);
    void (*_onProcessBodyEnds)(StaticPose* pose, double time);
    void (*_processHead)(StaticPose* pose, double progress);
    void (*_onProcessHeadEnds)(StaticPose* pose, double time);
    void (*_rotateHead)(StaticPose* pose, double progress);
    void (*_onRotateHeadEnds)(StaticPose* pose, double time);
    double _bodyNextKeyPointTime;
    double _bodyMoveLength;
    double _headNextKeyPointTime;
    double _headMoveLength;
    double _headRotationNextKeyPointTime;
    double _headRotationLength;
    Image* _body;
    Image* _head;
    bool _isHeadMoveReversed;
    Factory* _factory;
    
    void InitiateBodyMove(double time, double minLen, double maxLen, void (*f1)(StaticPose* pose, double progress), void (*f2)(StaticPose* pose, double time)){
        _bodyMoveLength = minLen == maxLen ? maxLen : sysRandomOfPeriodAsDouble(minLen, maxLen);
        _bodyNextKeyPointTime = time + _bodyMoveLength;
        _processBody = f1;
        _onProcessBodyEnds = f2;
    }
    void InitiateHeadMove(double time, double minLen, double maxLen, void (*f1)(StaticPose* pose, double progress), void (*f2)(StaticPose* pose, double time)){
        _headMoveLength = minLen == maxLen ? maxLen : sysRandomOfPeriodAsDouble(minLen, maxLen);
        _headNextKeyPointTime = time + _headMoveLength;
        _processHead = f1;
        _onProcessHeadEnds = f2;
    }
    void InitiateHeadRotation(double time, double minLen, double maxLen, void (*f1)(StaticPose* pose, double progress), void (*f2)(StaticPose* pose, double time)){
        _headRotationLength = minLen == maxLen ? maxLen : sysRandomOfPeriodAsDouble(minLen, maxLen);
        _headRotationNextKeyPointTime = time + _headRotationLength;
        _rotateHead = f1;
        _onRotateHeadEnds = f2;
    }
    
    static void OnWait(StaticPose* pose, double progress){}
    static void OnBodyTransitionForward(StaticPose* pose, double progress){
        progress = -pow(2.0, 3.0) * pow(abs(progress - 0.5), 3.0) + 1;
        pose->_body->Rotation(progress * 0.125);
    }
    static void OnBodyTransitionForwardEnds(StaticPose* pose, double time){
        pose->InitiateBodyMove(time, 0.2, 1.5,
                               StaticPose::OnWait,
                               StaticPose::OnBodyWaitAfterMovingForwardEnds);
    }
    static void OnBodyTransitionBackward(StaticPose* pose, double progress){
        progress = -pow(2.0, 3.0) * pow(abs(progress - 0.5), 3.0) + 1;
        pose->_body->Rotation(progress * -0.125);
    }
    static void OnBodyTransitionBackwardEnds(StaticPose* pose, double time){
        pose->InitiateBodyMove(time, 0.2, 3.5,
                               StaticPose::OnWait,
                               StaticPose::OnBodyWaitAfterMovingBackwardEnds);
    }
    static void OnBodyWaitAfterMovingForwardEnds(StaticPose* pose, double time){
        pose->InitiateBodyMove(time, pose->minTimeBodyMove, pose->maxTimeBodyMove,
                               StaticPose::OnBodyTransitionBackward,
                               StaticPose::OnBodyTransitionBackwardEnds);
    }
    static void OnBodyWaitAfterMovingBackwardEnds(StaticPose* pose, double time){
        pose->InitiateBodyMove(time, pose->minTimeBodyMove, pose->maxTimeBodyMove,
                               StaticPose::OnBodyTransitionForward,
                               StaticPose::OnBodyTransitionForwardEnds);
    }
    static void OnHeadTransition1(StaticPose* pose, double progress){
//cout.precision(9);
//cout << "OnHeadTransition1:" << progress << endl;
        poseStatHeadTrans1(pose->_head, progress);
    }
    static void OnHeadTransition1Ends(StaticPose* pose, double time){
        pose->InitiateHeadMove(time, 0.9, 2.5,
                               StaticPose::OnWait,
                               StaticPose::OnHeadWaitAfterTransition1Ends);
    }
    static void OnHeadWaitAfterTransition1Ends(StaticPose* pose, double time){
        pose->InitiateHeadMove(time, 0.9, 1.5,
                               StaticPose::OnHeadTransition2,
                               StaticPose::OnHeadTransition2Ends);
    }
    static void OnHeadTransition2(StaticPose* pose, double progress){
        poseStatHeadTrans2(pose->_head, progress);
    }
    static void OnHeadTransition2Ends(StaticPose* pose, double time){
        pose->InitiateHeadMove(time, 0.9, 2.5,
                               StaticPose::OnWait,
                               StaticPose::OnHeadWaitAfterTransition2Ends);
    }
    static void OnHeadWaitAfterTransition2Ends(StaticPose* pose, double time){
        double random = sysRandom();
        if(random > 0.33){
            pose->InitiateHeadMove(time, 0.9, 1.5,
                                   StaticPose::OnHeadTransition3,
                                   StaticPose::OnHeadTransition3Ends);
        }else{
            pose->InitiateHeadMove(time, 0.9, 1.5,
                                   StaticPose::OnHeadTransition1,
                                   StaticPose::OnHeadTransition1Ends);
        }
    }
    static void OnHeadTransition3(StaticPose* pose, double progress){
        poseStatHeadTrans3(pose->_head, progress);
    }
    static void OnHeadTransition3Ends(StaticPose* pose, double time){
        pose->InitiateHeadMove(time, 0.2, 2.5,
                               StaticPose::OnWait,
                               StaticPose::OnHeadWaitAfterTransition3Ends);
    }
    static void OnHeadWaitAfterTransition3Ends(StaticPose* pose, double time){
        if(pose->_isHeadMoveReversed){
            pose->InitiateHeadMove(time, 0.9, 1.5,
                                   StaticPose::OnHeadTransition1,
                                   StaticPose::OnHeadTransition1Ends);
            pose->_isHeadMoveReversed = false;
        } else {
            pose->InitiateHeadMove(time, 0.9, 1.5,
                                   StaticPose::OnHeadTransition3,
                                   StaticPose::OnHeadTransition3Ends);
            pose->_isHeadMoveReversed = true;
        }
    }
    static void OnHeadRotateBack(StaticPose* pose, double progress){
        double x = 1 - (2 * progress - 1) * (2 * progress - 1);
        pose->_head->Rotation(x * -0.12); // 0.17 ~ 10 degrees
    }
    static void OnHeadRotateBackEnds(StaticPose* pose, double time){
        pose->InitiateHeadRotation(time, 1.0, 5.0,
                             StaticPose::OnWait,
                             StaticPose::OnHeadWaitAfterRotateBackEnds);
    }
    static void OnHeadWaitAfterRotateBackEnds(StaticPose* pose, double time){
        pose->InitiateHeadRotation(time, 1.0, 3.0,
                                   StaticPose::OnHeadRotateFront,
                                   StaticPose::OnHeadRotateFrontEnds); 
    }
    static void OnHeadRotateFront(StaticPose* pose, double progress){
        double x = 1 - (2 * progress - 1) * (2 * progress - 1);
        pose->_head->Rotation(x * 0.10); // 0.1221731 = 7 degrees
    }
    static void OnHeadRotateFrontEnds(StaticPose* pose, double time){
        pose->InitiateHeadRotation(time, 1.0, 5.0,
                                   StaticPose::OnWait,
                                   StaticPose::OnHeadWaitAfterRotateFrontEnds);
    }
    static void OnHeadWaitAfterRotateFrontEnds(StaticPose* pose, double time){
        pose->InitiateHeadRotation(time, 1.0, 3.0,
                                   StaticPose::OnHeadRotateBack,
                                   StaticPose::OnHeadRotateBackEnds);
    }
};

#endif
