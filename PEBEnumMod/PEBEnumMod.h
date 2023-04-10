#pragma once
#include <Windows.h>
#include <winternl.h>
#include <vector>

class PEBInfo {
private:
    typedef struct _LDR_DATA_TABLE_ENTRY_SELF {
        LIST_ENTRY InLoadOrderModuleList;
        LIST_ENTRY InMemoryOrderLinks;
        LIST_ENTRY InInitializationOrderModuleList;
        PVOID DllBase;
        PVOID EntryPoint;
        PVOID ImageSize;
        UNICODE_STRING FullDllName;
        BYTE Reserved4[8];
        PVOID Reserved5[3];
#pragma warning(push)
#pragma warning(disable: 4201) // we'll always use the Microsoft compiler
        union {
            ULONG CheckSum;
            PVOID Reserved6;
        } DUMMYUNIONNAME;
#pragma warning(pop)
        ULONG TimeDateStamp;
    } LDR_DATA_TABLE_ENTRY_SELF, * PLDR_DATA_TABLE_ENTRY_SELF;

public:
    struct Info
    {
        PVOID DllBase;
        PVOID EntryPoint;
        PVOID ImageSize;
        UNICODE_STRING FullDllName;
        PLIST_ENTRY PLIST;
        bool isHidden;
        Info() : isHidden(false) {}
    };

private:
    PLDR_DATA_TABLE_ENTRY_SELF pData;
    int FrontDiff;
    PPEB_LDR_DATA Pldr;
    PLIST_ENTRY FIRST;
    PLIST_ENTRY CURLST;
    std::vector<Info> nowInfo;
    std::vector<Info> LoadInfo;
    std::vector<Info> IniInfo;
public:
    PEBInfo();
    std::vector<Info> GetInfo();
    bool HidInfo(LPVOID adrBase);
    void RecoverInfo();
};
