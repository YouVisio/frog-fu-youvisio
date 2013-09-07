//
//  KungFu.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 13-01-19.
//  Copyright (c) 2013 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_KungFu_hpp
#define FrogFu_KungFu_hpp

#import <iostream>
#import "BaseAnimation.hpp"
#import "Image.hpp"
#import "Poses.hpp"

#define KUNGFU_1 1
#define KUNGFU_2 2

using namespace std;

class KungFu : public BaseAnimation {
public:
    KungFu(int type, Image* body, Image* head, FuncAndState<int>* onEnd)
    : BaseAnimation(type == KUNGFU_1 ? 2.0 : 2.8){
        _type = type;
        _body = body;
        _head = head;
        _onEnd = onEnd;
    }
    ~KungFu(){
        if(_onEnd != NULL){
            delete _onEnd;
            _onEnd = NULL;
        }
    }
    void Initialize(double time){
        BaseAnimation::Initialize(time);
        _body->Rotation(-0.05);
        _head->Rotation(_type == KUNGFU_2 ? -0.1 : 0);
        
    }
    void Update(double time){
        if(_isFinished){ return; }
        
        double progress = GetProgress(time);
        
        if(_isFinished){
            _onEnd->Call();
        }
        
        if(_type == KUNGFU_1){
            poseHeadKungFu01(_head, progress);
            poseBodyKungFu01(_body, progress);
        } else if(_type == KUNGFU_2){
            poseHeadKungFu02(_head, progress);
            poseBodyKungFu02(_body, progress);
        }
        
    }
    int Type(){ return ANI_KungFu; }
    
private:
    int _type;
    Image* _body;
    Image* _head;
    FuncAndState<int>* _onEnd;
};

#endif
