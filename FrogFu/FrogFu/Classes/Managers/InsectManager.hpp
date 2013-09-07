//
//  InsectManager.h
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-17.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_InsectManager_hpp
#define FrogFu_InsectManager_hpp

#import <iostream>
#import <vector>
#import "Particle.hpp"
#import "Image.hpp"
#import "Interfaces.hpp"
#import "IAnimation.hpp"
#import "Factory.hpp"
#import "FrogManager.hpp"
#import "MapManager.hpp"
#import "TouchTrailManager.hpp"
#import "InsectMove.hpp"
#import "WaspMove.hpp"

using namespace std;

class InsectManager {
public:
    InsectManager(ISceneEngine* engine, FrogManager* frogManager, MapManager* mapManager, TouchTrailManager* ttm, simlist<Particle>* mapDots, simlist<Drawable>* drawables, simlist<IAnimation>* animations)
    :
    _engine(engine),
    _frogManager(frogManager),
    _mapManager(mapManager),
    _touchTrailManager(ttm),
    _nextCloudCreationTime(0),
    _mapDots(mapDots),
    _drawables(drawables),
    _animations(animations){
        _factory = Factory::Instance;
        frogManager->OnStartStaticPose = OnStaticStartingPoseStarts;
        frogManager->OnStartStaticPoseCaller = this;
        
        nextCreateInsectTimeRange = Range(5.0, 9.0);
        nextCreateWaspTimeRange = Range(2.0, 4.0);
    }
    static void OnStaticStartingPoseStarts(void* caller){
        ((InsectManager*)caller)->OnStartingPoseStarts();
    }
    void OnStartingPoseStarts(){
        ContinueAll();
        _mapManager->JumpAnimationEnds();
        _touchTrailManager->ClearAll();
    }
    void Initialize(double time){
        _nextCloudCreationTime = time - 1.0;
        _nextWaspCreationTime = time + 5.0;
        _lastCloudY = sysRandomOfPeriodAsInt(900, 1300);
    }
    
    void Update(double time){
        
        if(_factory->enableInsectCreation && _prevIsDisabled) _nextCloudCreationTime = time - 0.1;
        
        _prevIsDisabled = !_factory->enableInsectCreation;
        
        if(time >= _nextCloudCreationTime){
            CreateCloudOfInsects();
            _nextCloudCreationTime = time +
                sysRandomOfPeriodAsDouble(nextCreateInsectTimeRange.From, nextCreateInsectTimeRange.To);
        }
        if(time > _nextWaspCreationTime){
            CreateWasp();
            _nextWaspCreationTime = time +
                sysRandomOfPeriodAsDouble(nextCreateWaspTimeRange.From, nextCreateWaspTimeRange.To);
        }
    }
    void SetNextWaspCreationTime(double time) { _nextWaspCreationTime = time; }
    void SetEngine(ISceneEngine* engine) { _engine = engine; }
    void Disable(){
        _nextCloudCreationTime = BILLION;
    }
    void ContinueAll(){
        simnode<IAnimation>* node = _animations->head;
        while (node != NULL) {
            IAnimation * a = node->data;
            if(a->Type() == ANI_InsectMove){
                InsectMove* im = (InsectMove*)a;
                if(im->IsTouched()){
                    im->Continue();
                }
            }
            node = node->next;
        }
    }
    
    Range nextCreateInsectTimeRange;
    Range nextCreateWaspTimeRange;
    
private:
    Factory* _factory;
    int _lastCloudY;
    double _nextCloudCreationTime;
    double _nextWaspCreationTime;
    simlist<Particle>* _mapDots;
    simlist<Drawable>* _drawables;
    simlist<IAnimation>* _animations;
    ISceneEngine* _engine;
    FrogManager* _frogManager;
    MapManager* _mapManager;
    TouchTrailManager* _touchTrailManager;
    bool _prevIsDisabled;
    
    void CreateWasp(){

        Factory* f = Factory::Instance;
        
        if(!f->enableInsectCreation) return;
        
        vec2 pos = vec2(1200 + 200*sysRandom(), _lastCloudY + sysRandomOfPeriodAsDouble(-0.5, 2.5)*200);
        
        Particle* par = new Particle(f->frogPlainToMap(pos), vec4(1,0,0,1), 0);
        _mapDots->push_back(par);
        
        Image* img = new Image(f->TextureAtlas06, 0, 712, 142, 124);
        vec2 spos = f->frogPlainToScreen(pos);
        img->Position(spos.x, spos.y)
            ->Func(ImgCenter_MiddleCenter_NoRotation)
            ->Center(142.0/2.0, 124.0/2.0)
            ->Scale(sysRandomOfPeriodAsDouble(0.6, 0.8));
        
        _drawables->push_back(img);
        
        _animations->push_back(new WaspMove(_engine, _frogManager, pos, par, img));
    }
    void CreateCloudOfInsects(){
        Factory* f = Factory::Instance;
        
        if(!f->enableInsectCreation) return;
        
        int rows = sysRandomOfPeriodAsInt(2, 3);
        int cols = sysRandomOfPeriodAsInt(5, 6);
        double probability = sysRandomOfPeriodAsDouble(0.65, 0.9);
        vector<vec2>* dots = GetCloudRandomDots(rows, cols, 200, 30, probability);
        int yShift = f->isFive ? sysRandomOfPeriodAsInt(800, 1600) : sysRandomOfPeriodAsInt(800, 1400);
//yShift = 800;
        for(int i = 0; i < dots->size(); ++i){
            vec2 pos = (*dots)[i];
            pos.x += 640;
            pos.y += yShift;
            
            Particle* par = new Particle(f->frogPlainToMap(pos), vec4(0,0.2,0,0.8), 0);
            _mapDots->push_back(par);
            
            int r = sysRandomOfPeriodAsInt(1,3);
            Image* img = NULL;
            if(r == 1) img = new Image(f->TextureAtlas06, 0, 502, 73, 80);
            else if(r == 2) img = new Image(f->TextureAtlas06, 0, 584, 73, 63);
            else img = new Image(f->TextureAtlas06, 0, 648, 73, 65); // r == 3
            
            vec2 spos = f->frogPlainToScreen(pos);
            img->Position(spos.x, spos.y)->Func(ImgCenter_MiddleCenter_NoRotation);
            _drawables->push_back(img);
            InsectMove* im = new InsectMove(_engine, _frogManager, pos, par, img);
            _animations->push_back(im);
        }
        _lastCloudY = yShift;
        delete dots;
    }
    vector<vec2>* GetCloudRandomDots(int rows, int cols, double side, double pad, double prob){
        prob = sysMod(fabs(prob), 1.0);
        vector<vec2>* points = new vector<vec2>();
        if(prob <= 0){
            return points;
        }
        for(int y = 0; y < rows; ++y){
            for(int x = 0; x < cols; ++x){
                if(sysRandom() <= prob){
                    points->push_back(GetRandomDot(x, y, side, pad));
                }
            }
        }
        return points;
    }
    vec2 GetRandomDot(int x, int y, double side, double pad){
        double minX = side * x + pad;
        double maxX = side * x + side - pad;
        double minY = side * y + pad;
        double maxY = side * y + side - pad;
        
        double randX = sysRandom();
        double randY = sysRandom();
        return vec2((maxX - minX)*randX + minX, (maxY - minY)*randY + minY);
    }
};



#endif /* defined(__FrogFu__InsectManager__) */
