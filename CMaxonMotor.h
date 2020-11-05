#pragma once

#include "Definitions.h" // Maxon Motor Header file

#define TRUE 1
#define FALSE 0


class CMaxonMotor
{
private:
    const char* PortName_0;
    const char* PortName_1;
    DWORD errorCode;
    unsigned short nodeId_0;
    unsigned short nodeId_1;

    void* keyHandle_0, * keyHandle_1;

    void EnableDevice(void* keyHandle_, unsigned short nodeId);
    void DisableDevice(void* keyHandle_, unsigned short nodeId);
    void* ActivateDevice(const char* PortName, unsigned short nodeId);
    long HomingProcedure(void* keyHandle_, unsigned short nodeId);
    void CloseDevice(void* keyHandle_);
    void Move(void* keyHandle_, long TargetPosition, unsigned short nodeId);
    void GetCurrentPosition(void* keyHandle_, long& CurrentPosition, unsigned short nodeId);

    void Halt(void* keyHandle_, unsigned short nodeId);

public:
    CMaxonMotor();

    void InitializeAllDevices(); // Initialize devices at the start 
    void CloseAllDevice();
    void ActiviateAllDevice();
    void DisableAllDevice();
    void GetCurrentPositionAllDevice(long* CurrentPosition);
    void MoveAllDevice(const long* TargetPosition);
    void HomeAllDevices();
    //    CMaxonMotor(char[], unsigned int );


    //    void GetCurrentPosition(int& CurrentPosition); // need change

    //    void GetPositionProfile();
    //    void SetPositionProfile(unsigned int ProfileVelocity, unsigned int ProfileAcc, unsigned int ProfileDec);
};


