//
//  Image.cpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-05.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#import "Interfaces.hpp"
#import "Image.hpp"
#import <iostream>

using namespace std;

void AssignSquare(GLfloat* array, float left, float right, float bottom, float top){
    array[0] = left;
    array[1] = bottom;
    array[2] = right;
    array[3] = bottom;
    array[4] = left;
    array[5] = top;
    array[6] = right;
    array[7] = top;
}
void EnsureVertices(Image* image, float left, float right, float bottom, float top){
    // check is the image is out of screen
    if(left > 640 || right < 0 || bottom > 1136 || top < 0){
        if(!image->isOutOfScreen){
            image->isOutOfScreen = true;
        }
        return;
    }
    // if the image has previously been out of screen
    if(image->isOutOfScreen){
        // mark it as no longer out of screen
        image->isOutOfScreen = false;
    }
}
void ImgCenter_BottomLeft_NoRotation(Image* image){
    float x = image->X();
    float y = image->Y();
    float w = image->Width();
    float h = image->Height();
    float sx = image->Drawable::ScaleX();
    float sy = image->Drawable::ScaleY();
    
    float left = x;
    float right = x + w * sx;
    float bottom = y;
    float top = y + h * sy;
    
    EnsureVertices(image, left, right, bottom, top);
    
    AssignSquare(image->vertices, left, right, bottom, top);
}
void ImgCenter_MiddleCenter_NoRotation(Image* image){
    float x = image->X();
    float y = image->Y();
    float w = image->Width();
    float h = image->Height();
    float sx = image->Drawable::ScaleX();
    float sy = image->Drawable::ScaleY();
    
    float hw = (w / 2.0f) * sx;
    float hh = (h / 2.0f) * sy;
    
    float left = x - hw;
    float right = x + hw;
    float bottom = y - hh;
    float top = y + hh;
    
    EnsureVertices(image, left, right, bottom, top);
    
    AssignSquare(image->vertices, left, right, bottom, top);
}
void ImgCenter_Centered_NoRotation(Image* image){
    float x = image->X();
    float y = image->Y();
    float w = image->Width();
    float h = image->Height();
    float sx = image->Drawable::ScaleX();
    float sy = image->Drawable::ScaleY();
    
    float preX = image->CenterX() * sx;
    float postX = w * sx - preX;
    float preY = image->CenterY() * sy;
    float postY = h * sy - preY;
    
    float left = x - preX;
    float right = x + postX;
    float bottom = y - postY;
    float top = y + preY;
    
    EnsureVertices(image, left, right, bottom, top);
    
    AssignSquare(image->vertices, left, right, bottom, top);
}
void ImgCenter_Centered_WithRotation(Image* image){
    float x = image->X();
    float y = image->Y();
    float w = image->Width();
    float h = image->Height();
    float sx = image->Drawable::ScaleX();
    float sy = image->Drawable::ScaleY();
    float r = image->Rotation();
    
    float preX = image->CenterX() * sx;
    float postX = w * sx - preX;
    float preY = image->CenterY() * sy;
    float postY = h * sy - preY;
    
    float left = x - preX;
    float right = x + postX;
    float bottom = y - postY;
    float top = y + preY;
    
    EnsureVertices(image, left, right, bottom, top);
    
    if(r == 0 && image->poseRotation == r){
       AssignSquare(image->vertices, left, right, bottom, top);
       return; 
    }
    image->poseRotation = r;
    
    vec2 center = vec2(x, y);
    vec2 leftBottom = vec2(left, bottom).Rotate(center, r);
    vec2 rightBottom = vec2(right, bottom).Rotate(center, r);
    vec2 leftTop = vec2(left, top).Rotate(center, r);
    vec2 rightTop = vec2(right, top).Rotate(center, r);
    
    image->vertices[0] = leftBottom.x;
    image->vertices[1] = leftBottom.y;
    image->vertices[2] = rightBottom.x;
    image->vertices[3] = rightBottom.y;
    image->vertices[4] = leftTop.x;
    image->vertices[5] = leftTop.y;
    image->vertices[6] = rightTop.x;
    image->vertices[7] = rightTop.y;
}

Image::Image(TextureDescription* texture, int xTxt, int yTxt, int widthTxt, int heightTxt)
: Drawable(texture){
    //_name = strcpy(new char[strlen(name) + 1 ], name);
    
    _x = 0;
    _y = 0;
    _width = widthTxt;
    _height = heightTxt;
    
    _rotation = 0;
    
    _xTxt = xTxt;
    _yTxt = yTxt;
    _widthTxt = widthTxt;
    _heightTxt = heightTxt;
    
    _centerX = _centerY = 0;
    
    vertices = new GLfloat[8];
    texCoord = new GLfloat[8];
    
    type = DRAWABLE_IMAGE;
    
    Func(ImgCenter_BottomLeft_NoRotation);
    
    GenerateTextureCoords();
}
Image::~Image(){

}
void Image::GenerateTextureCoords(){
    float textureWidth = _texture->Size.x;
    float textureHeight = _texture->Size.y;
    
    float left = _xTxt / textureWidth;
    float right = (_xTxt + _widthTxt) / textureWidth;
    float bottom = _yTxt / textureHeight;
    float top = (_yTxt + _heightTxt) / textureHeight;
    
    texCoord[0] = left;
    texCoord[1] = top;
    texCoord[2] = right;
    texCoord[3] = top;
    texCoord[4] = left;
    texCoord[5] = bottom;
    texCoord[6] = right;
    texCoord[7] = bottom;
    
}

