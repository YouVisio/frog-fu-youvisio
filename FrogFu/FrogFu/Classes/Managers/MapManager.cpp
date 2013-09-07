//
//  MapManager.cpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-05.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#import "Drawable.hpp"
#import "MapManager.hpp"
#import <cmath>

MapManager::MapManager(simlist<Drawable>* drawables, simlist<Particle>* mapDots) :
point1(0,0),
point2(0,0)
{
    
    _ticker = NULL;
    
    _drawables = drawables;
    
    _mapDots = mapDots;
    
    Factory* f = Factory::Instance;
    
    _factory = f;
    
    _shiftFrog = f->isFive ? 20 : 10;
    
    double h = f->SCREEN_HEIGHT;
    
    if(f->isFive){
        iPhone5CorrectY = 200;
        
        pad =
             (new Image(f->TextureAtlas03, 899, 1, 100, 325))
             ->Position(0, h - (960 - 610)-24)->Func(ImgCenter_BottomLeft_NoRotation);
        arrow =
             (new Image(f->TextureAtlas03, 1000, 71, 22, 226))
             ->Position(100, h - (960 - 625)-24)->Func(ImgCenter_BottomLeft_NoRotation)->IsVisible(false);
        phoneFrame =
             (new Image(f->TextureAtlas03, 817, 11, 81, 143))
             ->Position(7, h - (960 - 621)-24)->Func(ImgCenter_BottomLeft_NoRotation);
        frog =
            (new Image(f->TextureAtlas03, 753, 100, 37, 47))
            ->Position(50, h - (960 - 670)-24)->Func(ImgCenter_Centered_WithRotation);
        hand =
            (new Image(f->TextureAtlas03, 799, 159, 99, 139))
            ->Position(50, h - (960 - 670)-24)->Func(ImgCenter_BottomLeft_NoRotation)->IsVisible(false);
    } else {
        if(f->isIpad){
            iPhone5CorrectY = -100;
        }else{
            iPhone5CorrectY = 0;
        }
        
        pad =
             (new Image(f->TextureAtlas03, 899, 327, 100, 300))
             ->Position(0, h - (960 - 610))->Func(ImgCenter_BottomLeft_NoRotation);
        arrow =
             (new Image(f->TextureAtlas03, 1000, 301, 23, 192))
             ->Position(100, h - (960 - 625))->Func(ImgCenter_BottomLeft_NoRotation)->IsVisible(false);
        phoneFrame =
             (new Image(f->TextureAtlas03, 816, 447, 81, 121))
             ->Position(7, h - (960 - 621))->Func(ImgCenter_BottomLeft_NoRotation);
        frog =
            (new Image(f->TextureAtlas03, 753, 100, 37, 47))
            ->Position(50, h - (960 - 670))->Func(ImgCenter_Centered_WithRotation);
        hand =
            (new Image(f->TextureAtlas03, 799, 159, 99, 139))
            ->Position(50, h - (960 - 670))->Func(ImgCenter_BottomLeft_NoRotation)->IsVisible(false);
    }
    
    
    SetFrogSit();
    
    _minFrameY = phoneFrame->Y();
    _minFrogY = frog->Y();
    
    _arrowMaxHeight = arrow->TextureHeight();
    _arrowMinHeight = arrow->TextureHeight() * 0.10;
}
void MapManager::Initialize(){
    _drawables->push_back(pad);
    _drawables->push_back(arrow);
    _drawables->push_back(phoneFrame);
    _drawables->push_back(frog);
    _drawables->push_back(hand);
    
}
void MapManager::SetBody(Image* body){
    _body = body;
}
void MapManager::ClearDots(){
    _mapDots->clear(true);
}
void MapManager::TakeOffScreen(){
    ClearDots();
    ClearTicker();
    arrow->IsVisible(false);
    hand->IsVisible(false);
}
void MapManager::Update(double time){
    
//_factory->blueDot->IsVisible(true);
//_factory->blueDot->Position(_factory->HALF_SCREEN_WIDTH-272, _factory->HALF_SCREEN_HEIGHT+200+156);
    
    if(_factory->isWithinJump){
        frog->Rotation(_body->Rotation());
    }
    
    if(_ticker != NULL){ _ticker = BaseManager::HandleTick(_ticker, time); }
    if(_mapDots->size() ==  0) return;
    simnode<Particle>* node = _mapDots->head;
    while (node != NULL) {
        Particle* p = node->data;
        if(p->Age() >= 1.0){
            node = _mapDots->removeAndGetNext(node, true);
        }else{
            node = node->next;
        }
    }
}
void PreviewOneAnimationFunc0(Ticker<MapManager>* ticker){
}
void PreviewOneAnimationFunc1(Ticker<MapManager>* ticker){
    MapManager* s = ticker->State();
    if(ticker->Progress() == 0){
        // init
        s->hand->IsVisible(true);
        s->SetHandOver();
        s->SetFrogSit();
        s->point1.Set(150, 600);
        s->point2.Set(30, 560);
    }
    vec2 curr = s->point1.Step(s->point2, ticker->Progress());
    s->hand->Position(curr.x, curr.y+s->iPhone5CorrectY);
}
void PreviewOneAnimationFunc2(Ticker<MapManager>* ticker){
    MapManager* s = ticker->State();
    if(ticker->Progress() == 0){
        s->SetHandPressed();
        s->SetFrogDuck();
        s->arrow->IsVisible(true);
        s->point1.Set(s->point2);
        s->point2.Set(30, 510);
    }
    s->ChangeMapArrowHeight(ticker->Progress());
    vec2 curr = s->point1.Step(s->point2, ticker->Progress());
    s->hand->Position(curr.x, curr.y+s->iPhone5CorrectY);
}
void PreviewOneAnimationFunc3(Ticker<MapManager>* ticker){
    MapManager* s = ticker->State();
    if(ticker->Progress() == 0){
        s->SetHandOver();
        s->SetFrogJump();
        s->point1.Set(s->point2);
        s->point2.Set(150, 510);
        s->InitializeJump(1.0);
        s->handPositionCounter = 0;
    }
    
    const double eccentricity = 3.0;
    double position = -pow(2, eccentricity) * pow(abs(ticker->Progress() - 0.5), eccentricity) + 1;
    
    s->JumpUpdate(position, ticker->Progress());
    
    if(s->handPositionCounter > 0 && ticker->Progress() < 0.8){
        vec2 curr = s->point1.Step(s->point2, (ticker->Progress() - s->minSubProgress)/(s->maxSubProgress - s->minSubProgress));
        s->hand->Position(curr.x, curr.y);
    }
    if(ticker->Progress() > 0.2 && s->handPositionCounter < 1){
        s->SetHandPressed();
        s->handPositionCounter = 1;
        s->minSubProgress = 0.2;
        s->maxSubProgress = 0.35;
        s->point1 = vec2(5, 620);
        s->point2 = vec2(60, 690+s->iPhone5CorrectY);
    }
    else if(ticker->Progress() > 0.35 && s->handPositionCounter < 2){
        s->handPositionCounter = 2;
        s->minSubProgress = 0.35;
        s->maxSubProgress = 0.5;
        s->point1 = s->point2;
        s->point2 = vec2(5, 690+s->iPhone5CorrectY);
    }
    else if(ticker->Progress() > 0.5 && s->handPositionCounter < 3){
        s->handPositionCounter = 3;
        s->minSubProgress = 0.5;
        s->maxSubProgress = 0.65;
        s->point1 = s->point2;
        s->point2 = vec2(60, 620+s->iPhone5CorrectY);
    }
    else if(ticker->Progress() > 0.65 && s->handPositionCounter < 4){
        s->handPositionCounter = 4;
        s->minSubProgress = 0.65;
        s->maxSubProgress = 0.8;
        s->point1 = s->point2;
        s->point2 = vec2(5, 650+s->iPhone5CorrectY);
    }
    else if(ticker->Progress() > 0.8 && s->handPositionCounter < 5){
        s->handPositionCounter = 5;
        s->hand->IsVisible(false);
    }
    
    if(ticker->Progress() == 1){
        s->SetFrogSit();
    }
}

