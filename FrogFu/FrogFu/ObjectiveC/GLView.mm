//
//  GLView.m
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-04.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#import "GLView.h"

@implementation GLView

+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

- (id)initWithFrame:(CGRect)frame
          forIphone:(int)iPhone
             isIpad:(bool)isIpad
        onSimulator:(bool)simulator
{
    srand(time(NULL));
    
    self = [super initWithFrame:frame];
    if (self) {
        CAEAGLLayer* eaglLayer = (CAEAGLLayer*) super.layer;
        eaglLayer.opaque = YES;
        
        EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES2;
        _context = [[EAGLContext alloc] initWithAPI:api];
        
        if (!_context || ![EAGLContext setCurrentContext:_context]) {
            return nil;
        }
        
        _factory = Factory::Get();
        
        sysLoadSound("M1", "mp3", true, true);
        sysLoadSound("M2", "mp3", true, true);
        sysLoadSound("Gong", "mp3", false, false);
        sysLoadSound("Eat", "mp3", false, false);
        sysLoadSound("Touch", "mp3", false, false);
        sysLoadSound("Stung", "mp3", false, false);
        sysLoadSound("Laughter", "mp3", false, false);
        sysLoadSound("Bonus", "mp3", false, false);
        sysLoadSound("NumbersUp", "mp3", true, false);
        sysLoadSound("Splash", "mp3", false, false);
        sysLoadSound("Stamp", "mp3", false, false);
        
        if(SYSTEM_VERSION_LESS_THAN(@"5.0")){
            _factory->iOSversion = 4;
        } else if(SYSTEM_VERSION_LESS_THAN(@"6.0")){
            _factory->iOSversion = 5;
        } else {
            _factory->iOSversion = 6;
        }
        
        if(_factory->iOSversion < 6){
            _factory->Renderer->SetDefaultBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        
        _factory->particleWidth = 20;
        _factory->mapDotWidth = 8;
        if(simulator){
            _factory->mapDotWidth = 6;
        }
        if(isIpad){
            _factory->isIpad = true;
            _factory->HALF_SCREEN_HEIGHT = 427.0;
            _factory->SCREEN_HEIGHT = 854.0;
            _factory->mapDotWidth = 11;
            _factory->particleWidth = 30;
        } else if(iPhone == 5){
            _factory->isFive = true;
            _factory->HALF_SCREEN_HEIGHT = 568.0;
            _factory->SCREEN_HEIGHT = 1136.0;
            _factory->particleWidth = 30;
            _factory->mapDotWidth = 11;
            if(simulator){
                _factory->mapDotWidth = 6;
            }
        }
        
        // Get documents directory
        NSArray *arrayPaths =
		NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,
                                            NSUserDomainMask,
                                            YES);
        NSString *docDir = [arrayPaths objectAtIndex:0];
        _factory->DocsDir = [docDir UTF8String];
        
        _factory->UserState = new UserState(concArr(_factory->DocsDir, "/s38.dat"));
        _factory->UserState->Deserialize();
        
        _factory->Settings = new Settings(concArr(_factory->DocsDir, "/set.dat"));
        _factory->Settings->Deserialize();
        
        if(_factory->UserState->w == 0){
            // write first run time
            _factory->UserState->w = time(NULL);
            _factory->UserState->Serialize();
        }
        
        
        
        [_context
             renderbufferStorage:GL_RENDERBUFFER
             fromDrawable:eaglLayer];
        
        _factory->Renderer->Initialize(Factory::Instance->SCREEN_WIDTH, Factory::Instance->SCREEN_HEIGHT);
        
        _factory->createScenes();
        
        _factory->Scene->Initialize();
        
        //[_context presentRenderbuffer:GL_RENDERBUFFER];
        
        CADisplayLink* displayLink;
        displayLink = [CADisplayLink displayLinkWithTarget:self
                                                  selector:@selector(drawView:)];
        
        [displayLink setFrameInterval:1];
        
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop]
                          forMode:NSDefaultRunLoopMode];
        
        
    }
    return self;
}


- (void) drawView: (CADisplayLink*) displayLink{

    double elapsedSeconds = displayLink.timestamp - _timestamp;
    _factory->Scene->Update(elapsedSeconds);
    _timestamp = displayLink.timestamp;
    
    _factory->Renderer->Render();
    
    [_context presentRenderbuffer:GL_RENDERBUFFER];
}

// Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
// Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
- (void)applicationWillResignActive:(UIApplication *)application{
    Factory::Get()->applicationWillResignActive();
}

// Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
// If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
- (void)applicationDidEnterBackground:(UIApplication *)application{
    Factory::Get()->applicationDidEnterBackground();
}

// Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
- (void)applicationWillEnterForeground:(UIApplication *)application{
    Factory::Get()->applicationWillEnterForeground();
}

// Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
- (void)applicationDidBecomeActive:(UIApplication *)application{
    Factory::Get()->applicationDidBecomeActive();
}

// Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
- (void)applicationWillTerminate:(UIApplication *)application{
    //Factory::Get()->applicationWillTerminate();
}

@end
