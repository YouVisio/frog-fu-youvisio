//
//  BaseManager.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-05.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_BaseManager_hpp
#define FrogFu_BaseManager_hpp

#import "Ticker.hpp"

class BaseManager{
protected:
    template<class T>
    Ticker<T>* HandleTick(Ticker<T>* ticker, double time){
        if(ticker != NULL){
            if(ticker->Duration() == 0){   
                while(ticker != NULL && ticker->Duration() == 0){
                    ticker->Tick(time);
                    Ticker<T>* old = ticker;
                    Ticker<T>* next = ticker->Next();
                    delete old;
                    ticker = next;
                }
            }else{
                ticker->Tick(time);
                if(ticker->Progress() >= 1.0){
                    Ticker<T>* next = ticker->Next();
                    delete ticker;
                    if(next != NULL){
                        next->Tick(time);
                    }
                    return next;
                }
            }
        }
        return ticker;
    }
};


#endif
