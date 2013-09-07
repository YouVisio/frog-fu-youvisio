//
//  BaseAnimation.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-05.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_BaseAnimation_hpp
#define FrogFu_BaseAnimation_hpp

#import "IAnimation.hpp"
#import "Interfaces.hpp"
#import "Factory.hpp"

class BaseAnimation : public IAnimation {
public:
    virtual void Initialize(double time);
    virtual bool IsFinished();
    virtual void Finish();
    virtual void Continue();
    virtual void Hide();
    virtual bool IsInitialized();
    virtual void Update(double time);
    virtual int ID();
    virtual int Type();
    virtual double Length();
    virtual void* State();
    virtual void State(void*);
    virtual ~BaseAnimation();
    
protected:
    BaseAnimation(double length);
    double GetProgress(double currentTime);
    bool CanStart(simlist<IAnimation>* animations);
    bool HasAnimationOfTheSameType(simlist<IAnimation>* animations);
    virtual bool IsPaused(double time);
    
    Factory* _factory;
    bool _allowProgressAfterOne;
    double _initialTime;
    double _length;
    bool _isFinished;
    bool _isInitialized;
    double _pauseTime;
private:
    static long _lastID;
    long _id;
};


#endif
