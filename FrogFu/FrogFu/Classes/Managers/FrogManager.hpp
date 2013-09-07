//
//  FrogManager.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-27.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_FrogManager_hpp
#define FrogFu_FrogManager_hpp

#import <iostream>
#import "BaseManager.hpp"
#import "Interfaces.hpp"
#import "Factory.hpp"
#import "Image.hpp"
#import "Particle.hpp"
#import "StaticPose.hpp"
#import "PointingPose.hpp"
#import "DuckingPose.hpp"
#import "JumpUpPose.hpp"
#import "JumpDownPose.hpp"
#import "CatchInsectMove.hpp"
#import "TouchTrailManager.hpp"
#import "MapManager.hpp"
#import "Stung.hpp"
#import "BonusFlash.hpp"
#import <cstring>
#import <map>

using namespace std;

#define MIN_BONUS 7

class FrogManager : BaseManager {
public:
    FrogManager(TouchTrailManager* ttm, MapManager* mapManager, simlist<Drawable>* drawables, simlist<IAnimation>* anims){
        _touchTrailManager = ttm;
        _mapManager = mapManager;
        _drawables = drawables;
        _animations = anims;
        _factory = Factory::Instance;
        _animation = NULL;
        _currCatchMove = NULL;
        SetCurrCatchMove(NULL);
        
        OnStartStaticPose = NULL;
        OnStartStaticPoseCaller = NULL;
        OnStungAnimationEnds = NULL;
        OnStungAnimationEndsCaller = NULL;
        
        _numHitPerJump = 0;
     
        _background =
            (new Image(_factory->TextureAtlas03, 746, 453, 60, 106))
            ->Position(-5, -5)->Func(ImgCenter_BottomLeft_NoRotation)->Scale(1150.0 / 106.0)
            ->IsVisible(false);
        
        _body  =
             (new Image(_factory->TextureAtlas03, 749, 570, 102, 130))
             ->Position(_factory->bodyPos.x, _factory->bodyPos.y)->Func(ImgCenter_Centered_WithRotation);
        _head =
              (new Image(_factory->TextureAtlas03, 1, 1, 184, 107))
              ->Position(0, 0)->Func(ImgCenter_Centered_WithRotation);
        _tongue =
             (new Image(_factory->TextureAtlas06, 694, 834, 330, 19))
             ->ScaleY(2.5)
             ->Position(100, 100)->Func(ImgCenter_Centered_WithRotation)->IsVisible(false)
             ->Center(320, 10);
        
        _lolWasp =
            (new Image(_factory->TextureAtlas06, 0, 836, 194, 174))
            ->Center(191.0/2.0, 172.0/2.0)
            ->Position(0, 0)->Func(ImgCenter_Centered_NoRotation)->IsVisible(false);
        
        _minusPoints =
            (new Image(_factory->TextureAtlas09, 835, 919, 89, 61))
            ->Center(89/2.0, 61/2.0)
            ->Position(640 - 130, 700)
            ->Func(ImgCenter_Centered_NoRotation)
            ->IsVisible(false);
#if ISDEBUG
        _factory->redDot =
             (new Image(_factory->TextureAtlas01, 2030, 440, 10, 10))
             ->Position(-100, -100)->Func(ImgCenter_Centered_NoRotation)->IsVisible(true)
             ->Center(5, 5)->Color(1,1,0,0);
        _factory->greenDot =
             (new Image(_factory->TextureAtlas01, 2030, 440, 10, 10))
             ->Position(-100, -100)->Func(ImgCenter_Centered_NoRotation)->IsVisible(true)
             ->Center(5, 5)->Color(1,0.8,1,0.8);
        _factory->blueDot =
             (new Image(_factory->TextureAtlas01, 2030, 440, 10, 10))
             ->Position(-100, -100)->Func(ImgCenter_Centered_NoRotation)->IsVisible(true)
             ->Center(5, 5)->Color(1,0,0,1);
#endif
        
        _catchInsectMoves = new simlist<CatchInsectMove>();
    
        //_animation = new VoidAnimation();
    }
    void Initialize(){
        _currCatchMove = NULL;
        SetCurrCatchMove(NULL);
        if(_animation != NULL){
            delete _animation;
            _animation = NULL;
        }
        
        _touchHasBegan = false;
   
        _drawables->push_back(_background);
        _drawables->push_back(_body);
        _drawables->push_back(_head);
        _drawables->push_back(_tongue);
        _drawables->push_back(_lolWasp);
        _drawables->push_back(_minusPoints);
//#if ISDEBUG
//        _drawables->push_back(_factory->redDot);
//        _drawables->push_back(_factory->greenDot);
//        _drawables->push_back(_factory->blueDot);
//#endif

        StartStaticPose();
    }
    void Update(double time){
        if(_animation == NULL) return;
        if(!_animation->IsInitialized()){
            _animation->Initialize(time);
        }
        _animation->Update(time);
        
        UpdateCatchInsectMove(time);
        
        vec2 bodyPos = _factory->frogPlainToScreen(_factory->bodyPos);
        _body->Pos(bodyPos);
        SetHeadPositionByBodyPosition(_head, _body, bodyPos, _body->poseID);
    }
    void ClearFrogAnimation(){
        if(_animation != NULL){
            delete _animation;
            _animation = NULL;
        }
        _animation = new VoidAnimation();
    }
    void TakeOffScreen(){
        _tongue->IsVisible(false);
        ClearCatchInsectMoveQueue();
        ClearFrogAnimation();
    }
    void UpdateCatchInsectMove(double time){
        // no currrent catching move but some waiting in the queue
        if(_currCatchMove == NULL && _catchInsectMoves->size() > 0){
            SetNextAsCurrentIfCloseEnough();
        }
        
        // there is running catch move
        if(_currCatchMove != NULL){
            // make sure it is initialized
            if(!_currCatchMove->IsInitialized()){
                _currCatchMove->Initialize(time);
            }
            _currCatchMove->Update(time);
            // if it's finished clear it
            if(_currCatchMove->IsFinished()){
                //delete _currCatchMove;
                SetCurrCatchMove(NULL);
                // current move, check if there are more waiting in the queue
                
                SetNextAsCurrentIfCloseEnough();
            }
        }
//cout << "CIM:" << _catchInsectMoves->size() << endl;
    }
    void SetNextAsCurrentIfCloseEnough(){
        simnode<CatchInsectMove>* node = _catchInsectMoves->head;
        while(node != NULL){
            CatchInsectMove* cim = node->data;
            Image* img = cim->Insect();
            double dist = img == NULL ? BILLION : img->Pos().Distance(_head->Pos());
            if(dist < (MAX_TONGUE_LENGTH)){
                _catchInsectMoves->removeAndGetNext(node);
                SetCurrCatchMove(cim);
                break;
            }
            node = node->next;
        }
    }
    void StartPointingPoseAnimation(){
        PointingPose* pose = new PointingPose(_body, _head);
        pose->OnEnd = FrogManager::PointingPoseEnds;
        pose->Caller = this;
        StartPose(pose);
    }
    void (*OnStungAnimationEnds)(void* caller);
    void* OnStungAnimationEndsCaller;
    
