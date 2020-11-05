// ExaltMototrization.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <iostream>
#include "CMaxonMotor.h"
#include "Definitions.h"

using namespace std;

int main(int argc, char* argv[])
{
    long p[2];
    CMaxonMotor motor;

    motor.InitializeAllDevices();
    motor.HomeAllDevices();
    motor.CloseAllDevice();
    //motor.ActiviateAllDevice();

    //long targetPos[2];
    //targetPos[0] = 0;*
    //targetPos[1] = 0;
    //    for (int i=0;i<100;i++){
    //    	targetpos[0]=i*400;
    //		targetpos[1]=i*400;
    //	    motor.movealldevice(targetpos);
    //	    if (i%5 == 1 ) {
    	/*	    motor.getcurrentpositionalldevice(p);
    	    	cout << "current position: " << p[0] << "\t" << p[1] << "\t";;
    	    	cout << "e: " << p[0] - targetpos[0] << "\t" << p[1] - targetpos[1] << endl;
    	    }
        }*/
    //
    //    for (int i=0;i<100;i++){
    //        	targetPos[0]=40000-i*800;
    //    		targetPos[1]=40000-i*800;
    //    	    motor.MoveAllDevice(targetPos);
    //    	    if (i%5 == 1 ) {
    //    	    	motor.GetCurrentPositionAllDevice(p);
    //    	    	cout << "Current Position: " << p[0] << "\t" << p[1] << "\t";;
    //    	    	cout << "e: " << p[0] - targetPos[0] << "\t" << p[1] - targetPos[1] << endl;
    //    	    }
    //	}
    //

    //    for (int i=0;i<1000;i++){
    //    	motor.MoveAllDevice(targetPos);
    //    	if (i%1000 == 1 ) {
    //    		cout << i << endl;
    //    	}
    //    }

    cout << "Press <Enter> to stop and quit..." << endl;
    getchar();


    motor.DisableAllDevice();

    return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
