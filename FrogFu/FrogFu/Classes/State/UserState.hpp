//
//  UserState.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-10-06.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_UserState_hpp
#define FrogFu_UserState_hpp

#import <iostream>
#import <map>
#import <fstream>
#import <sstream>

using namespace std;


class UserState {
public:
    float a;
    float b;
    float c;
    float d;
    float e;
    float f;
    float g;
    float h;
    float i;
    float j;
    float k;
    float l;
    float m;
    float n;
    long o;
    long p;
    long q;
    float r;
    float s;
    float t;
    float u;
    float v;
    long w;
    float x;
    float y;
    float z;
    
    UserState(const char* path){
        _path = path;
        a = 0; b = 0; c = 0; d = 0; e = 0; f = 0; g = 0; h = 0; i = 0; j = 0; k = 0; l = 0; m = 0; n = 0; o = 0; p = 0; q = 0; r = 0; s = 0; t = 0; u = 0; v = 0; w = 0; x = 0; y = 0; z = 0; 
    }
    const char* Path(){ return _path; }
    void Serialize(){
        a = sysRandomOfPeriodAsInt(0,10000);
        e = sysRandomOfPeriodAsInt(0,10000);
        g = sysRandomOfPeriodAsInt(0,10000);
        j = sysRandomOfPeriodAsInt(0,10000);
        r = sysRandomOfPeriodAsInt(0,10000);
        u = sysRandomOfPeriodAsInt(0,10000);
        v = sysRandomOfPeriodAsInt(0,10000);
        
        d *=  8.0f;
        f *=  4.0f;
        k *= 17.0f;
        n *= 33.0f;
        s *= 14.0f;
        t *= 21.0f;
        x *= 24.0f;
        y *= 24.0f;
        z *= 24.0f;
        
        ofstream outfile;
        outfile.open(_path);
        
        stringstream ss;
        ss << "a^";ss << a; ss << endl;
        ss << "b^";ss << b; ss << endl;
        ss << "c^";ss << c; ss << endl;
        ss << "d^";ss << d; ss << endl;
        ss << "e^";ss << e; ss << endl;
        ss << "f^";ss << f; ss << endl;
        ss << "g^";ss << g; ss << endl;
        ss << "h^";ss << h; ss << endl;
        ss << "i^";ss << i; ss << endl;
        ss << "j^";ss << j; ss << endl;
        ss << "k^";ss << k; ss << endl;
        ss << "l^";ss << l; ss << endl;
        ss << "m^";ss << m; ss << endl;
        ss << "n^";ss << n; ss << endl;
        ss << "o^";ss << o; ss << endl;
        ss << "p^";ss << p; ss << endl;
        ss << "q^";ss << q; ss << endl;
        ss << "r^";ss << r; ss << endl;
        ss << "s^";ss << s; ss << endl;
        ss << "t^";ss << t; ss << endl;
        ss << "u^";ss << u; ss << endl;
        ss << "v^";ss << v; ss << endl;
        ss << "w^";ss << w; ss << endl;
        ss << "x^";ss << x; ss << endl;
        ss << "y^";ss << y; ss << endl;
        ss << "z^";ss << z; ss << endl;
        
        outfile << ss.str();
        
        outfile.close();
        
        d /=  8.0f;
        f /=  4.0f;
        k /= 17.0f;
        n /= 33.0f;
        s /= 14.0f;
        t /= 21.0f;
        x /= 24.0f;
        y /= 24.0f;
        z /= 24.0f;
    }
    void Deserialize(){
        ifstream infile;
        infile.open(_path);
        
        string strLine;
        char* line;
        if(infile){
            while (infile.good()) {
                getline(infile, strLine);
                line = (char*)strLine.c_str();
                char* key = strtok(line, "^");
                if (key != NULL) {
                    char* value = strtok(NULL, "^");
                    if (value != NULL) {
                        if(strcmp(key, "a") == 0) a = atof(value);
                        else if(strcmp(key, "b") == 0) b = atof(value);
                        else if(strcmp(key, "c") == 0) c = atof(value);
                        else if(strcmp(key, "d") == 0) d = atof(value);
                        else if(strcmp(key, "e") == 0) e = atof(value);
                        else if(strcmp(key, "f") == 0) f = atof(value);
                        else if(strcmp(key, "g") == 0) g = atof(value);
                        else if(strcmp(key, "h") == 0) h = atof(value);
                        else if(strcmp(key, "i") == 0) i = atof(value);
                        else if(strcmp(key, "j") == 0) j = atof(value);
                        else if(strcmp(key, "k") == 0) k = atof(value);
                        else if(strcmp(key, "l") == 0) l = atof(value);
                        else if(strcmp(key, "m") == 0) m = atof(value);
                        else if(strcmp(key, "n") == 0) n = atof(value);
                        else if(strcmp(key, "o") == 0) o = atoll(value);
                        else if(strcmp(key, "p") == 0) p = atoll(value);
                        else if(strcmp(key, "q") == 0) q = atoll(value);
                        else if(strcmp(key, "r") == 0) r = atof(value);
                        else if(strcmp(key, "s") == 0) s = atof(value);
                        else if(strcmp(key, "t") == 0) t = atof(value);
                        else if(strcmp(key, "u") == 0) u = atof(value);
                        else if(strcmp(key, "v") == 0) v = atof(value);
                        else if(strcmp(key, "w") == 0) w = atoll(value);
                        else if(strcmp(key, "x") == 0) x = atof(value);
                        else if(strcmp(key, "y") == 0) y = atof(value);
                        else if(strcmp(key, "z") == 0) z = atof(value);
                    }
                }
            }
        }else{
            cout << "Cannot open file" << endl;
        }
        infile.close();
        
        d /=  8.0f;
        f /=  4.0f;
        k /= 17.0f;
        n /= 33.0f;
        s /= 14.0f;
        t /= 21.0f;
        x /= 24.0f;
        y /= 24.0f;
        z /= 24.0f;
    }
    float ClassicS(){ return d; }
    float ClassicP(){ return k; }
    float ClassicNum(){ return x; }
    void ClassicScene(double score, double perc){
        // real
        if(score > d) d = score;
        if(perc > k) k = perc;
        // fakes
        if(score > h) h = score;
        if(perc > i) i = perc;
        // number times finish
        ++x;
        // last time play
        o = time(NULL);
        
        Serialize();
    }
    float EasyPlayS(){ return n; }
    float EasyPlayP(){ return f; }
    float EasyPlayNum(){ return y; }
    void EasyPlayScene(double score, double perc){
        // real
        if(score > n) n = score;
        if(perc > f) f = perc;
        // fakes
        if(score > l) l = score;
        if(perc > m) m = perc;
        // number times finish
        ++y;
        // last time play
        p = time(NULL);
        
        Serialize();
    }
    float SprintS(){ return s; }
    float SprintP(){ return t; }
    float SprintNum(){ return z; }
    void SprintScene(double score, double perc){
        // real
        if(score > s) s = score;
        if(perc > t) t = perc;
        // fakes
        if(score > b) b = score;
        if(perc > c) c = perc;
        // number times finish
        ++z;
        // last time play
        q = time(NULL);
        
        Serialize();
    }
private:
    bool parseBool(const char* value){
        return ((strlen(value) == 1 && strncmp(value, "1", 1) == 0) || strncmp(value, "true", 4) == 0);
    }
    const char* _path;
};

#endif
