//
//  RenderingEngine.cpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-04.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#import <iostream>
#import "Interfaces.hpp"
#import <assert.h>
#import <map>
#import "Factory.hpp"
#import "Matrix.hpp"
#import "Particle.hpp"
#import "IAnimation.hpp"
#import "Image.hpp"
#import "IRenderingEngine.hpp"
#import "../Textures/ParticleTexture.h"
#define STRINGIFY(A)  #A
#import "../Shaders/VertexShader.vert"
#import "../Shaders/FragmentShader.frag"

using namespace std;

struct PVRTextureHeader {
    unsigned int HeaderSize;
    unsigned int Height;
    unsigned int Width;
    unsigned int MipMapCount;
    unsigned int Flags;
    unsigned int TextureDataSize;
    unsigned int BitCount;
    unsigned int RBitMask;
    unsigned int GBitMask;
    unsigned int BBitMask;
    unsigned int AlphaBitMask;
    unsigned int PVR;
    unsigned int NumSurfs;
};

enum PVRPixelType {
    OGL_RGBA_4444 = 0x10,
    OGL_RGBA_5551,
    OGL_RGBA_8888,
    OGL_RGB_565,
    OGL_RGB_555,
    OGL_RGB_888,
    OGL_I_8,
    OGL_AI_88,
    OGL_PVRTC2,
    OGL_PVRTC4,
    PVRTEX_PIXELTYPE = 0xff,
};

struct UniformHandles {
    GLuint Modelview;
    GLuint Projection;
    GLuint Sampler;
    GLuint IsParticle;
    GLuint HasColorChange;
    GLuint PointSize;
};

struct AttributeHandles {
    GLint Position;
    GLint TextureCoord;
    GLint Color;
};

using namespace std;

class RenderingEngine : public IRenderingEngine {
public:
    RenderingEngine(IResourceManager* resources);
    void Initialize(int width, int height);
    void Render();
    void Test();
    simlist<Drawable>* GetDrawables();
    ParticleList* GetTouchTrails();
    simlist<Particle>* GetMapDots();
    simlist<IAnimation>* GetAnimations();
    void LoadTextureById(int id);
    void UnloadTextureById(int id);
    void UpdateTxt(int id, TextureDescription* target, TextureDescription source);
    void ClearExpired();
    void ClearCurrentTexture();
    virtual void SetDefaultBlendFunc(GLenum sfactor, GLenum dfactor);
    virtual void applicationWillResignActive();
    virtual void applicationDidEnterBackground();
    virtual void applicationWillEnterForeground();
    virtual void applicationDidBecomeActive();
    virtual void applicationWillTerminate();
    
private:
    bool _isMultisamplingEnabled;
    GLenum _defaultSfactor;
    GLenum _defaultDfactor;
    GLenum _currentSfactor;
    GLenum _currentDfactor;
    bool _isDefaultChanged;
    Factory* _factory;
    TextureDescription* _currentTexture;
    TextureDescription* _voidTexture;
    simlist<Drawable>* _drawables;
    ParticleList* _touchTrails;
    simlist<Particle>* _mapDots;
    simlist<IAnimation>* _animations;
    Renderbuffers _renderbuffers;
    Framebuffers _framebuffers;
    ivec2 _screenSize;
    AttributeHandles _attributes;
    UniformHandles _uniforms;
    IResourceManager* _resourceManager;
//    PointArrays* _imageArrays;
//    int _pointArraysLength;
    PointArrays* _touchTrailArrays;
    int _touchTrailArraysLength;
    PointArrays* _mapDotsArrays;
    int _mapDotsArraysLength;
    long _frame;
    map<GLuint, PointArrays*>* _imageArraysByTexture;
    bool _isEnteredBackground;
    
