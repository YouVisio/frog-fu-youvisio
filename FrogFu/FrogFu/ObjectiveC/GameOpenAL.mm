//
//  GameOpenAL.m
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-06.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#import "GameOpenAL.h"
#import "GameOpenALSupport.h"

@implementation GameOpenAL

@synthesize bufferStorageArray;
@synthesize soundDictionary;

static GameOpenAL *instance = nil;

+ (GameOpenAL*)instance {
    if (instance == nil) {
        @synchronized(self) {
            if (instance == nil) {
                instance = [[self alloc] init]; // assignment not done here
            }
        }
    }
    return instance;
}

- (id)copyWithZone:(NSZone *)zone
{
    return self;
}

// seek in audio file for the property 'size'
// return the size in bytes
-(UInt32)audioFileSize:(AudioFileID)fileDescriptor
{
    
    UInt64 outDataSize = 0;
    UInt32 thePropSize = sizeof(UInt64);
    OSStatus result = AudioFileGetProperty(fileDescriptor, kAudioFilePropertyAudioDataByteCount, &thePropSize, &outDataSize);
    if(result != 0) NSLog(@"cannot find file size");
    return (UInt32)outDataSize;
}

// start up openAL
-(bool) initOpenAL
{
    
    // Initialization
    mDevice = alcOpenDevice(NULL); // select the "preferred device"
    if (mDevice) {
        // use the device to make a context
        mContext=alcCreateContext(mDevice,NULL);
        // set my context to the currently active one
        alcMakeContextCurrent(mContext);
        return true;
    }
    return false;
}

-(id) init {
    
    if (self = [super init] ) {
        if ([self initOpenAL]) {
            self.bufferStorageArray = [[NSMutableArray alloc]init];
            self.soundDictionary = [[NSMutableDictionary alloc]init];
            
        }
        return self;
    }
    //[self release];
    return nil;
}

-(void) dealloc {
    
    
    // delete the sources
    for (NSNumber * sourceNumber in [soundDictionary allValues]) {
        NSUInteger sourceID = [sourceNumber unsignedIntegerValue];
        alDeleteSources(1, &sourceID);
    }
    
    self.soundDictionary=nil;
    
    // delete the buffers
    for (NSNumber * bufferNumber in bufferStorageArray) {
        NSUInteger bufferID = [bufferNumber unsignedIntegerValue];
        alDeleteBuffers(1, &bufferID);
    }
    self.bufferStorageArray=nil;
    
    // destroy the context
    alcDestroyContext(mContext);
    // close the device
    alcCloseDevice(mDevice);
    
    //[super dealloc];
}

// the main method: grab the sound ID from the library
// and start the source playing
- (void)playSoundWithKey:(NSString*)soundKey
{
    
    NSNumber * numVal = [soundDictionary objectForKey:soundKey];
    if (numVal == nil) return;
    NSUInteger sourceID = [numVal unsignedIntValue];
    alSourcePlay(sourceID);
}

- (void)stopSoundWithKey:(NSString*)soundKey
{
    
    NSNumber * numVal = [soundDictionary objectForKey:soundKey];
    if (numVal == nil) return;
    NSUInteger sourceID = [numVal unsignedIntValue];
    alSourceStop(sourceID);
}

-(void) rewindSoundWithKey:(NSString *) soundKey {
    
    
    NSNumber * numVal = [soundDictionary objectForKey:soundKey];
    if (numVal == nil) return;
    NSUInteger sourceID = [numVal unsignedIntValue];
    alSourceRewind (sourceID);
}

-(void) changeSoundVolumeWithKey:(NSString *) soundKey Volume:(float)volume {
    NSNumber * numVal = [soundDictionary objectForKey:soundKey];
    if (numVal == nil) return;
    NSUInteger sourceID = [numVal unsignedIntValue];
    alSourcef(sourceID, AL_GAIN, volume);
}

-(bool) isPlayingSoundWithKey:(NSString *) soundKey {
    
    
    NSNumber * numVal = [soundDictionary objectForKey:soundKey];
    if (numVal == nil) return false;
    NSUInteger sourceID = [numVal unsignedIntValue];
    
    ALenum state;
    
    alGetSourcei(sourceID, AL_SOURCE_STATE, &state);
    
    return (state == AL_PLAYING);
}



-(bool) loadSoundWithKey:(NSString *)_soundKey File:(NSString *)_file Ext:(NSString *) _ext Loop:(bool)loops{
    
    
    
    ALvoid * outData;
    ALenum  error = AL_NO_ERROR;
    ALenum  format;
    ALsizei size;
    ALsizei freq;
    
    NSBundle * bundle = [NSBundle mainBundle];
    
    // get some audio data from a wave file
    CFURLRef fileURL = (__bridge_retained CFURLRef)[NSURL fileURLWithPath:[bundle pathForResource:_file ofType:_ext]];
    
    if (!fileURL)
    {
        NSLog(@"file not found.");
        return false;
    }
    
    outData = GameGetOpenALAudioData(fileURL, &size, &format, &freq);
    
    CFRelease(fileURL);
    
    if((error = alGetError()) != AL_NO_ERROR) {
        printf("error loading sound: %xn", error);
        exit(1);
    }
    
    //NSLog(@"getting a free buffer from openAL.");
    NSUInteger bufferID;
    // grab a buffer ID from openAL
    alGenBuffers(1, &bufferID);
    
    //NSLog(@"loading audio data into openAL buffer.");
    // load the awaiting data blob into the openAL buffer.
    alBufferData(bufferID,format,outData,size,freq);
    
    // save the buffer so we can release it later
    [bufferStorageArray addObject:[NSNumber numberWithUnsignedInteger:bufferID]];
    
    //NSLog(@"getting a free source from openAL.");
    NSUInteger sourceID;
    // grab a source ID from openAL
    alGenSources(1, &sourceID);
    
    //NSLog(@"attatching the buffer to the source and setting up preferences");
    // attach the buffer to the source
    alSourcei(sourceID, AL_BUFFER, bufferID);
    // set some basic source prefs
    alSourcef(sourceID, AL_PITCH, 1.0f);
    alSourcef(sourceID, AL_GAIN, 1.0f);
    if (loops) alSourcei(sourceID, AL_LOOPING, AL_TRUE);
    
    // store this for future use
    [soundDictionary setObject:[NSNumber numberWithUnsignedInt:sourceID] forKey:_soundKey];
    
    //NSLog(@"free %i bytes of temporary allocated memory.", size);
    // clean up the buffer
    if (outData)
    {
        free(outData);
        outData = NULL;
    }
    
    //NSLog(@"Audiofile successfully loaded.");
    return true;
    
}
@end