//
//  MainViewController.m
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-04.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#import "MainViewController.h"


using namespace std;

@interface MainViewController ()

@end

@implementation MainViewController

- (id)init{
    self = [super init];
    if (self) {
        _isIpad = false;
    }
    return  self;
}

- (void) setUiView:(UIView*) view
            isIpad:(bool)isIpad{
    _factory = Factory::Get();
    _isIpad = isIpad;
    [self setView:view];
}


- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

- (void) touchesBegan: (NSSet*) touches withEvent: (UIEvent*) event
{ 
    UITouch* touch = [touches anyObject];
    CGPoint location  = [touch locationInView: self.view];
//cout << "x:" << location.x << " y:" << location.y << endl;
    if(_isIpad){
        _factory->Scene->TouchBegan(location.x*0.833333, 854-location.y*0.833333);
    }else{
        _factory->Scene->TouchBegan(location.x*2, Factory::Instance->SCREEN_HEIGHT-location.y*2);
    }
}

- (void) touchesMoved: (NSSet*) touches withEvent: (UIEvent*) event
{
    UITouch* touch = [touches anyObject];
    CGPoint previous  = [touch previousLocationInView: self.view];
    CGPoint current = [touch locationInView: self.view];
    if(_isIpad){
        _factory->Scene->TouchMoved(current.x*0.833333, 854-current.y*0.833333, previous.x*0.833333, 854-previous.y*0.833333);
    }else{
        _factory->Scene->TouchMoved(current.x*2, Factory::Instance->SCREEN_HEIGHT-current.y*2, previous.x*2, Factory::Instance->SCREEN_HEIGHT-previous.y*2);
    }
}

- (void) touchesEnded: (NSSet*) touches withEvent: (UIEvent*) event
{
    UITouch* touch = [touches anyObject];
    CGPoint location  = [touch locationInView: self.view];
    if(_isIpad){
        _factory->Scene->TouchEnded(location.x*0.833333, 854-location.y*0.833333);
    }else{
        _factory->Scene->TouchEnded(location.x*2, Factory::Instance->SCREEN_HEIGHT-location.y*2);
    }
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
    cout << "Memory Warning!" << endl;
}


@end
