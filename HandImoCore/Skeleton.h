#pragma once
#include <string>

#include "Math/MathTypes.h"

struct Joint
{
    
public:
    explicit Joint(std::string InName, Joint* Parent)
    {
        ParentBone = Parent;
        BoneName = InName;

        //dumb, yes
        //working, also yes
        if(ParentBone)
        {
            ParentBone->Children[ParentBone->NumChildren] = this;
            ParentBone->NumChildren = ParentBone->NumChildren+1;
        }
        
    }
    
    Joint* ParentBone;
    
    Joint* Children[5];
    int NumChildren = 0;

    std::string BoneName;
};



namespace Skeleton
{
    static Joint ARM = Joint("Arm", nullptr);

    //Thumb
    static Joint THUMB_Metacarpal = Joint("Thumb", &ARM);
    static Joint THUMB_Proximal = Joint("ThumbProximal", &THUMB_Metacarpal);
    static Joint THUMB_Intermediate = Joint("ThumbIntermediate", &THUMB_Proximal);
    static Joint THUMB_Distal = Joint("ThumbDistal", &THUMB_Intermediate);
    
    //Index
    static Joint INDEX_Metacarpal = Joint("Index", &ARM);
    static Joint INDEX_Proximal = Joint("IndexProximal", &INDEX_Metacarpal);
    static Joint INDEX_Intermediate = Joint("IndexIntermediate", &INDEX_Proximal);
    static Joint INDEX_Distal = Joint("IndexDistal", &INDEX_Intermediate);

    //Middle
    static Joint MIDDLE_Metacarpal = Joint("Middle", &ARM);
    static Joint MIDDLE_Proximal = Joint("MiddleProximal", &MIDDLE_Metacarpal);
    static Joint MIDDLE_Intermediate = Joint("MiddleIntermediate", &MIDDLE_Proximal);
    static Joint MIDDLE_Distal = Joint("MiddleDistal", &MIDDLE_Intermediate);

    //Ring
    static Joint RING_Metacarpal = Joint("Ring", &ARM);
    static Joint RING_Proximal = Joint("RingProximal", &RING_Metacarpal);
    static Joint RING_Intermediate = Joint("RingIntermediate", &RING_Proximal);
    static Joint RING_Distal = Joint("RingDistal", &RING_Intermediate);

    //Pinkie
    static Joint PINKIE_Metacarpal = Joint("Pinkie", &ARM);
    static Joint PINKIE_Proximal = Joint("PinkieProximal", &PINKIE_Metacarpal);
    static Joint PINKIE_Intermediate = Joint("PinkieIntermediate", &PINKIE_Proximal);
    static Joint PINKIE_Distal = Joint("PinkieDistal", &PINKIE_Intermediate);
}