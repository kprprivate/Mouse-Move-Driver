#pragma once
#include <Windows.h>
#include <iostream>
#include <cstdint>
#include <string>
#include <thread>

#include <chrono>
#include <vector>

#include "ioctl.h"
#include "structure.h"
#include "auth/genOTP.h"

#include "init/driverInit.h"

/*
I'M THE BEST - MY3 • I'M THE BEST - MY3 • I'M THE BEST - MY3 • I'M THE BEST - MY3 • I'M THE BEST - MY3 • I'M THE BEST - MY3 • I'M THE BEST - MY3
I'M THE BEST - MY3 • I'M THE BEST - MY3 • I'M THE BEST - MY3 • I'M THE BEST - MY3 • I'M THE BEST - MY3 • I'M THE BEST - MY3 • I'M THE BEST - MY3
I'M THE BEST - MY3 • I'M THE BEST - MY3 • I'M THE BEST - MY3 • I'M THE BEST - MY3 • I'M THE BEST - MY3 • I'M THE BEST - MY3 • I'M THE BEST - MY3

                                                 ::::::::
                                                ::::::::::
                                                ::::::::::
                                                 :::::::::::
                                                 ::::::::::::
                                                  ::::::::::::
                                                  :::::::::::::
                                                :::::::::::::::
 ::::::::::::::  ::::                          :::::::::::::::::
:::::::::::::::::::::                         ::::::::::::::::::
---:::::::::::::::::::                       :::::::::::::::::::
  ---::::::::::::::::::                       :::::::::::::::::::
     --::::::::::::::::                 :::::::::::::::::::::::::
       ---:::::::::::::::::::::::::::::::::::  :::::::::::::::::::
          -::::::::::::          :::::::   ::::::::::::::::::::::::
           ----::::::::::::::           ::::::::::::::::::::::::::::  ::::::::::
           ------::::::                ::::::::::::::::::::::::::::::::::::::::::
           ---------:::                ::::::::::::::::::::::::::::::::::::::::::
           ----------::                ::::::::::::::::::::::::::::::::::::::::::
           ------------               ::::::::::::::::::::::::::::::::::::::::::::
           ------------             ::::::::::::::::::::::::::::::::::::::::::::::::
           ------------          -:::::::::::::::::::::::::::::::::::::::::::::::::::::
           ---------------------::::::::::::::::::::::::::::::::::::::::::::::::::::::::
            ----------------------::::::::::::::::::::::::::::::::::::::::::::::::::::::::
            -------------------------::::::::::::::::::::::::::::::::::::::::::::::::::::::
             --------------------------:::::::::::::::::::::::::::::::::::::::::::::::::::::
              ---------------------------::::::::::::::::::::::::::::::::::::::::::::::::::::
               -----------------------------:::::::::::::::::::::::::::::::::::::::::::::::::
                  -----------------------------:::::::::::::::::::::::::::::::::::::::::::::::
                    -----------------------------:::::::::::::::::::::::::::::::::::::::::::::
                      -----------------------------:::::::::::::::::::::::::::::::::::::::::::
                         ----------------------------:::::::::::::::::::::::::::::::::::::::::
                           -----------------------------::::::::::::::::::::::::::::::::::::::
                             ------------------------------:::::::::::::::::::::::::::::::::::
                              -------------------------------::::::::::::::::::::::::::::::::::
                               ---------------------------------:::::::::::::::::::::::::::::::
                                ----------------------------------:::::::::::::::::::::::::::::
                                 ------------------------------------::::::::::::::::::::::::::
                                 --------------------------------------::::::::::::::::::::::::
                                   ------------      ----------------    ::::::::::::::::::::::
                                    -----------                           --::::::::::::::::::::
                                     ----------                            ----:::::    ::::::::
                                      --------                             ------::::    ::::::::
                                      -------                               ---------     ::::::::
                                      ------                                 ---------     :::::::
                                     ------                                   --------      :::::::
                                     ------                                   --------      ::::::::
                                     ------                                   --------       :::::::
                                      ------                                 --------         -:::::
                                      --------                               ------           ----:-
                                      ---------                             ------            -----
                                      ------------                         ------             -----
                                     -----  --------                      ------              -----
                                     -----     -------  ---              ------               -----
                                    -----        -----------            ------                -----
                                    -----          ---------            -----                 -----
                                   ------              ------          -----                  ----
                                 -------                  --       --------                -------
                               ---------                          ---------              ---------


I'M THE BEST - MY3 • I'M THE BEST - MY3 • I'M THE BEST - MY3 • I'M THE BEST - MY3 • I'M THE BEST - MY3 • I'M THE BEST - MY3 • I'M THE BEST - MY3
I'M THE BEST - MY3 • I'M THE BEST - MY3 • I'M THE BEST - MY3 • I'M THE BEST - MY3 • I'M THE BEST - MY3 • I'M THE BEST - MY3 • I'M THE BEST - MY3
I'M THE BEST - MY3 • I'M THE BEST - MY3 • I'M THE BEST - MY3 • I'M THE BEST - MY3 • I'M THE BEST - MY3 • I'M THE BEST - MY3 • I'M THE BEST - MY3
*/