void MapManager::StartPreviewOneAnimation(){
    (_ticker = new Ticker<MapManager>(0.8, this, PreviewOneAnimationFunc0))
    ->FollowBy(1.0, PreviewOneAnimationFunc1)
    ->FollowBy(1.2, PreviewOneAnimationFunc2)
    ->FollowBy(2.5, PreviewOneAnimationFunc3);
}
void MapManager::JumpAnimationEnds(){
    arrow->IsVisible(false);
    
    SetFrogSit();
    
    phoneFrame->Y(_minFrameY);
    frog->Y(_minFrogY);
}
void MapManager::ClearTicker(){
    if(_ticker != NULL){
        delete _ticker;
        _ticker = NULL;
        phoneFrame->Y(_minFrameY);
        frog->Y(_minFrogY);
        hand->IsVisible(false);
    }
}
void MapManager::TouchBegins(){
    arrow->IsVisible(true);
    
    ClearTicker();
    
    SetFrogDuck();
}
void MapManager::ChangeMapArrowHeight(double progress){
    if(!arrow->Drawable::IsVisible()){
        arrow->IsVisible(true);
    }
    float diffHeight = (_arrowMaxHeight - _arrowMinHeight)*progress;
    arrow->SetTextureHeightAlignTop(diffHeight + _arrowMinHeight);
}
void MapManager::InitializeJump(double strength){
    SetFrogJump();
    // this number comes from YShiftOfTheBranchesLevel * MapZoom
    _frameShiftY = (Y_SHIFT_BG_FRONT * MAP_ZOOM) * strength;
    
}
void MapManager::JumpUpdate(double position, double progress){
    if(progress > 0.55 && arrow->Drawable::IsVisible()){
        arrow->IsVisible(false);
    }
    
    phoneFrame->Y(_minFrameY + _frameShiftY * position);
    frog->Y(_minFrogY + _frameShiftY * position + _shiftFrog);
}
void MapManager::SetHandOver(){
    hand->RedefineTextureCoords(799, 159, 99, 139);
}
void MapManager::SetHandPressed(){
    hand->RedefineTextureCoords(815, 300, 83, 146);
}
void MapManager::SetFrogJump(){
    frog->RedefineTextureCoords(750, 8, 43, 41);
    frog->Center(frog->TextureWidth()/2.0, frog->TextureHeight()/2.0);
}
void MapManager::SetFrogDuck(){
    frog->Rotation(0);
    frog->RedefineTextureCoords(744, 57, 55, 33);
    frog->Center(frog->TextureWidth()/2.0, frog->TextureHeight() - 35);
}
void MapManager::SetFrogSit(){
    frog->Rotation(0);
    frog->RedefineTextureCoords(753, 100, 37, 47);
    frog->Center(frog->TextureWidth()/2.0, frog->TextureHeight() - 35);
}
