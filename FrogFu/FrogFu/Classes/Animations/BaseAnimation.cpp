//
//  BaseAnimation.cpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-05.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#import "BaseAnimation.hpp"

#define TWO_BILLIONS 2000000000

BaseAnimation::BaseAnimation(double length) :
_length(length),
_initialTime(0),
_pauseTime(0),
_allowProgressAfterOne(false),
_isInitialized(false),
_isFinished(false){
    
    _factory = Factory::Instance;
    
    ++BaseAnimation::_lastID;
    if(BaseAnimation::_lastID > TWO_BILLIONS) BaseAnimation::_lastID = 500000000;
    _id = BaseAnimation::_lastID;
}
BaseAnimation::~BaseAnimation(){
    _isFinished = true;
}
long BaseAnimation::_lastID = 500000000;
void BaseAnimation::Initialize(double time){
    _initialTime = time;
    _isInitialized = true;
}
bool BaseAnimation::IsPaused(double time){
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
                _initialTime = time - diff;
            }
            _pauseTime = 0;
            return false;
        }
        return true;
    }
}
void BaseAnimation::Continue(){}
void BaseAnimation::Hide(){}
int BaseAnimation::ID(){ return _id; }
void BaseAnimation::Finish(){ _isFinished = true; }
double BaseAnimation::GetProgress(double currentTime){
    double progress = (currentTime - _initialTime) / _length;
    if(!_allowProgressAfterOne && progress > 1.0){
//cout << "BaseAnimation::GetProgress" << Type() << ":" << currentTime << ":" << _initialTime << ":" << _length << endl;
        _isFinished = true;
    }
    return progress;
}
bool BaseAnimation::IsInitialized(){ return _isInitialized; }
bool BaseAnimation::IsFinished(){ return _isFinished; }
double BaseAnimation::Length(){ return _length; }
bool BaseAnimation::CanStart(simlist<IAnimation>* animations){ return true; }
bool BaseAnimation::HasAnimationOfTheSameType(simlist<IAnimation>* animations){
    simnode<IAnimation>* node = animations->head;
    while (node != NULL) {
        IAnimation* a = node->data;
        if(a->Type() == Type()){
            return true;
        }
        node = node->next;
    }
    return false;
}
void* BaseAnimation::State(){ return NULL; }
void BaseAnimation::State(void* i){}
void BaseAnimation::Update(double time){}
 int BaseAnimation::Type() { return 0; }