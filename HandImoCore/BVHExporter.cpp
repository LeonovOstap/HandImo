#include "BVHExporter.h"

#include <fstream>
#include <iostream>

#include "Leap/Leap2Handimo.h"
#include "Leap/LeapInterleave.h"

void Export::BVHExporter::WriteToFile()
{
    ofstream Stream(FullPath);
    if(Stream.is_open())
    {
        //Write header
        Stream << "HIERARCHY\n";
        Stream << "ROOT " << Skeleton::ARM.BoneName << endl;
        Stream << "{\n";

        WriteJointData(&Skeleton::ARM, Stream, GetReferenceFrame());

        Stream << "}\n";
        

        

        //Write MOTION
        Stream << "MOTION\n";
        Stream << "Frames: " << RecordingData.Frames.size() << "\n";
        Stream << "Frame Time: " << 0.0081f << "\n";


        for(auto Frame : RecordingData.Frames)
        {
            WriteFrameMotionData(&Skeleton::ARM, Stream, Frame[0]);
            Stream << endl;
        }

        
        Stream.close();

    }
}

void Export::BVHExporter::WriteJointData(Joint* Root, ofstream& Stream, LEAP_HAND& Frame)
{
    if(Root->ParentBone != nullptr)
    {
        Stream << "OFFSET ";
        auto Vec1 = LeapInterleave::GetHandFromData(Root->ParentBone, Frame);
        auto Vec2 = LeapInterleave::GetHandFromData(Root, Frame);
        Vector3f Offset = Leap2ImoVec(Vec1->prev_joint) - Leap2ImoVec(Vec2->prev_joint);

        Stream << Offset.ToString();
            
        Stream << "\n";
        Stream << "CHANNELS 3 Zrotation Xrotation Yrotation\n";
    }else
    {
        Stream << "OFFSET ";
        auto Vec2 = LeapInterleave::GetHandFromData(Root, Frame);
        Vector3f Offset = Leap2ImoVec(Vec2->prev_joint) - Leap2ImoVec(Vec2->next_joint);
        Stream << Offset.ToString();
        Stream << "\nCHANNELS 6 Xposition Yposition Zposition Zrotation Xrotation Yrotation\n";
    }
    if(Root->NumChildren > 0)
    {
        for(int childIdx = 0; childIdx < Root->NumChildren; ++childIdx)
        {
            Joint* HBone = Root->Children[childIdx];
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
        auto Vec2 = LeapInterleave::GetHandFromData(Root, Frame);
        Vector3f Offset = Leap2ImoVec(Vec2->prev_joint) - Leap2ImoVec(Vec2->next_joint);

        Stream << Offset.ToString();
        Stream << "\n}\n";
    }
    

    
}

void Export::BVHExporter::WriteFrameMotionData(Joint* Root, ofstream& Stream, LEAP_HAND& Frame)
{
    if(Root->ParentBone != nullptr)
    {
       /* auto Vec1 = LeapInterleave::GetHandFromData(Root->ParentBone, Frame);
        auto Vec2 = LeapInterleave::GetHandFromData(Root, Frame);
        Vector3f Offset = Leap2ImoVec(Vec1->prev_joint) - Leap2ImoVec(Vec2->prev_joint);

        Stream << Offset.ToString() << " ";
        Stream << "0 0 0 ";*/
    }else
    {
        auto Vec2 = LeapInterleave::GetHandFromData(Root, Frame);
        Vector3f Offset = Leap2ImoVec(Vec2->prev_joint) - Leap2ImoVec(Vec2->next_joint);

        Stream << (-Offset).ToString();
        Stream << " ";
        
    }
    Quaternion Quat = Quaternion::Identity();
    float Yaw , Pitch, Roll;
    if(CalculateLocalToParentQuaternion(Root,Frame,Quat))
    {
        Quat.ToEuler(Yaw,Pitch, Roll);
    }
    else
    {
        Yaw = Pitch = Roll = 0;
    }

    Yaw = RadiansToDegrees(Yaw);
    Pitch = RadiansToDegrees(Pitch);
    Roll = RadiansToDegrees(Roll);
    
    Stream << Yaw << " " << Pitch << " " << Roll << " ";
    if(Root->NumChildren > 0)
    {
        for(int childIdx = 0; childIdx < Root->NumChildren; ++childIdx)
        {
            Joint* HBone = Root->Children[childIdx];
            WriteFrameMotionData(HBone, Stream, Frame);
        }
    }
}

LEAP_HAND& Export::BVHExporter::GetReferenceFrame()
{
    return RecordingData.Frames[0][0];
}

bool Export::BVHExporter::CalculateLocalToParentQuaternion(Joint* Joint, const LEAP_HAND& Frame, Quaternion& OutRotation)
{
    if(!Joint->ParentBone)
        return false;

    //return Leap2ImoQuat(LeapInterleave::GetHandFromData(Joint,Frame)->rotation);

    const LEAP_HAND& ReferenceFrame = RecordingData.Frames[0][0];
    
    Matrix4x4f CurrentBasis = Matrix4x4f(Leap2ImoQuat(LeapInterleave::GetHandFromData(Joint, Frame)->rotation));
    Matrix4x4f ReferenceBasis = Matrix4x4f(Leap2ImoQuat(LeapInterleave::GetHandFromData(Joint, ReferenceFrame)->rotation));
    
    Matrix4x4f ParentCurrentBasis = Matrix4x4f(Leap2ImoQuat(LeapInterleave::GetHandFromData(Joint->ParentBone, Frame)->rotation));
    Matrix4x4f ParentReferenceBasis = Matrix4x4f(Leap2ImoQuat(LeapInterleave::GetHandFromData(Joint->ParentBone, ReferenceFrame)->rotation));

    Matrix4x4f OutBasis = (ReferenceBasis * CurrentBasis.Transpose()) * (ParentReferenceBasis * ParentCurrentBasis.Transpose());
    OutRotation = OutBasis.Rotation();
    
    return true;
}

