#pragma once

namespace mcpp {
    template<typename T>
    struct Matrix4 {
    public:
        static constexpr int PROPERTY_PERSPECTIVE = 1 << 0;
        static constexpr int PROPERTY_AFFINE = 1 << 1;
        static constexpr int PROPERTY_IDENTITY = 1 << 2;
        static constexpr int PROPERTY_TRANSLATION = 1 << 3;
        static constexpr int PROPERTY_ORTHONORMAL = 1 << 4;

        int properties;
        T m00, m01, m02, m03;
        T m10, m11, m12, m13;
        T m20, m21, m22, m23;
        T m30, m31, m32, m33;

        Matrix4() :
            properties(PROPERTY_IDENTITY | PROPERTY_AFFINE | PROPERTY_TRANSLATION | PROPERTY_ORTHONORMAL),
            m00(1), m01(0), m02(0), m03(0),
            m10(0), m11(1), m12(0), m13(0),
            m20(0), m21(0), m22(1), m23(0),
            m30(0), m31(0), m32(0), m33(1) {}

        Matrix4& identity() {
            if ((properties & PROPERTY_IDENTITY) != 0)
            {
                return *this;
            }
            _identity();
            properties = PROPERTY_IDENTITY | PROPERTY_AFFINE | PROPERTY_TRANSLATION | PROPERTY_ORTHONORMAL;
            return *this;
        }

    private:
        void zero() {
            m00 = 0; m01 = 0; m02 = 0; m03 = 0;
            m10 = 0; m11 = 0; m12 = 0; m13 = 0;
            m20 = 0; m21 = 0; m22 = 0; m23 = 0;
            m30 = 0; m31 = 0; m32 = 0; m33 = 0;
        }

        void _identity() {
            m00 = 1; m01 = 0; m02 = 0; m03 = 0;
            m10 = 0; m11 = 1; m12 = 0; m13 = 0;
            m20 = 0; m21 = 0; m22 = 1; m23 = 0;
            m30 = 0; m31 = 0; m32 = 0; m33 = 1;
        }

        Matrix4& perspectiveGeneric(T fovy, T aspect, T zNear, T zFar, bool zZeroToOne) {
            T h = tan(fovy * 0.5f);
            // calculate right matrix elements
            T rm00 = 1.0f / (h * aspect);
            T rm11 = 1.0f / h;
            T rm22;
            T rm32;
            bool farInf = zFar > 0 && isinf(zFar);
            bool nearInf = zNear > 0 && isinf(zNear);
            if (farInf) {
                // See: "Infinite Projection Matrix" (http://www.terathon.com/gdc07_lengyel.pdf)
                T e = 1E-6f;
                rm22 = e - 1.0f;
                rm32 = (e - (zZeroToOne ? 1.0f : 2.0f)) * zNear;
            }
            else if (nearInf) {
                T e = 1E-6f;
                rm22 = (zZeroToOne ? 0.0f : 1.0f) - e;
                rm32 = ((zZeroToOne ? 1.0f : 2.0f) - e) * zFar;
            }
            else {
                rm22 = (zZeroToOne ? zFar : zFar + zNear) / (zNear - zFar);
                rm32 = (zZeroToOne ? zFar : zFar + zFar) * zNear / (zNear - zFar);
            }
            // perform optimized matrix multiplication
            T nm20 = m20 * rm22 - m30;
            T nm21 = m21 * rm22 - m31;
            T nm22 = m22 * rm22 - m32;
            T nm23 = m23 * rm22 - m33;
            m00 = m00 * rm00;
            m01 = m01 * rm00;
            m02 = m02 * rm00;
            m03 = m03 * rm00;
            m10 = m10 * rm11;
            m11 = m11 * rm11;
            m12 = m12 * rm11;
            m13 = m13 * rm11;
            m30 = m20 * rm32;
            m31 = m21 * rm32;
            m32 = m22 * rm32;
            m33 = m23 * rm32;
            m20 = nm20;
            m21 = nm21;
            m22 = nm22;
            m23 = nm23;
            properties = properties & ~(PROPERTY_AFFINE | PROPERTY_IDENTITY | PROPERTY_TRANSLATION | PROPERTY_ORTHONORMAL);
            return *this;
        }