    void (*OnStartStaticPose)(void* caller);
    void* OnStartStaticPoseCaller;
    Image* Background() { return _background; }
    Image* Head() { return _head; }
    Image* Body() { return _body; }
    Image* Tongue() { return _tongue; }
    void StartPose(IAnimation* animation){
        if(_animation != NULL){
            if(_animation == _duckingPose){
                _duckingPose = NULL;
            }
            IAnimation* old = _animation;
            delete old;
            _animation = NULL;
        }
        _animation = animation;
    }
    bool IsPoseOfType(int type){
        return _animation != NULL && _animation->Type() == type;
    }
    IAnimation* Animation(){
        return _animation;
    }
    void InsectTouchedWithoutJump(IAnimation* insectMove){

    }
    CatchInsectMove* SelectedInsect(IAnimation* insectMove, bool isWasp, bool hasFlash){
        if(!_factory->isWithinJump || _factory->isPaused){
            return NULL;
        }
        ++_factory->selectedInsects;
        
        sysPlaySound("Touch");
        sysSoundVolume("Touch", 0.1);
        
        int insectInQueue = _factory->selectedInsects;
        /*int insectInQueue = 0;
        simnode<CatchInsectMove>* node = _catchInsectMoves->head;
        while(node != NULL){
            CatchInsectMove* cim = node->data;
            Image* img = cim->Insect();
            double dist = img == NULL ? BILLION : img->Pos().Distance(_head->Pos());
            if(dist < (MAX_TONGUE_LENGTH*1.2)){
                ++insectInQueue;
            }
            node = node->next;
        }*/
        double speed =
            insectInQueue <= 1
                ? 0.70 : insectInQueue == 2
                ? 0.40 : insectInQueue == 3
                ? 0.20 : 0.08;
//cout << "insectInQueue: " << insectInQueue << endl;
        CatchInsectMove* cim = new CatchInsectMove(_head, _tongue, _background, insectMove, _animations, _drawables, speed, isWasp, hasFlash);
        cim->touchId = _factory->currentTouchId;
        if(isWasp){
            cim->OnEndState = this;
            cim->OnEnd = OnStaticCatchWaspMoveEnd;
            cim->OnWaspHitState = this;
            cim->OnWaspHit = OnStaticWaspHit;
        }
        cim->OnInsectHitState = this;
        cim->OnInsectHit = OnStaticInsectHit;
        cim->OnInsectMissState = this;
        cim->OnInsectMiss = OnStaticInsectMiss;
        /*if(isWasp) _catchInsectMoves->push_front(cim);
        else*/
        _catchInsectMoves->push_front(cim);
        
        return cim;
    }
    static void OnStaticInsectMiss(void* state, CatchInsectMove* cim){ ((FrogManager*)state)->OnInsectMiss(cim); }
    void OnInsectMiss(CatchInsectMove* cim){
        
    }
    static void OnStaticInsectHit(void* state, CatchInsectMove* cim){ ((FrogManager*)state)->OnInsectHit(cim); }
    void OnInsectHit(CatchInsectMove* cim){
        ++_numHitPerJump;
    }
    static void OnStaticWaspHit(void* state, CatchInsectMove* cim){ ((FrogManager*)state)->OnWaspHit(cim); }
    void OnWaspHit(CatchInsectMove* cim){
        _numHitPerJump = 0;
        
        _factory->enableInsectCreation = false;
        SetCurrCatchMove(cim);
        ClearCatchInsectMoveQueue();
        _touchTrailManager->IsEnabled(false);
    }
    static void OnStaticCatchWaspMoveEnd(void* state){
        ((FrogManager*)state)->OnCatchWaspMoveEnd();
    }
    void OnCatchWaspMoveEnd(){
        ClearFrogAnimation();
        
        simnode<IAnimation>* node = _animations->head;
        while (node != NULL) {
            IAnimation* a = node->data;
            int type = a->Type();
            if(type == ANI_BackgroundInJump ||
               type == ANI_JumpDownPose ||
               type == ANI_JumpUpPose ||
               type == ANI_InsectMove){
                node = _animations->removeAndGetNext(node, true);
                continue;
            }
            node = node->next;
        }
        
        
        
        Stung* s = new Stung(_body, _head, _background, _lolWasp, _minusPoints, _factory->sceneID == SCENE_SPRINT);
        s->OnEndState = this;
        s->OnEnd = OnStaticStungEnd;
        _animations->push_back(s);
        
        // clear any existing moves
        ClearCatchInsectMoveQueue();
    }
    static void OnStaticStungEnd(void* state){
        ((FrogManager*)state)->OnStungEnd();
    }
    void OnStungEnd(){
        _factory->isPaused = false;
        _factory->bodyPos = _factory->staticBodyPos;
        //StartStaticPose();
        
        if(OnStungAnimationEnds != NULL) OnStungAnimationEnds(OnStungAnimationEndsCaller);
    }
    bool _touchHasBegan;
    void TouchBegins(vec2 touch){
        if(!_factory->isWithinJump && !_factory->isPaused){
            _touchHasBegan = true;
            StartPose(_duckingPose = new DuckingPose(_body, _head));
        }
    }
    void TouchMoved(vec2 touch){
//        if(!_factory->isWithinJump){
//            
//        }
    }
    void StartStaticPose(){
        
        _head->Rotation(0);
        _body->Rotation(0);
        
        if(OnStartStaticPose != NULL) OnStartStaticPose(OnStartStaticPoseCaller);
        
        _factory->setEnableInput(true); 

        StartPose(new StaticPose(_body, _head));
    }
    void TouchEnded(vec2 touch){
        if(!_factory->isWithinJump && !_factory->isPaused){
            if(!_touchHasBegan){
                TouchBegins(touch);
            }
            if(_duckingPose != NULL){
                _duckingPose->StartTransitionOut(this, FrogManager::DuckingPoseEnds);
            } else if(_body->poseID < 700) {
                StartStaticPose();
            }
        }
        _touchHasBegan = false;
    }
    void StartJump(FuncAndState<int>* onStartJump,FuncAndState<int>* onJumpPicks, double strength){
        _touchTrailManager->IsEnabled(true);
        
        StartPose(new JumpUpPose(_body, _head, strength, onStartJump, onJumpPicks));
    }
    void JumpPicks(FuncAndState<int>* onJumpEnd, FuncAndState<int>* onTouchGround){
        StartPose(new JumpDownPose(_body, _head, onJumpEnd, onTouchGround));
    }
    void OnJumpEnd(){
        _numHitPerJump = 0;
        
        _tongue->IsVisible(false);// in case it is still visible

        StartStaticPose();
    }
    void ClearCatchInsectMoveQueue(){
        simnode<CatchInsectMove>* node = _catchInsectMoves->head;
        while (node != NULL) {
            CatchInsectMove* move = node->data;
            // make sure we don't call those - that would cause undesired notifications
            move->OnWaspHit = NULL;
            move->OnEnd = NULL;
            node = node->next;
        }
        _catchInsectMoves->clear(true);
    }
    void OnTouchGround(){

        ClearCatchInsectMoveQueue();
        // find highest and potentially award bonus
        
        _factory->selectedInsects = 0;
        _touchTrailManager->IsEnabled(false);
        
        if(_numHitPerJump >= MIN_BONUS){
            AssignBonusFor(_numHitPerJump);
        }
        _numHitPerJump = 0;
    }
    void AssignBonusFor(int bonus){
        
        _animations->push_back(new BonusFlash(bonus, _drawables));
        
    }
    static void DuckingPoseEnds(void* caller){
        FrogManager* fm = (FrogManager*)caller;
        fm->StartStaticPose();
    }
    static void PointingPoseEnds(void* caller){
        FrogManager* fm = (FrogManager*)caller;
        fm->StartStaticPose();
    }
private:
    
