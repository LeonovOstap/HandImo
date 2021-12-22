#pragma once
#include <functional>

#include "Bones.h"
#include "Math/MathTypes.h"
#include "Leap/LeapInterleave.h"
struct RecordingBulkData;


namespace BVH
{
    void WriteToFile(const char* DirectoryPath, const char* Prefix, RecordingBulkData* BulkData);

    namespace util
    {
        void WriteJointData(BVHBoneData* Root, ofstream& Stream, LEAP_HAND& Frame);
        void WriteFrameMotionData(BVHBoneData* Root, ofstream& Stream, LEAP_HAND& Frame);
    }
    
}
