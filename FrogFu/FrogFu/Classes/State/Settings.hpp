//
//  Settings.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-12-24.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_Settings_hpp
#define FrogFu_Settings_hpp

#import <iostream>
#import <map>
#import <fstream>
#import <sstream>

using namespace std;


class Settings {
public:
    bool music;
    bool sounds;
    bool warnToRate;

    
    Settings(const char* path){
        _path = path;
        music = true;
        sounds = true;
        warnToRate = true;
    }
    const char* Path(){ return _path; }
    void Serialize(){
        
        ofstream outfile;
        outfile.open(_path);
        
        stringstream ss;
        ss << "m^";ss << music; ss << endl;
        ss << "s^";ss << sounds; ss << endl;
        ss << "w^";ss << warnToRate; ss << endl;
        
        outfile << ss.str();
        
        outfile.close();
        
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
                        if(strcmp(key, "m") == 0) music = parseBool(value);
                        else if(strcmp(key, "s") == 0) sounds = parseBool(value);
                        else if(strcmp(key, "w") == 0) warnToRate = parseBool(value);
                    }
                }
            }
        }else{
            cout << "Cannot open file" << endl;
        }
        infile.close();
    }
    bool Music(){ return music; }
    void Music(bool m){
        music = m;
        
        Serialize();
    }
    bool Sounds(){ return sounds; }
    void Sounds(bool s){
        sounds = s;
        
        Serialize();
    }
    bool WarnToRate(){ return warnToRate; }
    void WarnToRate(bool w){
        warnToRate = w;
        
        Serialize();
    }
private:
    bool parseBool(const char* value){
        return ((strlen(value) == 1 && strncmp(value, "1", 1) == 0) || strncmp(value, "true", 4) == 0);
    }
    const char* _path;
};

#endif
