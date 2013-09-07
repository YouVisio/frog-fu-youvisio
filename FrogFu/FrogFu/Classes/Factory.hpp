//
//  Factory.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-11.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_Factory_hpp
#define FrogFu_Factory_hpp

#import "Drawable.hpp"
#import "IRenderingEngine.hpp"
#import "IResourceManager.hpp"
#import "ISceneEngine.hpp"
#import "Interfaces.hpp"
#import "Image.hpp"
#import "UserState.hpp"
#import "Settings.hpp"
#import <map>

class BackgroundImages{
public:
    Image* BgFront;
    Image* BgPlatform;
    Image* BgWave1;
    Image* BgWave2;
    Image* BgMain;
    Image* BgNearBg;//kamyshi
    Image* BgFarBg;//trees
    Image* BgBack;
    Image* BgSideOneLeft;
    Image* BgSideOneRight;
    Image* BgSideTwoLeft;
    Image* BgSideTwoRight;
    
    BackgroundImages(){
        BgFront = NULL;
        BgPlatform = NULL;
        BgWave1 = NULL;
        BgWave2 = NULL;
        BgMain = NULL;
        BgNearBg = NULL;
        BgFarBg = NULL;
        BgBack = NULL;
        BgSideOneLeft = NULL;
        BgSideOneRight = NULL;
        BgSideTwoLeft = NULL;
        BgSideTwoRight = NULL;
    }
};


class Factory{
public:
    static Factory* Instance;
    static Factory* Get();
    
    
    void createScenes();
    void (*ChangeSceneEngine)(ISceneEngine* engine);
    vec2 frogPlainToScreen(vec2 point);
    vec2 bgPlainToScreen(vec2 point);
    vec2 screenToFrogPlain(vec2 point);
    vec2 frogPlainToMap(vec2 point);
    vec2 getHeadBindingPoint(int bodyPoseID);
    FrogImage* getHeadBinding(int bodyPoseID);
    //vec2 getHeadTxtPosById(int headPoseID);
    vec2 getHeadCenterById(int headPoseID);
    vec2 getBodyCenterById(int bodyPoseID);
    vec2 getTongueLocationById(int headPoseID);
    rect getFrameInTxtByPoseId(int poseID);
    double getTongueLengthByPoseId(int tonguePoseID);
    void printIfDiff(double val);
    void printIfDiff2(double val);
    void transitionStartToClassic();
    void transitionClassicToStart();
    void transitionStartToSprint();
    void transitionSprintToStart();
    void transitionStartToEasyPlay();
    void transitionEasyPlayToStart();
    void setEnableInput(bool enable);
    bool getEnableInput();
    void applicationWillResignActive();
    void applicationDidEnterBackground();
    void applicationWillEnterForeground();
    void applicationDidBecomeActive();
    void applicationWillTerminate();
    map<int, PointArrays*>* getInitialFontDescription();
    map<int, PointArrays*>* getBigNumFontDescription();
    map<int, PointArrays*>* getMedNumFontDescription();
    map<int, PointArrays*>* getBonusNumFontDescription();
    map<int, PointArrays*>* getScoreNumFontDescription();
    map<int, PointArrays*>* getScoreBestNumFontDescription();
    map<int, PointArrays*>* getScoreTimeNumFontDescription();
    void copyAtlas(TextureDescription* source, TextureDescription* destination);
    
    map<std::string, SoundInfo*>* SoundInfos;
    
    double jumpProgress();
    void jumpProgress(double n);
    double jumpPosition();
    void jumpPosition(double n);
    int selectedInsects;
    int sceneID;
    int iOSversion;
    int skipWarnToRate;
    double lastJumpStrength;
    
    ISceneEngine* StartScreen;
    ISceneEngine* ClassicScene;
    ISceneEngine* SprintScene;
    ISceneEngine* EasyPlayScene;
    ISceneEngine* Scene;
    UserState* UserState;
    Settings* Settings;
    const char* DocsDir;
    IRenderingEngine* Renderer;
    IResourceManager* Resources;
    
    bool canPostToSocial;
    vec2 jumpInitBodyPos;
    vec2 staticBodyPos;
    double jumpRotation;
    long isDrawingTouchTrails;
    double particleWidth;
    double mapDotWidth;
    int numberMissed;
    int numberHit;
    bool forceClearExpired;
    bool enableInsectCreation;
    const char* stamp;
    
    int currentTouchId;
    bool isFive;
    bool isIpad;
    double SCREEN_WIDTH;
    double SCREEN_HEIGHT;
    double HALF_SCREEN_WIDTH;
    double HALF_SCREEN_HEIGHT;
    
    vector<double>* tongueDistances;
    
    //float frogPlainScale;
    float frogPlainShiftY;
    float bgPlainShiftY;
    float springDegree;
    vec2 bodyPos;
    
    bool isWithinJump;
    bool isPaused;
    float waspPositionAngle;

#if ISDEBUG
    Image* redDot;
    Image* blueDot;
    Image* greenDot;
#endif
    TextureDescription* TextureAtlas01;
    TextureDescription* TextureAtlas02;
    TextureDescription* TextureAtlas03;
    TextureDescription* TextureAtlas04;
    TextureDescription* TextureAtlas05;
    TextureDescription* TextureAtlas06;
    TextureDescription* TextureAtlas07;
    TextureDescription* TextureAtlas08;
    TextureDescription* TextureAtlas09;
    TextureDescription* TextureAtlas10;
    TextureDescription* TextureAtlas11;
    TextureDescription* TextureAtlas12;
    TextureDescription* TextureParticle;
    
private:
    Factory();
    Factory(Factory const&);
    void operator=(Factory const&);
    map<int, FrogImage>* _headBodyBindings;
    map<int, vec2>* _headTxtPosById;
    map<int, vec2>* _headCenterById;
    map<int, vec2>* _bodyCenterById;
    map<int, vec2>* _tongueLocationById;
    map<int, double>* _tongueLengthByPoseId;
    map<int, rect>* _frameInTxtByPoseId;
    map<long, vector<ActionAndState*>*>* _finalizersById;
    double _testVal;
    double _testVal2;
    bool _enableInput;
    map<int, PointArrays*>* _initialFont;
    map<int, PointArrays*>* _bigNumFont;
    map<int, PointArrays*>* _medNumFont;
    map<int, PointArrays*>* _bonusNumFont;
    map<int, PointArrays*>* _scoreNumFont;
    map<int, PointArrays*>* _scoreBestNumFont;
    map<int, PointArrays*>* _scoreTimeNumFont;
    double _jumpProgress;
    double _jumpPosition;
    
    void populateFonts();
    void populateTongueLengthByPoseId();
    void populateTongueDistances();
    void populateHeadBodyBindings();
    void populateHeadCenterById();
    void populateBodyCenterById();
    void populateTongueLocationById();
    void populateFrameInTxtByPoseId();
    PointArrays* CreateAndFillLetter(float x, float y, float w, float h, float textW, float textH);
};

#endif
