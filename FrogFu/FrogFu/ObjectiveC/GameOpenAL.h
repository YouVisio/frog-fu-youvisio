//
//  GameOpenAL.h
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-06.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//
#import <Foundation/Foundation.h>
#import <OpenAL/al.h>
#import <OpenAL/alc.h>
#import <AudioToolbox/AudioToolbox.h>

@interface GameOpenAL : NSObject {
    
    ALCcontext* mContext; // stores the context (the 'air')
    ALCdevice* mDevice; // stores the device
    NSMutableArray * bufferStorageArray; // stores the buffer ids from openAL
    NSMutableDictionary * soundDictionary; // stores our soundkeys
}

// if you want to access directly the buffers or our sound dictionary
@property (nonatomic, retain) NSMutableArray * bufferStorageArray;
@property (nonatomic, retain) NSMutableDictionary * soundDictionary;

- (id) init; // init once
- (bool) initOpenAL; // no need to make it public, but I post it here to show you which methods we need. initOpenAL will be called within init process once.
- (void) playSoundWithKey:(NSString*)soundKey; // play a sound by name
- (void) stopSoundWithKey:(NSString*)soundKey; // stop a sound by name
- (bool) isPlayingSoundWithKey:(NSString *) soundKey; // check if sound is playing by name
- (void) rewindSoundWithKey:(NSString *) soundKey; // rewind a sound by name so its playing again
- (void) changeSoundVolumeWithKey:(NSString *) soundKey Volume:(float)volume;
- (bool) loadSoundWithKey:(NSString *)_soundKey File:(NSString *)_file Ext:(NSString *) _ext Loop:(bool)loops; // load a sound and give it a name

+ (GameOpenAL*)instance; // access to our instance

@end