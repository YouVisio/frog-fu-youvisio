//
//  TextBlock.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-11.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_TextBlock_hpp
#define FrogFu_TextBlock_hpp

#import "Drawable.hpp"

class TextBlock : public Drawable {
public:
    TextBlock(TextureDescription * texture, const char* text, float x, float y, map<int, PointArrays*>* lettersInfo);
    virtual ~TextBlock();
    
    TextBlock* SetText(const char* text);
    float X();
    TextBlock* X(float x);
    float Y();
    TextBlock* Y(float y);
    TextBlock* Position(float x, float y);
    int FillArrays(PointArrays* arrays, int indexFrom, int indexTo);
    int NumberSquares();
    TextBlock* Color(float alpha, float red, float green, float blue);
    TextBlock* Alpha(float alpha);
    TextBlock* IsVisible(bool isVisible);
    TextBlock* SetInit();
    bool IsCentered();
    TextBlock* IsCentered(bool val);
    bool IsRightAligned();
    TextBlock* IsRightAligned(bool val);
    float Scale();
    TextBlock* Scale(float scale);
    
private:
    void GenerateLetterList();
    void PositionText();
    
    char* _text;
    int _textLength;
    float _x;
    float _y;
    bool _isCentered;
    bool _isRightAligned;
    float _scale;
    map<int, PointArrays*>* _lettersInfo;
};

#endif
