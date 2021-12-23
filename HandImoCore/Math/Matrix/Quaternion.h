#pragma once

//No need of double quaternions
struct Quaternion
{
    Quaternion();

    Quaternion(float x, float y, float z, float w)
    : X(x),
      Y(y),
      Z(z),
      W(w)
    {
    }

    Quaternion(float* data)
    {
       // assert(sizeof(data) < sizeof(float) * 4, "Incorrect data size passed");
        memcpy(Q, data, sizeof(data));
    }

    //Can be Vector4?
    union
    {
        float Q[4];
        struct
        {
            float X;
            float Y;
            float Z;
            float W;
        };
    };
    
};