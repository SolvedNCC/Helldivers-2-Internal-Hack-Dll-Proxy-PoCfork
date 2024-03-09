// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Windows.h"
#include "version.h"
#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include "Memory.h"
#include "GameData.h"
#include <conio.h> // For _getch() on Windows
#include "xorstr.hpp"


HANDLE hMainThread;
HMODULE g_hModule;

//Console Menu using checkbox
struct Checkbox {
    std::string title;
    bool checked;
};

DWORD WINAPI Payload(LPVOID lpParam)
{
    // Initialize the proxy for the DLL
    HMODULE dllfwd = dllforward::setup();
    if (dllfwd)
        CloseHandle(dllfwd);

    //Console Menu
    std::vector<Checkbox> checkboxes = { 
        , {_XOR_("24"), true}
        , {_XOR_("25"), true}
        , {_XOR_("26"), false}
    
    }; // Initialize all checkboxes to unchecked
    const int numCheckboxes = checkboxes.size();
    size_t selectedCheckbox = 0;
    char userInput;

    HMODULE moduleHandle = nullptr;
    GameData gData;

    do
    {
        moduleHandle = GetModuleHandle(_XOR_(L"game.dll"));
        Sleep(1000);
    } while (!moduleHandle);
    Sleep(100);

    for (size_t i = 0; i < checkboxes.size(); ++i) {
        if (checkboxes[i].checked)
        {

            if (checkboxes[i].title == _XOR_("24"))
            {
                if (!gData.AllStratagems)
                {
                    BYTE AllStratagemsByte[] =
                    {
                        0xB0, 0x01, 0xC3
                    };

                    uintptr_t AllStratagems = Memory::FindPattern(_XOR_("game.dll"), _XOR_("48 89 5C 24 ?? 48 8B D9 85 D2 75 09"));
                    Memory::Patch((LPVOID)(AllStratagems), AllStratagemsByte, 3);
                    gData.AllStratagems = !gData.AllStratagems;
                    printf(_XOR_("[Active] 24\n"));
                }
            }

            if (checkboxes[i].title == _XOR_("25"))
            {
                if (!gData.AllEquipment)
                {
                    BYTE AllEquipmentByte[] =
                    {
                        0xB0, 0x01, 0xC3
                    };

                    uintptr_t AllEquipment = Memory::FindPattern(_XOR_("game.dll"), _XOR_("83 B9 ?? ?? ?? ?? ?? 75 ?? 85 D2 74 ?? 44 8B 89 ?? ?? ?? ?? 45 33 C0 45 85 C9 74 ?? 48 8D 81 ?? ?? ?? ?? 39 50 ?? 74 ?? 41 FF C0 48 83 C0 ?? 45 3B C1 72 ?? 32 C0 C3 8B 00 48 69 C8"));
                    Memory::Patch((LPVOID)(AllEquipment+11), AllEquipmentByte, 3);
                    gData.AllEquipment = !gData.AllEquipment;
                    printf(_XOR_("[Active] 25\n"));
                }
            }

            if (checkboxes[i].title == _XOR_("26"))
            {
                if (!gData.AllArmor)
                {
                    BYTE AllArmorByte[] =
                    {
                        0xB0, 0x01, 0xC3
                    };

                    uintptr_t AllArmor = Memory::FindPattern(_XOR_("game.dll"), _XOR_("48 83 EC ?? 44 8B 49 ?? 45 33 C0"));
                    Memory::Patch((LPVOID)(AllArmor), AllArmorByte, 3);
                    gData.AllArmor = !gData.AllArmor;
                    printf(_XOR_("[Active] 26\n"));
                }
            }
        }
    }
    printf(_XOR_("[Exit] Unload\n"));
    FreeConsole();
    FreeLibraryAndExitThread(g_hModule, 0);
    return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(hModule);
        g_hModule = hModule;
        hMainThread = CreateThread(NULL, 0, Payload, hModule, 0, NULL);
        if (hMainThread)
            CloseHandle(hMainThread);
    }break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        FreeConsole();
        FreeLibraryAndExitThread(hModule, 0);
        break;
    }
    return TRUE;
}

