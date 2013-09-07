//
//  Drawable.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-11.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_Drawable_hpp
#define FrogFu_Drawable_hpp

#import <iostream>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import "TextureDescription.hpp"

#define DRAWABLE_IMAGE 1
#define DRAWABLE_TEXT_BLOCK 2


struct PointArrays{
    GLfloat* vertices;
    GLfloat* texCoord;
    GLfloat* colors;
    float width;
    float height;
    int length;
    
    PointArrays(){
        vertices = NULL;
        texCoord = NULL;
        colors = NULL;
    }
    ~PointArrays(){
        if(vertices != NULL){
            delete [] vertices;
            vertices = NULL;
        }
        if(texCoord != NULL){
            delete [] texCoord;
            texCoord = NULL;
        }
        if(colors != NULL){
            delete [] colors;
            colors = NULL;
        }

    }
};

class Drawable {
public:
    Drawable(TextureDescription* texture);
    virtual ~Drawable();
    
    GLfloat* vertices;
    GLfloat* texCoord;
    int poseID;
    int mark;
    double poseRotation;
    int textureGroup;
    long depth;
    int type;
    
    GLenum sfactor;
    GLenum dfactor;
    
    TextureDescription* Texture();
    void Texture(TextureDescription* texture);
    virtual int NumberSquares();
    virtual int FillArrays(PointArrays* arrays, int indexFrom, int indexTo);
    
    virtual int UniqueId();
    virtual float Alpha();
    virtual float Red();
    virtual float Green();
    virtual float Blue();
    virtual float Scale();
    virtual float ScaleX();
    virtual float ScaleY();
    virtual bool IsVisible();
    virtual bool IsExpired();
    virtual bool IsColorChanged();
    virtual Drawable* IsExpired(bool isExpired);
    virtual bool PreserveExpired();
    virtual Drawable* PreserveExpired(bool preserve);
    virtual Drawable* Depth(long d);
    
    bool isOutOfScreen;
    
    float initX;
    float initY;
    
protected:
    TextureDescription * _texture;
    float _alpha;
    float _red;
    float _green;
    float _blue;
    float _scaleX;
    float _scaleY;
    bool _isVisible;
    bool _isExpired;
    int _uniqueId;
    bool _isColorChanged;
    bool _preserveExpired;
private:
    static int _lastId;
};

#endif
