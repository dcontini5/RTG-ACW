//--------------------------------------------------------------------------------------
// File: Tutorial01.cpp
//
// This application demonstrates creating a Direct3D 11 device
//
// http://msdn.microsoft.com/en-us/library/windows/apps/ff729718.aspx
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

#include "Device.h"

using namespace DirectX;

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( const _In_ HINSTANCE hInstance, const _In_opt_ HINSTANCE hPrevInstance, const _In_ LPWSTR lpCmdLine, const _In_ int nCmdShow ) {

	Device* const device = new Device();
	const int IsCTRLDown = 32768;
	
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

    if( FAILED(device->InitWindow( hInstance, nCmdShow ) ) )
        return 0;

    if( FAILED(device->InitDevice() ) )
    {
        device->CleanupDevice();
        return 0;
    }

    // Main message loop
    MSG msg = {nullptr};
    while( WM_QUIT != msg.message )
    {
    	if( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );

			auto x = GetAsyncKeyState(VK_RCONTROL);

            switch (msg.wParam + GetAsyncKeyState(VK_LCONTROL)) {

				case(VK_ESCAPE):
            		msg.message = WM_QUIT;
            		continue; 
            	
				case(VK_F1):  device->SwapCamera(1); break;
				case(VK_F2):  device->SwapCamera(0); break;
				case(VK_F5):  //todo change effects
				case(VK_F6):  //todo switch lighting mode
				case(VK_F8):  //todo switch rendering mode
				case(0x31):  //todo boom boom
				case(0x32):  //todo boom boom
				case(0x33):  //todo boom boom
				case(0x54):  //todo speeds up time
				case(0x54 + 1):  //todo slows down time
				case(0x52):  break;//todo reset
				case(VK_PRIOR): device->MoveCameraUpDown(1.f); break;
				case(VK_NEXT):  device->MoveCameraUpDown(-1.f); break;
				case(VK_LEFT - IsCTRLDown):
				case(0x41 - IsCTRLDown):
				case(0x4A - IsCTRLDown):   device->MoveCameraLeftRight(1.f); break;
				case(VK_RIGHT - IsCTRLDown):
				case(0x44 - IsCTRLDown):
				case(0x4C - IsCTRLDown):   device->MoveCameraLeftRight(-1.f); break;
				case(VK_UP - IsCTRLDown):
				case(0x57 - IsCTRLDown):
				case(0x49 - IsCTRLDown):   device->MoveCameraBackForward(1.f); break;
				case(VK_DOWN - IsCTRLDown):
				case(0x53 - IsCTRLDown):
				case(0x4B - IsCTRLDown):   device->MoveCameraBackForward(-1.f); break;
				case(VK_LEFT):
				case(0x41):
				case(0x4A):   device->RotateCameraLeftRight(1.f); break;
				case(VK_RIGHT):
				case(0x44):
				case(0x4C):   device->RotateCameraLeftRight(-1.f); break;
				case(VK_UP):
				case(0x57):
				case(0x49):	  device->RotateCameraUpDown(1.f); break;
				case(VK_DOWN):
				case(0x53):
				case(0x4B):	  device->RotateCameraUpDown(-1.f); break;
		
            	
            }

    		
        }
        else
        {
		
			device->Integrate();
			device->CollisionDetection();
			device->Render();
        }
    }

	device->CleanupDevice();

	delete device;
	
    return static_cast<int>(msg.wParam);
}
