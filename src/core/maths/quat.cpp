#include "core/maths/maths.h"

using namespace maths;

quat::quat()
:
    x(0),
    y(0),
    z(0),
    w(1)
{}


quat::quat(const quat& q)
:
    x(q.x),
    y(q.y),
    z(q.z),
    w(q.w)
{}


quat::quat(const float x, const float y, const float z, const float w)
:
    x(x),
    y(y),
    z(z),
    w(w)
{}


quat::quat(const vec3& axis, const float angle)
{
    float s = SIN(angle / 2.0f);
    float c = COS(angle / 2.0f);

    this->x = axis.x * s;
    this->y = axis.y * s;
    this->z = axis.z * s;
    this->w = c;
}

quat quat::copy() const
{
    return quat(x, y, z, w);
}

void quat::set(const float x, const float y, const float z, const float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

float quat::magnitude() const
{
    return SQRT(x * x + y * y + z * z + w * w);
}

float quat::dot(const quat& r) const
{
	 return x * r.x + y * r.y + z * r.z + w * r.w;
}

quat quat::normalize() const
{
    float mag = magnitude();

    return quat(x / mag, y / mag, z / mag, w / mag);
}

quat quat::conjugate() const
{
    return quat(-x, -y, -z, w);
}

quat quat::mul(const quat& r)
{
  	quat result;

  	result.w = w * r.w - x * r.x - y * r.y - z * r.z;
  	result.x = x * r.w + w * r.x + y * r.z - z * r.y;
  	result.y = y * r.w + w * r.y + z * r.x - x * r.z;
  	result.z = z * r.w + w * r.z + x * r.y - y * r.x;

  	return result;
}

quat quat::mul(const vec3& r)
{
  	quat result;

  	result.w = -x * r.x - y * r.y - z * r.z;
  	result.x = w * r.x + y * r.z - z * r.y;
  	result.y = w * r.y + z * r.x - x * r.z;
  	result.z = w * r.z + x * r.y - y * r.x;

  	return result;
}

vec3 quat::getForward()
{
    vec3 result(0, 0, 1);

    return result.rotate(*this);
}

vec3 quat::getBack()
{
    vec3 result(0, 0, -1);

    return result.rotate(*this);
}

vec3 quat::getRight()
{
    vec3 result(1, 0, 0);

    return result.rotate(*this);
}

vec3 quat::getLeft()
{
    vec3 result(-1, 0, 0);

    return result.rotate(*this);
}

vec3 quat::getUp()
{
    vec3 result(0, 1, 0);

    return result.rotate(*this);
}

vec3 quat::getDown()
{
    vec3 result(0, -1, 0);

    return result.rotate(*this);
}

mat4 quat::toMatrix()
{
  	vec3 forward(2.0f * (x * z - w * y), 2.0f * (y * z + w * x), 1.0f - 2.0f * (x * x + y * y));
  	vec3 up(2.0f * (x * y + w * z), 1.0f - 2.0f * (x * x + z * z), 2.0f * (y * z - w * x));
  	vec3 right(1.0f - 2.0f * (y * y + z * z), 2.0f * (x * y - w * z), 2.0f * (x * z + w * y));

  	return mat4::rotate(forward, up, right);
}

std::vector<float> quat::values()
{
    return std::vector<float>
    {
        x, y, z, w
    };
}
