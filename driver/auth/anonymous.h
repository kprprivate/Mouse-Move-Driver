#include <minwinbase.h>
#include <sysinfoapi.h>
#include <iostream>
#include <errhandlingapi.h>
#include <winreg.h>
#include <winerror.h>

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

class ANONYMOUS {
private:
    SYSTEMTIME originalTime;
    bool timeModified;

public:
    ANONYMOUS() : timeModified(false) {
        GetSystemTime(&originalTime);
    }

    ~ANONYMOUS() {
        RestoreTime();
    }

    bool Magica() {
        if (!PrepareNTPSync()) {
        }

        SYSTEMTIME currentTime;
        GetSystemTime(&currentTime);

        SYSTEMTIME newTime = currentTime;
        newTime.wYear -= 5;

        if (newTime.wMonth == 2 && newTime.wDay == 29 && !IsLeapYear(newTime.wYear)) {
            newTime.wDay = 28;
        }

        if (SetSystemTime(&newTime)) {
            timeModified = true;

            if (ValidateTimeChange()) {
                return true;
            }
            else {
                return true; 
            }
        }
        else {
            std::cout << "[INFO] Run as Admin\n";
            return false;
        }
    }

private:
    bool IsLeapYear(WORD year) {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    bool PrepareNTPSync() {
        HKEY hKey;
        LONG result;

        result = RegCreateKeyExW(
            HKEY_LOCAL_MACHINE,
            L"SYSTEM\\CurrentControlSet\\Services\\W32Time\\Parameters",
            0, nullptr, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE,
            nullptr, &hKey, nullptr
        );

        if (result != ERROR_SUCCESS) {
            return false;
        }

        const wchar_t* ntpType = L"NTP";
        result = RegSetValueExW(hKey, L"Type", 0, REG_SZ,
            (const BYTE*)ntpType, (wcslen(ntpType) + 1) * sizeof(wchar_t));

        RegCloseKey(hKey);
        return result == ERROR_SUCCESS;
    }

    bool ValidateTimeChange() {
        DWORD uptime = GetTickCount64() / 1000; // seconds
        FILETIME ft;
        GetSystemTimeAsFileTime(&ft);

        ULARGE_INTEGER current;
        current.LowPart = ft.dwLowDateTime;
        current.HighPart = ft.dwHighDateTime;

        ULONG unixTime = (ULONG)((current.QuadPart - 116444736000000000ULL) / 10000000ULL);

        if (uptime > unixTime) {
            return false;
        }

        return true;
    }

public:
    void RestoreTime() {
        if (timeModified) {
            if (SetSystemTime(&originalTime)) {
                timeModified = false;
            }
            else {
            }
        }
    }

    void ShowCurrentTime() {
        SYSTEMTIME current;
        GetSystemTime(&current);
    }

    bool IsTimeModified() {
        return timeModified;
    }
};