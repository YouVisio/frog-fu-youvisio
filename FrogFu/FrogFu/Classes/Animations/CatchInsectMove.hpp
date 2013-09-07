//
//  CatchInsectMove.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-09-16.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_CatchInsectMove_hpp
#define FrogFu_CatchInsectMove_hpp

#import "Poses.hpp"
#import "Particle.hpp"
#import "Image.hpp"
#import "Factory.hpp"
#import "BaseAnimation.hpp"
#import "Interfaces.hpp"
#import "ISceneEngine.hpp"
#import "CatchFlash.hpp"

using namespace std;

class CatchInsectMove : public BaseAnimation{
public:
    CatchInsectMove(Image* head, Image* tongue, Image* background, IAnimation* insectMove, simlist<IAnimation>* animations, simlist<Drawable>* drawables, double speed, bool isWasp, bool hasFlash)
    : BaseAnimation(speed),
    touchId(0){
//cout << "SPEED:" << speed << endl;
        _factory = Factory::Instance;
        _isWasp = isWasp;
        _head = head;
        _tongue = tongue;
        _background = background;
        _drawables = drawables;
        _hasFlash = hasFlash;
        _distanceOnAttach = 0;
        _lastInsectPos = vec2(0,0);
        _waspWasHit = false;
        
//registerFinalizer(_insectMove->ID(), StaticInsectMoveDeleted, this);
        
        _insectMoveId = insectMove->ID();
        _animations = animations;
//registerFinalizer(_insect->UniqueId(), StaticInsectDeleted, this);
        
        _isFinished = false;
        _startTongueMoveAtProgress = 1.0 / 7.0;
        _toungueFuncN = 1.0 / ((1.0 - _startTongueMoveAtProgress) - _startTongueMoveAtProgress);
        _toungueFuncB = _startTongueMoveAtProgress * _toungueFuncN * -1;
        _tongue->IsVisible(false);
        _previousDistance = -1;
        _isOutOfRange = false;
        _insectIsAttached = false;
        
        OnEnd = NULL;
        OnEndState = NULL;
        OnWaspHit = NULL;
        OnWaspHitState = NULL;
        OnInsectHit = NULL;
        OnInsectHitState = NULL;
        OnInsectMiss = NULL;
        OnInsectMissState = NULL;
//_factory->redDot->IsVisible(true);
//_factory->blueDot->IsVisible(true);
//_factory->greenDot->IsVisible(true);
//cout << "TONGUE N:" << _toungueFuncN << " B:" << _toungueFuncB << " START:" << _startTongueMoveAtProgress << endl;
    }
    ~CatchInsectMove(){
        if(_waspWasHit){
            _background->Alpha(1);
            ChangeInsects(0.0, true);
        }
        if(OnEnd != NULL) OnEnd(OnEndState);
        poseHeadEatingInsects(_head, -1.0, 0.0, 0);
        if(_tongue != NULL) _tongue->IsVisible(false);
        _isFinished = true;
        
        //IAnimation* a = FindInsectMove();
        //if(a != NULL) a->Finish();
    }
    IAnimation* FindInsectMove(){
        simnode<IAnimation>* node = _animations->head;
        while (node != NULL) {
            IAnimation * a = node->data;
            if(a->ID() == _insectMoveId){
                return a;
            }
            node = node->next;
        }
        return NULL;
    }
    void Initialize(double time){
        if(_isFinished){
            return;
        }
        BaseAnimation::Initialize(time);
        
        _head->Texture(_factory->TextureAtlas06);
    }
    void Update(double time){
        if(_isFinished){
            return;
        }
        
        if(IsPaused(time)){ return; }
        
        if(!_factory->isWithinJump){
            Finish();
            return;
        }
        
        IAnimation* insectMove = FindInsectMove();
        Image* insect = NULL;
        if(insectMove != NULL){
            insect = (Image*)insectMove->State();
            if(insect != NULL) _lastInsectPos = insect->Pos();
        }
        
        double progress = GetProgress(time);
        double tweenProgress = CatchInsectMove::TweenFunc(progress);

        
        vec2 tongueLoc = _factory->getTongueLocationById(_head->poseID); 
        vec2 headPos = vec2(_head->X(), _head->Y());
        vec2 headCenter = vec2(_head->CenterX(), _head->CenterY());
        
        vec2 headTopLeft = vec2(headPos.x - headCenter.x, headPos.y + headCenter.y);
        
        vec2 from = vec2(headTopLeft.x + tongueLoc.x, headTopLeft.y - tongueLoc.y);
        from = from.Rotate(headPos, _head->Rotation());
        
        vec2 to = insect == NULL ? _lastInsectPos : insect->Pos();
        double distance = from.Distance(to);


//_factory->blueDot->Pos(from);
//_factory->redDot->Pos(to);
        vec2 diff = headPos - to;
        double angle = atan2(diff.x, diff.y);
        _tongue->Rotation(angle*-1 + M_PI_2);
        
        angle += _head->Rotation();
        angle = sysMod(angle, M_PI * 2);
//cout << "angle:" << angle << " from.x, from.y:" << from.x << "," << from.y << " to.x, to.y:" << to.x << "," << to.y <<
//" P:" << progress << " maid:" << _insectMove->ID() << endl;
        
        double tongueProgress = _toungueFuncN * tweenProgress + _toungueFuncB;
        if(tongueProgress >= 0.96){
            
            
            
            if(_insectIsAttached && insect != NULL && insect->Drawable::IsVisible()){
                --_factory->selectedInsects;
                insect->IsVisible(false);
                IAnimation* a = FindInsectMove();
                if(a != NULL) a->Finish();
            }
            
            if(_tongue->Drawable::IsVisible()){
                 // hide tongue
                _tongue->IsVisible(false);
                
            }
        } else if(tongueProgress >= 0){
            if(!_tongue->Drawable::IsVisible()){
                // show tongue
                _tongue->IsVisible(true);
            }
           
            
            // process tongue
            _tongue->Pos(from);
            const double coef = 1.4;
            double currTongue = sign(tongueProgress - 2) * pow(abs(tongueProgress - 0.5), coef) * pow(2, coef) + 1.0;
 
            if(_distanceOnAttach) distance = _distanceOnAttach * currTongue;
            
            poseTongue(_tongue, currTongue, distance, distance - _previousDistance, TONGUE_COMMON);
//_factory->printIfDiff(distance);
//_factory->printIfDiff2(_tongue->poseID);
            if(!_insectIsAttached && !_isOutOfRange && tongueProgress > 0.48){
                
                insect->mark = 9898;

                if(distance > MAX_TONGUE_LENGTH) {
                    if(insectMove != NULL){
                        if(insect != NULL) insect->mark = 0;
                        insectMove->Continue();
                    }
                    insectMove = NULL;
                    _isOutOfRange = true;
                    if(!_factory->enableInsectCreation){
                         if(OnInsectMiss != NULL && !_isWasp) OnInsectMiss(OnInsectMissState, this);
                    }
                } else if(insect != NULL) {
                    
                    // sounds:
                    if(_isWasp){
                        sysPlaySound("Stung");
                        sysSoundVolume("Stung", 0.3);
                    } else {
                        sysPlaySound("Eat");
                        sysSoundVolume("Eat", 0.3);
                    }
                    
                    _pickTongueProgress = tongueProgress;
                    _insectIsAttached = true;
                    _distanceOnAttach = distance;
                    if(_hasFlash){
                        CatchFlash* cf = new CatchFlash(_drawables, insect);
                        cf->Initialize(time);
                        _animations->push_back(cf);
                    }
                    
                    if(_isWasp){
                        if(insect != NULL){
                            _factory->waspPositionAngle = insect->Pos().AngleTo(_head->Pos());
                        }
                        
                        _waspWasHit = true;
                        _factory->enableInsectCreation = false;
                        

                        if(OnWaspHit != NULL) OnWaspHit(OnWaspHitState, this);
                        
                        _waspId = insect->UniqueId();
                        _background->IsVisible(true);
                        _background->Alpha(0);
                        _background->Color(1, 1, 0.2);
                        
                        ChangeInsects(0.0, false);
                        
                        // delete other catch animations
                        simnode<IAnimation>* node = _animations->head;
                        while (node != NULL) {
                            IAnimation* a = node->data;
                            int type = a->Type();
                            if(type == ANI_CatchInsectMove && a->ID() != ID()){
                                node = _animations->removeAndGetNext(node, true);
                                continue;
                            }
                            node = node->next;
                        } 
                    } else { // if(!_isWasp)
                        ++_factory->numberHit;
                        if(OnInsectHit != NULL) OnInsectHit(OnInsectHitState, this);
                    }
                }
            }
  
            if(_insectIsAttached){
                double goingBackProgress = (tongueProgress - _pickTongueProgress) / (1.0 - _pickTongueProgress);
                

                
                if(_isWasp){
                    
                    _background->Alpha(goingBackProgress);
                    
                    if(insect != NULL){
                        _factory->waspPositionAngle = insect->Pos().AngleTo(_head->Pos());
//cout << "WASP ANGLE:" << (_factory->waspPositionAngle * RTD) << endl;
                    }
                    
                }else if(insect != NULL){ // attach insect image to the tongue
                    double tongueLength = _factory->getTongueLengthByPoseId(_tongue->poseID);
                    vec2 tongueTip = vec2(from.x - tongueLength, from.y);
                    tongueTip = tongueTip.Rotate(from, _tongue->Rotation());
                    //if(distance > 60){
                        insect->Pos(tongueTip);
                    //}
                }
            }
        }
        poseHeadEatingInsects(_head, tweenProgress, angle, distance);
        
        _previousDistance = distance;
    }
    void ChangeInsects(double alpha, bool finish){
        simnode<IAnimation>* node = _animations->head;
        while (node != NULL) {
            IAnimation* a = node->data;
            if(a->Type() == ANI_InsectMove){
                Image* img = (Image*)a->State();
                if(img->UniqueId() != _waspId){
                    if(alpha == 0.0){
                        img->Alpha(1);
                        img->IsVisible(false);
                    }
                    else{
                        img->Alpha(alpha);
                        img->IsVisible(true);
                    }
                    img->Color(1, 1, 0.2);
                }
                if(finish){
                    node = _animations->removeAndGetNext(node, true);
                    continue;
                }
            }
            node = node->next;
        }
    }
    void Continue(){
        IAnimation* insectMove = FindInsectMove();
        if(insectMove != NULL){
            insectMove->Continue();
            insectMove = NULL;
            _isOutOfRange = true;
        }
    };
    Image* Insect() {
        IAnimation* insectMove = FindInsectMove();
        Image* insect = NULL;
        if(insectMove != NULL){
            insect = (Image*)insectMove->State();
        }
        return insect;
    }
    int Type(){ return ANI_CatchInsectMove; }
    long touchId;
    void (*OnEnd)(void*);
    void* OnEndState;
    void (*OnWaspHit)(void*, CatchInsectMove*);
    void* OnWaspHitState;
    void (*OnInsectHit)(void*, CatchInsectMove*);
    void* OnInsectHitState;
    void (*OnInsectMiss)(void*, CatchInsectMove*);
    void* OnInsectMissState;
    
private:
    bool _isWasp;
    int _waspId;
//IAnimation* _insectMove;
    Image* _head;
//Image* _insect;
    Image* _tongue;
    Image* _background;
    Factory* _factory;
    double _startTongueMoveAtProgress;
    double _toungueFuncN;
    double _toungueFuncB;
    double _previousDistance;
    bool _isOutOfRange;
    bool _insectIsAttached;
    double _pickTongueProgress;
    simlist<IAnimation>* _animations;
    simlist<Drawable>* _drawables;
    int _insectMoveId;
    bool _hasFlash;
    double _distanceOnAttach;
    vec2 _lastInsectPos;
    bool _waspWasHit;
    
    static double TweenFunc(double x){
        //return x;
        double coef = 1.3;
        //return sign(x - 0.5) * pow(abs(x - 0.5) / (0.5 / pow(0.5, coef)), 1 / coef) + 0.5;
        return (0.5 / pow(0.5, coef)) * pow(abs(x - 0.5), coef)*sign(x-0.5) + 0.5;
        
    }
};

#endif
