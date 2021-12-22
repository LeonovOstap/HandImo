#pragma once
#include <string>

#include "Math/MathTypes.h"

struct BVHBoneData
{
    
public:
    explicit BVHBoneData(std::string InName, BVHBoneData* Parent)
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
    
    BVHBoneData* ParentBone;
    
    BVHBoneData* Children[5];
    int NumChildren = 0;

    std::string BoneName;
};



namespace Bones
{
    static BVHBoneData ARM = BVHBoneData("Arm", nullptr);

    //Thumb
    static BVHBoneData THUMB_Metacarpal = BVHBoneData("Thumb", &ARM);
    static BVHBoneData THUMB_Proximal = BVHBoneData("ThumbProximal", &THUMB_Metacarpal);
    static BVHBoneData THUMB_Intermediate = BVHBoneData("ThumbIntermediate", &THUMB_Proximal);
    static BVHBoneData THUMB_Distal = BVHBoneData("ThumbDistal", &THUMB_Intermediate);
    
    //Index
    static BVHBoneData INDEX_Metacarpal = BVHBoneData("Index", &ARM);
    static BVHBoneData INDEX_Proximal = BVHBoneData("IndexProximal", &INDEX_Metacarpal);
    static BVHBoneData INDEX_Intermediate = BVHBoneData("IndexIntermediate", &INDEX_Proximal);
    static BVHBoneData INDEX_Distal = BVHBoneData("IndexDistal", &INDEX_Intermediate);

    //Middle
    static BVHBoneData MIDDLE_Metacarpal = BVHBoneData("Middle", &ARM);
    static BVHBoneData MIDDLE_Proximal = BVHBoneData("MiddleProximal", &MIDDLE_Metacarpal);
    static BVHBoneData MIDDLE_Intermediate = BVHBoneData("MiddleIntermediate", &MIDDLE_Proximal);
    static BVHBoneData MIDDLE_Distal = BVHBoneData("MiddleDistal", &MIDDLE_Intermediate);

    //Ring
    static BVHBoneData RING_Metacarpal = BVHBoneData("Ring", &ARM);
    static BVHBoneData RING_Proximal = BVHBoneData("RingProximal", &RING_Metacarpal);
    static BVHBoneData RING_Intermediate = BVHBoneData("RingIntermediate", &RING_Proximal);
    static BVHBoneData RING_Distal = BVHBoneData("RingDistal", &RING_Intermediate);

    //Pinkie
    static BVHBoneData PINKIE_Metacarpal = BVHBoneData("Pinkie", &ARM);
    static BVHBoneData PINKIE_Proximal = BVHBoneData("PinkieProximal", &PINKIE_Metacarpal);
    static BVHBoneData PINKIE_Intermediate = BVHBoneData("PinkieIntermediate", &PINKIE_Proximal);
    static BVHBoneData PINKIE_Distal = BVHBoneData("PinkieDistal", &PINKIE_Intermediate);
}