//
//  Poses.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-27.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_Poses_hpp
#define FrogFu_Poses_hpp

#import "Image.hpp"
#import "Interfaces.hpp"

int getIndexByProgress(double progress, int numFrames);
void poseTongue(Image* tongue, double progress, double maxStretchs, double lengthChange, int tongueType);
void poseHeadEatingInsects(Image* head, double progress, double direction, double distance);
void poseBodyPreJumpUp(Image* body, double progress);
void poseBodyPostJumpDown(Image* body, double progress);
void poseBodyJumpUp(Image* body, double progress);
void poseBodyJumpDown(Image* body, double progress);
void poseBodyDucking(Image* body, double progress);
void poseHeadDucking(Image* head, double progress);
void poseStatHeadTrans1(Image* head, double progress);
void poseStatHeadTrans2(Image* head, double progress);
void poseStatHeadTrans3(Image* head, double progress);
void poseStatBody(Image* body, double progress);
void poseBodyPointRightUp(Image* head, double progress);
void poseBodyTapPointing(Image* head, double progress);
void poseHeadLookRightUp(Image* head, double progress);
void poseBodyTransitionToDucking(Image* body, double progress);
void poseHeadTransitionToDucking(Image* head, double progress);
void poseHeadStung(Image* head, double progress);
void poseBodyStung(Image* body, double progress);
void poseHeadKungFu01(Image* head, double progress);
void poseBodyKungFu01(Image* body, double progress);
void poseHeadKungFu02(Image* head, double progress);
void poseBodyKungFu02(Image* body, double progress);
#endif
