#pragma once
#include <functional>

#include "Skeleton.h"
#include "Math/MathTypes.h"
#include "Leap/LeapInterleave.h"
struct RecordingBulkData;


namespace BVH
{
    void WriteToFile(const char* DirectoryPath, const char* Prefix, RecordingBulkData* BulkData);

    namespace util
    {
        void WriteJointData(Joint* Root, ofstream& Stream, LEAP_HAND& Frame);
        void WriteFrameMotionData(Joint* Root, ofstream& Stream, LEAP_HAND& Frame);
    }
    
}
