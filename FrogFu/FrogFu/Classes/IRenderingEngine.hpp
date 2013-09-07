//
//  IRenderingEngine.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-11.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_IRenderingEngine_hpp
#define FrogFu_IRenderingEngine_hpp

#import "Interfaces.hpp"
#import "Drawable.hpp"
#import "Particle.hpp"
#import "IResourceManager.hpp"
#import "IAnimation.hpp"
#import <vector>
#import <map>
#import <OpenGLES/ES2/gl.h>

using namespace std;

struct IRenderingEngine {
    virtual void Initialize(int width, int height) = 0;
    virtual void Render() = 0;
    virtual void Test() = 0;
    virtual simlist<Drawable>* GetDrawables() = 0;
    virtual ParticleList* GetTouchTrails() = 0;
    virtual simlist<Particle>* GetMapDots() = 0;
    virtual simlist<IAnimation>* GetAnimations() = 0;
    virtual void LoadTextureById(int id) = 0;
    virtual void UnloadTextureById(int id) = 0;
    virtual void ClearCurrentTexture() = 0;
    virtual void ClearExpired() = 0;
    virtual void SetDefaultBlendFunc(GLenum sfactor, GLenum dfactor) = 0;
    virtual ~IRenderingEngine() {}

    virtual void applicationWillResignActive() = 0;
    virtual void applicationDidEnterBackground() = 0;
    virtual void applicationWillEnterForeground() = 0;
    virtual void applicationDidBecomeActive() = 0;
    virtual void applicationWillTerminate() = 0;
};

IRenderingEngine* CreateRenderingEngine(IResourceManager* resources);

#endif