void Image::GenerateVertices(){
    func(this);
}

std::ostream& Image::out(std::ostream& os) const{
    return os << "[Image Texture=" << _texture <<
    " XTxt=" << _xTxt << " YTxt=" << _yTxt << " WTxt=" << _widthTxt << " HTxt=" << _heightTxt << "]";
}
std::ostream& operator<<(std::ostream& os, const Image& i){ return i.out(os); }

Image* Image::Func(void (*f)(Image* image)){
    func = f;
    GenerateVertices();
    return this;
}
Image* Image::TextureX(float x){ _xTxt = x; GenerateTextureCoords(); return this;  }
float Image::TextureX(){ return _xTxt; }
Image* Image::TextureY(float y){ _yTxt = y; GenerateTextureCoords(); return this;  }
float Image::TextureY(){ return _yTxt; }
Image* Image::TexturePosition(float x, float y){ _xTxt = x;_yTxt = y; GenerateTextureCoords(); return this;  }
Image* Image::Position(float x, float y) { _x = x; _y = y; GenerateVertices(); return this; }
Image* Image::Size(float w, float h) { _width = w; _height = h; GenerateVertices(); return this; }
Image* Image::PositionAndAutoSize(float x, float y)
{
    _x = x; _y = y; _width = _widthTxt / 80.0f; _height = _heightTxt / 80.0f; GenerateVertices(); return this;
}
float Image::X() { return _x; }
Image* Image::X(float x) { _x = x; GenerateVertices(); return this; }
float Image::Y() { return _y; }
Image* Image::Y(float y) { _y = y; GenerateVertices(); return this; }
Image* Image::Pos(vec2 pos) { _x = pos.x; _y = pos.y; GenerateVertices();  return this; }
vec2 Image::Pos() { return vec2(_x, _y); }
float Image::Width() { return _width; }
Image* Image::Width(float w) { _width = w; GenerateVertices(); return this; }
float Image::Height() { return _height; }
Image* Image::Height(float h) { _height = h; GenerateVertices(); return this; }
float Image::Rotation() { return _rotation; }
Image* Image::Rotation(float rotation) { _rotation = rotation; GenerateVertices(); return this; }
Image* Image::Scale(float scale) { _scaleX = scale; _scaleY = scale; GenerateVertices(); return this; }
Image* Image::ScaleX(float scaleX) { _scaleX = scaleX; GenerateVertices(); return this; }
Image* Image::ScaleY(float scaleY) { _scaleY = scaleY; GenerateVertices(); return this; }
Image* Image::Color(float alpha,float red,float green,float blue) {
    _alpha = alpha;
    _red = red;
    _green = green;
    _blue = blue;
    _isColorChanged = _alpha < 1.0f || _red < 1.0f || _green < 1.0f || _blue < 1.0f;
    return this;
}
Image* Image::Color(float red,float green,float blue) {
    _red = red;
    _green = green;
    _blue = blue;
    _isColorChanged = _red < 1.0f || _green < 1.0f || _blue < 1.0f || _alpha < 1.0f;
    return this;
}
Image* Image::Alpha(float alpha) {
    _alpha = alpha;
    _isColorChanged = _alpha < 1.0f || _red < 1.0f || _green < 1.0f || _blue < 1.0f;
    return this;
}

float Image::TextureWidth() { return _widthTxt; }
float Image::TextureHeight() { return _heightTxt; }
Image* Image::TextureWidth(float width) { _widthTxt = width; GenerateTextureCoords(); return this; }
Image* Image::TextureHeight(float height) { _heightTxt = height; GenerateTextureCoords(); return this; }
Image* Image::TextureWidthAndHeight(float width, float height) { _widthTxt = width; _heightTxt = height; GenerateTextureCoords(); return this; }
Image* Image::SetTextureHeightAlignTop(float height){
    float diff = _heightTxt - height;
    _heightTxt = height;
    _height -= diff;
    GenerateTextureCoords();
    GenerateVertices();
    return this;
}
Image* Image::RedefineTextureCoords(int xTxt, int yTxt, int widthTxt, int heightTxt){
    _xTxt = xTxt;
    _yTxt = yTxt;
    _widthTxt = widthTxt;
    _heightTxt = heightTxt;
    
    _width = widthTxt;
    _height = heightTxt;
    
    GenerateTextureCoords();
    GenerateVertices();
    return this;
}
Image* Image::SetInit(){ initX = _x; initY = _y; return this; }
Image* Image::IsVisible(bool isVisible){ _isVisible = isVisible; return this; }
float Image::CenterX() { return _centerX; }
Image* Image::CenterX(float centerX) { _centerX = centerX; GenerateVertices(); return this; }
float Image::CenterY() { return _centerY; }
Image* Image::CenterY(float centerY) { _centerY = centerY; GenerateVertices(); return this; }
Image* Image::Center(float centerX, float centerY) { _centerX = centerX; _centerY = centerY; GenerateVertices(); return this; }
