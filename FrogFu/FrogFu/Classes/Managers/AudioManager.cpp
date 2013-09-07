//
//  AudioManager.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-12-24.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_AudioManager_hpp
#define FrogFu_AudioManager_hpp

#import <iostream>
#import "BaseManager.hpp"
#import "Interfaces.hpp"
#import "IAudioManager.hpp"

class AudioManager : IAudioManager, BaseManager {
public:
    AudioManager(simlist<IAnimation>* animations){
        _animations = animations;
    }
private:
    simlist<IAnimation>* _animations;
};

IAudioManager* CreateAudioManager(simlist<IAnimation>* animations)
{
    return new AudioManager(animations);
}


#endif
