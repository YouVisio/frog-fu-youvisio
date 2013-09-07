//
//  MapManager.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-05.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_MapManager_hpp
#define FrogFu_MapManager_hpp


#import <iostream>
#import "BaseManager.hpp"
#import "Interfaces.hpp"
#import "Image.hpp"
#import "Particle.hpp"
#import "Factory.hpp"

using namespace std;

class MapManager : BaseManager {
public:
    MapManager(simlist<Drawable>* drawables, simlist<Particle>* mapDots);
    void JumpAnimationEnds();
    void TouchBegins();
    void ChangeMapArrowHeight(double progress);
    void InitializeJump(double strength);
    void JumpUpdate(double position, double progress);
    void SetFrogJump();
    void SetFrogDuck();
    void SetFrogSit();
    void SetHandOver();
    void SetHandPressed();
    void StartPreviewOneAnimation();
    void Initialize();
    void Update(double step);
    void ClearDots();
    void TakeOffScreen();
    void SetBody(Image* body);
    void ClearTicker();
    
    
    Image* pad;
    Image* arrow;
    Image* phoneFrame;
    Image* frog;
    Image* hand;
    
    vec2 point1;
    vec2 point2;
    int handPositionCounter;
    double minSubProgress;
    double maxSubProgress;
    float iPhone5CorrectY;
    
private:
    
    Factory* _factory;
    
    int _shiftFrog;
    
    Ticker<MapManager>* _ticker;
    simlist<Particle>* _mapDots;
    simlist<Drawable>* _drawables;
    
    Image* _body;
    
    float _arrowMaxHeight;
    float _arrowMinHeight;
    
    float _minFrameY;
    float _minFrogY;
    float _frameShiftY;
    
};



#endif
