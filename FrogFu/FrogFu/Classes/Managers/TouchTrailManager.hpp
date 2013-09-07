//
//  TouchTrailManager.h
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-14.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_TouchTrailManager_hpp
#define FrogFu_TouchTrailManager_hpp

#import <iostream>
#import <map>
#import <cmath>
#import "Particle.hpp"
#import "Interfaces.hpp"
//#import <pthread.h>

#define DIST_POINTS 10
#define FROM_AGE 0.14
#define TO_AGE 0.02

using namespace std;

class TouchTrailManager {
public:
    TouchTrailManager(ParticleList* particles){
        
        _factory = Factory::Instance;
        
        _isEnabled = false;
        _particles = particles;
        numLevelsConst = 2;
        padConst = 4;
        radConst = padConst * numLevelsConst;
        
        _last = vec2(0,0);
        _curr = vec2(0,0);
        _distance = 0;
        _touchState = -1;// -1 = no tuch; 0 = touch moves; 1 = first touch
    }
    void TouchBegins(vec2 point){
        if(!_isEnabled) return;
        _last = point;
        _curr = point;
        _touchState = 1;
        _distance = 0;
        //CreateCircleFigure(point, 12, 4);
    }
    void TouchMoved(vec2 point){
        if(!_isEnabled) return;
        _touchState = 0;
        _curr = point;
        _distance = (_curr - _last).Length();
        
//        if(distance > DIST_POINTS){
//            //CreateCircleFigure(point, 12, 6);
//            CreateFuzzyLine(_last, point, radConst, padConst);
//            _last = point;
//        }
    }
    void TouchEnded(vec2 point){
        if(!_isEnabled) return;
        _distance = 0;
        _touchState = -1;
    }
    void Initialize(){
        _particles->clear();
    }
    void Update(double time){
        
        if(_factory->isDrawingTouchTrails) return;
        
        long numParticles = _particles->size();
        
        double x = numParticles / 1000.0;
        double ageLimit = -pow(2.0, 3.0) * pow(abs(0.5 * x), 2.5)  + 1.0;


        ParticleNode* curr = _particles->head;
        
        while (curr != NULL) {
            
            Particle p = curr->particle;
            double age = p.Age();
            if(age >= ageLimit){
                curr = _particles->removeAndGetNext(curr);
            }else{
                // progress
                
                age += 0.04;
                p.Age(age);
                p.Color(GetColorByAge(age));
                curr->particle = p;
                curr = curr->next;
            }
        }
        
        numParticles = _particles->size();
        
        if(_touchState == 1){
            CreateCircleFigure(_curr, 12, 4);
            _touchState = 0;
        } else if(_touchState == 0 && _distance > DIST_POINTS && numParticles < 1000){
            CreateFuzzyLine(_last, _curr, radConst, padConst);
            _last = _curr;
        }
        
//_factory->printIfDiff(_particles->size());
    }
    void ClearAll(){
        _particles->clear();
    }
    void OnJumpEnd(){
        IsEnabled(false);
    }
    bool IsEnabled() { return _isEnabled; }
    TouchTrailManager* IsEnabled(bool enable){
        _isEnabled = enable;
        if(!enable){
            ClearAll();
        }
        _distance = 0;
        _touchState = -1;
        _curr = vec2(0,0);
        _last = vec2(0,0);
        return this;
    }
    
    double padConst;
    double radConst;
    double numLevelsConst;
    
private:
    Factory* _factory;
    bool _isEnabled;
    ParticleList* _particles;
    //list<Particle>* _particles;
    vec2 _last;
    vec2 _curr;
    int _distance;
    int _touchState;
    
    void DrawDottedLine(vec2 p1, vec2 p2, double pad, double ageFrom, double ageTo){
        double ageDiff = ageFrom - ageTo;
        
        double len = p1.LengthTo(p2);
        double num = floor(len/pad);
        double ageDiffPart = ageDiff / num;
        if(num == 0) return;
        double remainder = len - pad * num;
        double currpad = pad + remainder/num;
        double ageAdd = 0;
        for(int i = 0; i <= num; ++i){
            vec2 p = p2.DotOnTheWayTo(p1, -i * currpad);
            _particles->appendLast(Particle(vec2(p.x, p.y), GetColorByAge(ageFrom+ageAdd), ageFrom+ageAdd));
            ageAdd += ageDiffPart;
        }
    }
    void DrawSideDottedLines(vec2 p1, vec2 p2, vec2 curr, vec2 prev, double radius, double pad, int sign){
        int i = 1;
        double ang = 2;
        double n = 0;
        double nextLinePad = 8.0;
        double addAge = 0.16;
        if(_particles->size() > 800) addAge += 0.35;
        else if(_particles->size() > 400) addAge += 0.25;
        else if(_particles->size() > 200) addAge += 0.1;
        
        do{
            n += addAge;
            ang = asin(nextLinePad*i / radius);
            if(ang > 1.3) ang = 1.3;
            ++i;
            vec2 p1r = p1.Rotate(curr, ang*sign);
            vec2 p2r = p2.Rotate(prev, ang*sign);
            DrawDottedLine(p1r, p2r, pad, FROM_AGE+n, TO_AGE+n);
        } while(abs(ang) < 1.57);
    }
    void CreateFuzzyLine(vec2 prev, vec2 curr, double radius, double pad){
        vec2 p1 = prev.DotOnTheWayTo(curr, radius);
        vec2 p2 = curr.DotOnTheWayTo(prev, -radius);
        DrawDottedLine(p1, p2, pad, FROM_AGE, TO_AGE);
        
        DrawSideDottedLines(p1, p2, curr, prev, radius, pad, 1);
        DrawSideDottedLines(p1, p2, curr, prev, radius, pad, -1);
    }
    void CreateCircleFigure(vec2 center, double distToNear, int numCircles){
        _particles->appendLast(Particle(center, GetColorByAge(0), 0));
        for(int i = 1; i <= numCircles; ++i){
            CreateCircleLevel(center, distToNear, distToNear * i, i/(double)numCircles);
        }
    }
    void CreateCircleLevel(vec2 center, double distToNear, double radius, double relDistFromCen){
        vec2 temp = vec2(center.x + radius, center.y);
        double angPart = GetAngle(distToNear, radius);
        double ang = 0.0;
        double max = 2 * Pi - 0.1;
        while(true){
            if(ang > max){ break; }
            vec2 rotated = temp.GetRotated(center, ang);
            _particles->appendLast(Particle(vec2(rotated.x, rotated.y), GetColorByAge(relDistFromCen * 0.9), relDistFromCen * 0.9));
            ang += angPart;
        }
    }
    double GetAngle(double distToNear, double radius){
        return atan2(distToNear / 2.0, radius) * 2.0;
    }
    vec4 GetColorByAge(double x){
        float alpha = -1 * pow(x, 6) + 1;//-1*x^6 + 1
        double n = 0.2+0.8*alpha;
        return vec4(n,n,1,alpha*0.35);
    }
};

#endif
