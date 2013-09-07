//
//  ButtonBar.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-11-15.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_ButtonBar_hpp
#define FrogFu_ButtonBar_hpp

#import <iostream>
#import "BaseManager.hpp"
#import "Interfaces.hpp"
#import "Factory.hpp"
#import "Image.hpp"
#import "IAnimation.hpp"

using namespace std;

#define BTN_RADIUS 50

class ButtonBar : BaseManager {
public:
    ButtonBar(simlist<Drawable>* drawables, simlist<IAnimation>* anims){
        _factory = Factory::Instance;
        _drawables = drawables;
        _animations = anims;
        
        _onBackBtn = NULL;
        _onPauseBtn = NULL;
        
//        _btnBack = (new Image(_factory->TextureAtlas03, 335, 703, 95, 95))
//                    ->Position(70,  70)->Func(ImgCenter_Centered_NoRotation)
//                    ->Center(48, 48);
        
        _btnPause = (new Image(_factory->TextureAtlas03, 527, 703, 95, 95))
                    ->Position(640-55,  55)->Func(ImgCenter_Centered_NoRotation)
                    ->Center(48, 48);
        
        
        
    }
    void Initialize(ActionAndState* onBackBtn, ActionAndState* onPauseBtn){
        
//        _btnBackIsOn = false;
        _btnPauseIsOn = false;
        if(_onBackBtn != NULL) delete _onBackBtn;
        if(_onPauseBtn != NULL) delete _onPauseBtn;
        
        _onPauseBtn = onPauseBtn;
        _onBackBtn = onBackBtn;
        
//        _drawables->push_back(_btnBack);
        _drawables->push_back(_btnPause);
        
//        BackButton(false);
    }
    
    void TakeOffScreen(){
        
    }
    bool TouchBegan(vec2 touch){
//        if(touch.Distance(_btnBack->Pos()) < BTN_RADIUS*1.5){
//            BackButton(true);
//            return true;
//        }
        if(touch.Distance(_btnPause->Pos()) < BTN_RADIUS*1.5){
            PauseButton(true);
            return true;
        }
        return false;
    }
    bool TouchMoved(vec2 touch){
//        if(_btnBackIsOn && touch.Distance(_btnBack->Pos()) > BTN_RADIUS*1.5){
//            BackButton(false);
//            return true;
//        }
        if(_btnPauseIsOn && touch.Distance(_btnPause->Pos()) > BTN_RADIUS*1.5){
            PauseButton(false);
            return true;
        }
        return false;
    }
    bool TouchEnded(vec2 touch){
//        if(_btnBackIsOn && touch.Distance(_btnBack->Pos()) < BTN_RADIUS*1.5){
//            _onBackBtn->Call(0);
//            
//            BackButton(false);
//            return true;
//        }
        if(_btnPauseIsOn && touch.Distance(_btnPause->Pos()) < BTN_RADIUS*1.5){
            _onPauseBtn->Call(0);
            
            PauseButton(false);
            return true;
        }
        return false;
    }
    
private:
//    void BackButton(bool isOn){
//        _btnBackIsOn = isOn;
//        if(isOn) _btnBack->RedefineTextureCoords(431, 703, 95, 95);
//        else _btnBack->RedefineTextureCoords(335, 703, 95, 95);
//    }
    void PauseButton(bool isOn){
        _btnPauseIsOn = isOn;
        if(isOn) _btnPause->RedefineTextureCoords(623, 703, 95, 95);
        else _btnPause->RedefineTextureCoords(527, 703, 95, 95);
    }
    
    
    simlist<Drawable>* _drawables;
    simlist<IAnimation>* _animations;
    
    Factory* _factory;
    
//    Image* _btnBack;
    Image* _btnPause;
//    bool _btnBackIsOn;
    bool _btnPauseIsOn;
    
    ActionAndState* _onBackBtn;
    ActionAndState* _onPauseBtn;
};

#endif