        Matrix4& translateGeneric(T x, T y, T z) {
            m30 = fma(m00, x, fma(m10, y, fma(m20, z, m30)));
            m31 = fma(m01, x, fma(m11, y, fma(m21, z, m31)));
            m32 = fma(m02, x, fma(m12, y, fma(m22, z, m32)));
            m33 = fma(m03, x, fma(m13, y, fma(m23, z, m33)));
            properties = properties & ~(PROPERTY_PERSPECTIVE | PROPERTY_IDENTITY);
            return *this;
        }

        Matrix4& rotateGenericInternal(T ang, T x, T y, T z) {
            T s = sin(ang);
            T c = cos(ang);
            T C = 1.0f - c;
            T xx = x * x, xy = x * y, xz = x * z;
            T yy = y * y, yz = y * z;
            T zz = z * z;
            T rm00 = xx * C + c;
            T rm01 = xy * C + z * s;
            T rm02 = xz * C - y * s;
            T rm10 = xy * C - z * s;
            T rm11 = yy * C + c;
            T rm12 = yz * C + x * s;
            T rm20 = xz * C + y * s;
            T rm21 = yz * C - x * s;
            T rm22 = zz * C + c;
            T nm00 = m00 * rm00 + m10 * rm01 + m20 * rm02;
            T nm01 = m01 * rm00 + m11 * rm01 + m21 * rm02;
            T nm02 = m02 * rm00 + m12 * rm01 + m22 * rm02;
            T nm03 = m03 * rm00 + m13 * rm01 + m23 * rm02;
            T nm10 = m00 * rm10 + m10 * rm11 + m20 * rm12;
            T nm11 = m01 * rm10 + m11 * rm11 + m21 * rm12;
            T nm12 = m02 * rm10 + m12 * rm11 + m22 * rm12;
            T nm13 = m03 * rm10 + m13 * rm11 + m23 * rm12;
            m20 = m00 * rm20 + m10 * rm21 + m20 * rm22;
            m21 = m01 * rm20 + m11 * rm21 + m21 * rm22;
            m22 = m02 * rm20 + m12 * rm21 + m22 * rm22;
            m23 = m03 * rm20 + m13 * rm21 + m23 * rm22;
            m00 = nm00;
            m01 = nm01;
            m02 = nm02;
            m03 = nm03;
            m10 = nm10;
            m11 = nm11;
            m12 = nm12;
            m13 = nm13;
            m30 = m30;
            m31 = m31;
            m32 = m32;
            m33 = m33;
            properties = properties & ~(PROPERTY_PERSPECTIVE | PROPERTY_IDENTITY | PROPERTY_TRANSLATION);
            return *this;
        }

        Matrix4& rotationInternal(T angle, T x, T y, T z) {
            T sin = ::sin(angle), cos = ::cos(angle);
            T C = 1.0f - cos, xy = x * y, xz = x * z, yz = y * z;
            if ((properties & PROPERTY_IDENTITY) == 0)
                _identity();
            m00 = cos + x * x * C;
            m10 = xy * C - z * sin;
            m20 = xz * C + y * sin;
            m01 = xy * C + z * sin;
            m11 = cos + y * y * C;
            m21 = yz * C - x * sin;
            m02 = xz * C - y * sin;
            m12 = yz * C + x * sin;
            m22 = cos + z * z * C;
            properties = PROPERTY_AFFINE | PROPERTY_ORTHONORMAL;
            return *this;
        }

        Matrix4& rotateGeneric(T ang, T x, T y, T z) {
            if (y == 0.0 && z == 0.0 && abs(x) == 1.0)
                return rotateX(x * ang);
            else if (x == 0.0 && z == 0.0 && abs(y) == 1.0)
                return rotateY(y * ang);
            else if (x == 0.0 && y == 0.0 && abs(z) == 1.0)
                return rotateZ(z * ang);
            return rotateGenericInternal(ang, x, y, z);
        }

