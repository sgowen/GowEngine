//
//  OpenGLWrapper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "PlatformMacros.hpp"

#if IS_IOS
    #include <OpenGLES/ES3/gl.h>
#elif IS_MACOS
    #include <OpenGL/OpenGL.h>
    #include <OpenGL/gl.h>
#elif IS_ANDROID
    #ifdef GL3
        #include <GLES3/gl3.h>
    #elif GL3_2
        #include <GLES3/gl32.h>
    #else
        #include <GLES2/gl2.h>
        #include <GLES2/gl2ext.h>
    #endif
#elif IS_LINUX
    #include <GL/glew.h>
#elif IS_WINDOWS
   #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <Windows.h>
    #include <glad/gl.h>
#endif

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
    static GLenum TEXTURE_SLOTS[NUM_SUPPORTED_TEXTURE_SLOTS];

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
    void bindTexture(Shader& s, std::string uniformName, GLuint index, Texture& t);
    void bindTexture(Shader& s, std::string uniformName, GLuint index, GLuint texture);
    void unbindTexture(GLuint index);
    void bindVertexBuffer(GLuint buffer);
    void bindVertexBuffer(GLuint buffer, size_t size, const void* data);
    void unbindVertexBuffer();
    void bindScreenFramebuffer(GLsizei width, GLsizei height, const Color& clearColor = Color::CLEAR);
    void bindFramebuffer(Framebuffer& fb, const Color& clearColor = Color::CLEAR);
    void clearFramebuffer(const Color& c);
    void clearFramebuffer(float red, float green, float blue, float alpha);
    void draw(GLenum mode, uint32_t first, uint32_t count);
    void drawIndexed(GLenum mode, GLuint indexBuffer, uint32_t count, size_t first = 0);
    GLuint loadRektangleIndexBuffer(int numRektangles);
    GLuint loadVertexBuffer(size_t size);
    GLuint loadVertexBuffer(size_t size, const void* data);
    void unloadBuffer(GLuint& buffer);
    void loadFramebuffer(Framebuffer& fb, bool sharp = false);
    void unloadFramebuffer(Framebuffer& fb);
    void loadTexture(Texture& t);
    void unloadTexture(Texture& t);
    void loadShader(Shader& s, const uint8_t* vertexShaderSrc, const long vertexShaderSrcLength, const uint8_t* fragmentShaderSrc, const long fragmentShaderSrcLength);
    void unloadShader(Shader& s);

private:
    GLuint loadTexture(int width, int height, uint8_t* data, GLint filterMin, GLint filterMag, bool mipmap);
    void unloadTexture(GLuint texture);
    GLuint loadShader(const uint8_t* vertexShaderSrc, const long vertexShaderSrcLength, const uint8_t* fragmentShaderSrc, const long fragmentShaderSrcLength);
    void unloadShader(GLuint program);
    GLuint compileShader(const GLenum type, const uint8_t* source, const GLint length);

    OpenGLWrapper() {}
    ~OpenGLWrapper() {}
    OpenGLWrapper(const OpenGLWrapper&);
    OpenGLWrapper& operator=(const OpenGLWrapper&);
};
