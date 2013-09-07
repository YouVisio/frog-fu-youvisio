//
//  GameOpenALSupport.m
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-06.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#include "GameOpenALSupport.h"

ALvoid  alBufferDataStaticProc(const ALint bid, ALenum format, ALvoid* data, ALsizei size, ALsizei freq)
{
    static  alBufferDataStaticProcPtr   proc = NULL;
    
    if (proc == NULL) {
        proc = (alBufferDataStaticProcPtr) alcGetProcAddress(NULL, (const ALCchar*) "alBufferDataStatic");
    }
    
    if (proc)
        proc(bid, format, data, size, freq);
    
    return;
}

void* GameGetOpenALAudioData(CFURLRef inFileURL, ALsizei *outDataSize, ALenum *outDataFormat, ALsizei*    outSampleRate)
{
    OSStatus err = noErr;
    SInt64 theFileLengthInFrames = 0;
    AudioStreamBasicDescription theFileFormat;
    UInt32 thePropertySize = sizeof(theFileFormat);
    ExtAudioFileRef extRef = NULL;
    void* theData = NULL;
    AudioStreamBasicDescription theOutputFormat;
    
    // Open a file with ExtAudioFileOpen()
    err = ExtAudioFileOpenURL(inFileURL, &extRef);
    if(err) {
        printf("GetGetOpenALAudioData: ExtAudioFileOpenURL FAILED, Error = %ldn", err);
        if (extRef) ExtAudioFileDispose(extRef);
        return theData;
        //goto Exit;
    }
    
    // Get the audio data format
    err = ExtAudioFileGetProperty(extRef, kExtAudioFileProperty_FileDataFormat, &thePropertySize, &theFileFormat);
    if(err) {
        printf("GetGetOpenALAudioData: ExtAudioFileGetProperty(kExtAudioFileProperty_FileDataFormat) FAILED, Error = %ldn", err);
        if (extRef) ExtAudioFileDispose(extRef);
        return theData;
        //goto Exit;
    }
    if (theFileFormat.mChannelsPerFrame > 2)  {
        printf("GetGetOpenALAudioData - Unsupported Format, channel count is greater than stereon");
        if (extRef) ExtAudioFileDispose(extRef);
        return theData;
        //goto Exit;
    }
    
    // Set the client format to 16 bit signed integer (native-endian) data
    // Maintain the channel count and sample rate of the original source format
    theOutputFormat.mSampleRate = theFileFormat.mSampleRate;
    theOutputFormat.mChannelsPerFrame = theFileFormat.mChannelsPerFrame;
    
    theOutputFormat.mFormatID = kAudioFormatLinearPCM;
    theOutputFormat.mBytesPerPacket = 2 * theOutputFormat.mChannelsPerFrame;
    theOutputFormat.mFramesPerPacket = 1;
    theOutputFormat.mBytesPerFrame = 2 * theOutputFormat.mChannelsPerFrame;
    theOutputFormat.mBitsPerChannel = 16;
    theOutputFormat.mFormatFlags = kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked | kAudioFormatFlagIsSignedInteger;
    
    // Set the desired client (output) data format
    err = ExtAudioFileSetProperty(extRef, kExtAudioFileProperty_ClientDataFormat, sizeof(theOutputFormat), &theOutputFormat);
    if(err) {
        printf("GetGetOpenALAudioData: ExtAudioFileSetProperty(kExtAudioFileProperty_ClientDataFormat) FAILED, Error = %ldn", err);
        if (extRef) ExtAudioFileDispose(extRef);
        return theData;
        //goto Exit;
    }
    
    // Get the total frame count
    thePropertySize = sizeof(theFileLengthInFrames);
    err = ExtAudioFileGetProperty(extRef, kExtAudioFileProperty_FileLengthFrames, &thePropertySize, &theFileLengthInFrames);
    if(err) {
        printf("GetGetOpenALAudioData: ExtAudioFileGetProperty(kExtAudioFileProperty_FileLengthFrames) FAILED, Error = %ldn", err);
        if (extRef) ExtAudioFileDispose(extRef);
        return theData;
        //goto Exit;
    }
    
    // Read all the data into memory
    UInt32      dataSize = theFileLengthInFrames * theOutputFormat.mBytesPerFrame;;
    theData = malloc(dataSize);
    if (theData)
    {
        AudioBufferList     theDataBuffer;
        theDataBuffer.mNumberBuffers = 1;
        theDataBuffer.mBuffers[0].mDataByteSize = dataSize;
        theDataBuffer.mBuffers[0].mNumberChannels = theOutputFormat.mChannelsPerFrame;
        theDataBuffer.mBuffers[0].mData = theData;
        
        // Read the data into an AudioBufferList
        err = ExtAudioFileRead(extRef, (UInt32*)&theFileLengthInFrames, &theDataBuffer);
        if(err == noErr)
        {
            // success
            *outDataSize = (ALsizei)dataSize;
            *outDataFormat = (theOutputFormat.mChannelsPerFrame > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
            *outSampleRate = (ALsizei)theOutputFormat.mSampleRate;
        }
        else
        {
            // failure
            free (theData);
            theData = NULL; // make sure to return NULL
            printf("GameGetOpenALAudioData: ExtAudioFileRead FAILED, Error = %ldn", err);
            //goto Exit;
        }
    }
    
    // Dispose the ExtAudioFileRef, it is no longer needed
    if (extRef) ExtAudioFileDispose(extRef);
    return theData;
}
