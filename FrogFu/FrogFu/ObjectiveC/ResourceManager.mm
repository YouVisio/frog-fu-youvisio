#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import <cstring>
#import <iostream>
#import "IResourceManager.hpp"
#import "TextureDescription.hpp"
#import "../PowerVR/PVRTTexture.h"

using namespace std;

class ResourceManager : public IResourceManager {
public:
    ResourceManager()
    {
        m_imageData = 0;
    }
    string GetResourcePath() const
    {
        NSString* bundlePath =[[NSBundle mainBundle] resourcePath];
        return [bundlePath UTF8String];
    }
    TextureDescription LoadImage(const string& file)
    {
        NSString* basePath = [NSString stringWithUTF8String:file.c_str()];
        NSString* resourcePath = [[NSBundle mainBundle] resourcePath];
        NSString* fullPath = [resourcePath stringByAppendingPathComponent:basePath];
        UIImage* uiImage = [UIImage imageWithContentsOfFile:fullPath];
        
        TextureDescription description;
        //description.ID = textureID;
        description.Size.x = CGImageGetWidth(uiImage.CGImage);
        description.Size.y = CGImageGetHeight(uiImage.CGImage);
        description.BitsPerComponent = 8;
        description.Format = TextureFormatRgba;
        description.MipCount = 1;
        m_hasPvrHeader = false;
        
        int bpp = description.BitsPerComponent / 2;
        int byteCount = description.Size.x * description.Size.y * bpp;
        unsigned char* data = (unsigned char*) calloc(byteCount, 1);
        
        CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
        CGBitmapInfo bitmapInfo = kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big;
        CGContextRef context = CGBitmapContextCreate(data,
                                                     description.Size.x,
                                                     description.Size.y,
                                                     description.BitsPerComponent,
                                                     bpp * description.Size.x,
                                                     colorSpace,
                                                     bitmapInfo);
        CGColorSpaceRelease(colorSpace);
        CGRect rect = CGRectMake(0, 0, description.Size.x, description.Size.y);
        CGContextDrawImage(context, rect, uiImage.CGImage);
        CGContextRelease(context);
        
        m_imageData = [NSData dataWithBytesNoCopy:data length:byteCount freeWhenDone:YES];
        return description;
    }
    TextureDescription LoadPngImage(const string& file)
    {
        NSString* basePath = [NSString stringWithUTF8String:file.c_str()];
        NSString* resourcePath = [[NSBundle mainBundle] resourcePath];
        NSString* fullPath = [resourcePath stringByAppendingPathComponent:basePath];
        
        //NSLog(@"Loading PNG image %@...", fullPath);
        
        UIImage* uiImage = [UIImage imageWithContentsOfFile:fullPath];
        CGImageRef cgImage = uiImage.CGImage;
        
        CFDataRef dataRef = CGDataProviderCopyData(CGImageGetDataProvider(cgImage));
        m_imageData = [NSData dataWithData:(__bridge NSData*) dataRef];
        CFRelease(dataRef);
        
        m_hasPvrHeader = false;
        
        TextureDescription description;
        //description.ID = textureID;
        description.Size.x = CGImageGetWidth(cgImage);
        description.Size.y = CGImageGetHeight(cgImage);
        bool hasAlpha = CGImageGetAlphaInfo(cgImage) != kCGImageAlphaNone;
        CGColorSpaceRef colorSpace = CGImageGetColorSpace(cgImage);
        switch (CGColorSpaceGetModel(colorSpace)) {
            case kCGColorSpaceModelMonochrome:
                description.Format = hasAlpha ? TextureFormatGrayAlpha : TextureFormatGray;
                break;
            case kCGColorSpaceModelRGB:
                description.Format = hasAlpha ? TextureFormatRgba : TextureFormatRgb;
                break;
            default:
                assert(!"Unsupported color space.");
                break;
        }
        description.BitsPerComponent = CGImageGetBitsPerComponent(cgImage);
        
        return description;
    }
    TextureDescription LoadPngImage2(const string& file){
        NSString* basePath = [NSString stringWithUTF8String:file.c_str()];
        NSString* resourcePath = [[NSBundle mainBundle] resourcePath];
        NSString* fullPath = [resourcePath stringByAppendingPathComponent:basePath];
        
        
        CGImageRef spriteImage = [UIImage imageWithContentsOfFile:fullPath].CGImage;
        size_t width = CGImageGetWidth(spriteImage);
        size_t height = CGImageGetHeight(spriteImage);
        uint wpot = 1;
        uint hpot = 1;
        while(wpot<width) wpot*=2;
        while(hpot<height)hpot*=2;
        GLubyte * spriteData = (GLubyte *) calloc(wpot*hpot*4, sizeof(GLubyte));
        size_t bitsPerComponent = CGImageGetBitsPerComponent(spriteImage);
        CGContextRef spriteContext = CGBitmapContextCreate(spriteData, wpot, hpot, bitsPerComponent, wpot*4, CGImageGetColorSpace(spriteImage), kCGImageAlphaPremultipliedLast);
        CGContextDrawImage(spriteContext, CGRectMake(0, 0, wpot, hpot), spriteImage);
        CGContextRelease(spriteContext);
        GLuint texName;
        glGenTextures(1, &texName);
        
        glBindTexture(GL_TEXTURE_2D, texName);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, wpot, hpot, 0, GL_RGBA, GL_UNSIGNED_BYTE, spriteData);
        free(spriteData);
        