        Matrix4& rotateTranslationInternal(T ang, T x, T y, T z) {
            T s = sin(ang);
            T c = cos(ang);
            T C = 1.0f - c;
            T xx = x * x, xy = x * y, xz = x * z;
            T yy = y * y, yz = y * z;
            T zz = z * z;
            T rm00 = xx * C + c;
            T rm01 = xy * C + z * s;
            T rm02 = xz * C - y * s;
            T rm10 = xy * C - z * s;
            T rm11 = yy * C + c;
            T rm12 = yz * C + x * s;
            T rm20 = xz * C + y * s;
            T rm21 = yz * C - x * s;
            T rm22 = zz * C + c;
            m20 = rm20;
            m21 = rm21;
            m22 = rm22;
            m23 = 0.0f;
            m00 = rm00;
            m01 = rm01;
            m02 = rm02;
            m03 = 0.0f;
            m10 = rm10;
            m11 = rm11;
            m12 = rm12;
            m13 = 0.0f;
            m30 = m30;
            m31 = m31;
            m32 = m32;
            m33 = 1.0f;
            properties = properties & ~(PROPERTY_PERSPECTIVE | PROPERTY_IDENTITY | PROPERTY_TRANSLATION);
            return *this;
        }

        Matrix4& rotateAffineInternal(T ang, T x, T y, T z) {
            T s = sin(ang);
            T c = cos(ang);
            T C = 1.0f - c;
            T xx = x * x, xy = x * y, xz = x * z;
            T yy = y * y, yz = y * z;
            T zz = z * z;
            T rm00 = xx * C + c;
            T rm01 = xy * C + z * s;
            T rm02 = xz * C - y * s;
            T rm10 = xy * C - z * s;
            T rm11 = yy * C + c;
            T rm12 = yz * C + x * s;
            T rm20 = xz * C + y * s;
            T rm21 = yz * C - x * s;
            T rm22 = zz * C + c;
            // add temporaries for dependent values
            T nm00 = m00 * rm00 + m10 * rm01 + m20 * rm02;
            T nm01 = m01 * rm00 + m11 * rm01 + m21 * rm02;
            T nm02 = m02 * rm00 + m12 * rm01 + m22 * rm02;
            T nm10 = m00 * rm10 + m10 * rm11 + m20 * rm12;
            T nm11 = m01 * rm10 + m11 * rm11 + m21 * rm12;
            T nm12 = m02 * rm10 + m12 * rm11 + m22 * rm12;
            // set non-dependent values directly
            m20 = m00 * rm20 + m10 * rm21 + m20 * rm22;
            m21 = m01 * rm20 + m11 * rm21 + m21 * rm22;
            m22 = m02 * rm20 + m12 * rm21 + m22 * rm22;
            m23 = 0.0f;
            // set other values
            m00 = nm00;
            m01 = nm01;
            m02 = nm02;
            m03 = 0.0f;
            m10 = nm10;
            m11 = nm11;
            m12 = nm12;
            m13 = 0.0f;
            m30 = m30;
            m31 = m31;
            m32 = m32;
            m33 = 1.0f;
            properties = properties & ~(PROPERTY_PERSPECTIVE | PROPERTY_IDENTITY | PROPERTY_TRANSLATION);
            return *this;
        }

        Matrix4& rotateXInternal(T ang) {
            T sin = ::sin(ang), cos = ::cos(ang);
            T lm10 = m10, lm11 = m11, lm12 = m12, lm13 = m13, lm20 = m20, lm21 = m21, lm22 = m22, lm23 = m23;
            m20 = fma(lm10, -sin, lm20 * cos);
            m21 = fma(lm11, -sin, lm21 * cos);
            m22 = fma(lm12, -sin, lm22 * cos);
            m23 = fma(lm13, -sin, lm23 * cos);
            m10 = fma(lm10, cos, lm20 * sin);
            m11 = fma(lm11, cos, lm21 * sin);
            m12 = fma(lm12, cos, lm22 * sin);
            m13 = fma(lm13, cos, lm23 * sin);
            properties = properties & ~(PROPERTY_PERSPECTIVE | PROPERTY_IDENTITY | PROPERTY_TRANSLATION);
            return *this;
        }

