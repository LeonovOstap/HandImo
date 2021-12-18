#pragma once
#include <functional>

#include "Bones.h"
#include "LeapInterleave.h"
#include "Math/MathTypes.h"
struct RecordingBulkData;


namespace BVH
{
    void WriteToFile(const char* DirectoryPath, const char* Prefix, RecordingBulkData* BulkData);

    namespace util
    {
        void ForEachBoneInHierarchy(BVHBoneData* Root, std::function<void(BVHBoneData*, ofstream&)> Function, ofstream& inStream);
        void WriteBVHBoneRecursive(BVHBoneData* Root, ofstream& Stream, LEAP_HAND Frame);
    }
    
}
