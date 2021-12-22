#include "LeapInterleave.h"

#include <iostream>
#include <thread>
#include <vec3.hpp>
#include <detail/func_geometric.inl>

#include "Leap2Handimo.h"
#include "../BVHExporter.h"
#include "../imgui/backends/imgui_impl_opengl3_loader.h"
#define MAX_CONNECTION_RETRIES 10

void RecordingBulkData::AppendFrame(vector<LEAP_HAND> Hand)
{
    Frames.push_back(Hand);
}

void LeapInterleave::StartTracking()
{
    LeapCreateConnection(NULL, &DeviceConnection);
    LeapOpenConnection(DeviceConnection);
    LeapSetPolicyFlags(DeviceConnection, eLeapPolicyFlag_Images, 0);
    
    for(int i = 0; i < 10;++i)
    {
        if(PollData())
            break;
    }
    IsTracking = true;
    DataPollThread = std::thread([&, this]()
    {
            UpdateTracking(DeviceConnection,Hands, nullptr);
    });
    DataPollThread.detach();
}

void LeapInterleave::StopTracking()
{
    IsTracking = false;
    LeapCloseConnection(DeviceConnection);
    LeapDestroyConnection(DeviceConnection);
}

void LeapInterleave::UpdateTracking(LEAP_CONNECTION& Connection, vector<LEAP_HAND>& HandsRef, void* ImageRef)
{
    while(IsTracking)
    {
        LEAP_CONNECTION_MESSAGE Message;

        LeapPollConnection(Connection, 1000U, &Message);

        if(Message.type == eLeapEventType_Tracking)
        {
            HandsRef.clear();
            for(int handIdx = 0; handIdx < Message.tracking_event->nHands;++handIdx)
            {
                HandsRef.push_back(Message.tracking_event->pHands[handIdx]);
            }
            if(IsRecording && RecordingBulk)
            {
                RecordingBulk->AppendFrame(HandsRef);
            }
        }
        if(Message.type == eLeapEventType_Image)
        {
            const LEAP_IMAGE_PROPERTIES* properties = &Message.image_event->image[0].properties;
            if (properties->bpp != 1)
                 return;
            Image[0] = Message.image_event->image[0];
            Image[1] = Message.image_event->image[1];
        }
    }
    HandsRef.clear();
}

void LeapInterleave::StartRecording()
{
    IsRecording = true;
    RecordingBulk = new RecordingBulkData;
    RecordingBulk->TimeStarted = std::chrono::system_clock::now();
    
    

   // float b[3] = {5.000055452456745454564654,1,0};
    //Vector3f MyVector = Vector3f(b);
   // MyVector[1] = 5;
   // MyVector[2] = 5;
    //MyVector += -Vector3f(5.f,5.f);
    
    /*std::cout << Vector3f(b).X << endl;
    std::cout << Vector3f(1.0f,2.0f,3.0f).Dot(Vector3f(1.0f,5.0f,7.0f)) << endl;
    std::cout << glm::dot(glm::vec3(1,2,3),glm::vec3(1,5,7)) << endl;*/

    auto Bone = GetHandFromData(&Bones::THUMB_Metacarpal, Hands[0]);
    if(Bone)
    {
        cout << Leap2ImoVec(Bone->prev_joint).ToString() << endl;
        cout << Leap2ImoVec(Hands[0].thumb.metacarpal.prev_joint).ToString() << endl;
    }
    
}

void LeapInterleave::StopRecording(const char* Filename)
{
    IsRecording = false;
    
    BVH::WriteToFile(Filename, 0, RecordingBulk);
}

void LeapInterleave::SetTrackingMode(bool HMD)
{
    if(HMD)
        LeapSetPolicyFlags(DeviceConnection, eLeapPolicyFlag_OptimizeHMD, 0);
    else
        LeapSetPolicyFlags(DeviceConnection, 0, eLeapPolicyFlag_OptimizeHMD);
}

void LeapInterleave::Calibrate()
{
}

LEAP_BONE* LeapInterleave::GetHandFromData(BVHBoneData* Bone, LEAP_HAND InHand)
{
    if(Bone->BoneName == Bones::ARM.BoneName)
        return &InHand.arm;
    for(int i = 0; i < 5;++i)
    {

        BVHBoneData* FingerData = Bones::ARM.Children[i];
        BVHBoneData* PartData = FingerData;
        for(int j = 0; j < 4; ++j)
        {
            
            if(PartData->BoneName == Bone->BoneName)
            {
                return &InHand.digits[i].bones[j];
            }
            PartData = PartData->Children[0];
        }
    }
    return nullptr;
}

bool LeapInterleave::PollData()
{
    LEAP_CONNECTION_MESSAGE Message;
    LeapPollConnection(DeviceConnection, 1000U, &Message);
    
    uint32_t ArraySize = 0U;
    eLeapRS ReturnCode = LeapGetDeviceList(DeviceConnection, DeviceList.data(), &ArraySize);
    
    if(ReturnCode != eLeapRS_Success)
        return false;

    LeapGetDeviceList(DeviceConnection, DeviceList.data(), &ArraySize);
    
    
    return true;
    
}
