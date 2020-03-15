#include "core/maths/maths.h"

using namespace maths;

vec4::vec4()
:
    x(0),
    y(0),
    z(0),
    w(0)
{}

vec4::vec4(const float v)
:
    x(v),
    y(v),
    z(v),
    w(v)
{}


vec4::vec4(const float x, const float y, const float z, const float w)
:
    x(x),
    y(y),
    z(z),
    w(w)
{}

vec4::vec4(const vec4& v)
:
    x(v.x),
    y(v.y),
    z(v.z),
    w(v.w)
{}


void vec4::operator =(const vec4& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
}

vec4 vec4::copy() const
{
    return vec4(x, y, z, w);
}

void vec4::set(const float x, const float y, const float z, const float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

float vec4::magnitude() const
{
    return SQRT(x * x + y * y + z * z + w * w);
}

vec4 vec4::normalize() const
{
    float m = magnitude();

    return vec4(x / m, y / m, z / m, w / m);
}

std::string vec4::toString() const
{
    return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + " " + std::to_string(w);
}

std::ostream& maths::operator <<(std::ostream& out, const vec4& v)
{
    out << v.toString();

    return out;
}

vec4 vec4::negate()
{
    x *= -1;
    y *= -1;
    z *= -1;
    w *= -1;

    return copy();
}

vec4 vec4::add(const vec4& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;

    return copy();
}

vec4 vec4::add(const float v)
{
    x += v;
    y += v;
    z += v;
    w += v;

    return copy();
}

vec4 vec4::operator +(const float v) const
{
    return vec4(x + v, y + v, z + v, w + v);
}

vec4 vec4::operator +(const vec4& v) const
{
    return vec4(x + v.x, y + v.y, z + v.z, w + v.w);
}

vec4 vec4::operator +=(const vec4& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;

    return *this;
}

vec4 vec4::sub(const vec4& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;

    return copy();
}

vec4 vec4::sub(const float v)
{
    x -= v;
    y -= v;
    z -= v;
    w -= v;

    return copy();
}

vec4 vec4::operator -(const float v) const
{
    return vec4(x - v, y - v, z - v, w - v);
}

vec4 vec4::operator -(const vec4& v) const
{
    return vec4(x - v.x, y - v.y, z - v.z, w - v.w);
}

vec4 vec4::operator -=(const vec4& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;

    return *this;
}

vec4 vec4::mul(const vec4& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;

    return copy();
}

vec4 vec4::mul(const float v)
{
    x *= v;
    y *= v;
    z *= v;
    w *= v;

    return copy();
}

vec4 vec4::operator *(const float v) const
{
    return vec4(x * v, y * v, z * v, w * v);
}

vec4 vec4::operator *(const vec4& v) const
{
    return vec4(x * v.x, y * v.y, z * v.z, w * v.w);
}

vec4 vec4::operator *=(const vec4& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;

    return *this;
}

vec4 vec4::div(const vec4& v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;

    return copy();
}

vec4 vec4::div(const float v)
{
    x /= v;
    y /= v;
    z /= v;
    w /= v;

    return copy();
}

vec4 vec4::operator /(const float v) const
{
    return vec4(x / v, y / v, z / v, w / v);
}

vec4 vec4::operator /(const vec4& v) const
{
    return vec4(x / v.x, y / v.y, z / v.z, w / v.w);
}

vec4 vec4::operator /=(const vec4& v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.z;

    return *this;
}

std::vector<float> vec4::values()
{
    return std::vector<float>
    {
        x, y, z, w
    };
}