        Matrix4& rotateYInternal(T ang) {
            T sin = ::sin(ang);
            T cos = ::cos(ang);
            // add temporaries for dependent values
            T nm00 = fma(m00, cos, m20 * -sin);
            T nm01 = fma(m01, cos, m21 * -sin);
            T nm02 = fma(m02, cos, m22 * -sin);
            T nm03 = fma(m03, cos, m23 * -sin);
            // set non-dependent values directly
            m20 = fma(m00, sin, m20 * cos);
            m21 = fma(m01, sin, m21 * cos);
            m22 = fma(m02, sin, m22 * cos);
            m23 = fma(m03, sin, m23 * cos);
            // set other values
            m00 = nm00;
            m01 = nm01;
            m02 = nm02;
            m03 = nm03;
            properties = properties & ~(PROPERTY_PERSPECTIVE | PROPERTY_IDENTITY | PROPERTY_TRANSLATION);
            return *this;
        }

        Matrix4& rotateZInternal(T ang) {
            T sin = ::sin(ang);
            T cos = ::cos(ang);
            return rotateTowardsXY(sin, cos);
        }

    public:
        Matrix4& setPerspective(T fovy, T aspect, T zNear, T zFar, bool zZeroToOne = false) {
            zero();
            T h = tan(fovy * 0.5f);
            m00 = 1.0f / (h * aspect);
            m11 = 1.0f / h;
            bool farInf = zFar > 0 && isinf(zFar);
            bool nearInf = zNear > 0 && isinf(zNear);
            if (farInf) {
                // See: "Infinite Projection Matrix" (http://www.terathon.com/gdc07_lengyel.pdf)
                T e = 1E-6f;
                m22 = e - 1.0f;
                m32 = (e - (zZeroToOne ? 1.0f : 2.0f)) * zNear;
            }
            else if (nearInf) {
                T e = 1E-6f;
                m22 = (zZeroToOne ? 0.0f : 1.0f) - e;
                m32 = ((zZeroToOne ? 1.0f : 2.0f) - e) * zFar;
            }
            else {
                m22 = (zZeroToOne ? zFar : zFar + zNear) / (zNear - zFar);
                m32 = (zZeroToOne ? zFar : zFar + zFar) * zNear / (zNear - zFar);
            }
            m23 = -1.0f;
            properties = PROPERTY_PERSPECTIVE;
            return *this;
        }

        Matrix4& perspective(T fovy, T aspect, T zNear, T zFar, bool zZeroToOne = false) {
            if ((properties & PROPERTY_IDENTITY) != 0)
                return setPerspective(fovy, aspect, zNear, zFar, zZeroToOne);
            return perspectiveGeneric(fovy, aspect, zNear, zFar, zZeroToOne);
        }

        Matrix4& translation(T x, T y, T z) {
            if ((properties & PROPERTY_IDENTITY) == 0)
                _identity();
            m30 = x;
            m31 = y;
            m32 = z;
            properties = PROPERTY_AFFINE | PROPERTY_TRANSLATION | PROPERTY_ORTHONORMAL;
            return *this;
        }

        Matrix4& translate(T x, T y, T z) {
            if ((properties & PROPERTY_IDENTITY) != 0)
                return translation(x, y, z);
            return translateGeneric(x, y, z);
        }

        Matrix4& setTranslation(T x, T y, T z) {
            m30 = x;
            m31 = y;
            m32 = z;
            properties = properties & ~(PROPERTY_PERSPECTIVE | PROPERTY_IDENTITY);
            return *this;
        }

        Matrix4& rotationX(T ang) {
            T sin = ::sin(ang), cos = ::cos(ang);
            if ((properties & PROPERTY_IDENTITY) == 0)
                _identity();
            m11 = cos;
            m12 = sin;
            m21 = -sin;
            m22 = cos;
            properties = PROPERTY_AFFINE | PROPERTY_ORTHONORMAL;
            return *this;
        }

        Matrix4& rotationY(T ang) {
            T sin = ::sin(ang), cos = ::cos(ang);
            if ((properties & PROPERTY_IDENTITY) == 0)
                _identity();
            m00 = cos;
            m02 = -sin;
            m20 = sin;
            m22 = cos;
            properties = PROPERTY_AFFINE | PROPERTY_ORTHONORMAL;
            return *this;
        }

        Matrix4& rotationZ(T ang) {
            T sin = ::sin(ang), cos = ::cos(ang);
            if ((properties & PROPERTY_IDENTITY) == 0)
                _identity();
            m00 = cos;
            m01 = sin;
            m10 = -sin;
            m11 = cos;
            properties = PROPERTY_AFFINE | PROPERTY_ORTHONORMAL;
            return *this;
        }

