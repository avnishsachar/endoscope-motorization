#include "CMaxonMotor.h"

using namespace std;

/**
 * This is the CMaxonMotor class that enables communication between the Maxon DLL and the Application *
*/
CMaxonMotor::CMaxonMotor()
{
    PortName_0 = "USB0";
    PortName_1 = "USB1";

    nodeId_0 = 1;
    nodeId_1 = 1;

    errorCode = 0x00; // Error Code for no error
}

/**
 * Closes any active port and releases it for other applications.
 * 
 * @param keyHandle_ Keyhandle value of device
 * 
*/
void CMaxonMotor::CloseDevice(void* keyHandle_)
{
    DWORD errorCode = 0;

    cout << "Closing device port(s). Port(s) released for other applications." << endl;

    if (keyHandle_ != 0)
        VCS_CloseDevice(keyHandle_, &errorCode); // closes specific ports
    VCS_CloseAllDevices(&errorCode); // closes all active ports
}

/**
 * @brief Checks for device state. Enables device and sets device to "Position Profile Mode".
 * @param keyHandle_ 
 * @param nodeId 
*/
void CMaxonMotor::EnableDevice(void* keyHandle_, unsigned short nodeId)
{
    DWORD errorCode = 0;
    int IsInFault = FALSE;

    if (VCS_GetFaultState(keyHandle_, nodeId, &IsInFault, &errorCode))
    {
        if (IsInFault && !VCS_ClearFault(keyHandle_, nodeId, &errorCode))
        {
            cout << "Failed to clear fault. [ERROR]: " << errorCode << endl;
            return;
        }

        int IsEnabled = FALSE;
        if (VCS_GetEnableState(keyHandle_, nodeId, &IsEnabled, &errorCode))
        {
            if (!IsEnabled && !VCS_SetEnableState(keyHandle_, nodeId, &errorCode))
            {
                cout << "Failed to enable device. [ERROR]: " << errorCode << endl;
            }
            else
            {
                cout << "Enabled device successfully." << endl;
            }
        }
    }
    else
    {
        cout << "Unable to get fault state. [ERROR]:" << errorCode << endl;
    }

    if (!VCS_ActivateProfilePositionMode(keyHandle_, nodeId, &errorCode)) {
        cout << "Failed to activate Profile Position Mode. [ERROR]" << errorCode << endl;
    }
    //else
    //{
    //    cout << "Successfully activated Profile Position Mode." << endl;
    //


    DWORD ProfileVelocity = 5000;
    DWORD ProfileAcceleration = 100000;
    DWORD ProfileDeceleration = 100000;

    if (!VCS_SetPositionProfile(keyHandle_, nodeId, ProfileVelocity, ProfileAcceleration, ProfileDeceleration, &errorCode)) {
        cout << "Failed to change operation mode to \"Profile Position Mode\" for device. [ERROR]: " << errorCode << endl;
    }
    //else
    //{
    //    cout << "Successfully set Profile Position Mode." << endl;
    //}


    if (!VCS_ActivateProfileVelocityMode(keyHandle_, nodeId, &errorCode)) {
        cout << "Failed to activate Profile Velocity Mode. [ERROR]" << errorCode << endl;
    }
    //else
    //{
    //    cout << "Successfully activated Profile Velocity Mode." << endl;
        //}

    if (!VCS_SetVelocityProfile(keyHandle_, nodeId, ProfileAcceleration, ProfileDeceleration, &errorCode)) {
        cout << "Failed to change operation mode to \"Profile Velocity Mode\" for device. [ERROR]: " << errorCode << endl;
    }
    //else
    //{
    //    cout << "Successfully set Profile Velocity Mode." << endl;
    //}

    long TargetVelocity = 0;
    if (!VCS_MoveWithVelocity(keyHandle_, nodeId, TargetVelocity, &errorCode)) {
        cout << "Failed to change Move Velocity for device. [ERROR]: " << errorCode << endl;
    }
    //else
    //{
    //    cout << "Successfully set Move Velocity." << endl;
    //}

}

void CMaxonMotor::DisableDevice(void* keyHandle_, unsigned short nodeId)
{
    DWORD errorCode = 0;

    int IsEnabled = FALSE;
    if (VCS_GetEnableState(keyHandle_, nodeId, &IsEnabled, &errorCode))
    {
        cout << "Device is enabled" << endl;
        VCS_SetDisableState(keyHandle_, nodeId, &errorCode);
        cout << "Set disable state succeeded!" << endl;
    }
}

