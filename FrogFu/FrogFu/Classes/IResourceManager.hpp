//
//  IResourceManager.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-11.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_IResourceManager_hpp
#define FrogFu_IResourceManager_hpp

#import <cstring>
#import "TextureDescription.hpp"

using namespace std;

struct IResourceManager {
    virtual string GetResourcePath() const = 0;
    virtual TextureDescription LoadPngImage(const string& filename) = 0;
    virtual TextureDescription LoadPngImage2(const string& filename) = 0;
    virtual TextureDescription LoadPngImage3(const string& filename) = 0;
    virtual TextureDescription LoadPvrImage(const string& filename) = 0;
    virtual TextureDescription LoadImage(const string& filename) = 0;
    virtual void* GetImageData() = 0;
    virtual void UnloadImage() = 0;
    virtual ~IResourceManager() {}
};

IResourceManager* CreateResourceManager();

#endif
