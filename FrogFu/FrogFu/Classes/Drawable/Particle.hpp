//
//  Particle.h
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-13.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_Particle_hpp
#define FrogFu_Particle_hpp

#import <iostream>
#import "Vector.hpp"

using namespace std;

class Particle {
public:
    Particle(){ }
    Particle(vec2 pos, vec4 color, float age){
        _pos = pos;
        _color = color;
        _age = age;
    }
    
    vec2 Pos(){ return _pos; }
    Particle* Pos(vec2 pos){ _pos = pos; return this; }
    vec4 Color(){ return _color; }
    Particle* Color(vec4 color){ _color = color; return this; }
    float Age(){ return _age; }
    Particle* Age(float age){ _age = age; return this; }
    
private:
    vec2 _pos;
    vec4 _color;
    float _age;
};

struct ParticleNode{
public:
    Particle particle;
	ParticleNode* next;
    ParticleNode* prev;
	ParticleNode(Particle p){
        //_id = ++ParticleNode::_lastId;
        //if(_id > 2000000000) ParticleNode::_lastId = 0;
		particle = p;
		next = NULL;
        prev = NULL;
    }
    //long ID() { return _id; }
private:
    //long _id;
    //static long _lastId;
};


class ParticleList{
    
public:
	ParticleList(){
		head = NULL;
        tail = NULL;
        _count = 0;
    }
    ParticleNode *head;
    ParticleNode *tail;
    void appendLast(Particle p){
        
//cout << "ADD:" << pthread_self() << endl;
        
        ParticleNode* node = new ParticleNode(p);
        if(head == NULL || tail == NULL){
            head = tail = node;
            _count = 1;
        }
        else{
            tail->next = node;
            node->prev = tail;
            tail = node;
            ++_count;
        }
    }
    ParticleNode* removeAndGetNext(ParticleNode* node){
        
//cout << "REM:" << pthread_self() << endl;
        
        if(head == NULL) return NULL;
        
        --_count;
        
        ParticleNode* prev = node->prev;
        ParticleNode* next = node->next;
        
        if(prev == NULL){
            head = next;
            if(head != NULL) head->prev = NULL;
        } else {
            prev->next = next;
        }
        if(next == NULL){
            tail = prev;
            if(tail != NULL) tail->next = NULL;
        } else {
            next->prev = prev;
        }
        
        
        node->prev = NULL;
        node->next = NULL;
        delete node;

        return next;
    }
    void clear() {
        if(_count == 0) return;
        
        ParticleNode* curr = head;
        while (curr != NULL) {
            
            ParticleNode* next = curr->next;

            curr->next = curr->prev = NULL;
            
            delete curr;
            
            //--_count;
            
            curr = next;
        }
        head = NULL;
        tail = NULL;
        
        _count = 0;
    }
    long size() { return _count; }
private:
    long _count;
    
};

#endif /* defined(__FrogFu__Particle__) */