long CMaxonMotor::HomingProcedure(void* keyHandle_, unsigned short nodeId) {

    long TargetVelocity = 2000;
    if (!VCS_MoveWithVelocity(keyHandle_, nodeId, TargetVelocity, &errorCode)) {
        cout << "Failed to change Move Velocity for homing device. [ERROR]: " << errorCode << endl;
    }
    else
    {
        cout << "Successfully set Move Velocity for homing device." << endl;
    }

    short currentValue = 0;
    long positionValue = 0;
     do {
        VCS_GetCurrentIs(keyHandle_, nodeId, &currentValue, &errorCode);
        VCS_GetPositionIs(keyHandle_, nodeId, &positionValue, &errorCode);
     } while (currentValue < 500); // value at 500 when load is attached
    // stopping motor
    VCS_MoveWithVelocity(keyHandle_, nodeId, 0, &errorCode);
    VCS_GetPositionIs(keyHandle_, nodeId, &positionValue, &errorCode);
    long kmaxPosition = positionValue;

    TargetVelocity = -TargetVelocity;
    VCS_MoveWithVelocity(keyHandle_, nodeId, TargetVelocity, &errorCode);
    do {
        VCS_GetCurrentIs(keyHandle_, nodeId, &currentValue, &errorCode);
        VCS_GetPositionIs(keyHandle_, nodeId, &positionValue, &errorCode);
    } while (currentValue > -500);
    // stopping motor
    VCS_MoveWithVelocity(keyHandle_, nodeId, 0, &errorCode);
    VCS_GetPositionIs(keyHandle_, nodeId, &positionValue, &errorCode);
    const long kminPosition = positionValue;

    //cout << "minimum position:" << kminPosition << " and max position: " << kmaxPosition << endl;

    long homePosition = (kminPosition + kmaxPosition) / 2;
    VCS_ActivateProfilePositionMode(keyHandle_, nodeId, &errorCode);
    VCS_SetPositionProfile(keyHandle_, nodeId, 5000, 100000, 100000, &errorCode);
    VCS_MoveToPosition(keyHandle_, nodeId, homePosition, TRUE, TRUE, &errorCode);

    while (abs(positionValue - homePosition) > 100) {
         VCS_GetPositionIs(keyHandle_, nodeId, &positionValue, &errorCode);
    }
    return homePosition;
}

void CMaxonMotor::Move(void* keyHandle_, long TargetPosition, unsigned short nodeId)
{

    DWORD errorCode = 0;

    int Absolute = TRUE; // FALSE;
    int Immediately = TRUE;
    long positionValue = 0;


    VCS_ActivateProfilePositionMode(keyHandle_, nodeId, &errorCode);
    VCS_SetPositionProfile(keyHandle_, nodeId, 5000, 100000, 100000, &errorCode);
    VCS_MoveToPosition(keyHandle_, nodeId, TargetPosition, TRUE, TRUE, &errorCode);

    while (abs(positionValue - TargetPosition) > 100) {
        VCS_GetPositionIs(keyHandle_, nodeId, &positionValue, &errorCode);
    }
}

void CMaxonMotor::Halt(void* keyHandle_, unsigned short nodeId)
{
    DWORD errorCode = 0;

    if (!VCS_HaltPositionMovement(keyHandle_, nodeId, &errorCode))
    {
        cout << "Halt position movement failed!, error code=" << errorCode << endl;
    }
}

/**
 * Activate and Initialize the devices connected to the Serial Ports. Calls 
 * @param portName Port on which the device is connected.
 * @param nodeId Device Node ID.
 * @return Keyhandle to the device.
*/
void* CMaxonMotor::ActivateDevice(const char* portName, unsigned short nodeId)
{
    char deviceName[] = "EPOS4";
    char protocolStackName[] = "MAXON SERIAL V2";
    char interfaceName[] = "USB";
    DWORD errorCode = 0;
    unsigned long timeout_ = 500;
    unsigned long baudrate_ = 1000000;
    void* keyHandle_;

    keyHandle_ = VCS_OpenDevice(deviceName, protocolStackName, interfaceName, portName, &errorCode);

    if (keyHandle_ == 0)
    {
        cout << "Failed to open device at:" << portName <<" [ERROR]: " << errorCode << endl;
        exit(0);
    }
    else
    {
        cout << "Device at:" << portName << " opened successfully" << endl;
    }


    if (!VCS_SetProtocolStackSettings(keyHandle_, baudrate_, timeout_, &errorCode))
    {
        cout << "Unable to set Protocol Stack Settings. [ERROR]: " << errorCode << endl;
        CloseDevice(keyHandle_);
        exit(0);
    }
    else
    {
        cout << "Protocol Stack Settings Set to, Baudrate[bit/s]: " << baudrate_ << " ,Timeout[ms]: " << timeout_ << endl;
    }

    EnableDevice(keyHandle_, nodeId);
    return keyHandle_;
}

