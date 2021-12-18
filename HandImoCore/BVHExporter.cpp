#include "BVHExporter.h"

#include <fstream>
#include <iostream>

#include "LeapInterleave.h"

void BVH::WriteToFile(const char* DirectoryPath, const char* Prefix, RecordingBulkData* BulkData)
{
    ofstream Stream(DirectoryPath);
    if(Stream.is_open())
    {
        //Write header
        Stream << "HIERARCHY\n";
        Stream << "ROOT " << Bones::ARM.BoneName << endl;
        Stream << "{\n";

        util::WriteBVHBoneRecursive(&Bones::ARM, Stream, BulkData->Frames[BulkData->Frames.size()/2][0]);

        Stream << "}\n";
        

        

        //Write MOTION
        Stream << "MOTION\n";
        Stream << "Frames: " << BulkData->Frames.size() << "\n";
        Stream << "Frame Time: " << 0.0081f << "\n";
        Stream.close();

    }
}

void BVH::util::ForEachBoneInHierarchy(BVHBoneData* Root, std::function<void(BVHBoneData*, ofstream&)> Function, ofstream& inStream)
{
    for(int childIdx = 0; childIdx < Root->NumChildren; ++childIdx)
    {
        BVHBoneData* HBone = Root->Children[childIdx];
        Function(HBone, inStream);
    }
}

void BVH::util::WriteBVHBoneRecursive(BVHBoneData* Root, ofstream& Stream, LEAP_HAND Frame)
{
    for(int childIdx = 0; childIdx < Root->NumChildren; ++childIdx)
    {
        BVHBoneData* HBone = Root->Children[childIdx];
        if(Root->ParentBone != nullptr)
        {
            Stream << "OFFSET ";
            float Offset[3];
            for (int i = 0; i < 3; ++i)
            {
                Offset[i] = LeapInterleave::GetHandFromData(Root, Frame)->next_joint.v[i] - LeapInterleave::GetHandFromData(Root, Frame)->prev_joint.v[i];
            }
        
            for (int i = 0; i < 3; ++i)
            {
                Stream << Root->offset[i] << " ";
            }
            Stream << "\n";
            Stream << "CHANNELS 6 Xposition Yposition Zposition Zrotation Xrotation Yrotation\n";
            Stream << "JOINT " << Root->BoneName.c_str() << "\n";
            Stream << "{\n";
        }
       // if(Root->NumChildren > 0)
           // WriteBVHBoneRecursive(Root, Stream,);
    }
    Stream << "}\n";
    
}

