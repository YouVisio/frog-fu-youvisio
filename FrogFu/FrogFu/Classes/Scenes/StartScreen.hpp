//
//  StartScreen.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-10-07.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_StartScreen_hpp
#define FrogFu_StartScreen_hpp

#import <iostream>
#import "Interfaces.hpp"
#import "BaseSceneEngine.hpp"
#import "IRenderingEngine.hpp"
#import "Factory.hpp"
#import "Image.hpp"
#import <vector>
#import "StartScreenAction.hpp"
#import "CatchInsectMove.hpp"
#import "KungFu.hpp"

using namespace std;

#define INFO_BOX_ANIMATION_FRAMES 30

class StartScreen : public BaseSceneEngine {
public:
    StartScreen(IRenderingEngine* renderingEngine)
    : BaseSceneEngine(renderingEngine){}
    virtual void Construct(){
        _factory = Factory::Get();
        
        _state = new StartScreenState();
        
        _state->Background =
            (new Image(_factory->TextureAtlas01, 640, 912, 640, 1136))
            ->Position(0, 0)->Func(ImgCenter_BottomLeft_NoRotation);
        _state->Title =
            (new Image(_factory->TextureAtlas07, 1, 919, 511, 104))
            ->Func(ImgCenter_Centered_NoRotation)->Center(497/2, 95/2);
        
//_state->Background->textureGroup = 1;
//_state->Background->sfactor = GL_ONE;
//_state->Background->dfactor = GL_ONE_MINUS_SRC_ALPHA;
        
        _hasBeenInitialized = false;
        
        if(_factory->isFive){
            _state->Title->Position(320, 1136 - 240);
        }else if(_factory->isIpad){
            _state->Title->Position(320, 854 - 80);
        }else{
            _state->Title->Position(320, 960 - 140);
        }
        _state->TitlePos = _state->Title->Pos();
        
        _state->BtnPlay =
            (new Image(_factory->TextureAtlas07, 1, 527, 390, 390))
            ->Position(_factory->HALF_SCREEN_WIDTH, 300)->Func(ImgCenter_Centered_NoRotation)
            ->Center(390/2, 390/2);
        
        _state->BtnClassic =
            (new Image(_factory->TextureAtlas07, 195, 1, 193, 193))
            ->Position(544, 484)->Func(ImgCenter_Centered_WithRotation)
            ->Center(98, 98);
        _state->BtnSprint =
            (new Image(_factory->TextureAtlas07, 389, 1, 193, 193))
            ->Position(442, 212)->Func(ImgCenter_Centered_WithRotation)
            ->Center(98, 98);
        _state->BtnEasyPlay =
            (new Image(_factory->TextureAtlas07, 1, 1, 193, 193))
            ->Position(442, 212)->Func(ImgCenter_Centered_WithRotation)
            ->Center(98, 98);
        
        _state->Loading =
            (new Image(_factory->TextureAtlas07, 933, 933, 92, 92))
            ->Position(576, 64)->Func(ImgCenter_Centered_WithRotation)
            ->Center(50, 50);
        
        
        
        
        _state->BtnInfo =
            (new Image(_factory->TextureAtlas07, 1, 1, 1, 1)) // textures will be set later
            ->Position(0, 0)->Func(ImgCenter_Centered_NoRotation)
            ->Center(48, 48);
        _state->BtnMusic =
            (new Image(_factory->TextureAtlas07, 1, 1, 1, 1))
            ->Position(0, 0)->Func(ImgCenter_Centered_NoRotation)
            ->Center(48, 48);
        _state->BtnEffects =
            (new Image(_factory->TextureAtlas07, 1, 1, 1, 1))
            ->Position(0, 0)->Func(ImgCenter_Centered_NoRotation)
            ->Center(48, 48);
        _state->InfoBox =
            (new Image(_factory->TextureAtlas07, 520, 199, 484, 431))
            ->Position(_factory->HALF_SCREEN_WIDTH, _factory->HALF_SCREEN_HEIGHT)
            ->Func(ImgCenter_Centered_NoRotation)
            ->Center(484/2.0, 431/2.0)->IsVisible(false);
        _state->BtnAchievements =
            (new Image(_factory->TextureAtlas07, 1, 1, 1, 1))
            ->Position(0, 0)->Func(ImgCenter_Centered_NoRotation)
            ->Center(48, 48);
        _state->BtnLeaderboards =
            (new Image(_factory->TextureAtlas07, 1, 1, 1, 1))
            ->Position(0, 0)->Func(ImgCenter_Centered_NoRotation)
            ->Center(48, 48);
        
//        _state->InfoBox->textureGroup = 1;
//        _state->InfoBox->sfactor = GL_SRC_ALPHA;
//        _state->InfoBox->dfactor = GL_ONE_MINUS_SRC_ALPHA;
        
        _isPlayBtnTapped = false;
        
        _touchTrailManager = new TouchTrailManager(_touchTrails);
        _mapManager = new MapManager(_drawables, _mapDots);
        _frogManager = new FrogManager(_touchTrailManager, _mapManager, _drawables, _animations);
        _mapManager->SetBody(_frogManager->Body());
        _insectManager = new InsectManager(this, _frogManager, _mapManager, _touchTrailManager, _mapDots, _drawables, _animations);
        _buttonBar = new ButtonBar(_drawables, _animations);
        _statusBar = new StatusBar(_drawables);
        _endOfGame = new EndOfGame(_drawables, _animations);
        _pauseManager = new PauseManager(_drawables, _animations);
        
        
        _infoBoxRelBtn1 = vec2(-112, -160);
        _infoBoxRelBtn2 = vec2(0, -160);
        _infoBoxRelBtn3 = vec2(112, -160);

        _lastKungFuPose = 0;
        
//        _factory->redDot->Pos(_state->InfoBox->Pos() + vec2(-112, -160));
//        _factory->greenDot->Pos(_state->InfoBox->Pos() + vec2(0, -160));
//        _factory->blueDot->Pos(_state->InfoBox->Pos() + vec2(112, -160));
        
    }
    void TakeOffScreen(){
        if(_state->InsectClassic != NULL) _state->InsectClassic->IsExpired(true);
        if(_state->InsectSprint != NULL) _state->InsectSprint->IsExpired(true);
        if(_state->InsectEasyPlay != NULL) _state->InsectEasyPlay->IsExpired(true);
        
        _renderingEngine->ClearExpired();
        
        _state->FlyClassic = NULL;
        _state->FlySprint = NULL;
        _state->FlyEasyPlay = NULL;
        
        ClearAllAnimations();
        
        
    }
    void SetBtnAchievements(int n){
        if(n != 0) _btnAchievementsState = n;
        _btnAchievementsPressed = n == 0;
        if(n == 0) _state->BtnAchievements->RedefineTextureCoords(385, 293, 95, 95);
        else if(n > 0) _state->BtnAchievements->RedefineTextureCoords(289, 293, 95, 95);
    }
    void SetBtnLeaderboards(int n){
        if(n != 0) _btnLeaderboardsState = n;
        _btnLeaderboardsPressed = n == 0;
        if(n == 0) _state->BtnLeaderboards->RedefineTextureCoords(97, 389, 95, 95);
        else if(n > 0) _state->BtnLeaderboards->RedefineTextureCoords(1, 389, 95, 95);
    }
    void SetBtnPlay(int n){
        if(n != 0) _btnPlayState = n;
        _btnPlayPressed = n == 0;
        if(n == 0) _state->BtnPlay->RedefineTextureCoords(514, 632, 390, 390);
        else if(n > 0) _state->BtnPlay->RedefineTextureCoords(1, 527, 390, 390);
    }
    void SetBtnInfo(int n){
        if(n != 0) _btnInfoState = n;
        _btnInfoPressed = n == 0;
        if(n == 0) _state->BtnInfo->RedefineTextureCoords(385, 196, 95, 95);
        else if(n > 0) _state->BtnInfo->RedefineTextureCoords(289, 196, 95, 95);
    }
    void SetBtnMusic(int n){
        if(n != 0) _btnMusicState = n;
        _btnMusicPressed = n == 0;
        if(n == 0) _state->BtnMusic->RedefineTextureCoords(97, 196, 95, 95);
        else if(n > 0) _state->BtnMusic->RedefineTextureCoords(1, 196, 95, 95);
        else _state->BtnMusic->RedefineTextureCoords(193, 196, 95, 95); // if(n < 0)
    }
    void SetBtnEffects(int n){
        if(n != 0) _btnEffectsState = n;
        _btnEffectsPressed = n == 0;
        if(n == 0) _state->BtnEffects->RedefineTextureCoords(97, 293, 95, 95);
        else if(n > 0) _state->BtnEffects->RedefineTextureCoords(1, 293, 95, 95);
        else _state->BtnEffects->RedefineTextureCoords(193, 293, 95, 95); // if(n < 0)
    }
    void InitFromStartScene(ISceneEngine* startScene){ }
    void Initialize(){
        
        _factory->sceneID = SCENE_START;
        
        _insectManager->SetEngine(this);
        
        if(!sysIsPlayingSound("M1")){
            sysPlaySound("M1");
        }
        
        sysSoundVolume("M1", 1.0);
        
        _isInfoBoxVisible = false;
        _infoBoxAnimation = 0;
        
        
        _factory->selectedInsects = 0;
        
        StartScreenState* s = _state;
        
        _hasBeenInitialized = true;

        s->IsClassicHit = false;
        s->IsEasyPlayHit = false;
        s->IsSprintHit = false;
        
        if(TransitionIndex() == 1){
            TransitionIndex(0);
            _renderingEngine->UnloadTextureById(2);
        } else if(TransitionIndex() == 2){
            TransitionIndex(0);
            _renderingEngine->UnloadTextureById(8);
        }
        _factory->setEnableInput(true);
        _factory->frogPlainShiftY = 0;
        _factory->bgPlainShiftY = 0;
        
        s->Drawables = _drawables;
        
        s->InsectClassic =
            (new Image(_factory->TextureAtlas06, 0, 584, 73, 63))
            ->Func(ImgCenter_Centered_NoRotation)->Center(34, 35);//2
        s->InsectSprint =
            (new Image(_factory->TextureAtlas06, 0, 502, 73, 80))
            ->Func(ImgCenter_Centered_NoRotation)->Center(34, 40);//1
        s->InsectEasyPlay =
            (new Image(_factory->TextureAtlas06, 0, 648, 73, 65))
            ->Func(ImgCenter_Centered_NoRotation)->Center(34, 35);//3
        
        _btnInfoPressed = _btnMusicPressed = _btnEffectsPressed = _btnAchievementsPressed = _btnLeaderboardsPressed = false;
        _btnClassicState = _btnSprintState = _btnEasyPlayState = _btnLeaderboardsState = _btnAchievementsState = 1;
        
        SetBtnInfo(1);
        SetBtnMusic(_factory->Settings->Music() ? 1 : -1);
        SetBtnEffects(_factory->Settings->Sounds() ? 1 : -1);
        SetBtnAchievements(1);
        SetBtnLeaderboards(1);
        
        //_state->Title->Position(320, 1500);
        s->BtnInfo->Position(-100, 60);
        s->BtnMusic->Position(-100, 60);
        s->BtnEffects->Position(-100, 60);
        s->BtnAchievements->Position(-100, 60);
        s->BtnLeaderboards->Position(-100, 60);
        
        _factory->isWithinJump = false;
        
        _drawables->clear();
        int i = 0;
        _drawables->push_back(s->Background->Depth(++i));
        _drawables->push_back(s->Title->Depth(++i));
        _drawables->push_back(s->Loading->Depth(++i));

        s->Loading->IsVisible(false);
        
        
        _drawables->push_back(s->BtnClassic->Depth(++i));
        _drawables->push_back(s->BtnSprint->Depth(++i));
        _drawables->push_back(s->BtnEasyPlay->Depth(++i));
        _drawables->push_back(s->BtnEffects->Depth(++i));
        _drawables->push_back(s->BtnMusic->Depth(++i));
        _drawables->push_back(s->BtnInfo->Depth(++i));
        _drawables->push_back(s->BtnAchievements->Depth(++i));
        _drawables->push_back(s->BtnLeaderboards->Depth(++i));
        
        
        
        _frogManager->Initialize();
        
        _frogManager->Body()->Depth(500);
        _frogManager->Head()->Depth(501);
        _frogManager->Tongue()->Depth(502);

        _drawables->push_back(s->InsectClassic->Depth(++i));
        _drawables->push_back(s->InsectSprint->Depth(++i));
        _drawables->push_back(s->InsectEasyPlay->Depth(++i));
        
        _drawables->push_back(s->InfoBox->Depth(9999));
        

        BaseSceneEngine::Initialize();
        
        _endOfGame->Initialize();
        _pauseManager->Initialize();
        
        s->Tongue = _frogManager->Tongue();
        
        _drawables->push_back(s->BtnPlay->Depth(99999));
        
        _factory->bodyPos = vec2(_factory->staticBodyPos.x, _factory->staticBodyPos.y + 150 );//95
        
        s->FrogPos = _factory->bodyPos + vec2(0,110);
        
        s->BtnPlay->Pos(s->FrogPos);
        
        vec2 initialPos = s->FrogPos + vec2(0,100);
        
        s->BtnClassic->Pos(initialPos);
        s->BtnSprint->Pos(initialPos);
        s->BtnEasyPlay->Pos(initialPos);
        
        double thirdOfCircle = (2.0*Pi)/3.0;
        s->BtnSprint->Pos(vec2(initialPos).Rotate(s->FrogPos, thirdOfCircle));
        s->BtnEasyPlay->Pos(vec2(initialPos).Rotate(s->FrogPos,-thirdOfCircle));
        
        s->InsectClassic->Pos(s->BtnClassic->Pos());
        s->InsectSprint->Pos(s->BtnSprint->Pos());
        s->InsectEasyPlay->Pos(s->BtnEasyPlay->Pos());

        vec2 p = s->InsectClassic->Pos();
        _animations->push_back(s->FlyClassic = new InsectMove(this, _frogManager, p, NULL, s->InsectClassic));
        p = s->InsectSprint->Pos();
        _animations->push_back(s->FlySprint = new InsectMove(this, _frogManager, p, NULL, s->InsectSprint));
        p = s->InsectEasyPlay->Pos();
        _animations->push_back(s->FlyEasyPlay = new InsectMove(this, _frogManager, p, NULL, s->InsectEasyPlay));

        
        if(_isPlayBtnTapped){
            _state->BtnPlay->Y(-1000);
            _animations->push_back(new StartScreenAction(false, _state));
        }
        
        analyticsScreen("startInit");
        
#if ISDEBUG
        _drawables->push_back(_factory->redDot->Depth(999999+1));
        _drawables->push_back(_factory->greenDot->Depth(999999+2));
        _drawables->push_back(_factory->blueDot->Depth(999999+3));
#endif
 
/*
_testText =
    (new TextBlock(_factory->TextureAtlas06, NULL, 0, 0, _factory->getBonusNumFontDescription()))
    ->IsCentered(true)
    ->SetText("+456")
    ->Position(150, 150)
    ->Scale(2.0);
_drawables->push_back(_testText);
_factory->redDot->Position(150, 150);
*/
    }
    void applicationWillEnterForeground(){
        
    }
    void applicationDidEnterBackground(){
//        _hasBeenInitialized = false;
//        ClearAllAnimations();
    }
    void applicationDidBecomeActive(){
//        if(!_hasBeenInitialized){
//            Initialize();
//        }
    }
    virtual void Update(double step){
        BaseSceneEngine::Update(step);
        
        ProcessInfoBoxAnimation();
        
        _frogManager->Update(_time);
        
        _endOfGame->Update(_time);
        _pauseManager->Update(_time);
        
        
        
    }
    
