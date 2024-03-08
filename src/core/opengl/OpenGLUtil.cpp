//
//  OpenGLUtil.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#if IS_ANDROID
    #include <GLES2/gl2.h>
#elif IS_APPLE
    #if IS_IOS
        #include <OpenGLES/ES2/gl.h>
    #elif IS_MACOS
        #include <OpenGL/gl.h>
    #endif
#elif IS_LINUX || IS_WINDOWS
    #define GLAD_GL_IMPLEMENTATION
    #include <glad/gl.h>
#endif

uint32_t OpenGLUtil::TEXTURE_SLOTS[NUM_SUPPORTED_TEXTURE_SLOTS] = {GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3};

void OpenGLUtil::loadOpenGL(GLADloadfunc load)
{
#if IS_LINUX || IS_WINDOWS
    gladLoadGL(load);
#endif
}

void OpenGLUtil::enableBlending(bool srcAlpha)
{
    uint32_t src = srcAlpha ? GL_SRC_ALPHA : GL_ONE;
    glEnable(GL_BLEND);
    glBlendFuncSeparate(src, GL_ONE_MINUS_SRC_ALPHA, src, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
}

void OpenGLUtil::disableBlending()
{
    glDisable(GL_BLEND);
}

void OpenGLUtil::bindShader(Shader& s)
{
    assert(s._glHandle > 0);
    
    glUseProgram(s._glHandle);
    
    std::vector<ShaderAttribute>& attributes = s._attributes;
    for (const auto& sa : attributes)
    {
        glEnableVertexAttribArray(sa._glHandle);
        glVertexAttribPointer(sa._glHandle, sa._size, GL_FLOAT, GL_FALSE, sa._stride, BUFFER_OFFSET(sa._offset));
    }
}

void OpenGLUtil::unbindShader(Shader& s)
{
    std::vector<ShaderAttribute>& attributes = s._attributes;
    for (const auto& sa : attributes)
    {
        glDisableVertexAttribArray(sa._glHandle);
    }
    
    glUseProgram(0);
}

void OpenGLUtil::bindMatrix(Shader& s, std::string uniform, mat4& matrix)
{
    ShaderUniform& su = s.uniform(uniform);
    assert(su._type == GL_FLOAT_MAT4);
    
    glUniformMatrix4fv(su._glHandle, 1, GL_FALSE, (GLfloat*)matrix);
}

void OpenGLUtil::bindColor(Shader& s, std::string uniform, Color& c)
{
    ShaderUniform& su = s.uniform(uniform);
    assert(su._type == GL_FLOAT_VEC4);
    
    glUniform4f(su._glHandle, c._red, c._green, c._blue, c._alpha);
}

void OpenGLUtil::bindInt(Shader& s, std::string uniform, int value)
{
    ShaderUniform& su = s.uniform(uniform);
    assert(su._type == GL_INT);
    
    glUniform1i(su._glHandle, value);
}

void OpenGLUtil::bindInt4(Shader& s, std::string uniform, ivec4& value)
{
    ShaderUniform& su = s.uniform(uniform);
    assert(su._type == GL_INT_VEC4);
    
    glUniform4i(su._glHandle, value[0], value[1], value[2], value[3]);
}

void OpenGLUtil::bindFloat(Shader& s, std::string uniform, float value)
{
    ShaderUniform& su = s.uniform(uniform);
    assert(su._type == GL_FLOAT);
    
    glUniform1f(su._glHandle, value);
}

void OpenGLUtil::bindFloat4(Shader& s, std::string uniform, vec4& value)
{
    ShaderUniform& su = s.uniform(uniform);
    assert(su._type == GL_FLOAT_VEC4);
    
    glUniform4f(su._glHandle, value[0], value[1], value[2], value[3]);
}

void OpenGLUtil::bindFloat4Array(Shader& s, std::string uniform, int count, vec4* value)
{
    ShaderUniform& su = s.uniform(uniform);
    assert(su._type == GL_FLOAT_VEC4);
    
    glUniform4fv(su._glHandle, count, (const GLfloat*)value);
}

void OpenGLUtil::bindTexture(Shader& s, std::string uniform, uint32_t index, Texture& t)
{
    bindTexture(s, uniform, index, t._glHandle);
}

void OpenGLUtil::bindTexture(Shader& s, std::string uniform, uint32_t index, uint32_t texture)
{
    assert(index < NUM_SUPPORTED_TEXTURE_SLOTS);
    assert(texture > 0);
    
    ShaderUniform& su = s.uniform(uniform);
    assert(su._type == GL_SAMPLER_2D);
    
    glActiveTexture(TEXTURE_SLOTS[index]);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(su._glHandle, index);
}

void OpenGLUtil::unbindTexture(uint32_t index)
{
    assert(index < NUM_SUPPORTED_TEXTURE_SLOTS);
    
    glActiveTexture(TEXTURE_SLOTS[index]);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLUtil::bindVertexBuffer(uint32_t buffer)
{
    assert(buffer > 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
}

void OpenGLUtil::bindVertexBuffer(uint32_t buffer, size_t size, const void* data)
{
    bindVertexBuffer(buffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

void OpenGLUtil::unbindVertexBuffer()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLUtil::bindScreenFramebuffer(int32_t width, int32_t height, Color& clearColor)
{
    Framebuffer fb(width, height);
    
#if IS_IOS
    fb._fbo = 1;
#else
    fb._fbo = 0;
#endif
    
    bindFramebuffer(fb);
}

void OpenGLUtil::bindFramebuffer(Framebuffer& fb, Color& c)
{
    assert(fb._width > 0);
    assert(fb._height > 0);
    
    glBindFramebuffer(GL_FRAMEBUFFER, fb._fbo);
    
    glViewport(0, 0, fb._width, fb._height);
    glScissor(0, 0, fb._width, fb._height);
    glEnable(GL_SCISSOR_TEST);
    
    clearFramebuffer(c);
}

void OpenGLUtil::clearFramebuffer(Color c)
{
    clearFramebuffer(c._red, c._green, c._blue, c._alpha);
}

void OpenGLUtil::clearFramebuffer(float red, float green, float blue, float alpha)
{
    glClearColor(red, green, blue, alpha);
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLUtil::drawLines(uint32_t first, uint32_t count)
{
    draw(GL_LINES, first, count);
}

void OpenGLUtil::drawTriangles(uint32_t first, uint32_t count)
{
    draw(GL_TRIANGLES, first, count);
}

void OpenGLUtil::drawTrianglesIndexed(uint32_t indexBuffer, uint32_t count, size_t first)
{
    drawIndexed(GL_TRIANGLES, indexBuffer, count, first);
}

void OpenGLUtil::drawTriangleStrip(uint32_t first, uint32_t count)
{
    draw(GL_TRIANGLE_STRIP, first, count);
}

uint32_t OpenGLUtil::loadRektangleIndexBuffer(int numRektangles)
{
    assert(numRektangles > 0);
    
    uint32_t ret;
    
    std::vector<uint16_t> indices;
    indices.reserve(numRektangles * NUM_INDICES_PER_RECTANGLE);
    
    uint16_t j = 0;
    for (int i = 0; i < numRektangles; ++i, j += NUM_VERTICES_PER_RECTANGLE)
    {
        indices.push_back(j);
        indices.push_back(j + 1);
        indices.push_back(j + 2);
        indices.push_back(j + 2);
        indices.push_back(j + 3);
        indices.push_back(j);
    }
    
    glGenBuffers(1, &ret);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ret);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * indices.size(), &indices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    return ret;
}

uint32_t OpenGLUtil::loadVertexBuffer(size_t size)
{
    assert(size > 0);
    
    uint32_t ret;
    
    glGenBuffers(1, &ret);
    bindVertexBuffer(ret);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    unbindVertexBuffer();
    
    return ret;
}

uint32_t OpenGLUtil::loadVertexBuffer(size_t size, const void* data)
{
    assert(size > 0);
    
    uint32_t ret;
    
    glGenBuffers(1, &ret);
    bindVertexBuffer(ret);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    unbindVertexBuffer();
    
    return ret;
}

void OpenGLUtil::unloadBuffer(uint32_t& buffer)
{
    assert(buffer > 0);
    
    glDeleteBuffers(1, &buffer);
    buffer = 0;
}

void OpenGLUtil::loadFramebuffer(Framebuffer& fb)
{
    uint32_t texture;
    uint32_t fbo;
    
    int32_t currentFBO;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFBO);
    
    texture = loadTexture(fb._width, fb._height, nullptr, fb._filterMin, fb._filterMag, GL_RGBA, false, false);
    
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    
    uint32_t status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    assert(status == GL_FRAMEBUFFER_COMPLETE);
    
    glBindFramebuffer(GL_FRAMEBUFFER, currentFBO);
    
    fb._texture = texture;
    fb._fbo = fbo;
}

void OpenGLUtil::unloadFramebuffer(Framebuffer& fb)
{
    assert(fb._texture > 0);
    assert(fb._fbo > 0);
    
    glDeleteTextures(1, &fb._texture);
    fb._texture = 0;
    
    glDeleteFramebuffers(1, &fb._fbo);
    fb._fbo = 0;
}

void OpenGLUtil::loadTexture(Texture& t)
{
    if (ENGINE_CFG.logOpenGL())
    {
        LOG("OpenGLUtil::loadTexture %s", t._desc._filePath.c_str());
    }
    
    assert(t._data != nullptr);
    
    t._glHandle = loadTexture(t._width, t._height, t._data, t._desc._filterMin, t._desc._filterMag, GL_RGBA, t._desc._repeatS, t._desc._mipMap);
}

void OpenGLUtil::unloadTexture(Texture& t)
{
    if (ENGINE_CFG.logOpenGL())
    {
        LOG("OpenGLUtil::unloadTexture %s", t._desc._filePath.c_str());
    }
    
    uint32_t glHandle = t._glHandle;
    assert(glHandle > 0);
    
    glDeleteTextures(1, &glHandle);
    t._glHandle = 0;
}

void OpenGLUtil::loadShader(Shader& s)
{
    if (ENGINE_CFG.logOpenGL())
    {
        LOG("OpenGLUtil::loadShader %s", s._desc._name.c_str());
    }
    
    const uint8_t* vertexShaderSrc = s._vertexShaderFileData->_data;
    const long vertexShaderSrcLength = s._vertexShaderFileData->_length;
    const uint8_t* fragmentShaderSrc = s._fragmentShaderFileData->_data;
    const long fragmentShaderSrcLength = s._fragmentShaderFileData->_length;
    
    s._glHandle = loadShader(vertexShaderSrc, vertexShaderSrcLength, fragmentShaderSrc, fragmentShaderSrcLength);
    
    GLint glslVarNumElements;
    GLenum glslVarType;
    static const GLsizei glslVarNameBufferSize = 32;
    GLchar glslVarName[glslVarNameBufferSize];
    GLsizei glslVarNameLength;

    GLint numAttributes;
    glGetProgramiv(s._glHandle, GL_ACTIVE_ATTRIBUTES, &numAttributes);
    if (ENGINE_CFG.logOpenGL())
    {
        LOG("Active Attributes: %d", numAttributes);
    }

    std::vector<ShaderAttribute>& attributes = s._attributes;
    for (GLint i = 0; i < numAttributes; ++i)
    {
        glGetActiveAttrib(s._glHandle, (GLuint)i, glslVarNameBufferSize, &glslVarNameLength, &glslVarNumElements, &glslVarType, glslVarName);

        if (ENGINE_CFG.logOpenGL())
        {
            LOG("Attribute #%d Type: %u Name: %s Length: %d Count: %d", i, glslVarType, glslVarName, glslVarNameLength, glslVarNumElements);
        }
        
        uint32_t glHandle = glGetAttribLocation(s._glHandle, glslVarName);
        
        attributes.emplace_back(std::string(glslVarName), glslVarType, glHandle);
    }

    GLint numUniforms;
    glGetProgramiv(s._glHandle, GL_ACTIVE_UNIFORMS, &numUniforms);
    if (ENGINE_CFG.logOpenGL())
    {
        LOG("Active Uniforms: %d", numUniforms);
    }

    std::vector<ShaderUniform>& uniforms = s._uniforms;
    for (GLint i = 0; i < numUniforms; ++i)
    {
        glGetActiveUniform(s._glHandle, (GLuint)i, glslVarNameBufferSize, &glslVarNameLength, &glslVarNumElements, &glslVarType, glslVarName);
        
        if (ENGINE_CFG.logOpenGL())
        {
            LOG("Uniform #%d Type: %u Name: %s Length: %d Count: %d", i, glslVarType, glslVarName, glslVarNameLength, glslVarNumElements);
        }
        
        uint32_t glHandle = glGetUniformLocation(s._glHandle, glslVarName);
        
        uniforms.emplace_back(std::string(glslVarName), glslVarType, glHandle);
    }
    
    size_t offset = 0;
    uint32_t totalSize = 0;
    
    for (auto& sa : attributes)
    {
        sa._offset = offset * sizeof(GL_FLOAT);
        sa._size = sizeForGlslType(sa._type);
        offset += sa._size;
        totalSize += sa._size;
    }
    for (auto& sa : attributes)
    {
        sa._stride = totalSize * sizeof(GLfloat);
    }
}

void OpenGLUtil::unloadShader(Shader& s)
{
    if (ENGINE_CFG.logOpenGL())
    {
        LOG("OpenGLUtil::unloadShader %s", s._desc._name.c_str());
    }
    
    uint32_t program = s._glHandle;
    assert(program > 0);
    
    glDeleteProgram(program);
    s._glHandle = 0;
}

void OpenGLUtil::draw(uint32_t mode, uint32_t first, uint32_t count)
{
    assert(count > 0);
    
    glDrawArrays(mode, first, count);
}

void OpenGLUtil::drawIndexed(uint32_t mode, uint32_t indexBuffer, uint32_t count, size_t first)
{
    assert(indexBuffer > 0);
    assert(count > 0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glDrawElements(mode, count * NUM_INDICES_PER_RECTANGLE, GL_UNSIGNED_SHORT, BUFFER_OFFSET(first * NUM_INDICES_PER_RECTANGLE));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint32_t OpenGLUtil::loadTexture(int width, int height, const uint8_t* data, std::string filterMin, std::string filterMag, uint32_t type, bool repeat_s, bool mipmap)
{
    assert(width > 0);
    assert(height > 0);

    uint32_t ret;
    
    glActiveTexture(TEXTURE_SLOTS[0]);
    
    glGenTextures(1, &ret);
    assert(ret != 0);

    glBindTexture(GL_TEXTURE_2D, ret);
    
    bool filterMinSharp = filterMin == "SHARP";
    bool filterMagSharp = filterMag == "SHARP";
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap ? (filterMinSharp ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR_MIPMAP_LINEAR) : (filterMinSharp ? GL_NEAREST : GL_LINEAR));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMagSharp ? GL_NEAREST : GL_LINEAR);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat_s ? GL_REPEAT : GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    if (mipmap)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    
    return ret;
}

uint32_t OpenGLUtil::loadShader(const uint8_t* vertexShaderSrc, const long vertexShaderSrcLength, const uint8_t* fragmentShaderSrc, const long fragmentShaderSrcLength)
{
    assert(vertexShaderSrc != nullptr);
    assert(vertexShaderSrcLength > 0);
    assert(fragmentShaderSrc != nullptr);
    assert(fragmentShaderSrcLength > 0);

    uint32_t ret;
    
    uint32_t vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSrc, (int32_t)vertexShaderSrcLength);
    assert(vertexShader > 0);
    
    uint32_t fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc, (int32_t)fragmentShaderSrcLength);
    assert(fragmentShader > 0);
    
    ret = glCreateProgram();
    assert(ret != GL_FALSE);

    glAttachShader(ret, vertexShader);
    glAttachShader(ret, fragmentShader);
    glLinkProgram(ret);
    
    int32_t linkStatus;
    glGetProgramiv(ret, GL_LINK_STATUS, &linkStatus);
    assert(linkStatus != GL_FALSE);

    glDetachShader(ret, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(ret, fragmentShader);
    glDeleteShader(fragmentShader);
    
    return ret;
}

uint32_t OpenGLUtil::compileShader(const uint32_t type, const uint8_t* source, const int32_t length)
{
    assert(source != nullptr);
    assert(length > 0);

    uint32_t ret = glCreateShader(type);
    assert(ret != GL_FALSE);

    glShaderSource(ret, 1, (const GLchar **)&source, &length);
    glCompileShader(ret);
    
    int32_t compileStatus;
    glGetShaderiv(ret, GL_COMPILE_STATUS, &compileStatus);

    if (compileStatus == GL_FALSE)
    {
        int32_t maxLength = 0;
        glGetShaderiv(ret, GL_INFO_LOG_LENGTH, &maxLength);

        GLchar* errorLog = (GLchar*) malloc(maxLength);
        glGetShaderInfoLog(ret, maxLength, &maxLength, errorLog);
        LOG(errorLog);

        glDeleteShader(ret);

        free(errorLog);
    }
    
    assert(compileStatus == GL_TRUE);

    return ret;
}

uint32_t OpenGLUtil::sizeForGlslType(uint32_t type)
{
    uint32_t ret = 0;

    if (type == GL_FLOAT_VEC4 ||
        type == GL_INT_VEC4)
    {
        ret = 4;
    }
    else if (type == GL_FLOAT_VEC3 ||
             type == GL_INT_VEC3)
    {
        ret = 3;
    }
    else if (type == GL_FLOAT_VEC2 ||
             type == GL_INT_VEC2)
    {
        ret = 2;
    }

    assert(ret > 0);

    return ret;
}
