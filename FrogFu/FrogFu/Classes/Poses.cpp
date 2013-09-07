//
//  Poses.cpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-28.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#import "Poses.hpp"
#import "Factory.hpp"

int getIndexByProgress(double progress, int numFrames){
    if(numFrames <= 1) return 0;
    return (int)ensureRange(floor(numFrames * progress), 0.0, numFrames - 1);
}
void poseTongue(Image* tongue, double progress, double maxStretchs, double lengthChange, int tongueType){
    int numFrames = 10;
    bool isApproach = lengthChange < 0;
    Factory* f = Factory::Instance;
    for(int i = 0; i < f->tongueDistances->size(); ++i){
        double curr = (*f->tongueDistances)[i];
        if(curr > maxStretchs){
            numFrames = isApproach && i > 0 ? i : i + 1;
            break;
        }
    }
    int index = getIndexByProgress(progress, numFrames);
    bool isIron = tongueType > 0;
    int id = index + 900 + (isIron ? 10 : 0);
    if(id == tongue->poseID) return; // same pose
    tongue->poseID = id;
    const double w = 330;
    const double h = 19;
    int xTxt = 694;
    int yTxt = (isIron ? 836 : 384) + index * h;
    tongue->RedefineTextureCoords(xTxt, yTxt, w, h);
}
void poseHeadEatingInsects(Image* head, double progress, double angle, double distance){
    // remark: distance is not used for now
    int id = 0;
    if(progress >= 0) {
        int indexProgress = getIndexByProgress(progress, 7);
        const double TWO_PI = M_PI * 2.0;
        angle = sysMod(angle, TWO_PI);
        if(angle > M_PI) angle = (TWO_PI - angle) * -1;
        int indexAngle = getIndexByProgress(abs(angle) / M_PI, 5);
        if(indexProgress == 0 || indexProgress == 6){
            id = 631;
        } else if(indexProgress == 1 || indexProgress == 5){
            id = 632;
        } else if(indexProgress == 2 || indexProgress == 4){
            if(angle >= 0){
                id = 650 + indexAngle;
            } else {
                id = 660 + indexAngle;
                if (id == 660) id = 650;
                else if (id == 664) id = 654;
            }
        } else { // if(indexProgress == 3)
            if(angle >= 0){
                id = 670 + indexAngle;
            } else {
                id = 680 + indexAngle;
                if (id == 680) id = 670;
                else if (id == 684) id = 674;
            }
        }
//cout << "id:" << id << " indexAngle: " << indexAngle << "|" << angle << endl;
    }
    if(id == 0) id = 630;
    if(id == head->poseID) return; // same pose
    head->poseID = id;
    rect r = Factory::Instance->getFrameInTxtByPoseId(id);
    head->RedefineTextureCoords(r.x, r.y, r.w, r.h);
}
void poseBodyPreJumpUp(Image* body, double progress){
    int index = getIndexByProgress(progress, 5);
    int id = index + 700;
    if(id == body->poseID) return; // same pose
    body->poseID = id;
//cout << "poseBodyPreJumpUp:" << id << "|" << progress << "|" << body->Texture()->ID << endl;
    rect r = Factory::Instance->getFrameInTxtByPoseId(id);
    body->RedefineTextureCoords(r.x, r.y, r.w, r.h);
}
void poseBodyJumpUp(Image* body, double progress){
    int index = getIndexByProgress(progress, 8);
    int id = index + 705;
    if(id == body->poseID) return; // same pose
    body->poseID = id;
//cout << "poseBodyJumpUp:" << id << "|" << progress << endl;
    rect r = Factory::Instance->getFrameInTxtByPoseId(id);
    body->RedefineTextureCoords(r.x, r.y, r.w, r.h);
}
void poseBodyJumpDown(Image* body, double progress){
    int index = getIndexByProgress(progress, 5);
    int id = index + 800;
    if(id == body->poseID) return; // same pose
    body->poseID = id;
    rect r = Factory::Instance->getFrameInTxtByPoseId(id);
    body->RedefineTextureCoords(r.x, r.y, r.w, r.h);
}
void poseBodyPostJumpDown(Image* body, double progress){
    int index = getIndexByProgress(progress, 7);
    int id = index + 806;
    if(id == body->poseID) return; // same pose
//cout << "pID:" << id << "|" << progress << endl;
    body->poseID = id;
    rect r = Factory::Instance->getFrameInTxtByPoseId(id);
    body->RedefineTextureCoords(r.x, r.y, r.w, r.h);
}
void poseBodyTransitionToDucking(Image* body, double progress){
    int index = getIndexByProgress(progress, 2);
    int id = index + 101;
    if(id == body->poseID) return; // same pose
    body->poseID = id;
    rect r = Factory::Instance->getFrameInTxtByPoseId(id);
    body->RedefineTextureCoords(r.x, r.y, r.w, r.h);
}
void poseHeadTransitionToDucking(Image* head, double progress){
    int index = getIndexByProgress(progress, 2);
    int id = index == 0 ? 61 : 62;
    if(id == head->poseID) return; // same pose
    head->poseID = id;
    const double w = 185;
    const double h = 108;
    int xTxt = index == 0 ? 555 : 0;
    int yTxt = index == 0 ? 432 : 540;
    head->RedefineTextureCoords(xTxt, yTxt, w, h);
}
void poseBodyDucking(Image* body, double progress){
    int index = getIndexByProgress(progress, 16);
    int id = index + 500;
    if(id == body->poseID) return; // same pose
    body->poseID = id;
//cout << "poseBodyDucking:" << id << "|" << progress << "|" << body->Texture()->ID << endl;
    rect r = Factory::Instance->getFrameInTxtByPoseId(id);
    body->RedefineTextureCoords(r.x, r.y, r.w, r.h);
}
void poseHeadDucking(Image* head, double progress){
    int index = getIndexByProgress(progress, 8);
    int id = index + 600;
    if(id == head->poseID) return; // same pose
    head->poseID = id;
//cout << "poseHeadDucking:" << id << "|" << progress << "|" << head->Texture()->ID << endl;
    rect r = Factory::Instance->getFrameInTxtByPoseId(id);
    head->RedefineTextureCoords(r.x, r.y, r.w, r.h);
}
void poseBodyPointRightUp(Image* body, double progress){
    int index = getIndexByProgress(progress, 7);
    int id = index + 400;
    if(id == body->poseID) return; // same pose
    body->poseID = id;
    rect r = Factory::Instance->getFrameInTxtByPoseId(id);
    body->RedefineTextureCoords(r.x, r.y, r.w, r.h);
}
void poseBodyTapPointing(Image* head, double progress){
    int index = getIndexByProgress(progress, 5);
    int id = index + 406;
    if(id == head->poseID) return; // same pose
    head->poseID = id;
    rect r = Factory::Instance->getFrameInTxtByPoseId(id);
    head->RedefineTextureCoords(r.x, r.y, r.w, r.h);
}
void poseHeadLookRightUp(Image* head, double progress){
    int index = getIndexByProgress(progress, 5);
    int id = index + 300;
    if(id == head->poseID) return; // same pose
    head->poseID = id;
    rect r = Factory::Instance->getFrameInTxtByPoseId(id);
    head->RedefineTextureCoords(r.x, r.y, r.w, r.h);
}
void poseStatHeadTrans1(Image* head, double progress){
    int index = getIndexByProgress(progress, 11);
    int id = index > 5 ? 55 - (index - 5) : index + 50;
    if(id == head->poseID) return; // same pose
    head->poseID = id;
    rect r = Factory::Instance->getFrameInTxtByPoseId(id);
    head->RedefineTextureCoords(r.x, r.y, r.w, r.h);
}
void poseStatHeadTrans2(Image* head, double progress){
    int index = getIndexByProgress(progress, 9);
    int id = index + 56;
    if(id == head->poseID) return; // same pose
    head->poseID = id;
    rect r = Factory::Instance->getFrameInTxtByPoseId(id);
    head->RedefineTextureCoords(r.x, r.y, r.w, r.h);
}
void poseStatHeadTrans3(Image* head, double progress){
    int index = getIndexByProgress(progress, 10);
    int id = index + 64;
    if(id == head->poseID) return; // same pose
    head->poseID = id;
    rect r = Factory::Instance->getFrameInTxtByPoseId(id);
    head->RedefineTextureCoords(r.x, r.y, r.w, r.h);
}
void poseStatBody(Image* body, double progress){
    //int index = getIndexByProgress(progress, 34);
    int id = 100;
    if(id == body->poseID) return; // same pose
    const double w = 104;
    const double h = 132;
    body->poseID = id;
    int xTxt = 230;
    int yTxt = 650;
    body->RedefineTextureCoords(xTxt, yTxt, w, h);
}
void poseHeadStung(Image* head, double progress){
    int id = 699;
    head->Scale(1 + 0.5 * sin(20 * progress));
    if(id == head->poseID) return;
    head->poseID = id;
    head->RedefineTextureCoords(541, 386, 145, 189);
    
}
void poseBodyStung(Image* body, double progress){
    int id = 799;
    body->Scale(1 + 0.5 * sin(20 * progress));
    if(id == body->poseID) return;
    body->poseID = id;
    body->RedefineTextureCoords(430, 582, 211, 129);
}
static int mapHeadKungFu01[] = {
    50, 201, 201, 202, 202,
    203, 203, 204, 204, 204,
    204, 204, 204, 204, 204,
    204, 204, 204, 204, 204,
    205, 205, 206, 207, 207,
    207, 207, 207, 207, 207,
    207, 207, 207, 207, 207,
    207, 207, 207, 207, 207,
    207, 207, 207, 207, 207,
    208, 209, 210, 50, 50,
    50
};
void poseHeadKungFu01(Image* head, double progress){
    int index = getIndexByProgress(progress, 51);
    int id = mapHeadKungFu01[index];
    if(id == head->poseID) return;
    head->poseID = id;
    
    if(id == 50 && head->Texture()->ID != 3) head->Texture(Factory::Instance->TextureAtlas03);
    else if(id != 50 && head->Texture()->ID != 12) head->Texture(Factory::Instance->TextureAtlas12);
   
    rect r = Factory::Instance->getFrameInTxtByPoseId(id);
    head->RedefineTextureCoords(r.x, r.y, r.w, r.h);
}
static int mapBodyKungFu01[] = {
    100, 120, 121, 122, 123,
    124, 125, 126, 126, 126,
    125, 125, 125, 125, 125,
    124, 124, 124, 124, 124,
    125, 125, 125, 125, 125,
    126, 126, 126, 126, 126,
    125, 125, 125, 125, 125,
    124, 124, 124, 124, 124,
    125, 125, 125, 125, 125,
    126, 127, 128, 129, 130,
    100
};
void poseBodyKungFu01(Image* body, double progress){
    int index = getIndexByProgress(progress, 51);
    int id = mapBodyKungFu01[index];
    
    if(id == 100 && body->Texture()->ID != 3)  body->Texture(Factory::Instance->TextureAtlas03);
    
    if(id == body->poseID) return;
    body->poseID = id;
    if(id > 100 && body->Texture()->ID != 12) body->Texture(Factory::Instance->TextureAtlas12);
    rect r = Factory::Instance->getFrameInTxtByPoseId(id);
    body->RedefineTextureCoords(r.x, r.y, r.w, r.h);
    
}
static int mapHeadKungFu02[] = {
    50, 210, 209, 208, 211,
    211, 211, 211, 211, 211,
    211, 211, 211, 211, 211,
    211, 211, 211, 211, 211,
    211, 211, 211, 211, 211,
    211, 211, 211, 211, 211,
    211, 211, 211, 211, 211,
    212, 212, 213, 213, 214,
    214, 214, 214, 214, 214,
    214, 214, 214, 214, 214,
    214, 214, 214, 214, 214,
    214, 214, 214, 214, 214,
    215, 216, 217, 218, 218,
    218, 218, 218, 218, 218,
    50
};
void poseHeadKungFu02(Image* head, double progress){
    int index = getIndexByProgress(progress, 71);
    int id = mapHeadKungFu02[index];
    if(id == head->poseID) return;
    head->poseID = id;
    
    if(id == 50 && head->Texture()->ID != 3) head->Texture(Factory::Instance->TextureAtlas03);
    else if(id != 50 && head->Texture()->ID != 12) head->Texture(Factory::Instance->TextureAtlas12);
    
    rect r = Factory::Instance->getFrameInTxtByPoseId(id);
    head->RedefineTextureCoords(r.x, r.y, r.w, r.h);
}
static int mapBodyKungFu02[] = {
    100, 131, 131, 132, 132,
    133, 133, 134, 134, 134,
    135, 135, 135, 135, 135,
    136, 136, 136, 136, 136,
    137, 137, 137, 137, 137,
    136, 136, 136, 136, 136,
    135, 135, 135, 135, 135,
    136, 136, 136, 136, 136,
    137, 137, 137, 137, 137,
    136, 136, 136, 136, 136,
    135, 135, 135, 135, 135,
    136, 136, 136, 136, 136,
    137, 137, 137, 138, 138,
    139, 139, 140, 141, 142,
    100
};
void poseBodyKungFu02(Image* body, double progress){
    int index = getIndexByProgress(progress, 71);
    int id = mapBodyKungFu02[index];
    
    if(id == 100 && body->Texture()->ID != 3)  body->Texture(Factory::Instance->TextureAtlas03);
    
    if(id == body->poseID) return;
    body->poseID = id;
    if(id > 100 && body->Texture()->ID != 12) body->Texture(Factory::Instance->TextureAtlas12);
    rect r = Factory::Instance->getFrameInTxtByPoseId(id);
    body->RedefineTextureCoords(r.x, r.y, r.w, r.h);
}