    void ProcessInfoBoxAnimation(){
        if(_infoBoxAnimation > 0){
            --_infoBoxAnimation;

            double x = 1.0 - abs(_infoBoxAnimation) / (double)INFO_BOX_ANIMATION_FRAMES;
            
            double s = pow(pow(5, -(pow((x - 0.77), 2) / pow((2 * 0.334197), 2))) * 1.2, 3.0);

            _state->InfoBox->Scale(s);

            if(!_isInfoBoxVisible){
                analyticsScreen("infoBoxInit");
                _isInfoBoxVisible = true;
                _state->InfoBox->IsVisible(true);
            }
        }
        else if(_infoBoxAnimation < 0){
            ++_infoBoxAnimation;

            double x = 1.0 - abs(_infoBoxAnimation) /  (double)INFO_BOX_ANIMATION_FRAMES;

            x = 1.0 - x;
            
            double s = pow(pow(5, -(pow((x - 0.77), 2) / pow((2 * 0.334197), 2))) * 1.2, 3.0);

            _state->InfoBox->Scale(s);

            if(x < 0.12 && _state->InfoBox->Drawable::IsVisible()){
                _isInfoBoxVisible = false;
                _state->InfoBox->IsVisible(false);
            }
        }
    }
    
    void TouchBegan(float x, float y){
//cout << "SC x:" << x << " y:" << y << endl;
        if(_isInfoBoxVisible){
            return;
        }
        
        StartScreenState* s = _state;
        if(!_factory->getEnableInput()) return;
        vec2 p = vec2(x,y);
        if(!_isPlayBtnTapped){
            if(p.Distance(s->BtnPlay->Pos()) <= 200) SetBtnPlay(0);
            return;
        }
        if(p.Distance(s->BtnInfo->Pos()) <= smallBtnRadius) SetBtnInfo(0);
        else if(p.Distance(s->BtnMusic->Pos()) <= smallBtnRadius) SetBtnMusic(0);
        else if(p.Distance(s->BtnEffects->Pos()) <= smallBtnRadius) SetBtnEffects(0);
        else if(p.Distance(s->BtnAchievements->Pos()) <= smallBtnRadius) SetBtnAchievements(0);
        else if(p.Distance(s->BtnLeaderboards->Pos()) <= smallBtnRadius) SetBtnLeaderboards(0);
    }
    void TouchMoved(float x, float y, float prevX, float prevY){ }
    void TouchEnded(float x, float y){
        
        if(_isInfoBoxVisible){
            
            vec2 touch = vec2(x, y);
            
            vec2 p = _state->InfoBox->Pos();
            vec2 btn1 = p + _infoBoxRelBtn1;
            vec2 btn2 = p + _infoBoxRelBtn2;
            vec2 btn3 = p + _infoBoxRelBtn3;
            if(touch.Distance(btn1) < 55){
                OnInfoBoxBtn1();
            }
            else if(touch.Distance(btn2) < 55){
                OnInfoBoxBtn2();
            }
            else if(touch.Distance(btn3) < 55){
                OnInfoBoxBtn3();
            }
            OnInfoTapped();
            _isInfoBoxVisible = false;
            return;
        }
        
        
        if(!_factory->getEnableInput()) return;
        
        vec2 p = vec2(x,y);
        if(!_isPlayBtnTapped){
            _isPlayBtnTapped = true;
            if(_btnPlayPressed){
                SetBtnPlay(1);
                OnPlayTapped();
            }
            return;
        }
        
        
        bool isBtnTapped = false;
        if(p.Distance(_state->BtnInfo->Pos()) <= smallBtnRadius*1.4){
            SetBtnInfo(1);
            OnInfoTapped();
            isBtnTapped = true;
        }
        else if(p.Distance(_state->BtnMusic->Pos()) <= smallBtnRadius*1.4){
            OnMusicTapped();
            isBtnTapped = true;
        }
        else if(p.Distance(_state->BtnEffects->Pos()) <= smallBtnRadius*1.4){
            OnEffectsTapped();
            isBtnTapped = true;
        }
        else if(p.Distance(_state->BtnAchievements->Pos()) <= smallBtnRadius*1.4){
            SetBtnAchievements(1);
            OnAchievementsTapped();
            isBtnTapped = true;
        }
        else if(p.Distance(_state->BtnLeaderboards->Pos()) <= smallBtnRadius*1.4){
            SetBtnLeaderboards(1);
            OnLeaderboardsTapped();
            isBtnTapped = true;
        }
        else if(p.Distance(_state->BtnClassic->Pos()) <= bigBtnRadius*1.2){
            OnClassicTapped();
            isBtnTapped = true;
        }
        else if(p.Distance(_state->BtnSprint->Pos()) <= bigBtnRadius*1.2){
            OnSprintTapped();
            isBtnTapped = true;
        }
        else if(p.Distance(_state->BtnEasyPlay->Pos()) <= bigBtnRadius*1.2){
            OnEasyPlayTapped();
            isBtnTapped = true;
        }
        
        
        if(_btnInfoPressed) SetBtnInfo(1);
        if(_btnMusicPressed) SetBtnMusic(_btnMusicState);
        if(_btnEffectsPressed) SetBtnEffects(_btnEffectsState);
        if(_btnAchievementsPressed) SetBtnAchievements(1);
        if(_btnLeaderboardsPressed) SetBtnLeaderboards(1);
        
        if(!isBtnTapped){
            if(p.Distance(_state->FrogPos) <= 110){
                if(_state->IsAnyGameHit()){
                    return;
                }
                
                OnTapTheFrog();
            }
        }
    }
    void OnInfoBoxBtn1(){
        sysGoToUrl("https://twitter.com/YouVisio");
    }
    void OnInfoBoxBtn2(){
        sysSendEmail();
    }
    void OnInfoBoxBtn3(){
        sysGoToUrl("https://www.facebook.com/YouVisio");
    }
    void GoToClassic(){
        SetFrogHeadBackToStatic(this);
        ClearAllAnimations();
        _renderingEngine->ClearExpired();
        _factory->transitionStartToClassic();
    }
    void GoToSprint(){
        SetFrogHeadBackToStatic(this);
        ClearAllAnimations();
        _renderingEngine->ClearExpired();
        _factory->transitionStartToSprint();
    }
    void GoToEasyPlay(){
        SetFrogHeadBackToStatic(this);
        ClearAllAnimations();
        _renderingEngine->ClearExpired();
        _factory->transitionStartToEasyPlay();
    }
    void OnStartCatchingMove(InsectMove* move, void (*onEnd)(void*), void (*onInsectHit)(void*,CatchInsectMove*)){
        
        
        _factory->isWithinJump = true;
        
        _frogManager->ClearFrogAnimation();
        
        Image* image = (Image*)move->State();
        image->TextureX(move->InitialTextureX() + image->TextureWidth()*4);
        image->Color(0.8,1,1,0.8);
        move->SetIsTouched(true);
        
        CatchInsectMove* cim = _frogManager->SelectedInsect(move, false, false);
        cim->OnEndState = this;
        cim->OnEnd = onEnd;
        cim->OnInsectHitState = this;
        cim->OnInsectHit = onInsectHit;
    }
    static void SetFrogHeadBackToStatic(StartScreen* ss){
        ss->_frogManager->StartStaticPose();
    }
    void OnEasyPlayTapped(){
        if(_btnEasyPlayState < 0){
            // show message
        } else if(_btnEasyPlayState > 0) {
            OnStartCatchingMove(_state->FlyEasyPlay, OnEasyPlayFlyCaught, OnEasyPlayFlyHit);
        }
    }
    static void OnEasyPlayFlyHit(void* p, CatchInsectMove* cim){
        StartScreen* ss = (StartScreen*)p;
        ss->_state->IsEasyPlayHit = true;
    }
    static void OnEasyPlayFlyCaught(void* p){
        StartScreen* ss = (StartScreen*)p;
        ss->_btnEasyPlayState = 0;
        ss->InsectImagesToNull();
        ss->GoToEasyPlay();
    }
    void OnEasyPlayFlyCaught(){
        _btnEasyPlayState = 0;
        InsectImagesToNull();
    }
    void OnClassicTapped(){
        if(_btnClassicState < 0){
            // show message
        } else if(_btnClassicState > 0) {
            OnStartCatchingMove(_state->FlyClassic, OnClassicFlyCaught, OnClassicFlyHit);
            //backgroundWorker(F1,F2,this);
        }
    }
    static void F1(void* caller){
        StartScreen* ss = (StartScreen*)caller;
        
        /*for(int i = 0; i < 99999999; ++i){
            int x = rand();
            if(pow(x,3.0) > 17) ++x;
        }*/
        
cout << "F1:" << ss->smallBtnRadius << "!" << pthread_self() << endl;
    }
    static void F2(void* caller){
        StartScreen* ss = (StartScreen*)caller;
        
cout << "F2:" << ss->smallBtnRadius << "!" << pthread_self() << endl;
    }
    static void OnClassicFlyHit(void* p, CatchInsectMove* cim){
        StartScreen* ss = (StartScreen*)p;
        ss->_state->IsClassicHit = true;
    }
    static void OnClassicFlyCaught(void* p){
        StartScreen* ss = (StartScreen*)p;
        ss->_btnClassicState = 0;
        ss->InsectImagesToNull();
        ss->GoToClassic();
    }
    void OnSprintTapped(){
        if(_btnSprintState < 0){
            // show message
        } else if(_btnSprintState > 0) {
            OnStartCatchingMove(_state->FlySprint, OnSprintFlyCaught, OnSprintFlyHit);
        }
    }
    static void OnSprintFlyHit(void* p, CatchInsectMove* cim){
        StartScreen* ss = (StartScreen*)p;
        ss->_state->IsSprintHit = true;
    }
    static void OnSprintFlyCaught(void* p){
        StartScreen* ss = (StartScreen*)p;
        ss->_btnSprintState = 0;
        ss->InsectImagesToNull();
        ss->GoToSprint();
    }
    void OnPlayTapped(){
        
        sysPlaySound("Gong");
        sysSoundVolume("Gong", 0.5);
        
        _animations->push_back(new StartScreenAction(true, _state));
    }
    void OnInfoTapped(){
        _infoBoxAnimation = INFO_BOX_ANIMATION_FRAMES;
        if(_isInfoBoxVisible){
            _infoBoxAnimation *= -1;
        }
        
        analyticsScreen("info");
    }
    void OnMusicTapped(){
        _factory->Settings->Music(!_factory->Settings->Music());
        SetBtnMusic(_factory->Settings->Music() ? 1 : -1);
        
        if(_factory->Settings->Music()){
            sysSoundVolume("M1", 1.0);
        }else{
            sysSoundVolume("M1", 0.0);
        }
    }
    void OnEffectsTapped(){
        _factory->Settings->Sounds(!_factory->Settings->Sounds());
        SetBtnEffects(_factory->Settings->Sounds() ? 1 : -1);
    }
    void OnAchievementsTapped(){
        showAchievements();
        
        analyticsScreen("achievements");
    }
    void OnLeaderboardsTapped(){
        showLeaderboards();
        
        analyticsScreen("leaderboards");
    }
    const double smallBtnRadius = 45;
    const double bigBtnRadius = 97;
private:
    void OnTapTheFrog(){
        
        if(_frogManager->IsPoseOfType(ANI_KungFu)){
            return;
        }
        if(_state->IsAnyGameHit()){
            return;
        }
        
        int type = ((int)(sysRandom()*100)) % 2 == 0 ? KUNGFU_1 : KUNGFU_2;
        if(type == _lastKungFuPose){
            type = ((int)(sysRandom()*100)) % 2 == 0 ? KUNGFU_1 : KUNGFU_2;
        }
        
        _lastKungFuPose = type;
        
        _frogManager->StartPose(
            new KungFu(type, _frogManager->Body(), _frogManager->Head(),
                       new FuncAndState<int>(OnStaticKongFuEnd, this)));
        
        analyticsEvent("evt_kungFuPose", "pose", type);
    }
    static int OnStaticKongFuEnd(void* caller){
        ((StartScreen*)caller)->OnKongFuEnd();
        return 0;
    }
    void OnKongFuEnd(){
        _frogManager->StartStaticPose();
    }
    void InsectImagesToNull(){
        _state->InsectClassic = NULL;
        _state->InsectSprint = NULL;
        _state->InsectEasyPlay = NULL;
    }
    vec2 _infoBoxRelBtn1;
    vec2 _infoBoxRelBtn2;
    vec2 _infoBoxRelBtn3;
    
    Factory* _factory;
    StartScreenState* _state;
    
    int _lastKungFuPose;
    int _btnInfoState;
    int _btnMusicState;
    int _btnEffectsState;
    int _btnPlayState;
    int _btnAchievementsState;
    int _btnLeaderboardsState;
    bool _btnInfoPressed;
    bool _btnMusicPressed;
    bool _btnEffectsPressed;
    bool _btnLeaderboardsPressed;
    bool _btnAchievementsPressed;
    bool _btnPlayPressed;
    int _btnClassicState;
    int _btnSprintState;
    int _btnEasyPlayState;
    bool _isPlayBtnTapped;
    bool _hasBeenInitialized;
    int _infoBoxAnimation;
    bool _isInfoBoxVisible;

    
    TextBlock* _testText;
    


};
StartScreen* CreateStartScreen(IRenderingEngine* renderer){
    return new StartScreen(renderer);
}
#endif
