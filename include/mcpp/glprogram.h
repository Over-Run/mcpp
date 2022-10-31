#pragma once
#include "stdafx.h"
#include "mcpp/matrix.h"

namespace mcpp {
    enum class gl_uniform_type : unsigned char {
        I1,
        M4F
    };

    struct gl_uniform {
    private:
        bool dirty;
        gl_uniform_type type;
        int location, count;
        void* value;

        inline void markDirty() {
            dirty = true;
        }
    public:
        gl_uniform(int location, gl_uniform_type type);
        ~gl_uniform();

        int getLocation() const;
        void set(int value);
        void set(const mcpp::matrix4f& value);
        void upload();
    };

    class gl_program {
    private:
        unsigned int id;
        std::map<std::string_view, gl_uniform*> uniforms;
    public:
        gl_program();
        ~gl_program();

        void attachShader(unsigned int shader);
        void detachShader(unsigned int shader);
        void bindAttribLocation(unsigned int index, const char* name);
        void getiv(unsigned int pname, int* params);
        void getInfoLog(int bufSize, int* length, char* infoLog);
        void link();
        void use();
        gl_uniform* findUniform(std::string_view name, gl_uniform_type type);
        void uploadUniforms();
        unsigned int getId() const;
    };
}
