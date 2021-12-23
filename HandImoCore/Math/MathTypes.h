#pragma once
#include "VectorBase.h"
#include "Matrix/Matrix.h"


//Vector defines
using Vector2 = MathImpl::VectorBase<float, 2>;
using Vector3f = MathImpl::VectorBase<float, 3>;
using Vector3d = MathImpl::VectorBase<double, 3>;

using Vector4f = MathImpl::VectorBase<float, 4>;
using Vector4d = MathImpl::VectorBase<float, 4>;


//Matrix
using Matrix4x4f = MathImpl::Matrix4Base<float>;
using Matrix4x4d = MathImpl::Matrix4Base<double>;