    PointArrays* GetPointArraysByTextureID(GLuint textureID, int numVars);
    void DrawImagesForTexture(simnode<Drawable>* from, simnode<Drawable>* to, int numSquares);
    PointArrays* CreatePointArrays(int numElements);
    PointArrays* CreateParticlePointArrays(int numElements);
    GLuint CreateTexture(const unsigned long* data);
    GLuint BuildProgram(const char* vertexShaderSource, const char* fragmentShaderSource) const;
    GLuint BuildShader(const char* source, GLenum shaderType) const;
    TextureDescription SetPngTexture(const string& filename);
    TextureDescription SetPvrTexture(const string& filename);
    void DrawTouchTrail();
    void DrawMapDots();
    void DrawParticles(simlist<Particle>* particles, long numElements, PointArrays* arrays);
    void DrawParticles(ParticleList* particles, long numElements, PointArrays* arrays);
    void SetBlendFunc(GLenum sfactor, GLenum dfactor);
};
IRenderingEngine* CreateRenderingEngine(IResourceManager* resources)
{
    return new RenderingEngine(resources);
}
RenderingEngine::RenderingEngine(IResourceManager* resources)
{
    _frame = 0;
    
    _resourceManager = resources;
 
    _isEnteredBackground = false;
    _isMultisamplingEnabled = false;
    _defaultSfactor = GL_ONE;
    _defaultDfactor = GL_ONE_MINUS_SRC_ALPHA;

//    _defaultSfactor = GL_SRC_ALPHA;
//    _defaultDfactor = GL_ONE_MINUS_SRC_ALPHA;
    
    _voidTexture = new TextureDescription();
    
    _currentTexture = _voidTexture;
    _currentTexture->Name = -1;
    
    _isDefaultChanged = false;
    
    _imageArraysByTexture = new map<GLuint, PointArrays*>();
    
    _touchTrailArraysLength = -1;
    _mapDotsArraysLength = -1;
    _touchTrailArrays = NULL;
    _mapDotsArrays = NULL;
    
    _drawables = new simlist<Drawable>();
    _touchTrails = new ParticleList();
    _mapDots = new simlist<Particle>();
    _animations = new simlist<IAnimation>();

    
    // Create the on-screen FBO.
    glGenFramebuffers(1, &_framebuffers.Resolve);
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffers.Resolve);
    
    
    glGenRenderbuffers(1, &_renderbuffers.ResolveColor);
    glBindRenderbuffer(GL_RENDERBUFFER, _renderbuffers.ResolveColor);
    
}
void RenderingEngine::Initialize(int width, int height)
{
    _isEnteredBackground = false;
    // load textures
    _factory = Factory::Instance;  
    LoadTextureById(1);
//    LoadTextureById(2);
    LoadTextureById(3);
    
//    LoadTextureById(4);
//    LoadTextureById(5);
    
    LoadTextureById(6);
    LoadTextureById(7);
    LoadTextureById(12);
    
    _factory->TextureParticle = new TextureDescription();
    _factory->TextureParticle->ID = 99999;
    _factory->TextureParticle->Name = CreateTexture(ParticleTexture);
//cout << "LoadTextureById: " << _factory->TextureParticle->ID << " GLid:" << _factory->TextureParticle->Name <<  endl;
    
    
    // Extract width and height from the color buffer.
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &_screenSize.x);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &_screenSize.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _renderbuffers.ResolveColor);
    
