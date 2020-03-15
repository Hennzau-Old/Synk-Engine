#include "core/maths/maths.h"

using namespace maths;

vec3::vec3()
:
    x(0),
    y(0),
    z(0)
{}

vec3::vec3(const float v)
:
    x(v),
    y(v),
    z(v)
{}


vec3::vec3(const float x, const float y, const float z)
:
    x(x),
    y(y),
    z(z)
{}

vec3::vec3(const vec3& v)
:
    x(v.x),
    y(v.y),
    z(v.z)
{}

void vec3::operator =(const vec3& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
}

vec3 vec3::copy() const
{
    return vec3(x, y, z);
}

int vec3::pack() const
{
    return (int) x << 16 | (int) y << 8 | (int) z;
}

vec3 vec3::unpack(const int& index)
{
    int x = (index & 0xff0000) >> 16;
    int y = (index & 0xff00) >> 8;
    int z = (index & 0xff);

    return vec3(x, y, z);
}

void vec3::set(const float x, const float y, const float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

float vec3::magnitude() const
{
    return sqrt(x * x + y * y + z * z);
}

vec3 vec3::normalize() const
{
    float m = magnitude();

    return vec3(x / m, y / m, z / m);
}

vec3 vec3::cross(const vec3& r)
{
    vec3 result;

    result.x = y * r.z - z * r.y;
  	result.y = z * r.x - x * r.z;
  	result.z = x * r.y - y * r.x;

    return result;
}

vec3 vec3::check()
{
		float max = MAX(MAX(x, y), z);
		float min = MIN(MIN(x, y), z);

		float absMax = abs(max - 1.0f);
		float absMin = abs(min);

		float v = 0.0f;

    if (absMax > absMin)
		  v = min;
		else
		  v = max;

    int rv = 1;

		if (v < 0.5f)
		  rv = -1;

		return vec3(v != x ? 0 : rv, v != y ? 0 : rv, v != z ? 0 : rv);
}

vec3 vec3::rotate(quat& rot)
{
   quat w = rot.mul(copy());
   w = w.mul(rot.conjugate());

   return vec3(w.x, w.y, w.z);
}

float vec3::dot(const vec3& r) const
{
    return x * r.x + y * r.y + z * r.z;
}

vec3 vec3::reflect(const vec3& normal)
{
    return copy() - (normal.copy() * (dot(normal) * 2.0f));
}

std::string vec3::toString() const
{
    return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z);
}

std::ostream& maths::operator <<(std::ostream& out, const vec3& v)
{
    out << v.toString();

    return out;
}

vec3 vec3::negate()
{
    x *= -1;
    y *= -1;
    z *= -1;

    return copy();
}

vec3 vec3::add(const vec3& v)
{
    x += v.x;
    y += v.y;
    z += v.z;

    return copy();
}

vec3 vec3::add(const float v)
{
    x += v;
    y += v;
    z += v;

    return copy();
}

vec3 vec3::operator +(const float v) const
{
    return vec3(x + v, y + v, z + v);
}

vec3 vec3::operator +(const vec3& v) const
{
    return vec3(x + v.x, y + v.y, z + v.z);
}

vec3 vec3::operator +=(const vec3& v)
{
    this->add(v);

    return *this;
}

vec3 vec3::sub(const vec3& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;

    return copy();
}

vec3 vec3::sub(const float v)
{
    x -= v;
    y -= v;
    z -= v;

    return copy();
}

vec3 vec3::operator -(const float v) const
{
    return vec3(x - v, y - v, z - v);
}

vec3 vec3::operator -(const vec3& v) const
{
    return vec3(x - v.x, y - v.y, z - v.z);
}

vec3 vec3::operator -=(const vec3& v)
{
    this->sub(v);

    return *this;
}

vec3 vec3::mul(const vec3& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;

    return copy();
}

vec3 vec3::mul(const float v)
{
    x *= v;
    y *= v;
    z *= v;

    return copy();
}

vec3 vec3::operator *(const float v) const
{
    return vec3(x * v, y * v, z * v);
}

vec3 vec3::operator *(const vec3& v) const
{
    return vec3(x * v.x, y * v.y, z * v.z);
}

vec3 vec3::operator *=(const vec3& v)
{
    this->mul(v);

    return *this;
}

vec3 vec3::div(const vec3& v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;

    return copy();
}

vec3 vec3::div(const float v)
{
    x /= v;
    y /= v;
    z /= v;

    return copy();
}

vec3 vec3::operator /(const float v) const
{
    return vec3(x / v, y / v, z / v);
}

vec3 vec3::operator /(const vec3& v) const
{
    return vec3(x / v.x, y / v.y, z / v.z);
}

vec3 vec3::operator /=(const vec3& v)
{
    this->div(v);

    return *this;
}

std::vector<float> vec3::values()
{
    return std::vector<float>
    {
        x, y, z
    };
}
