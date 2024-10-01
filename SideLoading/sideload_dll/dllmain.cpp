// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Windows.h>

// Proxify the Function call to the Original DLL Function
#pragma comment(linker, "/export:AddInts=_addlib.AddInts,@1")

// Overrides the initial SysPause Function that is defined in the Original DLL
extern "C" __declspec(dllexport) int SysPause(void) {

    MessageBoxA(NULL, "SysPause function Override in Malicious DLL!", "DLL SysPause Sideload", MB_OK | MB_ICONEXCLAMATION);

    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