//    if(_isMultisamplingEnabled){
//        int numSamples = 4;
//        GLint maxSamplesAllowed;
//        glGetIntegerv(GL_MAX_SAMPLES_APPLE, &maxSamplesAllowed);
//        if(numSamples > maxSamplesAllowed) numSamples = maxSamplesAllowed;
//
//        glGenFramebuffers(1, &_framebuffers.Msaa);
//        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffers.Msaa);
//
//
//        glGenRenderbuffers(1, &_renderbuffers.MsaaColor);
//        glBindRenderbuffer(GL_RENDERBUFFER, _renderbuffers.MsaaColor);
//        glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, numSamples, GL_RGBA8_OES, _screenSize.x, _screenSize.y);
//        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _renderbuffers.MsaaColor);
//    }
    
    // Create the GLSL program.
    GLuint program = BuildProgram(VertexShader, FragmentShader);
    glUseProgram(program);
    
    // Extract the handles to attributes and uniforms.
    _attributes.Position = glGetAttribLocation(program, "Position");
    _attributes.TextureCoord = glGetAttribLocation(program, "TextureCoord");
    _attributes.Color = glGetAttribLocation(program, "Color");
    _uniforms.Projection = glGetUniformLocation(program, "Projection");
    _uniforms.Modelview = glGetUniformLocation(program, "Modelview");
    _uniforms.Sampler  = glGetUniformLocation(program, "Sampler");
    _uniforms.IsParticle = glGetUniformLocation(program, "IsParticle");
    _uniforms.PointSize = glGetUniformLocation(program, "PointSize");
    _uniforms.HasColorChange = glGetUniformLocation(program, "HasColorChange");
    
    // Set up various GL state.    
    glViewport(0, 0, _screenSize.x, _screenSize.y);
    //glViewport(0, 0, 1536, 2300);
    
    /*if(_screenSize.y == 568 || _screenSize.y == (568*2)){
        _factory->isFive = true;
        _factory->HALF_SCREEN_HEIGHT = 568;
        _factory->SCREEN_HEIGHT = 568 * 2;
    }*/
    