        Matrix4& rotation(T ang, T x, T y, T z) {
            if (y == 0.0 && z == 0.0 && abs(x) == 1.0)
                return rotationX(x * ang);
            else if (x == 0.0 && z == 0.0 && abs(y) == 1.0)
                return rotationY(y * ang);
            else if (x == 0.0 && y == 0.0 && abs(z) == 1.0)
                return rotationZ(z * ang);
            return rotationInternal(ang, x, y, z);
        }

        Matrix4& rotateTranslation(T ang, T x, T y, T z) {
            T tx = m30, ty = m31, tz = m32;
            if (y == 0.0f && z == 0.0f && (abs(x)) == 1.0)
                return rotationX(x * ang).setTranslation(tx, ty, tz);
            else if (x == 0.0f && z == 0.0f && abs(y) == 1.0)
                return rotationY(y * ang).setTranslation(tx, ty, tz);
            else if (x == 0.0f && y == 0.0f && abs(z) == 1.0)
                return rotationZ(z * ang).setTranslation(tx, ty, tz);
            return rotateTranslationInternal(ang, x, y, z);
        }

        Matrix4& rotateX(T ang) {
            if ((properties & PROPERTY_IDENTITY) != 0)
                return rotationX(ang);
            else if ((properties & PROPERTY_TRANSLATION) != 0) {
                T x = m30, y = m31, z = m32;
                return rotationX(ang).setTranslation(x, y, z);
            }
            return rotateXInternal(ang);
        }

        Matrix4& rotateY(T ang) {
            if ((properties & PROPERTY_IDENTITY) != 0)
                return rotationY(ang);
            else if ((properties & PROPERTY_TRANSLATION) != 0) {
                T x = m30, y = m31, z = m32;
                return rotationY(ang).setTranslation(x, y, z);
            }
            return rotateYInternal(ang);
        }

        Matrix4& rotateZ(T ang) {
            if ((properties & PROPERTY_IDENTITY) != 0)
                return rotationZ(ang);
            else if ((properties & PROPERTY_TRANSLATION) != 0) {
                T x = m30, y = m31, z = m32;
                return rotationZ(ang).setTranslation(x, y, z);
            }
            return rotateZInternal(ang);
        }

        Matrix4& rotateAffine(T ang, T x, T y, T z) {
            if (y == 0.0f && z == 0.0f && abs(x) == 1.0)
                return rotateX(x * ang);
            else if (x == 0.0f && z == 0.0f && abs(y) == 1.0)
                return rotateY(y * ang);
            else if (x == 0.0f && y == 0.0f && abs(z) == 1.0)
                return rotateZ(z * ang);
            return rotateAffineInternal(ang, x, y, z);
        }

        Matrix4& rotate(T ang, T x, T y, T z) {
            if ((properties & PROPERTY_IDENTITY) != 0)
                return rotation(ang, x, y, z);
            else if ((properties & PROPERTY_TRANSLATION) != 0)
                return rotateTranslation(ang, x, y, z);
            else if ((properties & PROPERTY_AFFINE) != 0)
                return rotateAffine(ang, x, y, z);
            return rotateGeneric(ang, x, y, z);
        }

        Matrix4& rotateTowardsXY(T dirX, T dirY) {
            if ((properties & PROPERTY_IDENTITY) != 0)
                return rotationTowardsXY(dirX, dirY);
            T nm00 = fma(m00, dirY, m10 * dirX);
            T nm01 = fma(m01, dirY, m11 * dirX);
            T nm02 = fma(m02, dirY, m12 * dirX);
            T nm03 = fma(m03, dirY, m13 * dirX);
            m10 = fma(m00, -dirX, m10 * dirY);
            m11 = fma(m01, -dirX, m11 * dirY);
            m12 = fma(m02, -dirX, m12 * dirY);
            m13 = fma(m03, -dirX, m13 * dirY);
            m00 = nm00;
            m01 = nm01;
            m02 = nm02;
            m03 = nm03;
            properties = properties & ~(PROPERTY_PERSPECTIVE | PROPERTY_IDENTITY | PROPERTY_TRANSLATION);
            return *this;
        }
    };

    using Matrix4f = Matrix4<float>;
}
