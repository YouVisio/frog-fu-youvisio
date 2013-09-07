//
//  StartScreenState.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-11-28.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_StartScreenState_hpp
#define FrogFu_StartScreenState_hpp

#import "Image.hpp"
#import "InsectMove.hpp"

class StartScreenState {
public:
    StartScreenState(){
        IsClassicHit = false;
        IsEasyPlayHit = false;
        IsSprintHit = false;
        
        int firstBtnX = 80;
        int lastBtnX = Factory::Instance->SCREEN_WIDTH - 80;
        int btnLineLength = lastBtnX - firstBtnX;
        int btnLinePart = round(btnLineLength / 4.0);
        
        BtnInfoPos = vec2(lastBtnX, 60);
        BtnMusicPos = vec2(firstBtnX, 60);
        BtnEffectsPos = vec2(firstBtnX + btnLinePart * 1, 60);
        BtnAchievementsPos = vec2(firstBtnX + btnLinePart * 2, 60);
        BtnLeaderboardsPos = vec2(firstBtnX + btnLinePart * 3, 60);
    }
    bool IsClassicHit;
    bool IsEasyPlayHit;
    bool IsSprintHit;
    Image* Background;
    Image* Title;
    Image* BtnClassic;
    Image* BtnSprint;
    Image* BtnEasyPlay;
    Image* InsectClassic;
    Image* InsectSprint;
    Image* InsectEasyPlay;
    Image* Loading;
    Image* BtnInfo;
    Image* BtnMusic;
    Image* BtnEffects;
    Image* BtnAchievements;
    Image* BtnLeaderboards;
    Image* BtnPlay;
    Image* Tongue;
    Image* InfoBox;
    vec2 BtnInfoPos;
    vec2 BtnMusicPos;
    vec2 BtnEffectsPos;
    vec2 BtnAchievementsPos;
    vec2 BtnLeaderboardsPos;
    vec2 TitlePos;
    simlist<Drawable>* Drawables;
    
    InsectMove* FlyClassic;
    InsectMove* FlySprint;
    InsectMove* FlyEasyPlay;
    
    vec2 FrogPos;
    
    bool IsAnyGameHit(){
        return IsClassicHit || IsEasyPlayHit || IsSprintHit;
    }
};

#endif
