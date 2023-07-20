//
//  OpenGLUtil.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <string>

#include "ShaderInput.hpp"
#include "Color.hpp"

class Framebuffer;
struct Shader;
struct Texture;

typedef void (*GLADapiproc)(void);

typedef GLADapiproc (*GLADloadfunc)(const char *name);

class OpenGLUtil
{
public:
    static OpenGLUtil& getInstance()
    {
        static OpenGLUtil ret = OpenGLUtil();
        return ret;
    }
    
    void loadOpenGL(GLADloadfunc load);

    void enableBlending(bool srcAlpha);
    void disableBlending();
    void bindShader(Shader& s);
    void unbindShader(Shader& s);
    void bindMatrix(Shader& s, std::string uniform, mat4& matrix);
    void bindColor(Shader& s, std::string uniform, Color& c);
    void bindInt4(Shader& s, std::string uniform, ivec4& value);
    void bindFloat4(Shader& s, std::string uniform, vec4& value);
    void bindFloat4Array(Shader& s, std::string uniform, int count, vec4* value);
    void bindTexture(Shader& s, std::string uniform, uint32_t index, Texture& t);
    void bindTexture(Shader& s, std::string uniform, uint32_t index, uint32_t texture);
    void unbindTexture(uint32_t index);
    void bindVertexBuffer(uint32_t buffer);
    void bindVertexBuffer(uint32_t buffer, size_t size, const void* data);
    void unbindVertexBuffer();
    void bindScreenFramebuffer(int32_t width, int32_t height, Color& clearColor = Color::CLEAR);
    void bindFramebuffer(Framebuffer& fb, Color& c = Color::CLEAR);
    void clearFramebuffer(Color& c = Color::CLEAR);
    void clearFramebuffer(float red, float green, float blue, float alpha);
    void drawLines(uint32_t first, uint32_t count);
    void drawTriangles(uint32_t first, uint32_t count);
    void drawTrianglesIndexed(uint32_t indexBuffer, uint32_t count, size_t first = 0);
    void drawTriangleStrip(uint32_t first, uint32_t count);
    uint32_t loadRektangleIndexBuffer(int numRektangles);
    uint32_t loadVertexBuffer(size_t size);
    uint32_t loadVertexBuffer(size_t size, const void* data);
    void unloadBuffer(uint32_t& buffer);
    void loadFramebuffer(Framebuffer& fb);
    void unloadFramebuffer(Framebuffer& fb);
    void loadTexture(Texture& t);
    void unloadTexture(Texture& t);
    void loadShader(Shader& s);
    void unloadShader(Shader& s);
    void flush();

private:
    static uint32_t TEXTURE_SLOTS[NUM_SUPPORTED_TEXTURE_SLOTS];
    
    void draw(uint32_t mode, uint32_t first, uint32_t count);
    void drawIndexed(uint32_t mode, uint32_t indexBuffer, uint32_t count, size_t first = 0);
    
    uint32_t loadTexture(int width, int height, uint8_t* data, std::string filterMin, std::string filterMag, uint32_t type, bool repeat_s, bool mipmap);
    uint32_t loadShader(const uint8_t* vertexShaderSrc, const long vertexShaderSrcLength, const uint8_t* fragmentShaderSrc, const long fragmentShaderSrcLength);
    uint32_t compileShader(const uint32_t type, const uint8_t* source, const int32_t length);

    OpenGLUtil() {}
    ~OpenGLUtil() {}
    OpenGLUtil(const OpenGLUtil&);
    OpenGLUtil& operator=(const OpenGLUtil&);
};
