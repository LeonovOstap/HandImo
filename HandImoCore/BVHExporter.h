#pragma once
#include <functional>

#include "Skeleton.h"
#include "Math/Math.h"
#include "Leap/LeapInterleave.h"
struct RecordingBulkData;


namespace Export
{
    class BVHExporter
    {
    public:
         BVHExporter(const char* Path, const RecordingBulkData& recording_data)
            : RecordingData(recording_data)
        {
             FullPath = Path;
        }

        void WriteToFile();

        void WriteJointData(Joint* Root, ofstream& Stream, LEAP_HAND& Frame);
        void WriteFrameMotionData(Joint* Root, ofstream& Stream, LEAP_HAND& Frame);

    private:
        LEAP_HAND& GetReferenceFrame();
         bool CalculateLocalToParentQuaternion(Joint* Joint, const LEAP_HAND& Frame, Quaternion& OutRotation);
    private:
        string FullPath;
        RecordingBulkData RecordingData;
        
    };
}
