#pragma once

#include <math.h>
#include <string>

#include <vector>

#define PI 3.14159265358979323846264338327950288419716939937510582

#define TO_RADIANS(x) (float) (x * PI / 180.0f)
#define TO_DEGREES(x) (float) (x * 180.0f / PI)

#define SQRT(x) (float) (sqrt(x))
#define POW(x) (float) (pow(x))

#define COS(x) (float) (cos(x))
#define SIN(x) (float) (sin(x))
#define TAN(x) (float) (tan(x))

#define ACOS(x) (float) (acos(x))
#define ASIN(x) (float) (asin(x))
#define ATAN(x) (float) (atan(x))
#define ATAN2(x) (float) (atan2(x))

#define LERP(a, b, t) (float) (a + (b - a) * t)

#define MAX(a, b) (float) (a > b ? a : b)
#define MIN(a, b) (float) (a < b ? a : b)

namespace maths
{
    class vec2;
    class vec3;
    class vec4;
    class color4f;
    class mat4;
    class quat;

    class vec2
    {
        public:
            vec2();
            vec2(const float v);
            vec2(const float x, const float y);
            vec2(const vec2& v);
            void operator =(const vec2& v);

            vec2 copy() const;

            void set(const float x, const float y);

            float magnitude() const;
            vec2 normalize() const;
            float dot(const vec2& v) const;

            std::string toString() const;

            friend std::ostream& operator <<(std::ostream& out, const vec2& v);

            vec2 negate();

            vec2 add(const vec2& v);
            vec2 add(const float v);
            vec2 operator +(const float v) const;
            vec2 operator +(const vec2& v) const;
            vec2 operator +=(const vec2& v);

            vec2 sub(const vec2& v);
            vec2 sub(const float v);
            vec2 operator -(const float v) const;
            vec2 operator -(const vec2& v) const;
            vec2 operator -=(const vec2& v);

            vec2 mul(const vec2& v);
            vec2 mul(const float v);
            vec2 operator *(const float v) const;
            vec2 operator *(const vec2& v) const;
            vec2 operator *=(const vec2& v);

            vec2 div(const vec2& v);
            vec2 div(const float v);
            vec2 operator /(const float v) const;
            vec2 operator /(const vec2& v) const;
            vec2 operator /=(const vec2& v);

            std::vector<float> values();

            float x;
            float y;
    };

    class vec3
    {
        public:
            vec3();
            vec3(const float v);
            vec3(const float x, const float y, const float z);
            vec3(const vec3& v);
            void operator =(const vec3& v);

            vec3 copy() const;

            int pack() const;
            vec3 unpack(const int& index);

            void set(const float x, const float y, const float z);

            float magnitude() const;
            vec3 normalize() const;
            float dot(const vec3& r) const;

            vec3 cross(const vec3& r);
            vec3 check();
            vec3 rotate(quat& rot);
            vec3 reflect(const vec3& normal);

            std::string toString() const;
            friend std::ostream& operator <<(std::ostream& out, const vec3& v);

            vec3 negate();

            vec3 add(const vec3& v);
            vec3 add(const float v);
            vec3 operator +(const float v) const;
            vec3 operator +(const vec3& v) const;
            vec3 operator +=(const vec3& v);

            vec3 sub(const vec3& v);
            vec3 sub(const float v);
            vec3 operator -(const float v) const;
            vec3 operator -(const vec3& v) const;
            vec3 operator -=(const vec3& v);

            vec3 mul(const vec3& v);
            vec3 mul(const float v);
            vec3 operator *(const float v) const;
            vec3 operator *(const vec3& v) const;
            vec3 operator *=(const vec3& v);

            vec3 div(const vec3& v);
            vec3 div(const float v);
            vec3 operator /(const float v) const;
            vec3 operator /(const vec3& v) const;
            vec3 operator /=(const vec3& v);

            std::vector<float> values();

            float x;
            float y;
            float z;
    };

    class vec4
    {
        public:
            vec4();
            vec4(const float v);
            vec4(const float x, const float y, const float z, const float w);
            vec4(const vec4& v);
            void operator =(const vec4& v);

            vec4 copy() const;

            void set(const float x, const float y, const float z, const float w);

            float magnitude() const;
            vec4 normalize() const;

            std::string toString() const;
            friend std::ostream& operator <<(std::ostream& out, const vec4& v);

            vec4 negate();

            vec4 add(const vec4& v);
            vec4 add(const float v);
            vec4 operator +(const float v) const;
            vec4 operator +(const vec4& v) const;
            vec4 operator +=(const vec4& v);

            vec4 sub(const vec4& v);
            vec4 sub(const float v);
            vec4 operator -(const float v) const;
            vec4 operator -(const vec4& v) const;
            vec4 operator -=(const vec4& v);

            vec4 mul(const vec4& v);
            vec4 mul(const float v);
            vec4 operator *(const float v) const;
            vec4 operator *(const vec4& v) const;
            vec4 operator *=(const vec4& v);

            vec4 div(const vec4& v);
            vec4 div(const float v);
            vec4 operator /(const float v) const;
            vec4 operator /(const vec4& v) const;
            vec4 operator /=(const vec4& v);

            std::vector<float> values();

            float x;
            float y;
            float z;
            float w;
    };

    class mat4
    {
        public:
            mat4 mul(const mat4& m);

            static mat4 identity();
            static mat4 translate(const float x, const float y, const float z);
            static mat4 translate(const vec3& pos);
            static mat4 rotate(float x, float y, float z);
            static mat4 rotate(const vec3& forward,const vec3& up, const vec3& right);
            static mat4 scale(const float x, const float y, const float z);

            static mat4 cameraView(const vec3& forward, const vec3& up);
            static mat4 perspective(const float fov, const float aspect, const float near, const float far);
            static mat4 orthographic(const float right, const float left, const float top, const float bottom, const float near, const float far);
            static mat4 inverse(const mat4& m);
            static float determinant(const mat4& m);
            static float determinant3x3(const float t00, const float t01, const float t02,
                                        const float t10, const float t11, const float t12,
                                        const float t20, const float t21, const float t22);

            static vec3 transform(const mat4& m, const vec3& v);
            static vec4 transform(const mat4& m, const vec4& v);

            float mat(const int x, const int y) const;

            std::vector<float> values();

            float m_matrix[4 * 4];
    };

    class quat
    {
        public:
            quat();
            quat(const quat& q);
            quat(const float x, const float y, const float z, const float w);
            quat(const vec3& axis, const float angle);

            quat copy() const;

            void set(const float x, const float y, const float z, const float w);

            float magnitude() const;
            float dot(const quat& r) const;
            quat normalize() const;
            quat conjugate() const;

            quat mul(const quat& r);
            quat mul(const vec3& r);

            vec3 getForward();
            vec3 getBack();
            vec3 getRight();
            vec3 getLeft();
            vec3 getUp();
            vec3 getDown();

            mat4 toMatrix();

            std::vector<float> values();

            float x;
            float y;
            float z;
            float w;
    };

    class transform
    {
        public:
            transform();

            void rotate(const vec3& axis, const float angle);

            vec3 getForward();
            vec3 getUp();
            vec3 getRight();

            mat4 toMatrix();

            vec3  pos;
            quat  rot;
    };
}
