//
//  Interfaces.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-04.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_Interfaces_hpp
#define FrogFu_Interfaces_hpp

#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
//#import <vector>
#import <map>
#import <cstring>
#import <cmath>
#import "Vector.hpp"
#import "Rectangle.hpp"
#import <pthread.h>
using namespace std;

#define ISDEBUG 0

//#define HALF_SCREEN_WIDTH 320
//#define HALF_SCREEN_HEIGHT 480
//#define SCREEN_WIDTH 640
//#define SCREEN_HEIGHT 960

#define MAX_SPRING 300

#define Y_SHIFT_BG_FRONT 1400
#define Y_SHIFT_BG_MAIN 1050
#define Y_SHIFT_BG_NEAR_BG 920
#define Y_SHIFT_BG_FAR_BG 800
#define Y_SHIFT_BG_BACK 700
#define MAP_ZOOM 0.114
#define MAP_X -272
#define MAP_Y 200
#define BILLION 1000000000
#define DTR 0.0174532925199433
#define RTD 57.2957795130823
#define MAX_TONGUE_LENGTH 335
#define TONGUE_COMMON 0
#define TONGUE_IRON 1
#define END_OF_GAME_WASP 1
#define END_OF_GAME_MISSED 2
#define END_OF_GAME_TIME 3
#define SCENE_START 1
#define SCENE_CLASSIC 2
#define SCENE_SPRINT 3
#define SCENE_EASY_PLAY 4

#define MARK_BACKGROUND 777777


double ensureRange(double value, double min, double max);
int getFreeMemory();

void sysSendEmail();
void sysGoToUrl(const char* url);

void sysLoadSound(const char* name, const char* ext, bool loop, bool isMusic);
bool sysIsPlayingSound(const char* name);
void sysPlaySound(const char* name);
void sysStopSound(const char* name);
void sysRewindSound(const char* name);
void sysSoundVolume(const char* name, float volume);

void showRatingAlert();

void analyticsEvent(const char* action, const char* label, long value);
void analyticsScreen(const char* screen);

void submitScore(long score, const char* leaderboard);
void submitAchievement(const char* achievement);
void showLeaderboards();
void showAchievements();

void postToTwitter(long score, int type);
void postToFacebook(long score, int type);
void postToGooglePlus(long score, int type);


double sysRandom();
double sysRandomOfPeriodAsDouble(double min, double max);
int sysRandomOfPeriodAsInt(int min, int max);
void backgroundWorker(void (*work)(void*), void(*done)(void*), void* caller);

template <typename T>
int sign(T val) {
    return (T(0) < val) - (val < T(0));
}


template <typename T>
struct simnode{
public:
    T* data;
	simnode<T>* next;
    simnode<T>* prev;
	simnode(T* d){
		data = d;
		next = NULL;
        prev = NULL;
    }
};

template <typename T>
class simlist{
    
public:
	simlist(){
		head = NULL;
        tail = NULL;
        _count = 0;
        //simlist_mutex = PTHREAD_MUTEX_INITIALIZER;
    }
    //pthread_mutex_t simlist_mutex;
    simnode<T> *head;
    simnode<T> *tail;
    void push_front(T* d){
        
        //cout << "ADD:" << pthread_self() << endl;
        
        //pthread_mutex_lock (&simlist_mutex);
        
        simnode<T>* node = new simnode<T>(d);
        if(head == NULL || tail == NULL){
            head = tail = node;
            _count = 1;
        }
        else{
            simnode<T>* oldHead = head;
            head = node;
            node->next = oldHead;
            oldHead->prev = node;
            ++_count;
        }
        
        //pthread_mutex_unlock (&simlist_mutex);
    }
    void push_back(T* d){
        
        //cout << "ADD:" << pthread_self() << endl;
        
        //pthread_mutex_lock (&simlist_mutex);
        
        simnode<T>* node = new simnode<T>(d);
        if(head == NULL || tail == NULL){
            head = node;
            tail = node;
            _count = 1;
        }
        else{
            simnode<T>* oldTail = tail;
            tail = node;
            oldTail->next = node;
            node->prev = oldTail;
            ++_count;
        }
        
        //pthread_mutex_unlock (&simlist_mutex);
    }
    bool push_after(T* d, bool (*condition)(T* current)){ 
        simnode<T>* curr = head;
        while (curr != NULL) {
            
            if(condition(curr->data)){
                simnode<T>* prevNext = curr->next;
                simnode<T>* newNode = new simnode<T>(d);
                curr->next = newNode;
                newNode->prev = curr;
                newNode->next = prevNext;
                if(prevNext != NULL){
                    prevNext->prev = newNode;
                } else {
                    tail = newNode;
                }
                ++_count;
                return true;
            }
            curr = curr->next;
        }
        return false;
    }
    void sort(int (*compare)(T* one, T* two)){
        quickSort(this, this->head, this->tail, compare);
    }
    void sort(int (*compare)(T* one, T* two), simnode<T>* from, simnode<T>* to){
        quickSort(this, from, to, compare);
    }
    simnode<T>* removeAndGetNext(simnode<T>* node, bool deleteData = false){
        
        //cout << "REM:" << pthread_self() << endl;
        
        if(head == NULL) return NULL;
        
        //pthread_mutex_unlock (&simlist_mutex);
        
        --_count;
        
        simnode<T>* prev = node->prev;
        simnode<T>* next = node->next;
        
        
        
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
        
        deleteNode(node, deleteData);
        
        
        //pthread_mutex_unlock (&simlist_mutex);
        
        return next;
    }
    void clear(bool deleteData = false) {
        if(_count == 0) return;
        
        //pthread_mutex_unlock (&simlist_mutex);
        
        simnode<T>* curr = head;
        while (curr != NULL) {
            
            simnode<T>* next = curr->next;
            
            deleteNode(curr, deleteData);
            
            curr = next;
        }
        head = NULL;
        tail = NULL;
        
        _count = 0;
        
        //pthread_mutex_unlock (&simlist_mutex);
    }
    long size() { return _count; }
private:
    long _count;
    int _listID;
    
