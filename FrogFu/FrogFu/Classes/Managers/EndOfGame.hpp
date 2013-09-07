//
//  EndOfGame.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-12-02.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_EndOfGame_hpp
#define FrogFu_EndOfGame_hpp

#import <iostream>
#import "BaseManager.hpp"
#import "Interfaces.hpp"
#import "Factory.hpp"
#import "Image.hpp"
#import "TextBlock.hpp"
#import "IAnimation.hpp"

using namespace std;

#define BTN_RADIUS 50


class EndOfGame : BaseManager {
public:
    EndOfGame(simlist<Drawable>* drawables, simlist<IAnimation>* animations){
        _factory = Factory::Instance;
        
        _drawables = drawables;
        _animations = animations;
        
        _onBack = NULL;
        _onReplay = NULL;
        
        _ticker = NULL;
        
        _background =
            (new Image(_factory->TextureAtlas03, 746, 453, 60, 106))
            ->Position(-5, -5)->Func(ImgCenter_BottomLeft_NoRotation)
            ->Scale(1150.0 / 106.0);
        _screenPad =
            (new Image(_factory->TextureAtlas01, 642, 367, 460, 543))
            ->Position(_factory->HALF_SCREEN_HEIGHT, _factory->HALF_SCREEN_WIDTH)
            ->Func(ImgCenter_Centered_NoRotation)
            ->Center(460/2.0, 543/2.0);
        _screenPadSm =
            (new Image(_factory->TextureAtlas01, 1060, 2, 460, 413))
            ->Position(_factory->HALF_SCREEN_HEIGHT, _factory->HALF_SCREEN_WIDTH)
            ->Func(ImgCenter_Centered_NoRotation)
            ->Center(460/2.0, 543/2.0);
        _timeUp =
            (new Image(_factory->TextureAtlas03, 810, 628, 213, 44))->Position(0, 0)
            ->Func(ImgCenter_Centered_NoRotation)
            ->Center(213/2.0, 44/2.0);
        _gameOver =
            (new Image(_factory->TextureAtlas03, 720, 702, 303, 44))->Position(0, 0)
            ->Func(ImgCenter_Centered_NoRotation)
            ->Center(303/2.0, 44/2.0);
        _missedTooMany =
            (new Image(_factory->TextureAtlas03, 802, 678, 210, 19))->Position(0, 0)
            ->Func(ImgCenter_Centered_NoRotation)
            ->Center(210/2.0, 19/2.0);
        _stungByWasp =
            (new Image(_factory->TextureAtlas03, 833, 747, 188, 18))->Position(0, 0)
            ->Func(ImgCenter_Centered_NoRotation)
            ->Center(188/2.0, 18/2.0);
        _best1 =
            (new Image(_factory->TextureAtlas03, 774, 749, 56, 15))->Position(0, 0)
            ->Func(ImgCenter_BottomLeft_NoRotation)
            ->Center(0, 0);
        _best2 =
            (new Image(_factory->TextureAtlas03, 774, 749, 56, 15))->Position(0, 0)
            ->Func(ImgCenter_BottomLeft_NoRotation)
            ->Center(0, 0);
        _score =
            (new Image(_factory->TextureAtlas03, 904, 794, 119, 26))->Position(0, 0)
            ->Func(ImgCenter_Centered_NoRotation)
            ->Center(119/2.0, 26/2.0);
        _fliesCaught =
            (new Image(_factory->TextureAtlas03, 774, 766, 250, 27))->Position(0, 0)
            ->Func(ImgCenter_BottomLeft_NoRotation)
            ->Center(0, 0);
        _stampNewHighScore =
            (new Image(_factory->TextureAtlas05, 518, 119, 230, 206))->Position(0, 0)
            ->Func(ImgCenter_Centered_NoRotation)
            ->Center(230/2.0, 206/2.0);
        _stampNewBestCatch =
            (new Image(_factory->TextureAtlas05, 749, 119, 230, 206))->Position(0, 0)
            ->Func(ImgCenter_Centered_NoRotation)
            ->Center(230/2.0, 206/2.0);
        _btnBack =
            (new Image(_factory->TextureAtlas03, 335, 703, 95, 95))->Position(0, 0)
            ->Func(ImgCenter_Centered_NoRotation)
            ->Center(95/2.0, 95/2.0);
        _btnReplay =
            (new Image(_factory->TextureAtlas03, 833, 822, 95, 95))->Position(0, 0)
            ->Func(ImgCenter_Centered_NoRotation)
            ->Center(95/2.0, 95/2.0);
        
//        _shareTitle =
//            (new Image(_factory->TextureAtlas06, 681, 680, 257, 28))->Position(0, 0)
//            ->Func(ImgCenter_Centered_NoRotation)
//            ->Center(257/2.0, 28/2.0);
        _btnFacebook =
            (new Image(_factory->TextureAtlas06, 389, 836, 90, 90))->Position(0, 0)
            ->Func(ImgCenter_Centered_NoRotation)
            ->Center(90/2.0, 90/2.0);
        _btnTwitter =
            (new Image(_factory->TextureAtlas06, 389, 927, 90, 90))->Position(0, 0)
            ->Func(ImgCenter_Centered_NoRotation)
            ->Center(90/2.0, 90/2.0);
        _btnGooglePlus =
            (new Image(_factory->TextureAtlas06, 858, 255, 90, 90))->Position(0, 0)
            ->Func(ImgCenter_Centered_NoRotation)
            ->Center(90/2.0, 90/2.0);
        
        _isVisible = false;
        _numScore = new TextBlock(_factory->TextureAtlas04, NULL, 0, 0, _factory->getBigNumFontDescription());
        _numBestScore = new TextBlock(_factory->TextureAtlas04, NULL, 0, 0, _factory->getMedNumFontDescription());
        _numFliesCaught = new TextBlock(_factory->TextureAtlas04, NULL, 0, 0, _factory->getBigNumFontDescription());
        _numBestFliesCaught = new TextBlock(_factory->TextureAtlas04, NULL, 0, 0, _factory->getMedNumFontDescription());
    }
    ~EndOfGame(){
        ClearEvents();
    }
    void Initialize(){}
    void Update(double time){
        if(_ticker != NULL){ _ticker = BaseManager::HandleTick(_ticker, time); }
        
    }
    void Show(int type, int score, int bestScore, int perc, int bestPerc, ActionAndState* onBack, ActionAndState* onReplay){

        bool allAreLessThan100 = (_factory->numberHit + _factory->numberMissed) < 100;
        
        _factory->canPostToSocial = true;
        
        sysSoundVolume("M1", 1.0);
        sysSoundVolume("M2", 0.1);
        
        _factory->selectedInsects = 0;
        
        if(_isVisible){
            if(onBack != NULL) delete onBack;
            if(onReplay != NULL) delete onReplay;
            return;
        }
        
        _factory->enableInsectCreation = false;
        
        _currScore = score;
        _currBestScore = bestScore;
        _currPerc = perc;
        _currBestPerc = bestPerc;
        
        _isVisible = true;
        _factory->isPaused = true;
        _factory->setEnableInput(false);
        
        _background->Alpha(0.65);
        _background->Color(1.0, 1.0, 1.0);
        
        _background->textureGroup = 2;
        _background->sfactor = GL_SRC_ALPHA;
        _background->dfactor = GL_ONE_MINUS_SRC_ALPHA;
        _screenPad->textureGroup = 3;
        _screenPad->sfactor = GL_SRC_ALPHA;
        _screenPad->dfactor = GL_ONE_MINUS_SRC_ALPHA;
        _screenPadSm->textureGroup = 3;
        _screenPadSm->sfactor = GL_SRC_ALPHA;
        _screenPadSm->dfactor = GL_ONE_MINUS_SRC_ALPHA;
        
        
        _drawables->push_back(_background->IsExpired(false));
        if(allAreLessThan100){
            _drawables->push_back(_screenPadSm->IsExpired(false));
        }else{
            _drawables->push_back(_screenPad->IsExpired(false));
        }
        _screenPadSm->Position(_factory->HALF_SCREEN_WIDTH, _factory->HALF_SCREEN_HEIGHT - (_factory->isFive ? 100 : 0))->SetInit();
        _screenPad->Position(_factory->HALF_SCREEN_WIDTH, _factory->HALF_SCREEN_HEIGHT - (_factory->isFive ? 100 : 0))->SetInit();
        
        if(type == END_OF_GAME_WASP){
            _drawables->push_back(_gameOver->IsExpired(false));
            _drawables->push_back(_stungByWasp->IsExpired(false));
            
            _gameOver->Position(_screenPad->X(), _screenPad->Y()+210)->SetInit();
            _stungByWasp->Position(_screenPad->X(), _screenPad->Y()+165)->SetInit();
            
        } else if(type == END_OF_GAME_MISSED) {
            _drawables->push_back(_gameOver->IsExpired(false));
            _drawables->push_back(_missedTooMany->IsExpired(false));
            
            _gameOver->Position(_screenPad->X(), _screenPad->Y()+210)->SetInit();
            _missedTooMany->Position(_screenPad->X(), _screenPad->Y()+165)->SetInit();

        } else if(type == END_OF_GAME_TIME) {
            _drawables->push_back(_timeUp->IsExpired(false));
            
            _timeUp->Position(_screenPad->X(), _screenPad->Y()+195)->SetInit();
            
        } else cout << "UNKNOWN END OF GAME TYPE " << type << " !!!" << endl;
        
        _drawables->push_back(_btnBack->IsExpired(false));
        _drawables->push_back(_btnReplay->IsExpired(false));
        
        if(allAreLessThan100){
            _btnBack->Position(_screenPad->X()-72, _screenPad->Y()-87)->SetInit();
            _btnReplay->Position(_screenPad->X()+72, _screenPad->Y()-87)->SetInit();
        } else {
            _btnBack->Position(_screenPad->X()-72, _screenPad->Y()-217)->SetInit();
            _btnReplay->Position(_screenPad->X()+72, _screenPad->Y()-217)->SetInit();
        }
        ClearEvents();
        _onBack = onBack;
        _onReplay = onReplay;
        
        _drawables->push_back(_best1->IsVisible(false)->IsExpired(false));
        _drawables->push_back(_best2->IsVisible(false)->IsExpired(false));
        _drawables->push_back(_score->IsVisible(false)->IsExpired(false));
        _drawables->push_back(_fliesCaught->IsVisible(false)->IsExpired(false));

        
        _drawables->push_back(_numScore->IsVisible(false)->IsExpired(false));
        _drawables->push_back(_numBestScore->IsVisible(false)->IsExpired(false));
        _drawables->push_back(_numFliesCaught->IsVisible(false)->IsExpired(false));
        _drawables->push_back(_numBestFliesCaught->IsVisible(false)->IsExpired(false));
        

        _btnFacebook->Position(_factory->HALF_SCREEN_WIDTH - 70, _screenPad->Y() + 330)->SetInit();
        _btnTwitter->Position(_factory->HALF_SCREEN_WIDTH + 70, _screenPad->Y() + 330)->SetInit();
        //_btnGooglePlus->Position(_factory->HALF_SCREEN_WIDTH + 130, _screenPad->Y() + 330)->SetInit();
        

        _drawables->push_back(_btnFacebook->IsExpired(false));
        _drawables->push_back(_btnTwitter->IsExpired(false));
        //_drawables->push_back(_btnGooglePlus->IsExpired(false));
        
        if(score > bestScore) _drawables->push_back(_stampNewHighScore->IsVisible(false)->IsExpired(false));
        if(perc > bestPerc) _drawables->push_back(_stampNewBestCatch->IsVisible(false)->IsExpired(false));
        
        _score->Position(_screenPad->X()-125, _screenPad->Y()+60)->SetInit();
        _numScore->Position(_screenPad->X()-55, _screenPad->Y()+45)->SetInit();
        _stampNewHighScore->Position(_screenPad->X()+145, _screenPad->Y()+50)->SetInit();
        _best1->Position(_screenPad->X()-180, _screenPad->Y()+5+5)->SetInit();
        _numBestScore->Position(_screenPad->X()-110, _screenPad->Y()+3+5)->SetInit();
        
        _fliesCaught->Position(_screenPad->X()-180, _screenPad->Y()-65+3)->SetInit();
        _numFliesCaught->Position(_screenPad->X()+75, _screenPad->Y()-70+3)->SetInit();
        _stampNewBestCatch->Position(_screenPad->X()+145, _screenPad->Y()-130)->SetInit();
        _best2->Position(_screenPad->X()-180, _screenPad->Y()-105+3+3)->SetInit();
        _numBestFliesCaught->Position(_screenPad->X()-110, _screenPad->Y()-108+3+3)->SetInit();
        

        char ch[10];
        sprintf( ch,"%d", (int)sysMax(score, bestScore));
        _numBestScore->SetText(ch);
        
        sprintf( ch,"%d%%", (int)sysMax(perc, bestPerc));
        
        _numBestFliesCaught->SetText(ch);
        
        _ticker = new Ticker<EndOfGame>(0.5, this, StaticShowScoreLabel);
        
        Ticker<EndOfGame>* t = _ticker;
        
        if(score >= 5) t = t->FollowBy(0, StaticPlayNumbersUpSound);
        
        if(score < 5) t = t->FollowBy(0, StaticShowSmallScore);
        else if(score < 10) t = t->FollowBy(0.4, StaticShowBigScore);
        else  t = t->FollowBy(0.8, StaticShowBigScore);
        
        if(score >= 5) t = t->FollowBy(0, StaticStopNumbersUpSound);
        
        if(_currScore > _currBestScore){
            t = t->FollowBy(0.5, StaticShowStampBestScore);
            t = t->FollowBy(0, StaticPlayStampSound);
            t = t->FollowBy(0.2, StaticShakeAll);
        }
        
        t = t->FollowBy(0, StaticSetInitBack);
        
        if(allAreLessThan100){
            _score->Position(_screenPad->X()-125, _screenPad->Y()+60-10)->SetInit();
            _numScore->Position(_screenPad->X()-55, _screenPad->Y()+45-10)->SetInit();
            _stampNewHighScore->Position(_screenPad->X()+145-50, _screenPad->Y()+50-10)->SetInit();
            _best1->Position(_screenPad->X()-180, _screenPad->Y()+5+5-10)->SetInit();
            _numBestScore->Position(_screenPad->X()-110, _screenPad->Y()+3+5-10)->SetInit();
        }else{
            t = t->FollowBy(0.5, StaticShowPercLabel);
            
            if(perc >= 5) t = t->FollowBy(0, StaticPlayNumbersUpSound);
            
            if(perc < 5) t = t->FollowBy(0, StaticShowSmallPerc);
            else if(perc < 10) t = t->FollowBy(0.4, StaticShowBigPerc);
            else  t = t->FollowBy(0.8, StaticShowBigPerc);
            
            if(perc >= 5) t = t->FollowBy(0, StaticStopNumbersUpSound);
            
            if(_currPerc > _currBestPerc){
                t = t->FollowBy(0.5, StaticShowStampBestPerc);
                t = t->FollowBy(0, StaticPlayStampSound);
                t = t->FollowBy(0.2, StaticShakeAll);
            }
            
            t = t->FollowBy(0, StaticSetInitBack);
        }
        
        t = t->FollowBy(0, StaticEndPresentation);
        
        if(score > 99999) _stampNewHighScore->X(_screenPad->X()+145 + 70)->SetInit();
    }
    
