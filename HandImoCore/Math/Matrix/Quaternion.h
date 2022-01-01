#pragma once
#include "../MathDefines.h"

//No need of double quaternions
struct Quaternion
{
    Quaternion() : X(0),
                   Y(0),
                   Z(0),
                   W(1)
    {}

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

    float Norm() const
    {
        return X*X + Y*Y + Z*Z + W*W;
    }

    void ForceIdentity()
    {
        X = 0;
        Y = 0;
        Z = 0;

        W = 1;
    }

    static Quaternion Identity()
    {
        return Quaternion();
    }

    void ToEuler(float& Yaw, float &Pitch, float& Roll)
    {
        const double W2 = W*W;
        const double X2 = X*X;
        const double Y2 = Y*Y;
        const double Z2 = Z*Z;
        const double unitLength = W2 + X2 + Y2 + Z2;    // Normalised == 1, otherWise correction divisor.
        const double abcd = W*X + Y*Z;
        if (abcd > (0.5-K_EPSILON)*unitLength)
        {
            Yaw = 2 * atan2(Y, W);
            Pitch = PI;
            Roll = 0;
        }
        else if (abcd < (-0.5+K_EPSILON)*unitLength)
        {
            Yaw = -2 * atan2(Y, W);
            Pitch = -PI;
            Roll = 0;
        }
        else
        {
            const double adbc = W*Z - X*Y;
            const double acbd = W*Y - X*Z;
            Yaw = atan2(2*adbc, 1 - 2*(Z2+X2));
            Pitch = asin(2*abcd/unitLength);
            Roll = atan2(2*acbd, 1 - 2*(Y2+X2));
        }
    }

    bool operator==(const Quaternion& Other)
    {
        return Q == Other.Q;
    }

    bool operator!=(const Quaternion& Other)
    {
        return Q != Other.Q;
    }

    Quaternion operator - (const Quaternion& q) const
    {
        return Quaternion(X - q.X, Y - q.Y, Z - q.Z, W - q.W);
    }
    
    Quaternion operator + (const Quaternion& q) const
    {
        return Quaternion(X + q.X, Y + q.Y, Z + q.Z, W + q.W);
    }

    std::string ToString() const
    {
        return std::string(std::to_string(X) + " " + std::to_string(Y) + " " + std::to_string(Z) + " " + std::to_string(W));
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
