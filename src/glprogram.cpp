#include "stdafx.h"
#include "mcpp/glprogram.h"

mcpp::GLUniform::GLUniform(int _location, GLUniformType _type) :
    dirty(true), type(_type), location(_location) {
    switch (_type)
    {
    case mcpp::GLUniformType::I1:
        count = 1;
        value = new int[1];
        break;
    case mcpp::GLUniformType::F4:
        count = 4;
        value = new float[4];
        break;
    case mcpp::GLUniformType::M4F:
        count = 16;
        value = new float[16];
        break;
    default:
        count = 0;
        value = nullptr;
        break;
    }
}

mcpp::GLUniform::~GLUniform() {
    delete[] value;
}

int mcpp::GLUniform::getLocation() const {
    return location;
}

void mcpp::GLUniform::set(int _value) {
    *(int*)value = _value;
    markDirty();
}

void mcpp::GLUniform::set(float x, float y, float z, float w) {
    float* pValue = (float*)value;
    pValue[0] = x;
    pValue[1] = y;
    pValue[2] = z;
    pValue[3] = w;
    markDirty();
}

void mcpp::GLUniform::set(const mcpp::Matrix4f& _value) {
    float* pValue = (float*)value;
    pValue[0] = _value.m00;
    pValue[1] = _value.m01;
    pValue[2] = _value.m02;
    pValue[3] = _value.m03;
    pValue[4] = _value.m10;
    pValue[5] = _value.m11;
    pValue[6] = _value.m12;
    pValue[7] = _value.m13;
    pValue[8] = _value.m20;
    pValue[9] = _value.m21;
    pValue[10] = _value.m22;
    pValue[11] = _value.m23;
    pValue[12] = _value.m30;
    pValue[13] = _value.m31;
    pValue[14] = _value.m32;
    pValue[15] = _value.m33;
    markDirty();
}

void mcpp::GLUniform::upload(unsigned int program) {
    if (!dirty) { return; }
    const bool& ARB = GLAD_GL_ARB_separate_shader_objects;
    switch (type)
    {
    case GLUniformType::I1:
        if (ARB) glProgramUniform1iv(program, location, 1, (int*)value);
        else glUniform1iv(location, 1, (int*)value);
        break;
    case GLUniformType::F4:
        if (ARB) glProgramUniform4fv(program, location, 1, (float*)value);
        else glUniform4fv(location, 1, (float*)value);
        break;
    case mcpp::GLUniformType::M4F:
        if (ARB) glProgramUniformMatrix4fv(program, location, 1, false, (float*)value);
        else glUniformMatrix4fv(location, 1, false, (float*)value);
        break;
    default:
        break;
    }
    dirty = false;
}

mcpp::GLProgram::GLProgram() : id(glCreateProgram()) {}

mcpp::GLProgram::~GLProgram() {
    glDeleteProgram(id);
    for (auto& u : uniforms) {
        delete u.second;
    }
}

void mcpp::GLProgram::use() {
    glUseProgram(id);
}

void mcpp::GLProgram::attachShader(unsigned int shader) {
    glAttachShader(id, shader);
}

void mcpp::GLProgram::detachShader(unsigned int shader) {
    glDetachShader(id, shader);
}

void mcpp::GLProgram::bindAttribLocation(unsigned int index, const char* name) {
    glBindAttribLocation(id, index, name);
}

void mcpp::GLProgram::getiv(unsigned int pname, int* params) {
    glGetProgramiv(id, pname, params);
}

void mcpp::GLProgram::getInfoLog(int bufSize, int* length, char* infoLog) {
    glGetProgramInfoLog(id, bufSize, length, infoLog);
}

void mcpp::GLProgram::link() {
    glLinkProgram(id);
}

mcpp::GLUniform* mcpp::GLProgram::findUniform(std::string_view name, GLUniformType type) {
    if (uniforms.count(name) == 0)
    {
        GLUniform* uniform = new GLUniform(glGetUniformLocation(id, name.data()), type);
        uniforms[name] = uniform;
        return uniform;
    }
    return uniforms[name];
}

void mcpp::GLProgram::uploadUniforms() {
    for (auto& u : uniforms) {
        u.second->upload(id);
    }
}

unsigned int mcpp::GLProgram::getId() const {
    return id;
}
