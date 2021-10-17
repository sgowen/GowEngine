//
//  OpenGLUtil.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

uint32_t OpenGLUtil::TEXTURE_SLOTS[NUM_SUPPORTED_TEXTURE_SLOTS] = {GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3};
uint32_t OpenGLUtil::MODE_LINES = GL_LINES;
uint32_t OpenGLUtil::MODE_LINE_STRIP = GL_LINE_STRIP;
uint32_t OpenGLUtil::MODE_TRIANGLES = GL_TRIANGLES;
uint32_t OpenGLUtil::MODE_TRIANGLE_STRIP = GL_TRIANGLE_STRIP;

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
    assert(s._program > 0);
    
    glUseProgram(s._program);
    
    std::vector<ShaderAttribute>& attributes = s._desc._attributes;
    for (const auto& sa : attributes)
    {
        glEnableVertexAttribArray(sa._location);
        glVertexAttribPointer(sa._location, sa._size, GL_FLOAT, GL_FALSE, sa._stride, BUFFER_OFFSET(sa._offset));
    }
}

void OpenGLUtil::unbindShader(Shader& s)
{
    std::vector<ShaderAttribute>& attributes = s._desc._attributes;
    for (const auto& sa : attributes)
    {
        glDisableVertexAttribArray(sa._location);
    }
    
    glUseProgram(0);
}

void OpenGLUtil::bindMatrix(Shader& s, std::string uniform, mat4& matrix)
{
    ShaderUniform& su = s._desc.uniform(uniform);
    assert(su._type == "mat4");
    
    glUniformMatrix4fv(su._location, 1, GL_FALSE, (GLfloat*)matrix);
}

void OpenGLUtil::bindColor(Shader& s, std::string uniform, const Color& c)
{
    ShaderUniform& su = s._desc.uniform(uniform);
    assert(su._type == "vec4");
    
    glUniform4f(su._location, c._red, c._green, c._blue, c._alpha);
}

void OpenGLUtil::bindInt4(Shader& s, std::string uniform, ivec4& value)
{
    ShaderUniform& su = s._desc.uniform(uniform);
    assert(su._type == "ivec4");
    
    glUniform4i(su._location, value[0], value[1], value[2], value[3]);
}

void OpenGLUtil::bindFloat4(Shader& s, std::string uniform, vec4& value)
{
    ShaderUniform& su = s._desc.uniform(uniform);
    assert(su._type == "vec4");
    
    glUniform4f(su._location, value[0], value[1], value[2], value[3]);
}

void OpenGLUtil::bindFloat4Array(Shader& s, std::string uniform, int count, vec4* value)
{
    ShaderUniform& su = s._desc.uniform(uniform);
    assert(su._type == "vec4");
    
    glUniform4fv(su._location, count, (const GLfloat*)value);
}

void OpenGLUtil::bindTexture(Shader& s, std::string uniform, uint32_t index, Texture& t)
{
    bindTexture(s, uniform, index, t._texture);
}

void OpenGLUtil::bindTexture(Shader& s, std::string uniform, uint32_t index, uint32_t texture)
{
    assert(index < NUM_SUPPORTED_TEXTURE_SLOTS);
    assert(texture > 0);
    
    ShaderUniform& su = s._desc.uniform(uniform);
    assert(su._type == "sampler2D");
    
    glActiveTexture(TEXTURE_SLOTS[index]);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_TEXTURE_2D);
    glUniform1i(su._location, index);
}

void OpenGLUtil::unbindTexture(uint32_t index)
{
    assert(index < NUM_SUPPORTED_TEXTURE_SLOTS);
    
    glActiveTexture(TEXTURE_SLOTS[index]);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
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

void OpenGLUtil::bindScreenFramebuffer(int32_t width, int32_t height, const Color& clearColor)
{
    Framebuffer fb(width, height);
    
#if IS_IOS
    fb._fbo = 1;
#else
    fb._fbo = 0;
#endif
    
    bindFramebuffer(fb);
}

void OpenGLUtil::bindFramebuffer(Framebuffer& fb, const Color& c)
{
    assert(fb._width > 0);
    assert(fb._height > 0);
    
    glBindFramebuffer(GL_FRAMEBUFFER, fb._fbo);
    
    glViewport(0, 0, fb._width, fb._height);
    glScissor(0, 0, fb._width, fb._height);
    glEnable(GL_SCISSOR_TEST);
    
    clearFramebuffer(c);
}

void OpenGLUtil::clearFramebuffer(const Color& c)
{
    clearFramebuffer(c._red, c._green, c._blue, c._alpha);
}

void OpenGLUtil::clearFramebuffer(float red, float green, float blue, float alpha)
{
    glClearColor(red, green, blue, alpha);
    glClear(GL_COLOR_BUFFER_BIT);
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
    
    texture = loadTexture(fb._width, fb._height, nullptr, fb._filterMin, fb._filterMag, false);
    
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
    assert(t._data != nullptr);
    
    t._texture = loadTexture(t._width, t._height, t._data, t._desc._filterMin, t._desc._filterMag, t._desc._mipMap);
}

void OpenGLUtil::unloadTexture(Texture& t)
{
    unloadTexture(t._texture);
    t._texture = 0;
}

void OpenGLUtil::loadShader(Shader& s)
{
    const uint8_t* vertexShaderSrc = s._vertexShaderFileData->_data;
    const long vertexShaderSrcLength = s._vertexShaderFileData->_length;
    const uint8_t* fragmentShaderSrc = s._fragmentShaderFileData->_data;
    const long fragmentShaderSrcLength = s._fragmentShaderFileData->_length;
    s._program = loadShader(vertexShaderSrc, vertexShaderSrcLength, fragmentShaderSrc, fragmentShaderSrcLength);
    
    std::vector<ShaderUniform>& uniforms = s._desc._uniforms;
    for (auto& su : uniforms)
    {
        su._location = glGetUniformLocation(s._program, su._name.c_str());
    }
    
    size_t offset = 0;
    uint32_t totalSize = 0;
    
    std::vector<ShaderAttribute>& attributes = s._desc._attributes;
    for (auto& sa : attributes)
    {
        sa._offset = offset * sizeof(GL_FLOAT);
        offset += sa._size;
        totalSize += sa._size;
    }
    for (auto& sa : attributes)
    {
        sa._stride = totalSize * sizeof(GLfloat);
        
        sa._location = glGetAttribLocation(s._program, sa._name.c_str());
    }
}

void OpenGLUtil::unloadShader(Shader& s)
{
    unloadShader(s._program);
    s._program = 0;
}

uint32_t OpenGLUtil::loadTexture(int width, int height, uint8_t* data, std::string filterMin, std::string filterMag, bool mipmap)
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
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    if (mipmap)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    
    return ret;
}

void OpenGLUtil::unloadTexture(uint32_t texture)
{
    assert(texture > 0);
    
    glDeleteTextures(1, &texture);
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

void OpenGLUtil::unloadShader(uint32_t program)
{
    assert(program > 0);
    
    glDeleteProgram(program);
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