    static void StaticEndPresentation(Ticker<EndOfGame>* ticker){ ticker->State()->EndPresentation(ticker->Progress()); }
    void EndPresentation(double progress){
        // if user chose not to rate it
        if(!_factory->Settings->WarnToRate()) return;
        
        if(_factory->skipWarnToRate > 0){
            --_factory->skipWarnToRate;
            return;
        }
        
        if(
           (_factory->sceneID == SCENE_CLASSIC && _factory->UserState->ClassicS() > 20 && _factory->UserState->ClassicNum() > 1)
           ||
           (_factory->sceneID == SCENE_EASY_PLAY && _factory->UserState->EasyPlayS() > 20 && _factory->UserState->EasyPlayNum() > 1)
           || 
           (_factory->sceneID == SCENE_SPRINT && _factory->UserState->SprintS() > 15 && _factory->UserState->SprintNum() > 1)){
            
            showRatingAlert();
        }
    }
    
    static void StaticPlayStampSound(Ticker<EndOfGame>* ticker){ ticker->State()->PlayStampSound(ticker->Progress()); }
    void PlayStampSound(double progress){
        sysPlaySound("Stamp");
        sysSoundVolume("Stamp", 0.4);
    }
    static void StaticPlayNumbersUpSound(Ticker<EndOfGame>* ticker){ ticker->State()->PlayNumbersUpSound(ticker->Progress()); }
    void PlayNumbersUpSound(double progress){
        sysPlaySound("NumbersUp");
        sysSoundVolume("NumbersUp", 0.6);
    }
    static void StaticStopNumbersUpSound(Ticker<EndOfGame>* ticker){ ticker->State()->StopNumbersUpSound(ticker->Progress()); }
    void StopNumbersUpSound(double progress){
        sysStopSound("NumbersUp");
    }
    static void StaticShowStampBestPerc(Ticker<EndOfGame>* ticker){ ticker->State()->ShowStampBestPerc(ticker->Progress()); }
    void ShowStampBestPerc(double progress){
        _stampNewBestCatch->IsVisible(true);
        
        //double s = pow(2.71828,-(pow(progress, 0.2)/pow(2*0.2,2.0)))*4.0;
        double s = -pow(progress, 3.0) + 1.0;
        
        _stampNewBestCatch->Scale(0.615 + s*1.4);
    }
    static void StaticShowPercLabel(Ticker<EndOfGame>* ticker){ ticker->State()->ShowPercLabel(ticker->Progress()); }
    void ShowPercLabel(double progress){
        _fliesCaught->IsVisible(true);
        
        double x = progress;
        x = pow(2.71828,-(pow(x-0.825, 2)/pow((2*0.2),2)))*1.2;

        _fliesCaught->Scale(x);
    }
    static void StaticShowBigPerc(Ticker<EndOfGame>* ticker){ ticker->State()->ShowBigPerc(ticker->Progress()); }
    void ShowBigPerc(double progress){
        _numFliesCaught->IsVisible(true);
        char ch[10];
        sprintf( ch,"%d%%", (int)round(_currPerc * progress));
        _numFliesCaught->SetText(ch);
    }
    static void StaticShowSmallPerc(Ticker<EndOfGame>* ticker){ ticker->State()->ShowSmallPerc(ticker->Progress()); }
    void ShowSmallPerc(double progress){
        _numFliesCaught->IsVisible(true);
        char ch[10];
        sprintf( ch,"%d%%", _currPerc);
        _numFliesCaught->SetText(ch);
    }
    static void StaticShakeAll(Ticker<EndOfGame>* ticker){ ticker->State()->ShakeAll(ticker->Progress()); }
    void ShakeAll(double progress){
        double n = ((int)(progress*4.0)) % 2 == 0 ? 1 : -1;
        Shake(_screenPad, n);
        Shake(_screenPadSm, n);
        Shake(_timeUp, n);
        Shake(_gameOver, n);
        Shake(_missedTooMany, n);
        Shake(_stungByWasp, n);
        Shake(_best1, n);
        Shake(_best2, n);
        Shake(_score, n);
        Shake(_fliesCaught, n);
        Shake(_stampNewHighScore, n);
        Shake(_stampNewBestCatch, n);
        Shake(_btnBack, n);
        Shake(_btnReplay, n);
        Shake(_numScore, n);
        Shake(_numBestScore, n);
        Shake(_numFliesCaught, n);
        Shake(_numBestFliesCaught, n);
        Shake(_btnFacebook, n);
        Shake(_btnTwitter, n);
    }
    static void StaticSetInitBack(Ticker<EndOfGame>* ticker){ ticker->State()->SetInitBack(ticker->Progress()); }
    void SetInitBack(double progress){
        double n = 0;
        Shake(_screenPad, n);
        Shake(_timeUp, n);
        Shake(_gameOver, n);
        Shake(_missedTooMany, n);
        Shake(_stungByWasp, n);
        Shake(_best1, n);
        Shake(_best2, n);
        Shake(_score, n);
        Shake(_fliesCaught, n);
        Shake(_stampNewHighScore, n);
        Shake(_stampNewBestCatch, n);
        Shake(_btnBack, n);
        Shake(_btnReplay, n);
        Shake(_numScore, n);
        Shake(_numBestScore, n);
        Shake(_numFliesCaught, n);
        Shake(_numBestFliesCaught, n);
        Shake(_btnFacebook, n);
        Shake(_btnTwitter, n);
        
        _best1->IsVisible(true);
        _numBestScore->IsVisible(true);
        
        if(_numFliesCaught->Drawable::IsVisible()){
            _best2->IsVisible(true);
            _numBestFliesCaught->IsVisible(true);
        }
        
    }
    void Shake(Image* obj, double p){
        obj->Position(obj->initX + p*3, obj->initY + p*3);
    }
    void Shake(TextBlock* obj, double p){
        obj->Position(obj->initX + p*3, obj->initY + p*3);
    }
    static void StaticShowStampBestScore(Ticker<EndOfGame>* ticker){ ticker->State()->ShowStampBestScore(ticker->Progress()); }
    void ShowStampBestScore(double progress){
        _stampNewHighScore->IsVisible(true);
        
        //double s = pow(2.71828,-(pow(progress, 0.2)/pow(2*0.2,2.0)))*4.0;
        double s = -pow(progress, 3.0) + 1.0;
        
        _stampNewHighScore->Scale(0.615 + s*1.4);
    }
    static void StaticShowBigScore(Ticker<EndOfGame>* ticker){ ticker->State()->ShowBigScore(ticker->Progress()); }
    void ShowBigScore(double progress){
        _numScore->IsVisible(true);
        char ch[10];
        sprintf( ch,"%d", (int)ceil(_currScore * progress));
        _numScore->SetText(ch);
    }
    static void StaticShowSmallScore(Ticker<EndOfGame>* ticker){ ticker->State()->ShowSmallScore(ticker->Progress()); }
    void ShowSmallScore(double progress){
        _numScore->IsVisible(true);
        char ch[10];
        sprintf( ch,"%d", _currScore);
        _numScore->SetText(ch);
    }
    static void StaticShowScoreLabel(Ticker<EndOfGame>* ticker){ ticker->State()->ShowScoreLabel(ticker->Progress()); }
    void ShowScoreLabel(double progress){
        _score->IsVisible(true);
        
        double x = progress;
        x = pow(2.71828,-(pow(x-0.825, 2)/pow((2*0.2),2)))*1.2;
        
        _background->Alpha(progress * 0.65);
        _score->Scale(x);
    }
    void Hide(){
        
        if(sysIsPlayingSound("NumbersUp")){
            sysStopSound("NumbersUp");
        }
        
        //sysSoundVolume("M1", 0.0);
        
        _isVisible = false;
        _factory->forceClearExpired = true;
        _factory->enableInsectCreation = true;
        
        ClearTicker();
        
        _factory->isPaused = false;
        _factory->setEnableInput(true);
        
        _background->IsExpired(true)->PreserveExpired(true);
        _screenPad->IsExpired(true)->PreserveExpired(true);
        _screenPadSm->IsExpired(true)->PreserveExpired(true);
        _timeUp->IsExpired(true)->PreserveExpired(true);
        _gameOver->IsExpired(true)->PreserveExpired(true);
        _missedTooMany->IsExpired(true)->PreserveExpired(true);
        _stungByWasp->IsExpired(true)->PreserveExpired(true);
        _best1->IsExpired(true)->PreserveExpired(true);
        _best2->IsExpired(true)->PreserveExpired(true);
        _score->IsExpired(true)->PreserveExpired(true);
        _fliesCaught->IsExpired(true)->PreserveExpired(true);
        _stampNewHighScore->IsExpired(true)->PreserveExpired(true);
        _stampNewBestCatch->IsExpired(true)->PreserveExpired(true);
        _btnBack->IsExpired(true)->PreserveExpired(true);
        _btnReplay->IsExpired(true)->PreserveExpired(true);
        _numScore->IsExpired(true)->PreserveExpired(true);
        _numBestScore->IsExpired(true)->PreserveExpired(true);
        _numFliesCaught->IsExpired(true)->PreserveExpired(true);
        _numBestFliesCaught->IsExpired(true)->PreserveExpired(true);
        
        ClearEvents();
    }
    bool IsVisible() { return _isVisible; }
    void TakeOffScreen(){
        Hide();
    }
    bool TouchBegan(vec2 touch){
        if(touch.Distance(_btnBack->Pos()) < BTN_RADIUS){
            BackButton(true);
            return true;
        }
        if(touch.Distance(_btnReplay->Pos()) < BTN_RADIUS){
            ReplayButton(true);
            return true;
        }
        if(touch.Distance(_btnFacebook->Pos()) < BTN_RADIUS){
            OnFacebookTap();
            return true;
        }
        if(touch.Distance(_btnTwitter->Pos()) < BTN_RADIUS){
            OnTwitterTap();
            return true;
        }
        if(touch.Distance(_btnGooglePlus->Pos()) < BTN_RADIUS){
            OnGooglePlusTap();
            return true;
        }
        
        // pause button
        if(touch.y < 120){
            return true;
        }
        
        return false;
    }
    bool TouchMoved(vec2 touch){
        if(_btnBackIsOn && touch.Distance(_btnBack->Pos()) > BTN_RADIUS){
            BackButton(false);
            return true;
        }
        if(_btnReplayIsOn && touch.Distance(_btnReplay->Pos()) > BTN_RADIUS){
            ReplayButton(false);
            return true;
        }
        // pause button
        if(touch.y < 120){
            return true;
        }
        return false;
    }
    bool TouchEnded(vec2 touch){
        if(_btnBackIsOn && touch.Distance(_btnBack->Pos()) < BTN_RADIUS*1.5){
            OnGoBack();
            
            BackButton(false);
            return true;
        }
        if(_btnReplayIsOn && touch.Distance(_btnReplay->Pos()) < BTN_RADIUS*1.5){
            OnReplay();
            
            ReplayButton(false);
            return true;
        }
        // pause button
        if(touch.y < 120){
            return true;
        }
        return false;
    }
    

private:
    void OnFacebookTap(){
        if(!_factory->canPostToSocial) return;
        cout << "Post to Facebook" << endl;
        postToFacebook(_currScore, _factory->sceneID);
    }
    void OnTwitterTap(){
        if(!_factory->canPostToSocial) return;
        cout << "Post to Twitter" << endl;
        postToTwitter(_currScore, _factory->sceneID);
    }
    void OnGooglePlusTap(){
        if(!_factory->canPostToSocial) return;
        cout << "Post to Google+" << endl;
        postToGooglePlus(_currScore, _factory->sceneID);
    }
    void ClearTicker(){
        if(_ticker != NULL){
            delete _ticker;
            _ticker = NULL;
        } 
    }
    void ClearEvents(){
        if(_onBack != NULL) {
            delete _onBack;
            _onBack = NULL;
        }
        if(_onReplay != NULL) {
            delete _onReplay;
            _onReplay = NULL;
        }
    }
    void OnGoBack(){
        if(_onBack != NULL){
            _onBack->Call(0L);
        }
    }
    void OnReplay(){
        if(_onReplay != NULL){
            _onReplay->Call(0L);
        }
    }
    void BackButton(bool isOn){
        _btnBackIsOn = isOn;
        if(isOn) _btnBack->RedefineTextureCoords(431, 703, 95, 95);
        else _btnBack->RedefineTextureCoords(335, 703, 95, 95);
    }
    void ReplayButton(bool isOn){
        _btnReplayIsOn = isOn;
        if(isOn) _btnReplay->RedefineTextureCoords(929, 822, 95, 95);
        else _btnReplay->RedefineTextureCoords(833, 822, 95, 95);
    }
    
    Factory* _factory;
    simlist<Drawable>* _drawables;
    simlist<IAnimation>* _animations;
    
    Image* _background;
    Image* _screenPad;
    Image* _screenPadSm;
    Image* _timeUp;
    Image* _gameOver;
    Image* _missedTooMany;
    Image* _stungByWasp;
    Image* _best1;
    Image* _best2;
    Image* _score;
    Image* _fliesCaught;
    Image* _stampNewHighScore;
    Image* _stampNewBestCatch;
    Image* _btnBack;
    Image* _btnReplay;
    Image* _btnFacebook;
    Image* _btnTwitter;
    Image* _btnGooglePlus;
    
    bool _btnBackIsOn;
    bool _btnReplayIsOn;
    bool _isVisible;
    int _currScore;
    int _currBestScore;
    int _currPerc;
    int _currBestPerc;
    
    Ticker<EndOfGame>* _ticker;
    
    ActionAndState* _onBack;
    ActionAndState* _onReplay;
    
    TextBlock* _numScore;
    TextBlock* _numBestScore;
    TextBlock* _numFliesCaught;
    TextBlock* _numBestFliesCaught;
    
};

#endif
