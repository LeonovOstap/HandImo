#pragma once
#include "MathTypes.h"
#include "MathDefines.h"

template<typename  T>
T RadiansToDegrees(T Rad)
{
    return Rad * 180/PI;
}