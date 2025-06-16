#pragma once

#include <windows.h>
#include <stdio.h>
#include <bcrypt.h>

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

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif

const WCHAR g_Seed[] = L"dREAMpIKAcHu"; // same seed

typedef LONG NTSTATUS;
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)

// 832-bit state = 32 bytes
#define HASH_LENGTH 32

// portable 32-bit rotate
#define ROL32(x,n)  ( ((x) << (n)) | ((x) >> (32 - (n))) )

//------------------------------------------------------------------------------
// same fold-hash
// Function to generate a hash
NTSTATUS GenerateHash(BYTE* data, ULONG dataSize, BYTE* hashBuffer, ULONG hashBufferSize) {
    BCRYPT_ALG_HANDLE hAlgorithm = NULL;
    NTSTATUS status = BCryptOpenAlgorithmProvider(&hAlgorithm, BCRYPT_SHA256_ALGORITHM, NULL, 0);
    if (!NT_SUCCESS(status)) return status;

    BCRYPT_HASH_HANDLE hHash = NULL;
    status = BCryptCreateHash(hAlgorithm, &hHash, NULL, 0, NULL, 0, 0);
    if (!NT_SUCCESS(status)) {
        BCryptCloseAlgorithmProvider(hAlgorithm, 0);
        return status;
    }

    status = BCryptHashData(hHash, data, dataSize, 0);
    if (NT_SUCCESS(status)) {
        status = BCryptFinishHash(hHash, hashBuffer, hashBufferSize, 0);
    }

    BCryptDestroyHash(hHash);
    BCryptCloseAlgorithmProvider(hAlgorithm, 0);
    return status;
}

//------------------------------------------------------------------------------
// dynamic RtlGetVersion
void
GetWindowsBuildNumber(
    _Out_ WCHAR* buildNumber,
    _In_  ULONG  bufferChars
)
{
    typedef NTSTATUS(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);
    HMODULE h = GetModuleHandleW(L"ntdll.dll");
    if (h) {
        auto fn = (RtlGetVersionPtr)GetProcAddress(h, "RtlGetVersion");
        if (fn) {
            RTL_OSVERSIONINFOW os;
            os.dwOSVersionInfoSize = sizeof(os);
            if (fn(&os) == STATUS_SUCCESS) {
                swprintf_s(buildNumber,
                    bufferChars,
                    L"%u",
                    os.dwBuildNumber);
                return;
            }
        }
    }
    swprintf_s(buildNumber, bufferChars, L"UNKNOWN");
}

//------------------------------------------------------------------------------
// simple GetComputerName
void
GetComputerNameWstr(
    _Out_ WCHAR* computerName,
    _In_  ULONG  bufferChars
)
{
    DWORD len = bufferChars;
    GetComputerNameW(computerName, &len);
}

//------------------------------------------------------------------------------
// build-and-hash exactly like kernel
ULONG
GenerateOTPWithTime(
    _In_ ULONG timeStamp
)
{
    BYTE   hash[HASH_LENGTH] = { 0 };
    WCHAR  buildNumber[16] = { 0 };
    WCHAR  computerName[64] = { 0 };
    WCHAR  combined[256] = { 0 };

    // pieces
    GetWindowsBuildNumber(buildNumber, ARRAYSIZE(buildNumber));
    // printf("Build Number: %ws\n", buildNumber);

    GetComputerNameWstr(computerName, ARRAYSIZE(computerName));
    // printf("Computer Name: %ws\n", computerName);

    // printf("Seed: %ws\n", g_Seed);

    // combine
    swprintf_s(combined,
        ARRAYSIZE(combined),
        L"%ws%ws%lu%ws",
        buildNumber,
        computerName,
        timeStamp,
        g_Seed);
    // printf("Combined Data: %ws\n", combined);

    // exact byte-count
    ULONG dataSize = (ULONG)(wcslen(combined) * sizeof(WCHAR));
    // printf("Combined length: %u chars, %u bytes\n",
    //    (ULONG)wcslen(combined),
    //    dataSize);

    // hash
    NTSTATUS hs = GenerateHash((BYTE*)combined,
        dataSize,
        hash,
        sizeof(hash));
    // printf("GenerateHash status: 0x%08lx\n", hs);

    if (hs == STATUS_SUCCESS) {
        // dump state
        ULONG* st = (ULONG*)hash;
    }

    // OTP
    ULONG otp = *(ULONG*)hash;
    // printf("Generated OTP: %lu\n", otp);
    return otp;
}

//------------------------------------------------------------------------------
// 30-sec window
ULONG
GenerateOTP()
{
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    ULARGE_INTEGER uli = { ft.dwLowDateTime, ft.dwHighDateTime };

    ULONG seconds = (ULONG)(uli.QuadPart / 10000000ULL);
    ULONG window = seconds / 30;
    // printf("Time Window: %lu\n", window);

    return GenerateOTPWithTime(window);
}
