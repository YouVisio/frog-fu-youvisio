//
//  MainViewController.h
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-04.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#include <iostream>
#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import "Factory.hpp"

@interface MainViewController : UIViewController{
    Factory* _factory;
    bool _isIpad;
}

- (void) setUiView:(UIView*) view
            isIpad:(bool)isIpad;

@end
