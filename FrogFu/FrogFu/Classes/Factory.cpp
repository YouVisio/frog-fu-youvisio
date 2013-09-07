//
//  Factory.cpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-05.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#import "Factory.hpp"
#import "StartScreen.hpp"
#import "ClassicScene.hpp"
#import "SprintScene.hpp"
#import "EasyPlayScene.hpp"
#import <vector>


Factory* Factory::Instance = NULL;

Factory::Factory(){
    Resources = CreateResourceManager();
    Renderer = CreateRenderingEngine(Resources);
    
    SoundInfos = new map<std::string, SoundInfo*>();
    
    populateFonts();
    
    _enableInput = true;
    
    frogPlainShiftY = 0;
    bgPlainShiftY = 0;
    lastJumpStrength = 0;
    canPostToSocial = true;
    
    forceClearExpired = false;
    
    numberHit = 0;
    numberMissed = 0;
    sceneID = 0;
    skipWarnToRate = 0;
    
    jumpInitBodyPos = vec2(320,450);
    staticBodyPos = vec2(320,195);
    
    isWithinJump = false;
    isPaused = false;
    
    springDegree = -1.0;
    _jumpPosition = _jumpProgress = 0.0;
    jumpRotation = 0.0;
    particleWidth = 10;
    mapDotWidth = 10;
    currentTouchId = 0;
    selectedInsects = 0;
    
    isDrawingTouchTrails = false;
    enableInsectCreation = false;
    
    _testVal = -BILLION+1;
    
    isFive = false;
    isIpad = false;
    SCREEN_WIDTH = 640;
    SCREEN_HEIGHT = 960;
    HALF_SCREEN_WIDTH = SCREEN_WIDTH / 2.0;
    HALF_SCREEN_HEIGHT = SCREEN_HEIGHT / 2.0;
    
    _finalizersById = new map<long, vector<ActionAndState*>*>();
    
    TextureAtlas01 = new TextureDescription();
    TextureAtlas02 = new TextureDescription();
    TextureAtlas03 = new TextureDescription();
    TextureAtlas04 = new TextureDescription();
    TextureAtlas05 = new TextureDescription();
    TextureAtlas06 = new TextureDescription();
    TextureAtlas07 = new TextureDescription();
    TextureAtlas08 = new TextureDescription();
    TextureAtlas09 = new TextureDescription();
    TextureAtlas10 = new TextureDescription();
    TextureAtlas11 = new TextureDescription();
    TextureAtlas12 = new TextureDescription();
    
    TextureAtlas01->ID = 1;
    TextureAtlas02->ID = 2;
    TextureAtlas03->ID = 3;
    TextureAtlas04->ID = 4;
    TextureAtlas05->ID = 5;
    TextureAtlas06->ID = 6;
    TextureAtlas07->ID = 7;
    TextureAtlas08->ID = 8;
    TextureAtlas09->ID = 9;
    TextureAtlas10->ID =10;
    TextureAtlas11->ID =11;
    TextureAtlas12->ID =12;
    
    TextureAtlas01->Size = ivec2(2048,2048);
    TextureAtlas02->Size = ivec2(1024,1024);
    TextureAtlas03->Size = ivec2(1024,1024);
    TextureAtlas04->Size = ivec2(1024,1024);
    TextureAtlas05->Size = ivec2(1024,1024);
    TextureAtlas06->Size = ivec2(1024,1024);
    TextureAtlas07->Size = ivec2(1024,1024);
    TextureAtlas08->Size = ivec2(1024,1024);
    TextureAtlas09->Size = ivec2(1024,1024);
    TextureAtlas10->Size = ivec2(2048,2048);
    TextureAtlas11->Size = ivec2(1024,1024);
    TextureAtlas12->Size = ivec2(1024,1024);
}
Factory* Factory::Get(){
    if(Instance == NULL){
        Instance = new Factory();
    }
    return Instance;
}
double Factory::jumpProgress(){ return _jumpProgress; }
void Factory::jumpProgress(double n){ _jumpProgress = n; }
double Factory::jumpPosition(){ return _jumpPosition; }
void Factory::jumpPosition(double n){
    _jumpPosition = n;
}
void Factory::applicationWillResignActive(){
    Renderer->applicationWillResignActive();
    Scene->applicationWillResignActive();
    
    cout << "applicationWillResignActive" << endl;
}
void Factory::applicationDidEnterBackground(){
    Renderer->applicationDidEnterBackground();
    Scene->applicationDidEnterBackground();
    
    cout << "applicationDidEnterBackground" << endl;
}
void Factory::applicationWillEnterForeground(){
    Renderer->applicationWillEnterForeground();
    Scene->applicationWillEnterForeground();
    
    cout << "applicationWillEnterForeground" << endl;
}
void Factory::applicationDidBecomeActive(){
    Renderer->applicationDidBecomeActive();
    Scene->applicationDidBecomeActive();
    
    cout << "applicationDidBecomeActive" << endl;
}
void Factory::applicationWillTerminate(){
    Renderer->applicationWillTerminate();
    Scene->applicationWillTerminate();
    
    cout << "applicationWillTerminate" << endl;
}
void Factory::transitionStartToClassic(){
    Scene->TakeOffScreen();
    Renderer->UnloadTextureById(12);
    Renderer->LoadTextureById(2);
    copyAtlas(TextureAtlas02, TextureAtlas08);
    Scene = ClassicScene;
    Renderer->ClearCurrentTexture();
    Scene->TransitionIndex(1);
    Scene->Initialize();
    analyticsScreen("classicInit");
}
void Factory::transitionClassicToStart(){
    Scene->TakeOffScreen();
    Renderer->UnloadTextureById(4);
    Renderer->UnloadTextureById(5);
    Renderer->LoadTextureById(7);
    Renderer->LoadTextureById(12);
    Scene = StartScreen;
    Renderer->ClearCurrentTexture();
    Scene->TransitionIndex(1);
    Scene->Initialize();
}
void Factory::transitionStartToSprint(){
    Scene->TakeOffScreen();
    Renderer->UnloadTextureById(12);
    Renderer->UnloadTextureById(3);
    Renderer->LoadTextureById(8);
    Renderer->LoadTextureById(9);
    copyAtlas(TextureAtlas08, TextureAtlas02);
    copyAtlas(TextureAtlas09, TextureAtlas03);
    Scene = SprintScene;
    Renderer->ClearCurrentTexture();
    Scene->TransitionIndex(1);
    Scene->Initialize();
    analyticsScreen("sprintInit");
}
void Factory::transitionSprintToStart(){
    Scene->TakeOffScreen();
    Renderer->UnloadTextureById(4);
    Renderer->UnloadTextureById(5);
    Renderer->UnloadTextureById(9);
    Renderer->LoadTextureById(7);
    Renderer->LoadTextureById(3);
    Renderer->LoadTextureById(12);
    Scene = StartScreen;
    Renderer->ClearCurrentTexture();
    Scene->TransitionIndex(2);
    Scene->Initialize();
}
void Factory::transitionStartToEasyPlay(){
    Scene->TakeOffScreen();
    Renderer->UnloadTextureById(1);
    Renderer->UnloadTextureById(12);
    Renderer->LoadTextureById(10);
    copyAtlas(TextureAtlas10, TextureAtlas01);
    Renderer->LoadTextureById(11);
    copyAtlas(TextureAtlas11, TextureAtlas02);
    Scene = EasyPlayScene;
    Renderer->ClearCurrentTexture();
    Scene->TransitionIndex(1);
    Scene->Initialize();
    analyticsScreen("easyPlayInit");
}
void Factory::transitionEasyPlayToStart(){
    Scene->TakeOffScreen();
    Renderer->UnloadTextureById(11);
    Renderer->UnloadTextureById(10);
    Renderer->LoadTextureById(1);
    copyAtlas(TextureAtlas01, TextureAtlas10);
    Renderer->UnloadTextureById(4);
    Renderer->UnloadTextureById(5);
    Renderer->LoadTextureById(7);
    Renderer->LoadTextureById(12);
    Scene = StartScreen;
    Renderer->ClearCurrentTexture();
    Scene->Initialize();
}
void Factory::copyAtlas(TextureDescription* source, TextureDescription* destination){
    destination->Name = source->Name;
    destination->MipCount = source->MipCount;
    destination->Format = source->Format;
    destination->Size.x = source->Size.x;
    destination->Size.y = source->Size.y;
    destination->BitsPerComponent = source->BitsPerComponent;
}
void Factory::createScenes(){
    StartScreen = CreateStartScreen(Renderer);
    StartScreen->Construct();
    Scene = StartScreen;
    
    ClassicScene = CreateClassicScene(Renderer);
    ClassicScene->Construct();
    ClassicScene->InitFromStartScene(StartScreen);
    
    
    SprintScene = CreateSprintScene(Renderer);
    SprintScene->Construct();
    SprintScene->InitFromStartScene(StartScreen);
    
    
    EasyPlayScene = CreateEasyPlayScene(Renderer);
    EasyPlayScene->Construct();
    EasyPlayScene->InitFromStartScene(StartScreen);

    
    populateTongueDistances();
    populateTongueLengthByPoseId();
    populateHeadBodyBindings();
    populateHeadCenterById();
    populateBodyCenterById();
    populateTongueLocationById();
    populateFrameInTxtByPoseId();
}
void Factory::populateFrameInTxtByPoseId(){
    map<int, rect>* h = _frameInTxtByPoseId = new map<int, rect>();
    
    // head - static pose
    h->insert(pair<int, rect>(50, rect(1,0,184,108)));
    h->insert(pair<int, rect>(51, rect(186,0,184,108)));
    h->insert(pair<int, rect>(52, rect(371,1,184,107)));
    h->insert(pair<int, rect>(53, rect(556,1,184,107)));
    h->insert(pair<int, rect>(54, rect(1,109,184,107)));
    h->insert(pair<int, rect>(55, rect(186,109,184,107)));
    h->insert(pair<int, rect>(56, rect(371,109,184,107)));
    h->insert(pair<int, rect>(57, rect(556,109,184,107)));
    h->insert(pair<int, rect>(58, rect(1,217,184,108)));
    h->insert(pair<int, rect>(59, rect(186,217,184,107)));
    h->insert(pair<int, rect>(60, rect(371,218,184,106)));
    h->insert(pair<int, rect>(61, rect(556,217,184,106)));
    h->insert(pair<int, rect>(62, rect(1,326,184,106)));
    h->insert(pair<int, rect>(63, rect(186,325,184,107)));
    h->insert(pair<int, rect>(64, rect(371,325,184,107)));
    h->insert(pair<int, rect>(65, rect(556,324,184,108)));
    h->insert(pair<int, rect>(66, rect(1,433,184,108)));
    h->insert(pair<int, rect>(67, rect(186,433,184,107)));
    h->insert(pair<int, rect>(68, rect(371,433,184,107)));
    h->insert(pair<int, rect>(69, rect(556,434,184,106)));
    h->insert(pair<int, rect>(70, rect(1,542,184,106)));
    h->insert(pair<int, rect>(71, rect(186,542,184,106)));
    h->insert(pair<int, rect>(72, rect(371,541,184,107)));
    h->insert(pair<int, rect>(73, rect(556,541,184,107)));
    
    // body - static pose
    h->insert(pair<int, rect>(100, rect(230,650,104,132)));
    
    // body - transition: static - ducking
    h->insert(pair<int, rect>(101, rect(0,648,116,128)));
    h->insert(pair<int, rect>(102, rect(115,648,114,133)));
    
    // body - kung fu
    h->insert(pair<int, rect>(120, rect(1,1,128,131)));
    h->insert(pair<int, rect>(121, rect(130,1,143,138)));
    h->insert(pair<int, rect>(122, rect(274,1,153,142)));
    h->insert(pair<int, rect>(123, rect(428,1,144,139)));
    h->insert(pair<int, rect>(124, rect(573,1,150,122)));
    h->insert(pair<int, rect>(125, rect(724,1,150,124)));
    h->insert(pair<int, rect>(126, rect(874,1,150,127)));
    h->insert(pair<int, rect>(127, rect(1,140,164,125)));
    h->insert(pair<int, rect>(128, rect(166,140,113,126)));
    h->insert(pair<int, rect>(129, rect(280,144,114,126)));
    h->insert(pair<int, rect>(130, rect(395,141,118,134)));
    h->insert(pair<int, rect>(131, rect(514,141,115,125)));
    h->insert(pair<int, rect>(132, rect(630,124,171,147)));
    h->insert(pair<int, rect>(133, rect(802,129,205,172)));
    h->insert(pair<int, rect>(134, rect(1,267,202,162)));
    h->insert(pair<int, rect>(135, rect(204,271,181,176)));
    h->insert(pair<int, rect>(136, rect(386,276,178,179)));
    h->insert(pair<int, rect>(137, rect(565,272,176,182)));
    h->insert(pair<int, rect>(138, rect(742,302,175,185)));
    h->insert(pair<int, rect>(139, rect(1,430,175,187)));
    h->insert(pair<int, rect>(140, rect(177,448,181,166)));
    h->insert(pair<int, rect>(141, rect(359,456,169,152)));
    h->insert(pair<int, rect>(142, rect(529,455,186,132)));
    
    // head - kung fu
    h->insert(pair<int, rect>(201, rect(716,488,184,105)));
    h->insert(pair<int, rect>(202, rect(1,618,184,105)));
    h->insert(pair<int, rect>(203, rect(186,615,184,105)));
    h->insert(pair<int, rect>(204, rect(371,609,184,106)));
    h->insert(pair<int, rect>(205, rect(556,594,184,106)));
    h->insert(pair<int, rect>(206, rect(741,594,184,106)));
    h->insert(pair<int, rect>(207, rect(1,724,184,106)));
    h->insert(pair<int, rect>(208, rect(186,721,184,105)));
    h->insert(pair<int, rect>(209, rect(371,716,184,105)));
    h->insert(pair<int, rect>(210, rect(556,701,184,105)));
    h->insert(pair<int, rect>(211, rect(741,701,184,105)));
    h->insert(pair<int, rect>(212, rect(1,831,184,105)));
    h->insert(pair<int, rect>(213, rect(186,827,184,105)));
    h->insert(pair<int, rect>(214, rect(371,822,184,105)));
    h->insert(pair<int, rect>(215, rect(556,807,184,105)));
    h->insert(pair<int, rect>(216, rect(741,807,184,106)));
    h->insert(pair<int, rect>(217, rect(545,913,184,108)));
    h->insert(pair<int, rect>(218, rect(730,914,184,108)));
    
    // head - loook right up
    h->insert(pair<int, rect>(300, rect(271,132,183,110)));
    h->insert(pair<int, rect>(301, rect(271,243,183,110)));
    h->insert(pair<int, rect>(302, rect(271,353,183,110)));
    h->insert(pair<int, rect>(303, rect(455,195,183,110)));
    h->insert(pair<int, rect>(304, rect(455,306,183,110)));
    
    // body - pointing
    h->insert(pair<int, rect>(400, rect(0,0,109,132)));
    h->insert(pair<int, rect>(401, rect(110,0,112,131)));
    h->insert(pair<int, rect>(402, rect(223,0,115,131)));
    h->insert(pair<int, rect>(403, rect(339,0,139,132)));
    h->insert(pair<int, rect>(404, rect(479,0,136,195)));
    h->insert(pair<int, rect>(405, rect(616,0,136,194)));
    h->insert(pair<int, rect>(406, rect(753,0,136,194)));
    h->insert(pair<int, rect>(407, rect(0,133,136,194)));
    h->insert(pair<int, rect>(408, rect(137,133,135,194)));
    h->insert(pair<int, rect>(409, rect(0,328,135,193)));
    h->insert(pair<int, rect>(410, rect(136,328,135,193))); 
    
    // body - ducking
    h->insert(pair<int, rect>(500, rect(0,892,144,132)));
    h->insert(pair<int, rect>(501, rect(144,893,154,131)));
    h->insert(pair<int, rect>(502, rect(298,910,165,114)));
    h->insert(pair<int, rect>(503, rect(463,911,175,113)));
    h->insert(pair<int, rect>(504, rect(638,912,185,112)));
    h->insert(pair<int, rect>(505, rect(823,913,201,111)));
    h->insert(pair<int, rect>(506, rect(0,778,217,113)));
    h->insert(pair<int, rect>(507, rect(217,769,230,122)));
    h->insert(pair<int, rect>(508, rect(447,759,245,132)));
    h->insert(pair<int, rect>(509, rect(692,744,258,147)));
    h->insert(pair<int, rect>(510, rect(0,605,271,163)));
    h->insert(pair<int, rect>(511, rect(271,596,271,162)));
    h->insert(pair<int, rect>(512, rect(542,582,271,161)));
    h->insert(pair<int, rect>(513, rect(456,420,271,161)));
    h->insert(pair<int, rect>(514, rect(727,421,271,160)));
    h->insert(pair<int, rect>(515, rect(640,260,272,159)));
    
    // head - ducking
    h->insert(pair<int, rect>(600, rect(0,0,159,113)));
    h->insert(pair<int, rect>(601, rect(160,1,159,110)));
    h->insert(pair<int, rect>(602, rect(320,2,159,110)));
    h->insert(pair<int, rect>(603, rect(480,0,159,114)));
    h->insert(pair<int, rect>(604, rect(640,1,159,118)));
    h->insert(pair<int, rect>(605, rect(800,0,154,119)));
    h->insert(pair<int, rect>(606, rect(0,114,154,121)));
    h->insert(pair<int, rect>(607, rect(155,113,154,124)));
    
    
    // body - jump
    h->insert(pair<int, rect>(700, rect(0,864,271,159)));
    h->insert(pair<int, rect>(701, rect(271,861,271,163)));
    h->insert(pair<int, rect>(702, rect(542,884,251,140)));
    h->insert(pair<int, rect>(703, rect(0,735,238,129)));
    h->insert(pair<int, rect>(704, rect(793,837,79,187)));
    h->insert(pair<int, rect>(705, rect(793,837,79,187)));
    h->insert(pair<int, rect>(706, rect(872,898,153,126)));
    h->insert(pair<int, rect>(707, rect(238,737,175,123)));
    h->insert(pair<int, rect>(708, rect(413,732,202,129)));
    h->insert(pair<int, rect>(709, rect(615,711,211,126)));
    h->insert(pair<int, rect>(710, rect(0,601,216,134)));
    h->insert(pair<int, rect>(711, rect(216,604,214,131)));
    h->insert(pair<int, rect>(712, rect(430,582,211,129)));
    h->insert(pair<int, rect>(800, rect(641,551,203,160)));
    h->insert(pair<int, rect>(801, rect(641,551,203,160)));
    h->insert(pair<int, rect>(802, rect(0,384,199,216)));
    h->insert(pair<int, rect>(803, rect(845,595,178,242)));
    h->insert(pair<int, rect>(804, rect(199,337,187,264)));
    h->insert(pair<int, rect>(805, rect(199,337,187,264)));
    h->insert(pair<int, rect>(806, rect(386,329,187,253)));
    h->insert(pair<int, rect>(807, rect(573,386,206,165)));
    h->insert(pair<int, rect>(808, rect(1,288,195,94)));
    h->insert(pair<int, rect>(809, rect(198,238,170,98)));
    h->insert(pair<int, rect>(810, rect(371,213,146,116)));
    h->insert(pair<int, rect>(811, rect(845,457,134,124)));
    h->insert(pair<int, rect>(812, rect(901,325,122,132)));
    
    // head - jump
    h->insert(pair<int, rect>(630, rect(0,0,159,113)));
    h->insert(pair<int, rect>(631, rect(161,0,159,113)));
    h->insert(pair<int, rect>(632, rect(321,0,160,115)));
    h->insert(pair<int, rect>(654, rect(482,0,160,123)));
    h->insert(pair<int, rect>(674, rect(643,0,162,125)));
    h->insert(pair<int, rect>(663, rect(806,0,178,120)));
    h->insert(pair<int, rect>(683, rect(0,114,176,128)));
    h->insert(pair<int, rect>(662, rect(177,114,170,125)));
    h->insert(pair<int, rect>(682, rect(348,124,176,125)));
    h->insert(pair<int, rect>(661, rect(525,126,186,124)));
    h->insert(pair<int, rect>(681, rect(712,126,183,130)));
    h->insert(pair<int, rect>(650, rect(0,243,161,133)));
    h->insert(pair<int, rect>(670, rect(162,243,155,138)));
    h->insert(pair<int, rect>(651, rect(318,250,186,124)));
    h->insert(pair<int, rect>(671, rect(505,251,182,133)));
    h->insert(pair<int, rect>(652, rect(688,257,170,125)));
    h->insert(pair<int, rect>(672, rect(0,378,176,125)));
    h->insert(pair<int, rect>(653, rect(177,382,179,120)));
    h->insert(pair<int, rect>(673, rect(357,374,177,128)));
    
}
// tongue length:
void Factory::populateTongueDistances(){
    tongueDistances = new vector<double>();
    tongueDistances->push_back(8.0);
    tongueDistances->push_back(15.0);
    tongueDistances->push_back(25.0);
    tongueDistances->push_back(40.0);
    tongueDistances->push_back(70.0);
    tongueDistances->push_back(120.0);
    tongueDistances->push_back(170.0);
    tongueDistances->push_back(220.0);
    tongueDistances->push_back(270.0);
    tongueDistances->push_back(320.0);
}
void Factory::populateTongueLengthByPoseId(){
    map<int, double>* h = _tongueLengthByPoseId = new map<int, double>();
    h->insert(pair<int, double>(900, 8.0));
    h->insert(pair<int, double>(901, 15.0));
    h->insert(pair<int, double>(902, 25.0));
    h->insert(pair<int, double>(903, 40.0));
    h->insert(pair<int, double>(904, 70.0));
    h->insert(pair<int, double>(905, 120.0));
    h->insert(pair<int, double>(906, 170.0));
    h->insert(pair<int, double>(907, 220.0));
    h->insert(pair<int, double>(908, 270.0));
    h->insert(pair<int, double>(909, 320.0));
}
void Factory::populateTongueLocationById(){
    map<int, vec2>* h = _tongueLocationById = new map<int, vec2>();
    h->insert(pair<int, vec2>(630, vec2(79, 96)));
    h->insert(pair<int, vec2>(631, vec2(80, 93)));
    h->insert(pair<int, vec2>(632, vec2(80, 90)));
    
    h->insert(pair<int, vec2>(650, vec2(81, 109)));
    h->insert(pair<int, vec2>(651, vec2(64, 95)));
    h->insert(pair<int, vec2>(652, vec2(53, 96)));
    h->insert(pair<int, vec2>(653, vec2(66, 87)));
    h->insert(pair<int, vec2>(654, vec2(81, 80)));
    
    h->insert(pair<int, vec2>(661, vec2(134, 93)));
    h->insert(pair<int, vec2>(662, vec2(122, 98)));
    h->insert(pair<int, vec2>(663, vec2(123, 87)));
    
    h->insert(pair<int, vec2>(670, vec2(80, 120)));
    h->insert(pair<int, vec2>(671, vec2(53, 107)));
    h->insert(pair<int, vec2>(672, vec2(40, 95)));
    h->insert(pair<int, vec2>(673, vec2(63, 82)));
    h->insert(pair<int, vec2>(674, vec2(81, 75)));
    
    h->insert(pair<int, vec2>(681, vec2(142, 105)));
    h->insert(pair<int, vec2>(682, vec2(139, 95)));
    h->insert(pair<int, vec2>(683, vec2(117, 83)));
}
void Factory::populateHeadBodyBindings(){
    map<int, FrogImage>* h = _headBodyBindings = new map<int, FrogImage>();
    // static forward
    h->insert(pair<int, FrogImage>(100, FrogImage(0,113)));
    h->insert(pair<int, FrogImage>(101, FrogImage(3,113)));
    h->insert(pair<int, FrogImage>(102, FrogImage(2,116)));
    
    
    // kong fu
    h->insert(pair<int, FrogImage>(120, FrogImage(1,115)));
    h->insert(pair<int, FrogImage>(121, FrogImage(2,122)));
    h->insert(pair<int, FrogImage>(122, FrogImage(2,126)));
    h->insert(pair<int, FrogImage>(123, FrogImage(41,95)));
    h->insert(pair<int, FrogImage>(124, FrogImage(41,99)));
    h->insert(pair<int, FrogImage>(125, FrogImage(41,102)));
    h->insert(pair<int, FrogImage>(126, FrogImage(41,104)));
    h->insert(pair<int, FrogImage>(127, FrogImage(33,105)));
    h->insert(pair<int, FrogImage>(128, FrogImage(15,109)));
    h->insert(pair<int, FrogImage>(129, FrogImage(12,109)));
    h->insert(pair<int, FrogImage>(130, FrogImage(2,117)));
    h->insert(pair<int, FrogImage>(131, FrogImage(0,108)));
    h->insert(pair<int, FrogImage>(132, FrogImage(-3,130)));
    h->insert(pair<int, FrogImage>(133, FrogImage(-6,151)));
    h->insert(pair<int, FrogImage>(134, FrogImage(-3,145)));
    h->insert(pair<int, FrogImage>(135, FrogImage(0,136)));
    h->insert(pair<int, FrogImage>(136, FrogImage(0,135)));
    h->insert(pair<int, FrogImage>(137, FrogImage(0,134)));
    h->insert(pair<int, FrogImage>(138, FrogImage(0,131)));
    h->insert(pair<int, FrogImage>(139, FrogImage(0,127)));
    h->insert(pair<int, FrogImage>(140, FrogImage(0,124)));
    h->insert(pair<int, FrogImage>(141, FrogImage(0,120)));
    h->insert(pair<int, FrogImage>(142, FrogImage(0,116)));
    
    // pointing
    double a = 0;
    h->insert(pair<int, FrogImage>(400, FrogImage(0,114, a)));
    h->insert(pair<int, FrogImage>(401, FrogImage(3,114, a)));
    h->insert(pair<int, FrogImage>(402, FrogImage(5,113, a)));
    a = -6.8272*DTR;
    h->insert(pair<int, FrogImage>(403, FrogImage(13,114, a)));
    a = -13.6976*DTR;
    h->insert(pair<int, FrogImage>(404, FrogImage(26,114, a)));
    h->insert(pair<int, FrogImage>(405, FrogImage(26,112, a)));
    h->insert(pair<int, FrogImage>(406, FrogImage(26,111, a)));
    h->insert(pair<int, FrogImage>(407, FrogImage(26,111, a)));
    h->insert(pair<int, FrogImage>(408, FrogImage(26,111, a)));
    h->insert(pair<int, FrogImage>(409, FrogImage(26,111, a)));
    h->insert(pair<int, FrogImage>(410, FrogImage(26,111, a)));
    
    // ducking
    h->insert(pair<int, FrogImage>(500, FrogImage(0,115)));
    h->insert(pair<int, FrogImage>(501, FrogImage(0,115-1)));
    h->insert(pair<int, FrogImage>(502, FrogImage(0,115-20)));
    h->insert(pair<int, FrogImage>(503, FrogImage(0,115-23)));
    h->insert(pair<int, FrogImage>(504, FrogImage(0,115-26)));
    h->insert(pair<int, FrogImage>(505, FrogImage(0,115-29)));
    h->insert(pair<int, FrogImage>(506, FrogImage(0,115-32)));
    h->insert(pair<int, FrogImage>(507, FrogImage(0,115-35)));
    h->insert(pair<int, FrogImage>(508, FrogImage(0,115-38)));
    h->insert(pair<int, FrogImage>(509, FrogImage(0,115-41)));
    h->insert(pair<int, FrogImage>(510, FrogImage(0,115-44)));
    h->insert(pair<int, FrogImage>(511, FrogImage(0,115-47)));
    h->insert(pair<int, FrogImage>(512, FrogImage(0,115-50)));
    h->insert(pair<int, FrogImage>(513, FrogImage(0,115-55)));
    h->insert(pair<int, FrogImage>(514, FrogImage(0,115-59)));
    h->insert(pair<int, FrogImage>(515, FrogImage(0,115-65)));
    
    // jump up
    h->insert(pair<int, FrogImage>(700, FrogImage(0,70)));
    h->insert(pair<int, FrogImage>(701, FrogImage(0,85)));
    h->insert(pair<int, FrogImage>(702, FrogImage(0,90)));
    h->insert(pair<int, FrogImage>(703, FrogImage(0,95)));
    h->insert(pair<int, FrogImage>(704, FrogImage(0,212)));
    h->insert(pair<int, FrogImage>(705, FrogImage(0,0)));
    h->insert(pair<int, FrogImage>(706, FrogImage(0,0)));
    h->insert(pair<int, FrogImage>(707, FrogImage(0,0)));
    h->insert(pair<int, FrogImage>(708, FrogImage(0,0)));
    h->insert(pair<int, FrogImage>(709, FrogImage(0,0)));
    h->insert(pair<int, FrogImage>(710, FrogImage(0,0)));
    h->insert(pair<int, FrogImage>(711, FrogImage(0,0)));
    h->insert(pair<int, FrogImage>(712, FrogImage(0,0)));
    h->insert(pair<int, FrogImage>(799, FrogImage(0,0)));
    
    // jump down
    h->insert(pair<int, FrogImage>(800, FrogImage(0,0)));
    h->insert(pair<int, FrogImage>(801, FrogImage(0,0)));
    h->insert(pair<int, FrogImage>(802, FrogImage(0,0)));
    h->insert(pair<int, FrogImage>(803, FrogImage(0,0)));
    h->insert(pair<int, FrogImage>(804, FrogImage(0,0)));
    h->insert(pair<int, FrogImage>(805, FrogImage(4,169)));
    h->insert(pair<int, FrogImage>(806, FrogImage(4,169)));
    h->insert(pair<int, FrogImage>(807, FrogImage(3,84)));
    h->insert(pair<int, FrogImage>(808, FrogImage(2,81)));
    h->insert(pair<int, FrogImage>(809, FrogImage(2,84)));
    h->insert(pair<int, FrogImage>(810, FrogImage(3,102)));
    h->insert(pair<int, FrogImage>(811, FrogImage(2,110)));
    h->insert(pair<int, FrogImage>(812, FrogImage(2,118)));
    
}
void Factory::populateBodyCenterById(){
    map<int, vec2>* h = _bodyCenterById = new map<int, vec2>();
    
    // static and static to pointing
    h->insert(pair<int, vec2>(100, vec2(51, 120)));
    h->insert(pair<int, vec2>(101, vec2(59, 120)));
    h->insert(pair<int, vec2>(102, vec2(52, 125)));
    
    // kung fu
    h->insert(pair<int, vec2>(120, vec2(71, 122)));
    h->insert(pair<int, vec2>(121, vec2(83, 130)));
    h->insert(pair<int, vec2>(122, vec2(83, 134)));
    h->insert(pair<int, vec2>(123, vec2(47, 127)));
    h->insert(pair<int, vec2>(124, vec2(53, 110)));
    h->insert(pair<int, vec2>(125, vec2(53, 112)));
    h->insert(pair<int, vec2>(126, vec2(53, 115)));
    h->insert(pair<int, vec2>(127, vec2(61, 115)));
    h->insert(pair<int, vec2>(128, vec2(56, 117)));
    h->insert(pair<int, vec2>(129, vec2(56, 117)));
    h->insert(pair<int, vec2>(130, vec2(54, 124)));
    h->insert(pair<int, vec2>(131, vec2(62, 115)));
    h->insert(pair<int, vec2>(132, vec2(87, 137)));
    h->insert(pair<int, vec2>(133, vec2(91, 162)));
    h->insert(pair<int, vec2>(134, vec2(92, 152)));
    h->insert(pair<int, vec2>(135, vec2(93, 166)));
    h->insert(pair<int, vec2>(136, vec2(92, 169)));
    h->insert(pair<int, vec2>(137, vec2(92, 172)));
    h->insert(pair<int, vec2>(138, vec2(92, 175)));
    h->insert(pair<int, vec2>(139, vec2(92, 177)));
    h->insert(pair<int, vec2>(140, vec2(92, 156)));
    h->insert(pair<int, vec2>(141, vec2(92, 142)));
    h->insert(pair<int, vec2>(142, vec2(82, 122)));
    
    // pointing
    h->insert(pair<int, vec2>(400, vec2(50, 119)));
    h->insert(pair<int, vec2>(401, vec2(53, 120)));
    h->insert(pair<int, vec2>(402, vec2(56, 120)));
    h->insert(pair<int, vec2>(403, vec2(85, 120)));
    h->insert(pair<int, vec2>(404, vec2(70, 183)));
    h->insert(pair<int, vec2>(405, vec2(70, 182)));
    h->insert(pair<int, vec2>(406, vec2(70, 182)));
    h->insert(pair<int, vec2>(407, vec2(70, 182)));
    h->insert(pair<int, vec2>(408, vec2(69, 182)));
    h->insert(pair<int, vec2>(409, vec2(69, 181)));
    h->insert(pair<int, vec2>(410, vec2(69, 181)));
    
    // ducking
    h->insert(pair<int, vec2>(500, vec2(71, 120)));
    h->insert(pair<int, vec2>(501, vec2(77, 119)));
    h->insert(pair<int, vec2>(502, vec2(83, 102)));
    h->insert(pair<int, vec2>(503, vec2(89, 101)));
    h->insert(pair<int, vec2>(504, vec2(94, 100)));
    h->insert(pair<int, vec2>(505, vec2(102, 99)));
    h->insert(pair<int, vec2>(506, vec2(109, 101)));
    h->insert(pair<int, vec2>(507, vec2(115, 110)));
    h->insert(pair<int, vec2>(508, vec2(122, 120)));
    h->insert(pair<int, vec2>(509, vec2(128, 135)));
    h->insert(pair<int, vec2>(510, vec2(133, 151)));
    h->insert(pair<int, vec2>(511, vec2(133, 150)));
    h->insert(pair<int, vec2>(512, vec2(133, 149)));
    h->insert(pair<int, vec2>(513, vec2(133, 149)));
    h->insert(pair<int, vec2>(514, vec2(133, 148)));
    h->insert(pair<int, vec2>(515, vec2(134, 147)));
    
    // jump up
    h->insert(pair<int, vec2>(700, vec2(132,149)));
    h->insert(pair<int, vec2>(701, vec2(132,153)));
    h->insert(pair<int, vec2>(702, vec2(122,130)));
    h->insert(pair<int, vec2>(703, vec2(118,119)));
    h->insert(pair<int, vec2>(704, vec2(35,220)));
    h->insert(pair<int, vec2>(705, vec2(35,4)));
    h->insert(pair<int, vec2>(706, vec2(79,9)));
    h->insert(pair<int, vec2>(707, vec2(89,9)));
    h->insert(pair<int, vec2>(708, vec2(100,18)));
    h->insert(pair<int, vec2>(709, vec2(106,25)));
    h->insert(pair<int, vec2>(710, vec2(110,36)));
    h->insert(pair<int, vec2>(711, vec2(108,33)));
    h->insert(pair<int, vec2>(712, vec2(106,32)));
    h->insert(pair<int, vec2>(799, vec2(106,32)));
    
    // jump down
    h->insert(pair<int, vec2>(800, vec2(100,38)));
    h->insert(pair<int, vec2>(801, vec2(100,38)));
    h->insert(pair<int, vec2>(802, vec2(99,39)));
    h->insert(pair<int, vec2>(803, vec2(87,66)));
    h->insert(pair<int, vec2>(804, vec2(97,91)));
    h->insert(pair<int, vec2>(805, vec2(92,255)));
    h->insert(pair<int, vec2>(806, vec2(92,243)));
    h->insert(pair<int, vec2>(807, vec2(97,155)));
    h->insert(pair<int, vec2>(808, vec2(93,84)));
    h->insert(pair<int, vec2>(809, vec2(80,88)));
    h->insert(pair<int, vec2>(810, vec2(67,106)));
    h->insert(pair<int, vec2>(811, vec2(61,114)));
    h->insert(pair<int, vec2>(812, vec2(55,122)));
}
void Factory::populateHeadCenterById(){
    map<int, vec2>* h = _headCenterById = new map<int, vec2>();
    h->insert(pair<int, vec2>(50, vec2(74,108)));
    h->insert(pair<int, vec2>(51, vec2(74,108)));
    h->insert(pair<int, vec2>(52, vec2(74,107)));
    h->insert(pair<int, vec2>(53, vec2(74,107)));
    h->insert(pair<int, vec2>(54, vec2(74,107)));
    h->insert(pair<int, vec2>(55, vec2(74,107)));
    h->insert(pair<int, vec2>(56, vec2(74,107)));
    h->insert(pair<int, vec2>(57, vec2(74,107)));
    h->insert(pair<int, vec2>(58, vec2(74,108)));
    h->insert(pair<int, vec2>(59, vec2(74,107)));
    h->insert(pair<int, vec2>(60, vec2(74,106)));
    h->insert(pair<int, vec2>(61, vec2(74,106)));
    h->insert(pair<int, vec2>(62, vec2(74,106)));
    h->insert(pair<int, vec2>(63, vec2(74,107)));
    h->insert(pair<int, vec2>(64, vec2(74,107)));
    h->insert(pair<int, vec2>(65, vec2(74,108)));
    h->insert(pair<int, vec2>(66, vec2(74,108)));
    h->insert(pair<int, vec2>(67, vec2(74,107)));
    h->insert(pair<int, vec2>(68, vec2(74,107)));
    h->insert(pair<int, vec2>(69, vec2(74,106)));
    h->insert(pair<int, vec2>(70, vec2(74,106)));
    h->insert(pair<int, vec2>(71, vec2(74,106)));
    h->insert(pair<int, vec2>(72, vec2(74,107)));
    h->insert(pair<int, vec2>(73, vec2(74,107)));
    
    // kung fu
    h->insert(pair<int, vec2>(201, vec2(74,106)));
    h->insert(pair<int, vec2>(202, vec2(74,105)));
    h->insert(pair<int, vec2>(203, vec2(74,105)));
    h->insert(pair<int, vec2>(204, vec2(74,105)));
    h->insert(pair<int, vec2>(205, vec2(74,105)));
    h->insert(pair<int, vec2>(206, vec2(74,106)));
    h->insert(pair<int, vec2>(207, vec2(74,106)));
    h->insert(pair<int, vec2>(208, vec2(74,105)));
    h->insert(pair<int, vec2>(209, vec2(74,105)));
    h->insert(pair<int, vec2>(210, vec2(74,105)));
    h->insert(pair<int, vec2>(211, vec2(74,105)));
    h->insert(pair<int, vec2>(212, vec2(74,105)));
    h->insert(pair<int, vec2>(213, vec2(74,105)));
    h->insert(pair<int, vec2>(214, vec2(74,105)));
    h->insert(pair<int, vec2>(215, vec2(74,105)));
    h->insert(pair<int, vec2>(216, vec2(74,106)));
    h->insert(pair<int, vec2>(217, vec2(74,107)));
    h->insert(pair<int, vec2>(218, vec2(74,108)));
    
    h->insert(pair<int, vec2>(600, vec2(79,108)));
    h->insert(pair<int, vec2>(601, vec2(80,103)));
    h->insert(pair<int, vec2>(602, vec2(81,101)));
    h->insert(pair<int, vec2>(603, vec2(81,100)));
    h->insert(pair<int, vec2>(604, vec2(81,100)));
    h->insert(pair<int, vec2>(605, vec2(76,99)));
    h->insert(pair<int, vec2>(606, vec2(76,99)));
    h->insert(pair<int, vec2>(607, vec2(76,99)));
    
    h->insert(pair<int, vec2>(630, vec2(79,108)));
    h->insert(pair<int, vec2>(631, vec2(79,108)));
    h->insert(pair<int, vec2>(632, vec2(80,108)));
    
    h->insert(pair<int, vec2>(650, vec2(81,110)));
    h->insert(pair<int, vec2>(651, vec2(107,119)));
    h->insert(pair<int, vec2>(652, vec2(87,122)));
    h->insert(pair<int, vec2>(653, vec2(106,116)));
    h->insert(pair<int, vec2>(654, vec2(80,121)));
    
    h->insert(pair<int, vec2>(661, vec2(105,121)));
    h->insert(pair<int, vec2>(662, vec2(90,123)));
    h->insert(pair<int, vec2>(663, vec2(102,118)));
    
    h->insert(pair<int, vec2>(670, vec2(77,106)));
    h->insert(pair<int, vec2>(671, vec2(104,119)));
    h->insert(pair<int, vec2>(672, vec2(93,122)));
    h->insert(pair<int, vec2>(673, vec2(104,123)));
    h->insert(pair<int, vec2>(674, vec2(81,123)));
    
    h->insert(pair<int, vec2>(681, vec2(102,119)));
    h->insert(pair<int, vec2>(682, vec2(96,123)));
    h->insert(pair<int, vec2>(683, vec2(98,127)));
    
    h->insert(pair<int, vec2>(699, vec2(75,179)));
}
void Factory::setEnableInput(bool enable){
    _enableInput = enable;
}
bool Factory::getEnableInput(){ return _enableInput; }
rect Factory::getFrameInTxtByPoseId(int poseID){
    if(poseID == 0 || _frameInTxtByPoseId->find(poseID) == _frameInTxtByPoseId->end()){
        return rect(0,0,0,0);
    }
    return (*_frameInTxtByPoseId)[poseID];
}
void Factory::printIfDiff(double val){
    if(_testVal != val){
        cout << "1>>" << val << endl;
    }
    _testVal = val;
}
void Factory::printIfDiff2(double val){
    if(_testVal2 != val){
        cout << "2>>" << val << endl;
    }
    _testVal2 = val;
}
double Factory::getTongueLengthByPoseId(int tonguePoseID){
    if(tonguePoseID == 0 || _tongueLengthByPoseId->find(tonguePoseID) == _tongueLengthByPoseId->end()){
        return 5;
    }
    return (*_tongueLengthByPoseId)[tonguePoseID];
}
vec2 Factory::getTongueLocationById(int headPoseID){
    if(headPoseID == 0 || _tongueLocationById->find(headPoseID) == _tongueLocationById->end()){
        return vec2(110,107);
    }
    return (*_tongueLocationById)[headPoseID];
}
vec2 Factory::getHeadBindingPoint(int bodyPoseID){
    if(bodyPoseID == 0) return vec2(0,0);
    return (*_headBodyBindings)[bodyPoseID].pos;
}
FrogImage* Factory::getHeadBinding(int bodyPoseID){
    if(bodyPoseID == 0 || _headBodyBindings->find(bodyPoseID) == _headBodyBindings->end()){
        return NULL;
    }
    return &(*_headBodyBindings)[bodyPoseID];
}
vec2 Factory::getHeadCenterById(int headPoseID){
    if(headPoseID == 0 || _headCenterById->find(headPoseID) == _headCenterById->end()){
        return vec2(74, 108);
    }
    return (*_headCenterById)[headPoseID];
}
vec2 Factory::getBodyCenterById(int bodyPoseID){
    if(bodyPoseID == 0 || _bodyCenterById->find(bodyPoseID) == _bodyCenterById->end()){
        return vec2(NAN, NAN);
    }
    return (*_bodyCenterById)[bodyPoseID];
}
vec2 Factory::frogPlainToScreen(vec2 point){
    point.y -= frogPlainShiftY;
    return point;
}
vec2 Factory::bgPlainToScreen(vec2 point){
    point.y -= bgPlainShiftY;
    return point;
}
vec2 Factory::screenToFrogPlain(vec2 point){
    point.y += frogPlainShiftY;
    return point;
}
vec2 Factory::frogPlainToMap(vec2 point){
    
    if(isFive){
        point.Shift(MAP_X, MAP_Y+130);
        point = point.Scale(vec2(HALF_SCREEN_WIDTH+MAP_X, HALF_SCREEN_HEIGHT+MAP_Y+65), MAP_ZOOM);
    } else if(isIpad){
        point.Shift(MAP_X, MAP_Y);
        point = point.Scale(vec2(HALF_SCREEN_WIDTH+MAP_X, HALF_SCREEN_HEIGHT+MAP_Y-53), MAP_ZOOM);
    } else {
        point.Shift(MAP_X, MAP_Y);
        point = point.Scale(vec2(HALF_SCREEN_WIDTH+MAP_X, HALF_SCREEN_HEIGHT+MAP_Y), MAP_ZOOM);
    }
    return point;
}
map<int, PointArrays*>* Factory::getInitialFontDescription(){ return _initialFont; }
map<int, PointArrays*>* Factory::getBigNumFontDescription(){ return _bigNumFont; }
map<int, PointArrays*>* Factory::getMedNumFontDescription(){ return _medNumFont; }
map<int, PointArrays*>* Factory::getBonusNumFontDescription(){ return _bonusNumFont; }
map<int, PointArrays*>* Factory::getScoreNumFontDescription(){ return _scoreNumFont; }
map<int, PointArrays*>* Factory::getScoreBestNumFontDescription(){ return _scoreBestNumFont; }
map<int, PointArrays*>* Factory::getScoreTimeNumFontDescription(){ return _scoreTimeNumFont; }
PointArrays* Factory::CreateAndFillLetter(float x, float y, float w, float h, float texW, float texH){
    int numElements = 8;
    PointArrays * arrays = new PointArrays();
    arrays->vertices = new GLfloat[numElements];
    arrays->texCoord = new GLfloat[numElements];
    arrays->colors = new GLfloat[numElements*2];
    arrays->length = numElements;
    
    arrays->width = w;
    arrays->height = h;
    
    int i = 0;
    arrays->vertices[i++] = 0;
    arrays->vertices[i++] = 0;
    arrays->vertices[i++] = w;
    arrays->vertices[i++] = 0;
    arrays->vertices[i++] = 0;
    arrays->vertices[i++] = h;
    arrays->vertices[i++] = w;
    arrays->vertices[i++] = h;
    i = 0;
    float left = x/texW;
    float right = (x + w)/texW;
    float top = y/texH;
    float bottom = (y + h)/texH;
    
    arrays->texCoord[i++] = left;
    arrays->texCoord[i++] = bottom;
    arrays->texCoord[i++] = right;
    arrays->texCoord[i++] = bottom;
    arrays->texCoord[i++] = left;
    arrays->texCoord[i++] = top;
    arrays->texCoord[i++] = right;
    arrays->texCoord[i++] = top;
    return arrays;
}
void Factory::populateFonts(){
    _initialFont = new map<int, PointArrays*>();
    _initialFont->insert(pair<int, PointArrays*>('A', CreateAndFillLetter(1741.0f, 0.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('B', CreateAndFillLetter(1760.0f, 0.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('C', CreateAndFillLetter(1779.0f, 0.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('D', CreateAndFillLetter(1798.0f, 0.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('E', CreateAndFillLetter(1817.0f, 0.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('F', CreateAndFillLetter(1836.0f, 0.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('G', CreateAndFillLetter(1855.0f, 0.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('H', CreateAndFillLetter(1874.0f, 0.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('I', CreateAndFillLetter(1893.0f, 0.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('J', CreateAndFillLetter(1912.0f, 0.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('K', CreateAndFillLetter(1931.0f, 0.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('L', CreateAndFillLetter(1950.0f, 0.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('M', CreateAndFillLetter(1969.0f, 0.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('1', CreateAndFillLetter(1988.0f, 0.0f, 15.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('2', CreateAndFillLetter(2003.0f, 0.0f, 15.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('3', CreateAndFillLetter(2018.0f, 0.0f, 15.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('4', CreateAndFillLetter(2033.0f, 0.0f, 15.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('N', CreateAndFillLetter(1741.0f, 22.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('O', CreateAndFillLetter(1760.0f, 22.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('P', CreateAndFillLetter(1779.0f, 22.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('Q', CreateAndFillLetter(1798.0f, 22.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('R', CreateAndFillLetter(1817.0f, 22.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('S', CreateAndFillLetter(1836.0f, 22.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('T', CreateAndFillLetter(1855.0f, 22.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('U', CreateAndFillLetter(1874.0f, 22.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('V', CreateAndFillLetter(1893.0f, 22.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('W', CreateAndFillLetter(1912.0f, 22.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('X', CreateAndFillLetter(1931.0f, 22.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('Y', CreateAndFillLetter(1950.0f, 22.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('Z', CreateAndFillLetter(1969.0f, 22.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('5', CreateAndFillLetter(1988.0f, 22.0f, 15.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('6', CreateAndFillLetter(2003.0f, 22.0f, 15.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('7', CreateAndFillLetter(2018.0f, 22.0f, 15.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('8', CreateAndFillLetter(2033.0f, 22.0f, 15.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('a', CreateAndFillLetter(1741.0f, 44.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('b', CreateAndFillLetter(1760.0f, 44.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('c', CreateAndFillLetter(1779.0f, 44.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('d', CreateAndFillLetter(1798.0f, 44.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('e', CreateAndFillLetter(1817.0f, 44.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('f', CreateAndFillLetter(1836.0f, 44.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('g', CreateAndFillLetter(1855.0f, 44.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('h', CreateAndFillLetter(1874.0f, 44.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('i', CreateAndFillLetter(1893.0f, 44.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('j', CreateAndFillLetter(1912.0f, 44.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('k', CreateAndFillLetter(1931.0f, 44.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('l', CreateAndFillLetter(1950.0f, 44.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('m', CreateAndFillLetter(1969.0f, 44.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('9', CreateAndFillLetter(1988.0f, 44.0f, 15.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('0', CreateAndFillLetter(2003.0f, 44.0f, 15.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('.', CreateAndFillLetter(2018.0f, 44.0f, 15.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>(':', CreateAndFillLetter(2033.0f, 44.0f, 15.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('n', CreateAndFillLetter(1741.0f, 66.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('o', CreateAndFillLetter(1760.0f, 66.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('p', CreateAndFillLetter(1779.0f, 66.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('q', CreateAndFillLetter(1798.0f, 66.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('r', CreateAndFillLetter(1817.0f, 66.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('s', CreateAndFillLetter(1836.0f, 66.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('t', CreateAndFillLetter(1855.0f, 66.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('u', CreateAndFillLetter(1874.0f, 66.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('v', CreateAndFillLetter(1893.0f, 66.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('w', CreateAndFillLetter(1912.0f, 66.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('x', CreateAndFillLetter(1931.0f, 66.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('y', CreateAndFillLetter(1950.0f, 66.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('z', CreateAndFillLetter(1969.0f, 66.0f, 19.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('-', CreateAndFillLetter(1988.0f, 66.0f, 15.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('+', CreateAndFillLetter(2003.0f, 66.0f, 15.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>(',', CreateAndFillLetter(2018.0f, 66.0f, 15.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>(' ', CreateAndFillLetter(2033.0f, 66.0f, 15.0f, 22.0f, 2048.0f, 2048.0f)));
    _initialFont->insert(pair<int, PointArrays*>('%', CreateAndFillLetter(2033.0f, 88.0f, 15.0f, 22.0f, 2048.0f, 2048.0f)));
    
    double p = 1;
    _bigNumFont = new map<int, PointArrays*>();
    _bigNumFont->insert(pair<int, PointArrays*>('1', CreateAndFillLetter(  2.0f-p, 523.0f-p, 21.0f+p*2, 47.0f+p*2, 1024.0f, 1024.0f)));
    _bigNumFont->insert(pair<int, PointArrays*>('2', CreateAndFillLetter( 25.0f-p, 523.0f-p, 33.0f+p*2, 46.0f+p*2, 1024.0f, 1024.0f)));
    _bigNumFont->insert(pair<int, PointArrays*>('3', CreateAndFillLetter( 60.0f-p, 523.0f-p, 34.0f+p*2, 46.0f+p*2, 1024.0f, 1024.0f)));
    _bigNumFont->insert(pair<int, PointArrays*>('4', CreateAndFillLetter( 96.0f-p, 523.0f-p, 34.0f+p*2, 46.0f+p*2, 1024.0f, 1024.0f)));
    _bigNumFont->insert(pair<int, PointArrays*>('5', CreateAndFillLetter(132.0f-p, 523.0f-p, 36.0f+p*2, 46.0f+p*2, 1024.0f, 1024.0f)));
    _bigNumFont->insert(pair<int, PointArrays*>('6', CreateAndFillLetter(170.0f-p, 523.0f-p, 34.0f+p*2, 46.0f+p*2, 1024.0f, 1024.0f)));
    _bigNumFont->insert(pair<int, PointArrays*>('7', CreateAndFillLetter(206.0f-p, 523.0f-p, 34.0f+p*2, 46.0f+p*2, 1024.0f, 1024.0f)));
    _bigNumFont->insert(pair<int, PointArrays*>('8', CreateAndFillLetter(242.0f-p, 523.0f-p, 34.0f+p*2, 47.0f+p*2, 1024.0f, 1024.0f)));
    _bigNumFont->insert(pair<int, PointArrays*>('9', CreateAndFillLetter(278.0f-p, 523.0f-p, 32.0f+p*2, 46.0f+p*2, 1024.0f, 1024.0f)));
    _bigNumFont->insert(pair<int, PointArrays*>('0', CreateAndFillLetter(312.0f-p, 523.0f-p, 35.0f+p*2, 47.0f+p*2, 1024.0f, 1024.0f)));
    _bigNumFont->insert(pair<int, PointArrays*>('%', CreateAndFillLetter(349.0f-p, 521.0f-p, 39.0f+p*2, 50.0f+p*2, 1024.0f, 1024.0f)));
    _bigNumFont->insert(pair<int, PointArrays*>(' ', CreateAndFillLetter(390.0f-p, 523.0f-p, 39.0f+p*2, 50.0f+p*2, 1024.0f, 1024.0f)));
    
    _medNumFont = new map<int, PointArrays*>();
    _medNumFont->insert(pair<int, PointArrays*>('1', CreateAndFillLetter(  2.0f-p, 573.0f-p, 10.0f+p*2, 21.0f+p*2, 1024.0f, 1024.0f)));
    _medNumFont->insert(pair<int, PointArrays*>('2', CreateAndFillLetter( 14.0f-p, 573.0f-p, 15.0f+p*2, 21.0f+p*2, 1024.0f, 1024.0f)));
    _medNumFont->insert(pair<int, PointArrays*>('3', CreateAndFillLetter( 31.0f-p, 573.0f-p, 15.0f+p*2, 21.0f+p*2, 1024.0f, 1024.0f)));
    _medNumFont->insert(pair<int, PointArrays*>('4', CreateAndFillLetter( 48.0f-p, 573.0f-p, 15.0f+p*2, 21.0f+p*2, 1024.0f, 1024.0f)));
    _medNumFont->insert(pair<int, PointArrays*>('5', CreateAndFillLetter( 65.0f-p, 573.0f-p, 16.0f+p*2, 21.0f+p*2, 1024.0f, 1024.0f)));
    _medNumFont->insert(pair<int, PointArrays*>('6', CreateAndFillLetter( 83.0f-p, 573.0f-p, 15.0f+p*2, 21.0f+p*2, 1024.0f, 1024.0f)));
    _medNumFont->insert(pair<int, PointArrays*>('7', CreateAndFillLetter(100.0f-p, 573.0f-p, 16.0f+p*2, 21.0f+p*2, 1024.0f, 1024.0f)));
    _medNumFont->insert(pair<int, PointArrays*>('8', CreateAndFillLetter(118.0f-p, 573.0f-p, 15.0f+p*2, 21.0f+p*2, 1024.0f, 1024.0f)));
    _medNumFont->insert(pair<int, PointArrays*>('9', CreateAndFillLetter(135.0f-p, 573.0f-p, 14.0f+p*2, 21.0f+p*2, 1024.0f, 1024.0f)));
    _medNumFont->insert(pair<int, PointArrays*>('0', CreateAndFillLetter(151.0f-p, 573.0f-p, 16.0f+p*2, 21.0f+p*2, 1024.0f, 1024.0f)));
    _medNumFont->insert(pair<int, PointArrays*>('%', CreateAndFillLetter(169.0f-p, 573.0f-p, 17.0f+p*2, 22.0f+p*2, 1024.0f, 1024.0f)));
    _medNumFont->insert(pair<int, PointArrays*>(' ', CreateAndFillLetter(188.0f-p, 573.0f-p, 16.0f+p*2, 21.0f+p*2, 1024.0f, 1024.0f)));
    
    _bonusNumFont = new map<int, PointArrays*>();
    p = 0;// must be 0
    _bonusNumFont->insert(pair<int, PointArrays*>('1', CreateAndFillLetter(888.0f-p, 579.0f-p, 30.0f+p*2, 61.0f+p*2, 1024.0f, 1024.0f)));
    _bonusNumFont->insert(pair<int, PointArrays*>('2', CreateAndFillLetter(920.0f-p, 579.0f-p, 45.0f+p*2, 60.0f+p*2, 1024.0f, 1024.0f)));
    _bonusNumFont->insert(pair<int, PointArrays*>('3', CreateAndFillLetter(967.0f-p, 579.0f-p, 46.0f+p*2, 61.0f+p*2, 1024.0f, 1024.0f)));
    _bonusNumFont->insert(pair<int, PointArrays*>('4', CreateAndFillLetter(939.0f-p, 641.0f-p, 46.0f+p*2, 60.0f+p*2, 1024.0f, 1024.0f)));
    _bonusNumFont->insert(pair<int, PointArrays*>('5', CreateAndFillLetter(858.0f-p, 709.0f-p, 47.0f+p*2, 60.0f+p*2, 1024.0f, 1024.0f)));
    _bonusNumFont->insert(pair<int, PointArrays*>('6', CreateAndFillLetter(906.0f-p, 709.0f-p, 44.0f+p*2, 60.0f+p*2, 1024.0f, 1024.0f)));
    _bonusNumFont->insert(pair<int, PointArrays*>('7', CreateAndFillLetter(951.0f-p, 709.0f-p, 45.0f+p*2, 61.0f+p*2, 1024.0f, 1024.0f)));
    _bonusNumFont->insert(pair<int, PointArrays*>('8', CreateAndFillLetter(858.0f-p, 770.0f-p, 45.0f+p*2, 62.0f+p*2, 1024.0f, 1024.0f)));
    _bonusNumFont->insert(pair<int, PointArrays*>('9', CreateAndFillLetter(904.0f-p, 770.0f-p, 43.0f+p*2, 60.0f+p*2, 1024.0f, 1024.0f)));
    _bonusNumFont->insert(pair<int, PointArrays*>('0', CreateAndFillLetter(948.0f-p, 770.0f-p, 47.0f+p*2, 61.0f+p*2, 1024.0f, 1024.0f)));
    _bonusNumFont->insert(pair<int, PointArrays*>('+', CreateAndFillLetter(972.0f-p, 303.0f-2-p, 51.0f+p*2, 50.0f+4+p*2, 1024.0f, 1024.0f)));

    _scoreNumFont = new map<int, PointArrays*>();
    p = 0;
    _scoreNumFont->insert(pair<int, PointArrays*>('1', CreateAndFillLetter(642.0f-p, 907.0f-p, 19.0f+p*2, 41.0f+p*2, 1024.0f, 1024.0f)));
    _scoreNumFont->insert(pair<int, PointArrays*>('2', CreateAndFillLetter(663.0f-p, 907.0f-p, 29.0f+p*2, 40.0f+p*2, 1024.0f, 1024.0f)));
    _scoreNumFont->insert(pair<int, PointArrays*>('3', CreateAndFillLetter(694.0f-p, 907.0f-p, 31.0f+p*2, 40.0f+p*2, 1024.0f, 1024.0f)));
    _scoreNumFont->insert(pair<int, PointArrays*>('4', CreateAndFillLetter(727.0f-p, 907.0f-p, 30.0f+p*2, 40.0f+p*2, 1024.0f, 1024.0f)));
    _scoreNumFont->insert(pair<int, PointArrays*>('5', CreateAndFillLetter(759.0f-p, 907.0f-p, 31.0f+p*2, 40.0f+p*2, 1024.0f, 1024.0f)));
    _scoreNumFont->insert(pair<int, PointArrays*>('6', CreateAndFillLetter(795.0f-p, 868.0f-p, 30.0f+p*2, 40.0f+p*2, 1024.0f, 1024.0f)));
    _scoreNumFont->insert(pair<int, PointArrays*>('7', CreateAndFillLetter(792.0f-p, 910.0f-p, 30.0f+p*2, 40.0f+p*2, 1024.0f, 1024.0f)));
    _scoreNumFont->insert(pair<int, PointArrays*>('8', CreateAndFillLetter(642.0f-p, 950.0f-p, 29.0f+p*2, 41.0f+p*2, 1024.0f, 1024.0f)));
    _scoreNumFont->insert(pair<int, PointArrays*>('9', CreateAndFillLetter(673.0f-p, 950.0f-p, 29.0f+p*2, 41.0f+p*2, 1024.0f, 1024.0f)));
    _scoreNumFont->insert(pair<int, PointArrays*>('0', CreateAndFillLetter(704.0f-p, 950.0f-p, 32.0f+p*2, 41.0f+p*2, 1024.0f, 1024.0f)));
    
    _scoreBestNumFont = new map<int, PointArrays*>();
    p = 0;
    _scoreBestNumFont->insert(pair<int, PointArrays*>('1', CreateAndFillLetter(642.0f-p, 800.0f-p, 14.0f+p*2, 28.0f+p*2, 1024.0f, 1024.0f)));
    _scoreBestNumFont->insert(pair<int, PointArrays*>('2', CreateAndFillLetter(658.0f-p, 800.0f-p, 20.0f+p*2, 28.0f+p*2, 1024.0f, 1024.0f)));
    _scoreBestNumFont->insert(pair<int, PointArrays*>('3', CreateAndFillLetter(680.0f-p, 801.0f-p, 21.0f+p*2, 27.0f+p*2, 1024.0f, 1024.0f)));
    _scoreBestNumFont->insert(pair<int, PointArrays*>('4', CreateAndFillLetter(703.0f-p, 800.0f-p, 21.0f+p*2, 27.0f+p*2, 1024.0f, 1024.0f)));
    _scoreBestNumFont->insert(pair<int, PointArrays*>('5', CreateAndFillLetter(726.0f-p, 800.0f-p, 22.0f+p*2, 28.0f+p*2, 1024.0f, 1024.0f)));
    _scoreBestNumFont->insert(pair<int, PointArrays*>('6', CreateAndFillLetter(750.0f-p, 800.0f-p, 20.0f+p*2, 28.0f+p*2, 1024.0f, 1024.0f)));
    _scoreBestNumFont->insert(pair<int, PointArrays*>('7', CreateAndFillLetter(772.0f-p, 800.0f-p, 21.0f+p*2, 28.0f+p*2, 1024.0f, 1024.0f)));
    _scoreBestNumFont->insert(pair<int, PointArrays*>('8', CreateAndFillLetter(795.0f-p, 800.0f-p, 20.0f+p*2, 28.0f+p*2, 1024.0f, 1024.0f)));
    _scoreBestNumFont->insert(pair<int, PointArrays*>('9', CreateAndFillLetter(642.0f-p, 831.0f-p, 20.0f+p*2, 28.0f+p*2, 1024.0f, 1024.0f)));
    _scoreBestNumFont->insert(pair<int, PointArrays*>('0', CreateAndFillLetter(664.0f-p, 831.0f-p, 22.0f+p*2, 28.0f+p*2, 1024.0f, 1024.0f)));
    
    _scoreTimeNumFont = new map<int, PointArrays*>();
    p = 0;
    _scoreTimeNumFont->insert(pair<int, PointArrays*>('1', CreateAndFillLetter(688.0f-p, 830.0f-p, 18.0f+p*2, 36.0f+p*2, 1024.0f, 1024.0f)));
    _scoreTimeNumFont->insert(pair<int, PointArrays*>('2', CreateAndFillLetter(708.0f-p, 830.0f-p, 27.0f+p*2, 36.0f+p*2, 1024.0f, 1024.0f)));
    _scoreTimeNumFont->insert(pair<int, PointArrays*>('3', CreateAndFillLetter(737.0f-p, 830.0f-p, 27.0f+p*2, 36.0f+p*2, 1024.0f, 1024.0f)));
    _scoreTimeNumFont->insert(pair<int, PointArrays*>('4', CreateAndFillLetter(766.0f-p, 830.0f-p, 27.0f+p*2, 36.0f+p*2, 1024.0f, 1024.0f)));
    _scoreTimeNumFont->insert(pair<int, PointArrays*>('5', CreateAndFillLetter(795.0f-p, 830.0f-p, 27.0f+p*2, 36.0f+p*2, 1024.0f, 1024.0f)));
    _scoreTimeNumFont->insert(pair<int, PointArrays*>('6', CreateAndFillLetter(642.0f-p, 869.0f-p, 26.0f+p*2, 36.0f+p*2, 1024.0f, 1024.0f)));
    _scoreTimeNumFont->insert(pair<int, PointArrays*>('7', CreateAndFillLetter(670.0f-p, 869.0f-p, 27.0f+p*2, 36.0f+p*2, 1024.0f, 1024.0f)));
    _scoreTimeNumFont->insert(pair<int, PointArrays*>('8', CreateAndFillLetter(699.0f-p, 869.0f-p, 26.0f+p*2, 37.0f+p*2, 1024.0f, 1024.0f)));
    _scoreTimeNumFont->insert(pair<int, PointArrays*>('9', CreateAndFillLetter(727.0f-p, 869.0f-p, 26.0f+p*2, 36.0f+p*2, 1024.0f, 1024.0f)));
    _scoreTimeNumFont->insert(pair<int, PointArrays*>('0', CreateAndFillLetter(755.0f-p, 869.0f-p, 25.0f+p*2, 36.0f+p*2, 1024.0f, 1024.0f)));
    _scoreTimeNumFont->insert(pair<int, PointArrays*>(':', CreateAndFillLetter(817.0f-p, 797.0f-p, 15.0f+p*2, 31.0f+p*2, 1024.0f, 1024.0f)));

}