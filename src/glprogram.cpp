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

void mcpp::GLUniform::set(const mcpp::Matrix4f& _value) {
    float* p_Value = (float*)value;
    p_Value[0] = _value.m00;
    p_Value[1] = _value.m01;
    p_Value[2] = _value.m02;
    p_Value[3] = _value.m03;
    p_Value[4] = _value.m10;
    p_Value[5] = _value.m11;
    p_Value[6] = _value.m12;
    p_Value[7] = _value.m13;
    p_Value[8] = _value.m20;
    p_Value[9] = _value.m21;
    p_Value[10] = _value.m22;
    p_Value[11] = _value.m23;
    p_Value[12] = _value.m30;
    p_Value[13] = _value.m31;
    p_Value[14] = _value.m32;
    p_Value[15] = _value.m33;
    markDirty();
}

void mcpp::GLUniform::upload() {
    if (!dirty) { return; }
    switch (type)
    {
    case GLUniformType::I1:
        glUniform1i(location, *((int*)value));
        break;
    case mcpp::GLUniformType::M4F:
        glUniformMatrix4fv(location, 1, false, (float*)value);
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
    const auto& it = uniforms.find(name);
    if (it == uniforms.end())
    {
        GLUniform* uniform = new GLUniform(glGetUniformLocation(id, name.data()), type);
        uniforms[name] = uniform;
        return uniform;
    }
    return uniforms[name];
}

void mcpp::GLProgram::uploadUniforms() {
    for (auto& u : uniforms) {
        u.second->upload();
    }
}

unsigned int mcpp::GLProgram::getId() const {
    return id;
}
