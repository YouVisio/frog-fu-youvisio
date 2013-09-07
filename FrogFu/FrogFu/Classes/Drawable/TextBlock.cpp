//
//  TextBlock.cpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-05.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#import "Interfaces.hpp"
#import "TextBlock.hpp"

using namespace std;

TextBlock::TextBlock(TextureDescription * texture, const char* text, float x, float y, map<int,PointArrays*>* lettersInfo)
: Drawable(texture){
    _lettersInfo = lettersInfo;
    _text = NULL;
    _x = x;
    _y = y;
    _scale = 1.0f;
    
    type = DRAWABLE_TEXT_BLOCK;
    
    _isCentered = false;
    _isRightAligned = false;
    
    SetText(text);
}
TextBlock::~TextBlock(){
    if(_text != NULL){
        delete [] _text;
        _text = NULL;
    }
}
int TextBlock::NumberSquares(){
    return _isVisible ? _textLength : 0;
}
bool TextBlock::IsCentered(){ return _isCentered; }
TextBlock* TextBlock::IsCentered(bool val){ _isCentered = val; PositionText(); return this; }
bool TextBlock::IsRightAligned(){ return _isRightAligned; }
TextBlock* TextBlock::IsRightAligned(bool val){ _isRightAligned = val; PositionText(); return this; }
float TextBlock::Scale(){ return _scale; }
TextBlock* TextBlock::Scale(float scale){ _scale = scale; PositionText(); return this; }
TextBlock* TextBlock::SetInit(){ initX = _x; initY = _y; return this; }
TextBlock* TextBlock::IsVisible(bool isVisible){ _isVisible = isVisible; return this; }
TextBlock* TextBlock::SetText(const char* text){
    if(text == NULL) return this;
    
    if(_text != NULL){
        delete [] _text;
        _text = NULL;
    }
    
    _textLength = strlen(text);
    
    _text = new char[ _textLength + 1 ];
    
    strcpy(_text, text);
    
    _textLength = _textLength;
    
    GenerateLetterList();
    return this;
}
float TextBlock::X(){ return _x; }
TextBlock* TextBlock::X(float x){ _x = x; PositionText(); return this; }
float TextBlock::Y(){ return _y; }
TextBlock* TextBlock::Y(float y){ _y = y; PositionText(); return this; }
TextBlock* TextBlock::Position(float x, float y){
    _x = x;
    _y = y;
    PositionText();
    return this;
}
TextBlock* TextBlock::Color(float alpha,float red,float green,float blue) {
    _alpha = alpha;
    _red = red;
    _green = green;
    _blue = blue;
    _isColorChanged = _alpha < 1.0f || _red < 1.0f || _green < 1.0f || _blue < 1.0f;
    return this;
}
TextBlock* TextBlock::Alpha(float alpha) {
    _alpha = alpha;
    _isColorChanged = _alpha < 1.0f || _red < 1.0f || _green < 1.0f || _blue < 1.0f;
    return this;
}
void TextBlock::GenerateLetterList(){
    if(vertices != NULL){
        delete [] vertices;
        vertices = NULL;
    }
    if(texCoord != NULL){
        delete [] texCoord;
        texCoord = NULL;
    }
    
    int numPoints = _textLength * 8;
    vertices = new GLfloat[numPoints];
    texCoord = new GLfloat[numPoints];
    
    /*
     // -------- test STRTS
     float x = 0;//-1.8;
     float y = 0;//2.6;
     float w = 20.0f / 160.0f;
     float h = 25.0f / 160.0f;
     
     int i = 0;
     vertices[i++] = x;
     vertices[i++] = y;
     vertices[i++] = x + w;
     vertices[i++] = y;
     vertices[i++] = x;
     vertices[i++] = y + h;
     vertices[i++] = x + w;
     vertices[i++] = y + h;
     
     x += w;
     
     vertices[i++] = x;
     vertices[i++] = y;
     vertices[i++] = x + w;
     vertices[i++] = y;
     vertices[i++] = x;
     vertices[i++] = y + h;
     vertices[i++] = x + w;
     vertices[i++] = y + h;
     
     x = 341.0f / 1024.0f;//341.0f / 1024.0f;
     y = 25.0f / 1024.0f;//1.0f;
     w = 20.0f / 1024.0f;
     h = 25.0f / 1024.0f;
     
     i = 0;
     texCoord[i++] = x;
     texCoord[i++] = y;
     texCoord[i++] = x + w;
     texCoord[i++] = y;
     texCoord[i++] = x;
     texCoord[i++] = y - h;
     texCoord[i++] = x + w;
     texCoord[i++] = y - h;
     
     x += w;
     y += h;
     
     texCoord[i++] = x;
     texCoord[i++] = y;
     texCoord[i++] = x + w;
     texCoord[i++] = y;
     texCoord[i++] = x;
     texCoord[i++] = y - h;
     texCoord[i++] = x + w;
     texCoord[i++] = y - h;
     // -------- test ENDS
     */
    
    PositionText();
}
void TextBlock::PositionText(){
    if(_text == NULL){
        return;
    }
    
    PointArrays * space = (*_lettersInfo)[' '];
    float shiftX = 0;
    float shiftY = 0;
    float minX = BILLION;
    float minY = BILLION;
    float maxX = -BILLION;
    float maxY = -BILLION;
    //int rows;
    for(int i = 0; i < _textLength; ++i){
        
        int index = i * 8;
        char ch = _text[i];
        if(ch == '\n'){
            shiftX = 0;
            shiftY += (space->height * _scale);
            continue;
        }
        
        bool isToBeMoved = _isCentered || _isRightAligned;
        
        PointArrays* arrays = (*_lettersInfo)[ch];
        if(arrays == NULL) arrays = space;// if letter is not known - use space instead
        for(int j = 0; j < 8; j += 2){
            vertices[index] = _x + shiftX + (arrays->vertices[j] * _scale);
            texCoord[index] = arrays->texCoord[j];
            
            if(isToBeMoved){
                if(vertices[index] < minX) minX = vertices[index];
                if(vertices[index] > maxX) maxX = vertices[index];
            }
            
            ++index;
            
            int jy = j + 1;
            vertices[index] = _y - shiftY + (arrays->vertices[jy] * _scale);
            texCoord[index] = arrays->texCoord[jy];
            
            if(isToBeMoved){
                if(vertices[index] < minY) minY = vertices[index];
                if(vertices[index] > maxY) maxY = vertices[index];
            }
            
            ++index;
        }
        
        shiftX += (arrays->width * _scale);
    }
    
    if(_isCentered){
        float xCenterShift = (maxX - minX) / 2.0;
        float yCenterShift = (maxY - minY) / 2.0;
        
        for(int i = 0; i < _textLength; ++i){
            int index = i * 8;
            for(int j = 0; j < 8; j += 2){
                vertices[index] -= xCenterShift;
                ++index;
                vertices[index] -= yCenterShift;
                ++index;
            }
        }
    }else if(_isRightAligned){
        float xCenterShift = (maxX - minX);
        float yCenterShift = (maxY - minY) / 2.0;
        
        for(int i = 0; i < _textLength; ++i){
            int index = i * 8;
            for(int j = 0; j < 8; j += 2){
                vertices[index] -= xCenterShift;
                ++index;
                vertices[index] -= yCenterShift;
                ++index;
            }
        }
    }
    
}
int TextBlock::FillArrays(PointArrays* arrays, int indexFrom, int indexTo){
    for(int i = 0; i < _textLength; ++i){
        indexTo = Drawable::FillArrays(arrays, i * 8, indexTo);
    }
//    if(yvPrintStatus == 0){
//        yvPrintArray(vertices, 12, "vertices");
//        yvPrintArray(texCoord, 12, "texCoord");
//    }
    return indexTo;
}