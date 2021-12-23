#pragma once
#include <LeapC.h>

#include "../Math/MathTypes.h"


//Union messes up memcpy, or whatever. IE cant copy by data pointer (v)
inline Vector3f Leap2ImoVec(LEAP_VECTOR LeapVector)
{
    return {LeapVector.x, LeapVector.y, LeapVector.z};
}

inline Quaternion Leap2ImoQuat(LEAP_QUATERNION LeapQuat)
{
    return {LeapQuat.x, LeapQuat.y, LeapQuat.z, LeapQuat.w};
}