    Factory* _factory;
    IAnimation* _animation;
    DuckingPose* _duckingPose;
    simlist<CatchInsectMove>* _catchInsectMoves;
    IAnimation* _currCatchMove;
    simlist<Drawable>* _drawables;
    simlist<IAnimation>* _animations;
    TouchTrailManager* _touchTrailManager;
    MapManager* _mapManager;
    int _numHitPerJump;

    Image* _background;
    Image* _head;
    Image* _body;
    Image* _tongue;
    Image* _lolWasp;
    Image* _minusPoints;
    
    void SetCurrCatchMove(IAnimation* a){
        if(_currCatchMove != NULL && (a == NULL || a->ID() != _currCatchMove->ID())){
            IAnimation* toDelete = _currCatchMove;
            _currCatchMove = NULL;
            delete toDelete;
        }
        _currCatchMove = a;
    }
    
    void SetHeadPositionByBodyPosition(Image* head, Image* body, vec2 bodyPos, int bodyPoseID){
        vec2 center = _factory->getHeadCenterById(_head->poseID);
        _head->Center(center.x, center.y);
        
        FrogImage* img = _factory->getHeadBinding(bodyPoseID);
        if(img == NULL){
            head->Pos(bodyPos);
            return;
        }
        vec2 bodyCenter = _factory->getBodyCenterById(bodyPoseID);
        if(bodyCenter.x == bodyCenter.x){ // not a NAN
            _body->Center(bodyCenter.x, bodyCenter.y);
        }

        vec2 pos = vec2(bodyPos.x + img->pos.x, bodyPos.y + img->pos.y);
        pos = pos.Rotate(_body->Pos(), _body->Rotation());
        head->Pos(pos);

        if(img->rotation == img->rotation){ // is not NAN
            head->Rotation(img->rotation);
        }
    }
    
};

#endif
