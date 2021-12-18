#pragma once

//No need of double quaternions
//todo:: Just a conversion beetwen leap and imo
struct Quaternion
{
public:
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