//cout << ">>" << _screenSize.x << "|" << _screenSize.y << endl;
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDepthFunc(GL_NEVER);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    
    //glUniform1f(_uniforms.PointSize, 12.0f);
    
    // Set up the transforms.
    mat4 projection = mat4::Ortho(_factory->SCREEN_WIDTH, _factory->SCREEN_HEIGHT, 0, 1);
    glUniformMatrix4fv(_uniforms.Projection, 1, 0, projection.Pointer());
    
    vec3 eye(_factory->HALF_SCREEN_WIDTH, _factory->HALF_SCREEN_HEIGHT, 1);
    vec3 target(_factory->HALF_SCREEN_WIDTH, _factory->HALF_SCREEN_HEIGHT, 0);
    vec3 up(0, 1, 0);
    mat4 modelview = mat4::LookAt(eye, target, up);
    glUniformMatrix4fv(_uniforms.Modelview, 1, 0, modelview.Pointer());
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "Failed to make complete framebuffer object " << glCheckFramebufferStatus(GL_FRAMEBUFFER);
    
}
void RenderingEngine::applicationWillResignActive(){ }
void RenderingEngine::applicationDidEnterBackground(){ _isEnteredBackground = true; }
void RenderingEngine::applicationWillEnterForeground(){ }
void RenderingEngine::applicationDidBecomeActive(){ _isEnteredBackground = false; }
void RenderingEngine::applicationWillTerminate(){ }
void RenderingEngine::ClearCurrentTexture(){ _currentTexture = _voidTexture; }
void RenderingEngine::UpdateTxt(int id, TextureDescription* target, TextureDescription source){
    target->Name = source.Name;
    target->MipCount = source.MipCount;
    target->Format = source.Format;
    target->Size.x = source.Size.x;
    target->Size.y = source.Size.y;
    target->BitsPerComponent = source.BitsPerComponent;
    
cout << "LoadTextureById: " << target->ID << " GLid:" << target->Name <<  endl;
}
void RenderingEngine::LoadTextureById(int id){
    if(id == 1) UpdateTxt(id, _factory->TextureAtlas01, SetPvrTexture ("Atlas01.pvr"));
    else if(id == 2) UpdateTxt(id, _factory->TextureAtlas02, SetPngTexture("Atlas02.png"));
    else if(id == 3) UpdateTxt(id, _factory->TextureAtlas03, SetPngTexture("Atlas03.png"));
    else if(id == 4) UpdateTxt(id, _factory->TextureAtlas04, SetPngTexture("Atlas04.png"));
    else if(id == 5) UpdateTxt(id, _factory->TextureAtlas05, SetPngTexture("Atlas05.png"));
    else if(id == 6) UpdateTxt(id, _factory->TextureAtlas06, SetPngTexture("Atlas06.png"));
    else if(id == 7) UpdateTxt(id, _factory->TextureAtlas07, SetPngTexture("Atlas07.png"));
    else if(id == 8) UpdateTxt(id, _factory->TextureAtlas08, SetPngTexture("Atlas08.png"));
    else if(id == 9) UpdateTxt(id, _factory->TextureAtlas09, SetPngTexture("Atlas09.png"));
    else if(id ==10) UpdateTxt(id, _factory->TextureAtlas10, SetPvrTexture("Atlas10.pvr"));
    else if(id ==11) UpdateTxt(id, _factory->TextureAtlas11, SetPngTexture("Atlas11.png"));
    else if(id ==12) UpdateTxt(id, _factory->TextureAtlas12, SetPngTexture("Atlas12.png"));
}
void RenderingEngine::UnloadTextureById(int id){
cout << "UnloadTextureById: " << id << endl;
    if(id == 1) glDeleteTextures(1, &_factory->TextureAtlas01->Name);
    else if(id == 2) glDeleteTextures(1, &_factory->TextureAtlas02->Name);
    else if(id == 3) glDeleteTextures(1, &_factory->TextureAtlas03->Name);
    else if(id == 4) glDeleteTextures(1, &_factory->TextureAtlas04->Name);
    else if(id == 5) glDeleteTextures(1, &_factory->TextureAtlas05->Name);
    else if(id == 6) glDeleteTextures(1, &_factory->TextureAtlas06->Name);
    else if(id == 7) glDeleteTextures(1, &_factory->TextureAtlas07->Name);
    else if(id == 8) glDeleteTextures(1, &_factory->TextureAtlas08->Name);
    else if(id == 9) glDeleteTextures(1, &_factory->TextureAtlas09->Name);
    else if(id ==10) glDeleteTextures(1, &_factory->TextureAtlas10->Name);
    else if(id ==11) glDeleteTextures(1, &_factory->TextureAtlas11->Name);
    else if(id ==12) glDeleteTextures(1, &_factory->TextureAtlas12->Name);
}
void RenderingEngine::SetBlendFunc(GLenum sfactor, GLenum dfactor){
    _currentSfactor = sfactor;
    _currentDfactor = dfactor;
    glBlendFunc(_currentSfactor, _currentDfactor);
}
void RenderingEngine::SetDefaultBlendFunc(GLenum sfactor, GLenum dfactor){
    _defaultSfactor = sfactor;
    _defaultDfactor = dfactor;
}
GLuint RenderingEngine::CreateTexture(const unsigned long* data)
{
    PVRTextureHeader* header = (PVRTextureHeader*) data;
    GLsizei w = header->Width;
    GLsizei h = header->Height;
    const unsigned long* texels = data + header->HeaderSize / 4;
    
    GLuint name;
    glGenTextures(1, &name);
    glBindTexture(GL_TEXTURE_2D, name);
    //_currentTexture = name;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    switch (header->Flags & PVRTEX_PIXELTYPE) {
        case OGL_I_8: {
            GLenum format = GL_ALPHA;
            GLenum type = GL_UNSIGNED_BYTE;
            glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, type, texels);
            break;
        }
        case OGL_AI_88: {
            GLenum format = GL_LUMINANCE_ALPHA;
            GLenum type = GL_UNSIGNED_BYTE;
            glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, type, texels);
            break;
        }
        case OGL_RGB_565: {
            GLenum format = GL_RGB;
            GLenum type = GL_UNSIGNED_SHORT_5_6_5;
            glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, type, texels);
            break;
        }
        case OGL_RGBA_8888: {
            GLenum format = GL_RGBA;
            GLenum type = GL_UNSIGNED_BYTE;
            glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, type, texels);
            break;
        }
        default: {
            w = h = 512;
            GLsizei size = max(32, w * h / 2);
            GLenum format = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
            glCompressedTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, size, data);
            break;
        }
    }
    
    return name;
}
void RenderingEngine::Test(){
    //_factory->TextureAtlas03 = SetPngTexture("Atlas03.png");
}

