//
//  DukingPose.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-09-03.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_DuckingPose_hpp
#define FrogFu_DuckingPose_hpp

#import "Poses.hpp"
#import "Particle.hpp"
#import "Image.hpp"
#import "Factory.hpp"
#import "BaseAnimation.hpp"
#import "Interfaces.hpp"
#import "ISceneEngine.hpp"

using namespace std;

class DuckingPose : public BaseAnimation{
public:
    DuckingPose(Image* body, Image* head) : BaseAnimation(BILLION){
        _numFramesTransition = 6; // not less than 2!!!
        _body = body;
        _head = head;
        _factory = Factory::Instance;
       
    }
    ~DuckingPose(){
//cout << "~DuckingPose " << endl;
    }
    void* Caller;
    void (*OnTransitionBackEnd)(void* caller);
    void StartTransitionOut(void* callerObj, void (*onEnd)(void* caller)){
        Caller = callerObj;
        OnTransitionBackEnd = onEnd;
        OnTransitionOutStarts();
        //OnTransitionOutEnds();
    }
    void Initialize(double time){
        BaseAnimation::Initialize(time);
        bool isWithinJump = _body->poseID >= 700;
        _body->poseID = -1;
        _head->poseID = -1;
//cout << "DuckingPose " << isWithinJump << endl;
        if(isWithinJump) OnTransitionInEnds(); // within jump
        else OnTransitionInStarts();
    }
    void Update(double time){
        if(_transitionFrameIn < _numFramesTransition){
            double part = _transitionFrameIn / (double)_numFramesTransition;
            ++_transitionFrameIn;
            if(_isTransitionReversed){
                part = 1.0 - part;
            }
//cout << part << "|" << _transitionFrameIn << "|" << _isTransitionReversed << endl;
            poseBodyTransitionToDucking(_body, part);
            poseHeadTransitionToDucking(_head, part);
            if(_transitionFrameIn >= _numFramesTransition){
                if(_isTransitionReversed){
                    OnTransitionOutEnds();
                } else {
                    OnTransitionInEnds();
                }
            }
            return;
        }
        Factory* f = _factory;
        
        if(_springDegree != f->springDegree){
            _springDegree = f->springDegree;
            poseBodyDucking(_body, f->springDegree);
            poseHeadDucking(_head, f->springDegree);
        }
    }
    int Type(){ return ANI_DuckingPose; }
private:
    int _numFramesTransition; // not less than 2!!!
    int _transitionFrameIn;
    bool _isTransitionReversed;
    Factory* _factory;
    Image* _head;
    Image* _body;
    double _springDegree;
    void OnTransitionInStarts(){
        _transitionFrameIn = 0;
        _isTransitionReversed = false;
        _head->Texture(_factory->TextureAtlas03);
        _body->Texture(_factory->TextureAtlas03);
        _head->Rotation(0);
        _body->Rotation(0);
        //_head->RedefineTextureCoords(0, 1712, 189, 112);
        //_body->Center(127 / 2.0, 136 / 2.0);
        //_body->RedefineTextureCoords(0, 1576, 127, 136);
        _factory->bodyPos = _factory->staticBodyPos;//vec2(320,360);
    }
    void OnTransitionInEnds(){
        _transitionFrameIn = BILLION;
        _head->Texture(_factory->TextureAtlas05);
        _body->Texture(_factory->TextureAtlas04);
        _head->Rotation(0);
        //_head->RedefineTextureCoords(0, 0, 164, 139);
        //_body->Center(137, 40);
        //_body->RedefineTextureCoords(0, 689, 276, 166);
        //_factory->bodyPos = vec2(320,300);//vec2(320,420);
        _springDegree = -BILLION;
        poseBodyDucking(_body, 0.0);
        poseHeadDucking(_head, 0.0);
    }
    void OnTransitionOutStarts(){
        _transitionFrameIn = 0;
        _isTransitionReversed = true;
        _head->Texture(_factory->TextureAtlas03);
        _body->Texture(_factory->TextureAtlas03);
        _head->Rotation(0);
        //_head->RedefineTextureCoords(0, 1712, 189, 112);
        //_body->Center(127 / 2.0, 136 / 2.0);
        //_body->RedefineTextureCoords(0, 1576, 127, 136);
        //vec2(320,360);
    }
    void OnTransitionOutEnds(){
        //_factory->bodyPos = _factory->staticBodyPos;
        OnTransitionBackEnd(Caller);
    }
};

#endif