    void deleteNode(simnode<T>* node, bool deleteData){
        if(deleteData && node->data != NULL){
            T* d = node->data;
            delete d;
        }
        
        node->data = NULL;
        node->next = NULL;
        node->prev = NULL;
        delete node;
    }
    
    static void quickSort(simlist<T>* list, simnode<T>* left, simnode<T>* right, int (*compare)(T* one, T* two)){
        simnode<T>* start;
        simnode<T>* current;
        
        
        // If the left and right pointers are the same, then return
        if (left == right) return;
        
        // Set the Start and the Current item pointers
        start = left;
        current = start->next;
        
        // Loop until we get to the right
        while (current != NULL)
        {
            // If the start item is less then the right
            if (compare(start->data, current->data) < 0)
            {
                // Swap the items
                swap(current, start);
            }
            
            // Check if we have reached the right end
            if (current == right) break;
            
            // Move to the next item in the list
            current = current->next;
        }
        
        // Swap the First and Current items
        swap(left, current);
        
        // Save this Current item
        simnode<T>* oldCurrent = current;
        
        // Check if we need to sort the left hand size of the Current point
        current = current->prev;
        if (current != NULL)
        {
            if ((left->prev != current) && (current->next != left))
                quickSort(list, left, current, compare);
        }
        
        // Check if we need to sort the right hand size of the Current point
        current = oldCurrent;
        current = current->next;
        if (current != NULL)
        {
            if ((current->prev != right) && (right->next != current))
                quickSort(list, current, right, compare);
        }
    }
    static void swap(simnode<T>* one, simnode<T>* two){
        T* temp = one->data;
        one->data = two->data;
        two->data = temp;
    }
};




struct Range {
public:
    Range(){
        From = 0.0;
        To = 0.0;
    }
    Range(double from, double to){
        From = from;
        To = to;
    }
    void Set(double from, double to){
        From = from;
        To = to;
    }
    double From;
    double To;
    double Diff(){
        return To - From;
    }
};


struct TouchInfo {
public:
    TouchInfo(){
        Id = 0;
        Touched = 0;
        Hit = 0;
        Missed = 0;
    }
    long Id;
    int Touched;
    int Hit;
    int Missed;
};

class BgWorkerState{
public:
    void (*Work)(void*);
    void (*Done)(void*);
    void* Caller;
};


template <typename T>
class FuncAndState{
public:
    FuncAndState(T (*func)(void* caller), void* caller){
        Func = func;
        Caller = caller;
    }
    ~FuncAndState(){
    
    }
    T (*Func)(void* caller);
    void* Caller;
    T Call(){
        return Func(Caller);
    }
};

class ActionAndState{
public:
    ActionAndState(void (*action)(void* caller, long id), void* caller){
        Action = action;
        Caller = caller;
    }
    ~ActionAndState(){
        
    }
    void (*Action)(void* caller, long id);
    void* Caller;
    void Call(long id){
        Action(Caller, id);
    }
};


inline double computeJumpLengthByStrength(double strength){
    return 1.5 + 2.5 * strength;
}
inline double sysMax(double a, double b)
{
    if(a > b) return a;
    return b;
}
inline double sysMin(double a, double b)
{
    if(a < b) return a;
    return b;
}
inline double sysMod(double a, double b)
{
    int result = static_cast<int>( a / b );
    return a - static_cast<double>( result ) * b;
}
inline double round(double n, unsigned places){
    if(places == 0){
        return n < 0.0 ? ceil(n - 0.5) : floor(n + 0.5);
    }
	double off=pow(10.0,(double)places);
    n = n * off;
    n = floor(n);
	return n/off;
}
inline const char* concArr(const char* s1, const char* s2){
    int lenOne = strlen(s1);
    int lenTwo = strlen(s2);
    char* result = new char[lenOne + lenTwo];
    strcpy(result,s1);
    strcat(result,s2);
    return result;
}
struct Framebuffers {
    GLuint Resolve;
    GLuint Msaa;
};
struct Renderbuffers {
    GLuint ResolveColor;
    GLuint MsaaColor;
    GLuint Depth;
};
struct FrogImage {
    FrogImage() :
        pos(0,0),
        rotation(NAN){}
    FrogImage(double x, double y) :
        pos(x,y),
        rotation(NAN){}
    FrogImage(double x, double y, double a) :
        pos(x,y),
        rotation(a){}
    
    vec2 pos;
    double rotation;
};

class SoundInfo{
public:
    SoundInfo(){
        name = NULL;
        ext = NULL;
        loop = false;
        volume = 1.0f;
        isPlaying = false;
        isMusic = false;
    }
    
    const char* name;
    const char* ext;
    bool loop;
    float volume;
    bool isPlaying;
    bool isMusic;
};
#endif