        TextureDescription description;
        description.Name = texName;
        //description.ID = textureID;
        description.Size.x = width;
        description.Size.y = height;
        
        description.BitsPerComponent = bitsPerComponent;
        
        return description;
    }
    TextureDescription LoadPngImage3(const string& file){
        NSString* basePath = [NSString stringWithUTF8String:file.c_str()];
        NSString* resourcePath = [[NSBundle mainBundle] resourcePath];
        NSString* fullPath = [resourcePath stringByAppendingPathComponent:basePath];
        
        
        glEnable(GL_TEXTURE_2D);
        //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT_OES);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT_OES);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glBlendFunc(GL_ONE, GL_SRC_COLOR);
        

        NSString *extension = [fullPath pathExtension];
        NSString *baseFilenameWithExtension = [fullPath lastPathComponent];
        NSString *baseFilename = [baseFilenameWithExtension substringToIndex:[baseFilenameWithExtension length] - [extension length] - 1];
        
        NSString *path = [[NSBundle mainBundle] pathForResource:baseFilename ofType:extension];
        NSData *texData = [[NSData alloc] initWithContentsOfFile:path];
        
    
        UIImage *image = [[UIImage alloc] initWithData:texData];
        
        GLuint width = CGImageGetWidth(image.CGImage);
        GLuint height = CGImageGetHeight(image.CGImage);
        CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
        void *imageData = malloc( height * width * 4 );
        CGContextRef context = CGBitmapContextCreate( imageData, width, height, 8, 4 * width, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big );
        CGColorSpaceRelease( colorSpace );
        CGContextClearRect( context, CGRectMake( 0, 0, width, height ) );
        CGContextDrawImage( context, CGRectMake( 0, 0, width, height ), image.CGImage );
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        //			glGenerateMipmapEXT(GL_TEXTURE_2D);  //Generate mipmaps now!!!
        //GLuint errorcode = glGetError();
        CGContextRelease(context);
        
        free(imageData);
      
        
        TextureDescription description;
        description.Name = texture;
        //description.ID = textureID;
        description.Size.x = width;
        description.Size.y = height;
        
        description.BitsPerComponent = CGImageGetBitsPerComponent(image.CGImage);
        
        return description;
    }
    TextureDescription LoadPvrImage(const string& file)
    {
        NSString* basePath = [NSString stringWithUTF8String:file.c_str()];
        NSString* resourcePath = [[NSBundle mainBundle] resourcePath];
        NSString* fullPath = [resourcePath stringByAppendingPathComponent:basePath];
        
        m_imageData = [NSData dataWithContentsOfFile:fullPath];
        m_hasPvrHeader = true;
        PVR_Texture_Header* header = (PVR_Texture_Header*) [m_imageData bytes];
        bool hasAlpha = header->dwAlphaBitMask ? true : false;
        
        TextureDescription description;
        switch (header->dwpfFlags & PVRTEX_PIXELTYPE) {
            case OGL_RGB_565:
                description.Format = TextureFormat565;
                break;
            case OGL_RGBA_5551:
                description.Format = TextureFormat5551;
                break;
            case OGL_RGBA_4444:
                description.Format = TextureFormatRgba;
                description.BitsPerComponent = 4;
                break;
            case OGL_PVRTC2:
                description.Format = hasAlpha ? TextureFormatPvrtcRgba2 :
                TextureFormatPvrtcRgb2;
                break;
            case OGL_PVRTC4:
                description.Format = hasAlpha ? TextureFormatPvrtcRgba4 :
                TextureFormatPvrtcRgb4;
                break;
            default:
                assert(!"Unsupported PVR image.");
                break;
        }
        //description.ID = textureID;
        description.Size.x = header->dwWidth;
        description.Size.y = header->dwHeight;
        description.MipCount = header->dwMipMapCount;
        return description;
    }
    void* GetImageData()
    {
        if (!m_hasPvrHeader)
            return (void*) [m_imageData bytes];
        
        PVR_Texture_Header* header = (PVR_Texture_Header*) [m_imageData bytes];
        char* data = (char*) [m_imageData bytes];
        unsigned int headerSize = header->dwHeaderSize;
        return data + headerSize;
    }
    void UnloadImage()
    {
        m_imageData = 0;
    }
private:
    NSData* m_imageData;
    bool m_hasPvrHeader;
};

IResourceManager* CreateResourceManager()
{
    return new ResourceManager();
}
