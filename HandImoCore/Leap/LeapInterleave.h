#pragma once
#include <assert.h>
#include <string>
#include <thread>
#include "../Skeleton.h"
#include <vector>

#include "LeapC.h"
using namespace std;




struct RecordingBulkData
{
    vector<vector<LEAP_HAND>> Frames;
    float FrameRate = 115.f;
    
    chrono::time_point<std::chrono::system_clock> TimeStarted;
    chrono::time_point<std::chrono::system_clock> TimeFinished;
    
    float Duration;
    

    void AppendFrame(vector<LEAP_HAND> Hand);
};


class LeapInterleave
{

private:
    LEAP_CONNECTION DeviceConnection;
    std::vector<LEAP_DEVICE_REF> DeviceList;
 
    std::thread DataPollThread;
    
public:
    // DO NOT FUUCKING CALL THIS MANUALLY, this is thread only function
    vector<LEAP_HAND> Hands;
    bool IsTracking = false;
    bool IsRecording = false;

    bool HMDModeOn = false;

    RecordingBulkData* RecordingBulk;
    LEAP_IMAGE Image[2];
public:
    void StartTracking();
    void StopTracking();
    void UpdateTracking(LEAP_CONNECTION& Connection, vector<LEAP_HAND>& HandsRef, void* ImageRef);

    void StartRecording();
    void StopRecording(const char* Filename);
    void SetTrackingMode(bool HMD);
    void Calibrate();

    static LEAP_BONE* GetHandFromData(Joint* Bone, LEAP_HAND InHand);

private:
    bool PollData();
};
