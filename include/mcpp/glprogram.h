#pragma once
#include "stdafx.h"
#include "mcpp/math/matrix.h"

namespace mcpp {
    enum class GLUniformType : unsigned char {
        I1,
        F4,
        M4F
    };

    struct GLUniform {
    private:
        bool dirty;
        GLUniformType type;
        int location, count;
        void* value;

        inline void markDirty() {
            dirty = true;
        }
    public:
        GLUniform(int location, GLUniformType type);
        ~GLUniform();

        int getLocation() const;
        void set(int value);
        void set(float x, float y, float z, float w);
        void set(const mcpp::Matrix4f& value);
        void upload(unsigned int program);
    };

    class GLProgram {
    private:
        unsigned int id;
        std::map<std::string_view, GLUniform*> uniforms;
    public:
        GLProgram();
        ~GLProgram();

        void attachShader(unsigned int shader);
        void detachShader(unsigned int shader);
        void bindAttribLocation(unsigned int index, const char* name);
        void getiv(unsigned int pname, int* params);
        void getInfoLog(int bufSize, int* length, char* infoLog);
        void link();
        void use();
        GLUniform* findUniform(std::string_view name, GLUniformType type);
        void uploadUniforms();
        unsigned int getId() const;
    };
}
