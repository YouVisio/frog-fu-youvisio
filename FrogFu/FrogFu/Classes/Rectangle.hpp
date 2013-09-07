//
//  Rectangle.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-10-19.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_Rectangle_hpp
#define FrogFu_Rectangle_hpp

#import <cmath>
#import <iostream>

template <typename T>
struct Rectangle {
    Rectangle() {}
    Rectangle(T x, T y, T w, T h) : x(x), y(y), w(w), h(h) {}
    T Surface(){
        return w * h;
    }
    T x;
    T y;
    T w;
    T h;
};

typedef Rectangle<float> rect;

#endif