GLuint RenderingEngine::BuildProgram(const char* vertexShaderSource,
                                     const char* fragmentShaderSource) const
{
    GLuint vertexShader = BuildShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = BuildShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    
    GLuint programHandle = glCreateProgram();
    glAttachShader(programHandle, vertexShader);
    glAttachShader(programHandle, fragmentShader);
    glLinkProgram(programHandle);
    
    GLint linkSuccess;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);
    if (linkSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetProgramInfoLog(programHandle, sizeof(messages), 0, &messages[0]);
        //cout << messages;
        exit(1);
    }
    
    return programHandle;
}
TextureDescription RenderingEngine::SetPngTexture(const string& filename)
{
    //return _resourceManager->LoadPngImage2(filename);
    
    
    TextureDescription description = _resourceManager->LoadPngImage(filename);
    
    GLuint name;
    glGenTextures(1, &name);
    description.Name = name;
    glBindTexture(GL_TEXTURE_2D, name);
    //_currentTextureID = name;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    GLenum format = 0;
    switch (description.Format) {
        case TextureFormatGray:      format = GL_LUMINANCE;       break;
        case TextureFormatGrayAlpha: format = GL_LUMINANCE_ALPHA; break;
        case TextureFormatRgb:       format = GL_RGB;             break;
        case TextureFormatRgba:      format = GL_RGBA;            break;
        default:
            break;
    }
    
    GLenum type;
    switch (description.BitsPerComponent) {
        case 8: type = GL_UNSIGNED_BYTE; break;
        case 4:
            if (GL_RGBA == format) {
                type = GL_UNSIGNED_SHORT_4_4_4_4;
                break;
            }
            // intentionally fall through
        default:
            assert(!"Unsupported format.");
    }
    
    void* data = _resourceManager->GetImageData();
    ivec2 size = description.Size;
    glTexImage2D(GL_TEXTURE_2D, 0, format, size.x, size.y, 0, format, type, data);
    _resourceManager->UnloadImage();
    
    return description;
    
    
//    TextureDescription description = _resourceManager->LoadPngImage2(filename);
//
//    return description;
}


TextureDescription RenderingEngine::SetPvrTexture(const string& filename)
{
    TextureDescription description = _resourceManager->LoadPvrImage(filename);
    
    GLuint name;
    glGenTextures(1, &name);
    description.Name = name;
    glBindTexture(GL_TEXTURE_2D, name);
    //_currentTextureID = name;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    unsigned char* data = (unsigned char*) _resourceManager->GetImageData();
    int width = description.Size.x;
    int height = description.Size.y;
    
    int bitsPerPixel = 0;
    GLenum format;
    bool compressed = true;
    switch (description.Format) {
        case TextureFormatPvrtcRgba2:
            bitsPerPixel = 2;
            format = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
            break;
        case TextureFormatPvrtcRgb2:
            bitsPerPixel = 2;
            format = GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
            break;
        case TextureFormatPvrtcRgba4:
            bitsPerPixel = 4;
            format = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
            break;
        case TextureFormatPvrtcRgb4:
            bitsPerPixel = 4;
            format = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
            break;
        default:
            compressed = false;
            break;
    }
    
    if (compressed) {
        for (int level = 0; level < description.MipCount; ++level) {
            GLsizei size = max(32, width * height * bitsPerPixel / 8);
            glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, data);
            data += size;
            width >>= 1; height >>= 1;
        }
    } /*else {
//cout << "Format:" << description.Format << endl;
        GLenum type;
        switch (description.Format) {
            case TextureFormatRgba:
//cout << "GL_RGBA" << endl;
                assert(description.BitsPerComponent == 4);
                format = GL_RGBA;
                type = GL_UNSIGNED_SHORT_4_4_4_4;
                bitsPerPixel = 16;
                break;
            case TextureFormat565:
//cout << "GL_RGB" << endl;
                format = GL_RGB;
                type = GL_UNSIGNED_SHORT_5_6_5;
                bitsPerPixel = 16;
                break;
            case TextureFormat5551:
//cout << "GL_RGBA2" << endl;
                format = GL_RGBA;
                type = GL_UNSIGNED_SHORT_5_5_5_1;
                bitsPerPixel = 16;
                break;
            default:
                break;
        }
        for (int level = 0; level < description.MipCount; ++level) {
            GLsizei size = width * height * bitsPerPixel / 8;
            glTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, format, type, data);
            data += size;
            width >>= 1; height >>= 1;
        }
    }*/
    
    _resourceManager->UnloadImage();
    
    return description;
}

