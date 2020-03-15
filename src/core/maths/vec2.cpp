#include "core/maths/maths.h"

using namespace maths;

vec2::vec2()
:
    x(0),
    y(0)
{}

vec2::vec2(const float v)
:
    x(v),
    y(v)
{}


vec2::vec2(const float x, const float y)
:
    x(x),
    y(y)
{}

vec2::vec2(const vec2& v)
:
    x(v.x),
    y(v.y)
{}


void vec2::operator =(const vec2& v)
{
    x = v.x;
    y = v.y;
}

vec2 vec2::copy() const
{
    return vec2(x, y);
}

void vec2::set(const float x, const float y)
{
    this->x = x;
    this->y = y;
}

float vec2::magnitude() const
{
    return SQRT(x * x + y * y);
}

vec2 vec2::normalize() const
{
    return vec2(x / magnitude(), y / magnitude());
}

float vec2::dot(const vec2& v) const
{
    return x * v.x + y * v.y;
}

std::string vec2::toString() const
{
    return std::to_string(x) + " " + std::to_string(y);
}

std::ostream& maths::operator <<(std::ostream& out, const vec2& v)
{
    out << v.toString();

    return out;
}

vec2 vec2::negate()
{
    x *= -1;
    y *= -1;

    return copy();
}

vec2 vec2::add(const vec2&v)
{
    x += v.x;
    y += v.y;

    return copy();
}

vec2 vec2::add(const float v)
{
    x += v;
    y += v;

    return copy();
}

vec2 vec2::operator +(const float v) const
{
    return vec2(x + v, y + v);
}

vec2 vec2::operator +(const vec2& v) const
{
    return vec2(x + v.x, y + v.y);
}

vec2 vec2::operator +=(const vec2& v)
{
    add(v);

    return *this;
}

vec2 vec2::sub(const vec2&v)
{
    x -= v.x;
    y -= v.y;

    return copy();
}

vec2 vec2::sub(const float v)
{
    x -= v;
    y -= v;

    return copy();
}

vec2 vec2::operator -(const float v) const
{
    return vec2(x - v, y - v);
}

vec2 vec2::operator -(const vec2& v) const
{
    return vec2(x - v.x, y - v.y);
}

vec2 vec2::operator -=(const vec2& v)
{
    sub(v);

    return *this;
}

vec2 vec2::mul(const vec2&v)
{
    x *= v.x;
    y *= v.y;

    return copy();
}


vec2 vec2::mul(const float v)
{
    x *= v;
    y *= v;

    return copy();
}

vec2 vec2::operator *(const float v) const
{
    return vec2(x * v, y * v);
}

vec2 vec2::operator *(const vec2& v) const
{
    return vec2(x * v.x, y * v.y);
}

vec2 vec2::operator *=(const vec2& v)
{
    mul(v);

    return *this;
}

vec2 vec2::div(const vec2&v)
{
    x /= v.x;
    y /= v.y;

    return copy();
}

vec2 vec2::div(const float v)
{
    x /= v;
    y /= v;

    return copy();
}

vec2 vec2::operator /(const float v) const
{
    return vec2(x / v, y / v);
}

vec2 vec2::operator /(const vec2& v) const
{
    return vec2(x / v.x, y / v.y);
}

vec2 vec2::operator /=(const vec2& v)
{
    div(v);

    return *this;
}

std::vector<float> vec2::values()
{
    return std::vector<float>
    {
        x, y
    };
}
