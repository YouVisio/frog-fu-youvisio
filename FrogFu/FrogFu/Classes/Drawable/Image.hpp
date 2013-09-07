//
//  Image.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-11.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_Image_hpp
#define FrogFu_Image_hpp

#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import "Drawable.hpp"
#import "Vector.hpp"

class Image : public Drawable {
public:
    Image(TextureDescription* texture, int xTxt, int yTxt, int widthTxt, int heightTxt);
    virtual ~Image();
    
    Image* Func(void (*f)(Image* image));
    void (*func)(Image* image);
    
    Image* TextureX(float x);
    float TextureX();
    Image* TextureY(float y);
    float TextureY();
    Image* TexturePosition(float x, float y);
    Image* Position(float x, float y);
    Image* Size(float w, float h);
    Image* PositionAndAutoSize(float x, float y);
    float X();
    Image* X(float x);
    float Y();
    Image* Y(float y);
    vec2 Pos();
    Image* Pos(vec2 pos);
    float Width();
    Image* Width(float w);
    float Height();
    Image* Height(float h);
    float Rotation();
    Image* Rotation(float rotation);
    Image* Scale(float scale);
    Image* ScaleX(float scaleX);
    Image* ScaleY(float scaleY);
    Image* Color(float red,float green,float blue);
    Image* Color(float alpha,float red,float green,float blue);
    Image* Alpha(float alpha);
    Image* IsVisible(bool isVisible);
    float CenterX();
    Image* CenterX(float centerX);
    float CenterY();
    Image* CenterY(float centerY);
    Image* Center(float centerX, float centerY);
    
    float TextureWidth();
    float TextureHeight();
    Image* TextureWidth(float width);
    Image* TextureHeight(float height);
    Image* TextureWidthAndHeight(float width, float height);
    Image* SetTextureHeightAlignTop(float height);
    Image* RedefineTextureCoords(int xTxt, int yTxt, int widthTxt, int heightTxt);
    Image* SetInit();
    
    std::ostream& out(std::ostream& os) const;
    void GenerateTextureCoords();
    

    
private:
    
    void GenerateVertices();
    
    //const char* _name;
    float _xTxt;
    float _yTxt;
    float _widthTxt;
    float _heightTxt;
    
    float _x;
    float _y;
    float _width;
    float _height;
    float _rotation;
    
    float _centerX;
    float _centerY;
    
};

void AssignSquare(GLfloat* array, float left, float right, float bottom, float top);
void ImgCenter_BottomLeft_NoRotation(Image* image);
void ImgCenter_MiddleCenter_NoRotation(Image* image);
void ImgCenter_Centered_NoRotation(Image* image);
void ImgCenter_Centered_WithRotation(Image* image);

#endif
