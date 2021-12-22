#include "BVHExporter.h"

#include <fstream>
#include <iostream>

#include "Leap/Leap2Handimo.h"
#include "Leap/LeapInterleave.h"

void BVH::WriteToFile(const char* DirectoryPath, const char* Prefix, RecordingBulkData* BulkData)
{
    ofstream Stream(DirectoryPath);
    if(Stream.is_open())
    {
        //Write header
        Stream << "HIERARCHY\n";
        Stream << "ROOT " << Bones::ARM.BoneName << endl;
        Stream << "{\n";

        util::WriteJointData(&Bones::ARM, Stream, BulkData->Frames[0][0]);

        Stream << "}\n";
        

        

        //Write MOTION
        Stream << "MOTION\n";
        Stream << "Frames: " << BulkData->Frames.size() << "\n";
        Stream << "Frame Time: " << 0.0081f << "\n";


        for(auto Frame : BulkData->Frames)
        {
            util::WriteFrameMotionData(&Bones::ARM, Stream, Frame[0]);
            Stream << endl;
        }

        
        Stream.close();

    }
}


void BVH::util::WriteJointData(BVHBoneData* Root, ofstream& Stream, LEAP_HAND& Frame)
{
    if(Root->ParentBone != nullptr)
    {
        Stream << "OFFSET ";
        auto Vec1 = LeapInterleave::GetHandFromData(Root->ParentBone, Frame);
        auto Vec2 = LeapInterleave::GetHandFromData(Root, Frame);
        Vector3f Offset = Leap2ImoVec(Vec1->prev_joint) - Leap2ImoVec(Vec2->prev_joint);

        Stream << Offset.ToString();
            
        Stream << "\n";
        Stream << "CHANNELS 6 Xposition Yposition Zposition Zrotation Xrotation Yrotation\n";
    }else
    {
        Stream << "OFFSET 0 0 0\n";
        Stream << "CHANNELS 6 Xposition Yposition Zposition Zrotation Xrotation Yrotation\n";
    }
    if(Root->NumChildren > 0)
    {
        for(int childIdx = 0; childIdx < Root->NumChildren; ++childIdx)
        {
            BVHBoneData* HBone = Root->Children[childIdx];
            Stream << "JOINT " << HBone->BoneName.c_str() << "\n";
            Stream << "{\n";
            WriteJointData(HBone,Stream,Frame);
                
            Stream << "}\n";
        }
    }
    else
    {
        Stream << "End Site\n";
        Stream << "{\n";
        Stream << "OFFSET ";
        auto Vec1 = LeapInterleave::GetHandFromData(Root, Frame);
        auto Vec2 = LeapInterleave::GetHandFromData(Root, Frame);
        Vector3f Offset = Leap2ImoVec(Vec1->prev_joint) - Leap2ImoVec(Vec2->next_joint);

        Stream << Offset.ToString();
        Stream << "\n}\n";
    }
    

    
}

void BVH::util::WriteFrameMotionData(BVHBoneData* Root, ofstream& Stream, LEAP_HAND& Frame)
{
    if(Root->ParentBone != nullptr)
    {
        auto Vec1 = LeapInterleave::GetHandFromData(Root->ParentBone, Frame);
        auto Vec2 = LeapInterleave::GetHandFromData(Root, Frame);
        Vector3f Offset = Leap2ImoVec(Vec1->prev_joint) - Leap2ImoVec(Vec2->prev_joint);

        Stream << Offset.ToString() << " ";
    }else
    {
       Stream << "0 0 0 ";
    }

    auto Quat = LeapInterleave::GetHandFromData(Root, Frame)->rotation;
    Stream << Quat.z << " " << Quat.x << " " << Quat.y << " ";
    if(Root->NumChildren > 0)
    {
        for(int childIdx = 0; childIdx < Root->NumChildren; ++childIdx)
        {
            BVHBoneData* HBone = Root->Children[childIdx];
            WriteFrameMotionData(HBone, Stream, Frame);
        }
    }
}

