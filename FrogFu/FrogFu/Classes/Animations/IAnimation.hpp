//
//  IAnimation.h
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-11.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_IAnimation_h
#define FrogFu_IAnimation_h

#import "Interfaces.hpp"

using namespace std;

#define ANI_BackgroundInJump 1
#define ANI_KamyshiMove 2
#define ANI_InsectMove 3
#define ANI_StaticPose 4
#define ANI_PointingPose 5
#define ANI_DuckingPose 6
#define ANI_JumpUpPose 7
#define ANI_JumpDownPose 8
#define ANI_CatchInsectMove 9
#define ANI_SplashAction 10
#define ANI_Stung 11
#define ANI_KungFu 12
#define ANI_StarSparkle 13

struct IAnimation {
    virtual void Update(double time) = 0;
    virtual bool IsFinished() = 0;
    virtual void Finish() = 0;
    virtual void Hide() = 0;
    virtual bool IsInitialized() = 0;
    virtual void Continue() = 0;
    virtual void Initialize(double time) = 0;
    virtual int Type() = 0;
    virtual int ID() = 0;
    virtual void* State() = 0;
    virtual void State(void*) = 0;
    virtual bool CanStart(simlist<IAnimation>* animations) = 0;
    virtual ~IAnimation() {}
};

struct VoidAnimation : IAnimation {
    void Update(double time){}
    bool IsFinished(){ return false; }
    void Finish(){}
    void Hide(){}
    bool IsInitialized(){ return true; }
    void Continue(){}
    void Initialize(double time){}
    int Type(){ return 0; }
    int ID(){ return 0; }
    void* State(){ return NULL; }
    void State(void*){};
    bool CanStart(simlist<IAnimation>* animations){ return true; }
    void Finalize() {}
    ~VoidAnimation() {}
};

#endif
