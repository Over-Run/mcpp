#include "stdafx.h"
#include "mcpp/glprogram.h"

mcpp::gl_uniform::gl_uniform(int _location, gl_uniform_type _type) :
    dirty(true), type(_type), location(_location) {
    switch (_type)
    {
    case mcpp::gl_uniform_type::I1:
        count = 1;
        value = new int[1];
        break;
    case mcpp::gl_uniform_type::M4F:
        count = 16;
        value = new float[16];
        break;
    default:
        count = 0;
        value = nullptr;
        break;
    }
}

mcpp::gl_uniform::~gl_uniform() {
    delete[] value;
}

int mcpp::gl_uniform::getLocation() const {
    return location;
}

void mcpp::gl_uniform::set(int _value) {
    *(int*)value = _value;
    markDirty();
}

void mcpp::gl_uniform::set(const mcpp::matrix4f& _value) {
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

void mcpp::gl_uniform::upload() {
    if (!dirty) { return; }
    switch (type)
    {
    case gl_uniform_type::I1:
        glUniform1i(location, *((int*)value));
        break;
    case mcpp::gl_uniform_type::M4F:
        glUniformMatrix4fv(location, 1, false, (float*)value);
        break;
    default:
        break;
    }
    dirty = false;
}

mcpp::gl_program::gl_program() : id(glCreateProgram()) {}

mcpp::gl_program::~gl_program() {
    glDeleteProgram(id);
    for (auto& u : uniforms) {
        delete u.second;
    }
}

void mcpp::gl_program::use() {
    glUseProgram(id);
}

void mcpp::gl_program::attachShader(unsigned int shader) {
    glAttachShader(id, shader);
}

void mcpp::gl_program::detachShader(unsigned int shader) {
    glDetachShader(id, shader);
}

void mcpp::gl_program::bindAttribLocation(unsigned int index, const char* name) {
    glBindAttribLocation(id, index, name);
}

void mcpp::gl_program::getiv(unsigned int pname, int* params) {
    glGetProgramiv(id, pname, params);
}

void mcpp::gl_program::getInfoLog(int bufSize, int* length, char* infoLog) {
    glGetProgramInfoLog(id, bufSize, length, infoLog);
}

void mcpp::gl_program::link() {
    glLinkProgram(id);
}

mcpp::gl_uniform* mcpp::gl_program::findUniform(std::string_view name, gl_uniform_type type) {
    const auto& it = uniforms.find(name);
    if (it == uniforms.end())
    {
        gl_uniform* uniform = new gl_uniform(glGetUniformLocation(id, name.data()), type);
        uniforms[name] = uniform;
        return uniform;
    }
    return uniforms[name];
}

void mcpp::gl_program::uploadUniforms() {
    for (auto& u : uniforms) {
        u.second->upload();
    }
}

unsigned int mcpp::gl_program::getId() const {
    return id;
}
