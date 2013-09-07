//
//  Ticker.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-05.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_Ticker_hpp
#define FrogFu_Ticker_hpp

#import <iostream>
#import "Interfaces.hpp"

using namespace std;

template<class T>
class Ticker{
public:
    Ticker(double duration, T* state, void (*f)(Ticker<T>* ticker)){
        _duration = duration;
        _state = state;
        _func = f;
        _endTime = -1;
        _startTime = -1;
        _time = -1;
        _progress = 0;
        _isFinished = false;
        _next = NULL;
    }
    ~Ticker(){
        //cout << "destroying Ticker with duration " << _duration << endl;
    }
    Ticker* FollowBy(double duration, void (*f)(Ticker<T>* ticker)){
        if(_next != NULL){
            delete _next;
        }
        _next = new Ticker<T>(duration, _state, f);
        return _next;
    }
    
    void Tick(double time){
        if(_endTime < 0){
            _endTime = time + _duration;
            _startTime = time;
        }
        _time = time;
        _progress = ensureRange((time - _startTime) / _duration, 0.0, 1.0);
        if(!_isFinished){
            _func(this);
        }
        if(_progress >= 1.0){
            _isFinished = true;
        }
    }
    
    void (*Func())(Ticker<T>* ticker){ return _func; }
    T* State(){ return _state; }
    Ticker* Next(){ return _next; }
    double Duration(){ return _duration; }
    double StartTime(){ return _startTime; }
    double EndTime(){ return _endTime; }
    double Progress(){ return _progress; }
    double Time(){ return _time; }
    
    const static double MAX_DURATION;
private:
    void (*_func)(Ticker<T>* ticker);
    T* _state;
    Ticker* _next;
    double _duration;
    double _startTime;
    double _endTime;
    double _progress;
    double _time;
    bool _isFinished;
    
};



#endif