GLuint RenderingEngine::BuildShader(const char* source, GLenum shaderType) const
{
    GLuint shaderHandle = glCreateShader(shaderType);
    glShaderSource(shaderHandle, 1, &source, 0);
    glCompileShader(shaderHandle);
    
    GLint compileSuccess;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileSuccess);
    
    if (compileSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetShaderInfoLog(shaderHandle, sizeof(messages), 0, &messages[0]);
        //std::cout << messages;
        exit(1);
    }
    
    return shaderHandle;
}


void RenderingEngine::Render()
{
    if(_isEnteredBackground) return;
    
    ++_frame;
    /*if(_isMultisamplingEnabled) glBindFramebuffer(GL_FRAMEBUFFER, _framebuffers.Msaa);
    else*/ glBindFramebuffer(GL_FRAMEBUFFER, _framebuffers.Resolve);
    //glViewport(0, 0, _screenSize.x, _screenSize.y);
    
    glClearColor(0, 251.0/255.0, 98.0/255.0, 82.0/255.0);
    glClear(GL_COLOR_BUFFER_BIT/*| GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT*/);

    
    
    
    // erase expired check each 4 frames
    
    if(_factory->forceClearExpired ||  _frame % 10 == 0){
        _factory->forceClearExpired = false;
        ClearExpired();
    }
    
    // proces non expired
    GLuint lastTextureID = 0;
    int lastTextureGroup = 0;
    int numSquares = 0;
    
    simnode<Drawable>* curr = _drawables->head;
    simnode<Drawable>* from = curr;
    while (curr != NULL) {
        Drawable* d = curr->data;
        if(d->isOutOfScreen || !d->IsVisible()){
            curr = curr->next;
            continue;
        }
        GLuint texID = d->Texture()->Name;
        int texGroup = d->textureGroup;
        if(lastTextureID != 0 && (lastTextureID != texID || texGroup != lastTextureGroup)){
            DrawImagesForTexture(from, curr, numSquares);
            from = curr;
            numSquares = 0;
        }
        numSquares += d->NumberSquares();
        lastTextureID = texID;
        lastTextureGroup = texGroup;
        curr = curr->next;
    }
    DrawImagesForTexture(from, NULL, numSquares);
    
    if(!_factory->isPaused){
        DrawTouchTrail();
        DrawMapDots();
    }
    
//    if(_isMultisamplingEnabled){
//        //glDisable(GL_SCISSOR_TEST);
//        
//        glBindFramebuffer(GL_DRAW_FRAMEBUFFER_APPLE, _framebuffers.Resolve);
//        glBindFramebuffer(GL_READ_FRAMEBUFFER_APPLE, _framebuffers.Msaa);
//        glResolveMultisampleFramebufferAPPLE(); 
//    }
    
    const GLenum discards[]  = { GL_COLOR_ATTACHMENT0 };
    glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE, 1, discards);
    
    glBindRenderbuffer(GL_RENDERBUFFER, _renderbuffers.ResolveColor);
}
void RenderingEngine::ClearExpired(){
    simnode<Drawable>* node = _drawables->head;
    while (node != NULL) {
        Drawable* d = node->data;
        if(d->IsExpired()){
            node = _drawables->removeAndGetNext(node, !d->PreserveExpired());
            continue;
        }
        node = node->next;
    }
}
void RenderingEngine::DrawImagesForTexture(simnode<Drawable>* from, simnode<Drawable>* to, int numSquares)
{
    if(from == to){
        return;
    }
    GLuint textureID = from->data->Texture()->Name;
    if(textureID != _currentTexture->Name){
        glBindTexture(GL_TEXTURE_2D, textureID);
        glEnableVertexAttribArray(_attributes.Position);
        glEnableVertexAttribArray(_attributes.TextureCoord);
        glEnableVertexAttribArray(_attributes.Color);
        glUniform1i(_uniforms.IsParticle, GL_FALSE);
        
        //glUniform1f(_uniforms.PointSize, 10.0f);
        //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        SetBlendFunc(_defaultSfactor, _defaultDfactor);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        
        _currentTexture = from->data->Texture();
    }
    int numVertices = 6 * numSquares;
    int numVars = numVertices * 2;
 
    PointArrays* imageArrays = GetPointArraysByTextureID(_currentTexture->ID, numVars);

    int index = 0;
    bool hasColorChange = false;
    
    while (from != to) {
        Drawable* d = from->data;

//Image* img = static_cast<Image*>(d);
//cout << img->Name() << "|" << _currentTextureID << endl;
        
        if(!d->isOutOfScreen && d->IsVisible()){
            
            if(d->sfactor == 0 && d->dfactor == 0){
                if(_isDefaultChanged){
                    SetBlendFunc(_defaultSfactor, _defaultDfactor);
                    _isDefaultChanged = false;
                }
            }else if(_currentSfactor != d->sfactor || _currentDfactor != d->dfactor){
                _isDefaultChanged = true;
                SetBlendFunc(d->sfactor, d->dfactor);
            }
            
            if(d->IsColorChanged()) hasColorChange = true;
            
            index = d->FillArrays(imageArrays, 0, index);
        }
        from = from->next;
    }
    glUniform1i(_uniforms.HasColorChange, hasColorChange ? GL_TRUE : GL_FALSE);
    glVertexAttribPointer(_attributes.Position, 2, GL_FLOAT, GL_FALSE, 0, imageArrays->vertices);
    glVertexAttribPointer(_attributes.TextureCoord, 2, GL_FLOAT, GL_FALSE, 0, imageArrays->texCoord);
    glVertexAttribPointer(_attributes.Color, 4, GL_FLOAT, GL_TRUE, 0, imageArrays->colors);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, numVertices);
}
simlist<Drawable>* RenderingEngine::GetDrawables(){ return _drawables; }
ParticleList* RenderingEngine::GetTouchTrails(){ return _touchTrails; }
simlist<Particle>* RenderingEngine::GetMapDots(){ return _mapDots; }
simlist<IAnimation>* RenderingEngine::GetAnimations(){ return _animations; }
PointArrays* RenderingEngine::CreatePointArrays(int numElements){
    PointArrays * arrays = new PointArrays();
    arrays->vertices = new GLfloat[numElements];
    arrays->texCoord = new GLfloat[numElements];
    arrays->colors = new GLfloat[numElements*2];
    arrays->length = numElements;
    return arrays;
}
PointArrays* RenderingEngine::CreateParticlePointArrays(int numElements){
    PointArrays* arrays = new PointArrays();
    arrays->vertices = new GLfloat[numElements];
    arrays->colors = new GLfloat[numElements*2];
    arrays->length = numElements;
    return arrays;
}
PointArrays* RenderingEngine::GetPointArraysByTextureID(GLuint textureID, int numVars){
    PointArrays* pointArrays = (*_imageArraysByTexture)[textureID];
    if(pointArrays == NULL){
        //cout << "create from NULL " << textureID << endl;
        return (*_imageArraysByTexture)[textureID] = CreatePointArrays(numVars);
    }
    if(pointArrays->length < numVars){
        //cout << "create from SMTH " << textureID << " " << pointArrays->length << " " << numVars << endl;
        delete pointArrays;
        return (*_imageArraysByTexture)[textureID] = CreatePointArrays(numVars);
    }
    return (*_imageArraysByTexture)[textureID];
}
void RenderingEngine::DrawTouchTrail(){
    long length = _touchTrails->size();
    if(length == 0) return;
    if(length > _touchTrailArraysLength){
        if(_touchTrailArrays != NULL){
            delete _touchTrailArrays;
        }
        length *= 2;
        _touchTrailArrays = CreateParticlePointArrays(length*2);
        _touchTrailArraysLength = length;
    }
    glUniform1f(_uniforms.PointSize, _factory->particleWidth);
    DrawParticles(_touchTrails, _touchTrails->size(), _touchTrailArrays);
}
void RenderingEngine::DrawMapDots(){
    int length = _mapDots->size();
    if(length == 0) return;
    if(length >  _mapDotsArraysLength){
        if(_mapDotsArrays != NULL){
            delete _mapDotsArrays;
        }
        length *= 2;
        _mapDotsArrays = CreateParticlePointArrays(length*2);
        _mapDotsArraysLength = length;
    }
    glUniform1f(_uniforms.PointSize, _factory->mapDotWidth);
    DrawParticles(_mapDots, _mapDots->size(), _mapDotsArrays);
};
void RenderingEngine::DrawParticles(ParticleList* particles, long size, PointArrays* arrays){
    if(_currentTexture->Name != Factory::Instance->TextureParticle->Name){
        glBindTexture(GL_TEXTURE_2D, Factory::Instance->TextureParticle->Name);
        glEnableVertexAttribArray(_attributes.Position);
        glDisableVertexAttribArray(_attributes.TextureCoord);
        glUniform1i(_uniforms.IsParticle, GL_TRUE);
        glUniform1i(_uniforms.HasColorChange, GL_TRUE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        _currentTexture = _factory->TextureParticle;
    }
    
    int v = 0;
    int c = 0;
    _factory->isDrawingTouchTrails = true;
    ParticleNode* curr = particles->head;

    while (curr != NULL) {
        


        vec2 pos = curr->particle.Pos();
        vec4 color = curr->particle.Color();
        arrays->vertices[v++] = pos.x;
        arrays->vertices[v++] = pos.y;
        
        arrays->colors[c++] = color.x;
        arrays->colors[c++] = color.y;
        arrays->colors[c++] = color.z;
        arrays->colors[c++] = color.w;
        
        curr = curr->next;
    }
    _factory->isDrawingTouchTrails = false;
    glVertexAttribPointer(_attributes.Position, 2, GL_FLOAT, GL_FALSE, 0, arrays->vertices);
    glVertexAttribPointer(_attributes.Color, 4, GL_FLOAT, GL_FALSE, 0, arrays->colors);
    
    glDrawArrays(GL_POINTS, 0, size);
    
    
}
void RenderingEngine::DrawParticles(simlist<Particle>* particles, long particlesLength, PointArrays* arrays){
    if(_currentTexture->Name != Factory::Instance->TextureParticle->Name){
        glBindTexture(GL_TEXTURE_2D, Factory::Instance->TextureParticle->Name);
        glEnableVertexAttribArray(_attributes.Position);
        glDisableVertexAttribArray(_attributes.TextureCoord);
        glUniform1i(_uniforms.IsParticle, GL_TRUE);
        glUniform1i(_uniforms.HasColorChange, GL_TRUE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        _currentTexture = Factory::Instance->TextureParticle;
    }
    int v = 0;
    int c = 0;
    simnode<Particle>* node = particles->head;
    while (node != NULL) {
        Particle* p = node->data;
        
        vec2 pos = p->Pos();
        vec4 color = p->Color();
        arrays->vertices[v++] = pos.x;
        arrays->vertices[v++] = pos.y;
        
        arrays->colors[c++] = color.x;
        arrays->colors[c++] = color.y;
        arrays->colors[c++] = color.z;
        arrays->colors[c++] = color.w;
        
        node = node->next;
    }
    glVertexAttribPointer(_attributes.Position, 2, GL_FLOAT, GL_FALSE, 0, arrays->vertices);
    glVertexAttribPointer(_attributes.Color, 4, GL_FLOAT, GL_FALSE, 0, arrays->colors);
    
    glDrawArrays(GL_POINTS, 0, particlesLength);
    
}

