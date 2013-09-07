//
//  BackgroundInJump.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-05.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_BackgroundInJump_hpp
#define FrogFu_BackgroundInJump_hpp

#import <iostream>
#import "BaseAnimation.hpp"
#import "Image.hpp" 
#import "MapManager.hpp"
#import "Factory.hpp"
#import <vector>
#import <cmath>

using namespace std;

class BgImageData{
public:
    BgImageData(Image * img, double changeYVal){
        image = img;
        initY = image->Y();
        initScale = image->Drawable::Scale();
        changeY = changeYVal;
    }
    ~BgImageData(){ }
    Image* image;
    double initY;
    double initScale;
    double changeY;
};

class BackgroundInJump : public BaseAnimation {
public:
    double progress; // 0 - 1
    double position; // 0 - 1 - 0
    
    BackgroundInJump(double strength,
                     MapManager* mapManager,
                     BackgroundImages* bgImages)
    : BaseAnimation(computeJumpLengthByStrength(strength)){
        _strength = strength;
        _bgImages = bgImages;
        //cout << "BackgroundInJump time:" << (0.5 + 3.5*strength) << " strength:" << _strength << endl;
        _images = new vector<BgImageData*>();
        if(_bgImages->BgFront != NULL)
            _images->push_back(new BgImageData(_bgImages->BgFront, -Y_SHIFT_BG_FRONT*_strength));
        if(_bgImages->BgPlatform != NULL)
            _images->push_back(new BgImageData(_bgImages->BgPlatform, -Y_SHIFT_BG_MAIN*_strength));
        if(_bgImages->BgMain != NULL)
            _images->push_back(new BgImageData(_bgImages->BgMain, -Y_SHIFT_BG_MAIN*_strength));
        if(_bgImages->BgSideOneLeft != NULL)
            _images->push_back(new BgImageData(_bgImages->BgSideOneLeft, -Y_SHIFT_BG_FRONT*_strength));
        if(_bgImages->BgSideOneRight != NULL)
            _images->push_back(new BgImageData(_bgImages->BgSideOneRight, -Y_SHIFT_BG_FRONT*_strength));
        if(_bgImages->BgSideTwoLeft != NULL)
            _images->push_back(new BgImageData(_bgImages->BgSideTwoLeft, -Y_SHIFT_BG_MAIN*_strength));
        if(_bgImages->BgSideTwoRight != NULL)
            _images->push_back(new BgImageData(_bgImages->BgSideTwoRight, -Y_SHIFT_BG_MAIN*_strength));
        if(_bgImages->BgWave1 != NULL)
            _images->push_back(new BgImageData(_bgImages->BgWave1, -Y_SHIFT_BG_MAIN*_strength));
        if(_bgImages->BgWave2 != NULL)
            _images->push_back(new BgImageData(_bgImages->BgWave2, -Y_SHIFT_BG_MAIN*_strength));
        if(_bgImages->BgNearBg != NULL)
            _images->push_back(new BgImageData(_bgImages->BgNearBg, -Y_SHIFT_BG_NEAR_BG*_strength));
        if(_bgImages->BgFarBg != NULL)
            _images->push_back(new BgImageData(_bgImages->BgFarBg, -Y_SHIFT_BG_FAR_BG*_strength));
        if(_bgImages->BgBack != NULL)
            _images->push_back(new BgImageData(_bgImages->BgBack, -Y_SHIFT_BG_BACK*_strength));
        
        _mapManager = mapManager;
        
        _mapManager->InitializeJump(strength);
        
        _factory = Factory::Instance;
        
        _factory->lastJumpStrength = _strength;
    }
    ~BackgroundInJump(){
        _factory->jumpProgress(0.0);
        _factory->jumpPosition(0.0);
        
        int count = _images->size();
        for(int i = 0; i < count; ++i){
            BgImageData * d = (*_images)[i];
            d->image->Y(d->initY);
            delete d;
        }
        _images->clear();
        delete _images;
        _images = NULL;
    }
    
    bool CanStart(simlist<IAnimation>* animations){
        //return !HasAnimationWithSameID(animations);
        return true;//the control is ecxternal
    }
    
    void Update(double time){

        if(IsPaused(time)){ return; }
        
        progress = GetProgress(time);

        _factory->jumpProgress(progress);
        
        if(progress > 1.0) return;
        const double eccentricity = 3.0;
        // y = -(2 ^ 3) * abs(x - 0.5) ^ 3 + 1
        position = -pow(2, eccentricity) * pow(abs(progress - 0.5), eccentricity) + 1;
        
        _factory->jumpPosition(position);
        
        _mapManager->JumpUpdate(position, position);
        
        
        // progress: 0 - 1 position: 0 - 1 - 0
        //cout << progress << " | " << position << endl;
        //cout << position << endl;
        
        int length = _images->size();
        for(int i = 0; i < length; ++i){
            BgImageData * d = (*_images)[i];
            d->image->Y(d->changeY * position + d->initY);
        }
    }
    int Type(){ return ANI_BackgroundInJump; }
private:
    double _strength;
    vector<BgImageData*>* _images;
    Factory* _factory;
    
    MapManager* _mapManager;
    BackgroundImages* _bgImages;
};


#endif
