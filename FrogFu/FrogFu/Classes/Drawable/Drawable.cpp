//
//  Drawable.cpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-04.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#import "Drawable.hpp"
#import "Interfaces.hpp"
#import "TextureDescription.hpp"

using namespace std;
int Drawable::_lastId = 0;
Drawable::Drawable(TextureDescription* texture) :
_texture(texture),
vertices(NULL),
texCoord(NULL),
_alpha(1.0f),
_red(1.0f),
_green(1.0f),
_blue(1.0f),
_scaleX(1.0f),
_scaleY(1.0f),
_isColorChanged(false),
_isVisible(true),
_isExpired(false),
_preserveExpired(false),
poseID(0),
mark(0),
poseRotation(0),
isOutOfScreen(false),
sfactor(0),
dfactor(0),
textureGroup(0),
depth(0),
type(0),
_uniqueId(++Drawable::_lastId)
{ }
Drawable::~Drawable(){
    if(vertices != NULL){
        delete [] vertices;
    }
    if(texCoord != NULL){
        delete [] texCoord;
    }
}
TextureDescription* Drawable::Texture(){ return _texture; }
void Drawable::Texture(TextureDescription* texture) { _texture = texture; }
int Drawable::NumberSquares(){
    return 1;//_isVisible ? 1 : 0;
}
bool Drawable::PreserveExpired() { return _preserveExpired; }
Drawable* Drawable::PreserveExpired(bool preserve) { _preserveExpired = preserve; return this; }
Drawable* Drawable::Depth(long d) { depth = d; return this; }
int Drawable::UniqueId() { return _uniqueId; }
float Drawable::Alpha() { return _alpha; }
float Drawable::Red() { return _red; }
float Drawable::Green() { return _green; }
float Drawable::Blue() { return _blue; }
float Drawable::Scale() { return (_scaleX + _scaleY) / 2.0; }
float Drawable::ScaleX() { return _scaleX; }
float Drawable::ScaleY() { return _scaleY; }
bool Drawable::IsColorChanged() { return _isColorChanged; }
bool Drawable::IsVisible() { return _isVisible; }
bool Drawable::IsExpired() { return _isExpired; }
Drawable* Drawable::IsExpired(bool isExpired) { _isExpired = isExpired; return this; }
int Drawable::FillArrays(PointArrays* arrays, int indexFrom, int indexTo){
    
    int colorIndex = indexTo * 2;
    
    // A
    arrays->vertices[indexTo] = vertices[indexFrom];
    arrays->texCoord[indexTo] = texCoord[indexFrom];
    ++indexTo;
    ++indexFrom;
    arrays->vertices[indexTo] = vertices[indexFrom];
    arrays->texCoord[indexTo] = texCoord[indexFrom];
    --indexFrom;// return back to x:A
    
    arrays->colors[colorIndex] = _red;
    arrays->colors[++colorIndex] = _green;
    arrays->colors[++colorIndex] = _blue;
    arrays->colors[++colorIndex] = _alpha;
    
    // ABCD
    
    for(int k = 0; k < 4; ++k){
        ++indexTo;
        arrays->vertices[indexTo] = vertices[indexFrom];
        arrays->texCoord[indexTo] = texCoord[indexFrom];
        ++indexTo;
        ++indexFrom;
        arrays->vertices[indexTo] = vertices[indexFrom];
        arrays->texCoord[indexTo] = texCoord[indexFrom];
        ++indexFrom;
        
        arrays->colors[++colorIndex] = _red;
        arrays->colors[++colorIndex] = _green;
        arrays->colors[++colorIndex] = _blue;
        arrays->colors[++colorIndex] = _alpha;
        
    }
    indexFrom -= 2;// return back to x:D
    // D
    ++indexTo;
    arrays->vertices[indexTo] = vertices[indexFrom];
    arrays->texCoord[indexTo] = texCoord[indexFrom];
    
    ++indexTo;
    ++indexFrom;
    arrays->vertices[indexTo] = vertices[indexFrom];
    arrays->texCoord[indexTo] = texCoord[indexFrom];
    ++indexTo;
    
    
    arrays->colors[++colorIndex] = _red;
    arrays->colors[++colorIndex] = _green;
    arrays->colors[++colorIndex] = _blue;
    arrays->colors[++colorIndex] = _alpha;
    
    
    //    if(yvPrintStatus == 0){
    //        yvPrintArray(vertices, 8, "!vertices");
    //        yvPrintArray(texCoord, 8, "!texCoord");
    //    }
    
    return indexTo;
}
