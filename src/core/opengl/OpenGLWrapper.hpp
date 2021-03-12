//
//  OpenGLWrapper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <string>

#include "ShaderInput.hpp"
#include "Color.hpp"

struct Framebuffer;
struct Shader;
struct Texture;

#define NUM_VERTICES_PER_LINE 2
#define NUM_VERTICES_PER_TRIANGLE 3
#define NUM_VERTICES_PER_RECTANGLE 4
#define NUM_INDICES_PER_RECTANGLE 6
#define NUM_SUPPORTED_TEXTURE_SLOTS 4

#define OGL OpenGLWrapper::getInstance()

class OpenGLWrapper
{
public:
    static uint32_t TEXTURE_SLOTS[NUM_SUPPORTED_TEXTURE_SLOTS];

    static OpenGLWrapper& getInstance()
    {
        static OpenGLWrapper ret = OpenGLWrapper();
        return ret;
    }

    void enableBlending(bool srcAlpha);
    void disableBlending();
    void bindShader(Shader& s);
    void unbindShader(Shader& s);
    void bindMatrix(Shader& s, std::string uniformName, mat4& matrix);
    void bindColor(Shader& s, std::string uniformName, const Color& c);
    void bindInt4(Shader& s, std::string uniformName, ivec4& value);
    void bindFloat4(Shader& s, std::string uniformName, vec4& value);
    void bindFloat4Array(Shader& s, std::string uniformName, int count, vec4* value);
    void bindTexture(Shader& s, std::string uniformName, uint32_t index, Texture& t);
    void bindTexture(Shader& s, std::string uniformName, uint32_t index, uint32_t texture);
    void unbindTexture(uint32_t index);
    void bindVertexBuffer(uint32_t buffer);
    void bindVertexBuffer(uint32_t buffer, size_t size, const void* data);
    void unbindVertexBuffer();
    void bindScreenFramebuffer(int32_t width, int32_t height, const Color& clearColor = Color::CLEAR);
    void bindFramebuffer(Framebuffer& fb, const Color& c = Color::CLEAR);
    void clearFramebuffer(const Color& c = Color::CLEAR);
    void clearFramebuffer(float red, float green, float blue, float alpha);
    void draw(uint32_t mode, uint32_t first, uint32_t count);
    void drawIndexed(uint32_t mode, uint32_t indexBuffer, uint32_t count, size_t first = 0);
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

private:
    uint32_t loadTexture(int width, int height, uint8_t* data, std::string filterMin, std::string filterMag, bool mipmap);
    void unloadTexture(uint32_t texture);
    uint32_t loadShader(const uint8_t* vertexShaderSrc, const long vertexShaderSrcLength, const uint8_t* fragmentShaderSrc, const long fragmentShaderSrcLength);
    void unloadShader(uint32_t program);
    uint32_t compileShader(const uint32_t type, const uint8_t* source, const int32_t length);

    OpenGLWrapper() {}
    ~OpenGLWrapper() {}
    OpenGLWrapper(const OpenGLWrapper&);
    OpenGLWrapper& operator=(const OpenGLWrapper&);
};