class _driver {
    HANDLE driverHandler = nullptr; // Handle to the driver.

public:
    uint32_t _processid     = 0;
    uint64_t _dirbase       = 0;
    uintptr_t _processPEB   = 0;

    // Initialize the driver with a given process ID.
    bool initdriver(uint32_t processid) {
		_processid = processid;

        const int maxRetries = 3;
        const char* errorMessage = "An error has occurred - Startup #01";
        bool success = false;
        int retryDelay = 1000; // Initial delay in milliseconds (1 second)

        for (int retry = 0; retry < maxRetries; ++retry) {
            if (!driver_init()) {
                std::cerr << "driver_init failed. Retrying in " << retryDelay << " ms..." << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(retryDelay));
                retryDelay *= 2; // Exponential backoff
                continue;
            }
            
            driverHandler = CreateFileA(
                "\\\\.\\JustWokeUp",
                GENERIC_READ | GENERIC_WRITE,
                0,
                nullptr,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                nullptr
            );

            if (!driverHandler || (driverHandler == INVALID_HANDLE_VALUE)) {
                // std::cerr << "CreateFileA failed with error: " << GetLastError() << ". Retrying in " << retryDelay << " ms..." << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(retryDelay));
                retryDelay *= 2; // Exponential backoff
                continue;
            }

            success = authDriver();
            break;
        }

        return success;
    }

    bool authDriver() {
        auto otp = GenerateOTP();
        
        DWORD bytesReturned;

        return DeviceIoControl(
            driverHandler, 
            IOCTL_VERIFY, 
            &otp, 
            sizeof(otp), 
            NULL,
            0, 
            &bytesReturned, 
            NULL
        );
    }

    auto move_mouse(LONG x, LONG y, USHORT button_flags) -> void
    {
        MOUSE_MOVE_DATA mouseData = { x, y, button_flags };
                    
        DeviceIoControl(
            driverHandler,                  // Device handle
            IOCTL_MOVE_MOUSE,               // Control code
            &mouseData,                     // Input buffer
            sizeof(mouseData),              // Input buffer size
            &mouseData,                     // Output buffer (reuse input buffer for output)
            sizeof(mouseData),              // Output buffer size
            nullptr,                        // Bytes returned
            nullptr                         // Overlapped structure (not using overlapped I/O in this example)
        );
    }

    auto changePID(int src_pid, int value) -> void
    {
        CHANGE_PID changePID = { src_pid, value };

        DeviceIoControl(
            driverHandler,                  // Device handle
            IOCTL_CHANGE_PID,               // Control code
            &changePID,                     // Input buffer
            sizeof(changePID),              // Input buffer size
            &changePID,                     // Output buffer (reuse input buffer for output)
            sizeof(changePID),              // Output buffer size
            nullptr,                        // Bytes returned
            nullptr                         // Overlapped structure (not using overlapped I/O in this example)
        );
    }

    auto StreamMode(uint32_t WindowHandle, uint32_t value)
    {
        STREAMMODE screendata = { WindowHandle, value };

        DeviceIoControl(
            driverHandler,                  // Device handle
            IOCTL_DISPLAY_AFFINITY,         // Control code
            &screendata,                    // Input buffer
            sizeof(screendata),             // Input buffer size
            &screendata,                    // Output buffer (reuse input buffer for output)
            sizeof(screendata),             // Output buffer size
            nullptr,                        // Bytes returned
            nullptr                         // Overlapped structure (not using overlapped I/O in this example)
        );
    }

};

_driver driver;
