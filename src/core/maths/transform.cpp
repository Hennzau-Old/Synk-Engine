#include "core/maths/maths.h"

using namespace maths;

transform::transform()
:
    pos(vec3()),
    rot(quat())
{ }


void transform::rotate(const vec3& axis, const float angle)
{
    rot = rot.mul(quat(axis, TO_RADIANS(angle)).normalize());
}

vec3 transform::getForward()
{
    return rot.getForward();
}

vec3 transform::getUp()
{
    return rot.getUp();
}

vec3 transform::getRight()
{
    return rot.getRight();
}

mat4 transform::toMatrix()
{
		mat4 translationMatrix = mat4::translate(pos);
		mat4 rotationMatrix = rot.toMatrix();

		mat4 transformationMatrix = rotationMatrix.mul(translationMatrix);

		return transformationMatrix;
}