void CMaxonMotor::HomeAllDevices(vector<long> home_position) {
    VCS_ActivateProfilePositionMode(keyHandle_0, nodeId_0, &errorCode);
    SetPositionProfile(keyHandle_0, nodeId_0);
    Move(keyHandle_0, home_position[0], nodeId_0);

    VCS_ActivateProfilePositionMode(keyHandle_1, nodeId_1, &errorCode);
    SetPositionProfile(keyHandle_1, nodeId_1);
    Move(keyHandle_1, home_position[1], nodeId_1);
}

void CMaxonMotor::ActivateProfileVelocityModeAll() {
    VCS_ActivateProfileVelocityMode(keyHandle_0, nodeId_0, &errorCode);
    VCS_ActivateProfileVelocityMode(keyHandle_1, nodeId_1, &errorCode);
}

void CMaxonMotor::SetPositionProfile(void* keyHandle_, unsigned short nodeId){
    DWORD ProfileVelocity = 5000;
    DWORD ProfileAcceleration = 100000;
    DWORD ProfileDeceleration = 100000;
    if (!VCS_SetPositionProfile(keyHandle_, nodeId, ProfileVelocity, ProfileAcceleration, ProfileDeceleration, &errorCode)) {
        cout << "Failed to change operation mode to \"Profile Position Mode\" for device. [ERROR]: " << errorCode << endl;
    }
    //else
    //{
    //    cout << "Successfully set Profile Position Mode." << endl;
    //}
}

/**
 * Closes any active devices, releases the port(s). Activates and Initializes devices at the port(s).
*/
void CMaxonMotor::InitializeAllDevices(){
    CloseDevice(keyHandle_0);
    CloseDevice(keyHandle_1);
    keyHandle_0 = ActivateDevice(PortName_0, nodeId_0);
    keyHandle_1 = ActivateDevice(PortName_1, nodeId_1);

}

std::vector<long> CMaxonMotor::HomingProcedureAllDevices() {
    vector<long> home_position;
    home_position.push_back(HomingProcedure(keyHandle_0, nodeId_0));
    home_position.push_back(HomingProcedure(keyHandle_1, nodeId_1));
    return home_position;
}

void CMaxonMotor::CloseAllDevice() {
    CloseDevice(keyHandle_0);
    CloseDevice(keyHandle_1);
}

void CMaxonMotor::DisableAllDevice() {
    DisableDevice(keyHandle_0, nodeId_0);
    DisableDevice(keyHandle_1, nodeId_1);
}

void CMaxonMotor::GetCurrentPosition(void* keyHandle_, long& CurrentPosition, unsigned short nodeId) {
    DWORD errorCode = 0;

    if (!VCS_GetPositionIs(keyHandle_, nodeId, &CurrentPosition, &errorCode)) {
        cout << " error while getting current position , error code=" << errorCode << endl;
    }
}

void CMaxonMotor::GetCurrentPositionAllDevice(long* CurrentPosition) {
    long Pos;
    GetCurrentPosition(keyHandle_0, Pos, nodeId_0);
    CurrentPosition[0] = Pos;
    GetCurrentPosition(keyHandle_1, Pos, nodeId_1);
    CurrentPosition[1] = Pos;
}

void CMaxonMotor::MoveAllDevice(const long* TargetPosition) {
    Move(keyHandle_0, TargetPosition[0], nodeId_0);
    Move(keyHandle_1, TargetPosition[0], nodeId_1);
}

void CMaxonMotor::MoveWithVelocityOne(long motor_velocity) {
    if (!VCS_MoveWithVelocity(keyHandle_0, nodeId_0, motor_velocity, &errorCode)) {
        cout << "Failed to change Move Velocity through Joystick. [ERROR]: " << errorCode << endl;
    }
}
void CMaxonMotor::MoveWithVelocityTwo(long motor_velocity) {
    if (!VCS_MoveWithVelocity(keyHandle_1, nodeId_1, motor_velocity, &errorCode)) {
        cout << "Failed to change Move Velocity through Joystick. [ERROR]: " << errorCode << endl;
    }
}
