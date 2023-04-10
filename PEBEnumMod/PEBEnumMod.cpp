#include <iostream>
#include <Windows.h>
#include <winternl.h>
#include <vector>
#include "PEBEnumMod.h"

PEBInfo::PEBInfo() : pData(nullptr)
{
    PPEB _peb;
    _asm {
        mov eax, fs: [0x30] ;
        mov _peb, eax
    }

    FrontDiff = sizeof(pData->InLoadOrderModuleList);
    Pldr = _peb->Ldr;
    FIRST = &(Pldr->InMemoryOrderModuleList);
    CURLST = FIRST->Flink;
}

std::vector<PEBInfo::Info> PEBInfo::GetInfo()
{
    std::vector<Info> Allinfo;
    bool setLoadInfo = false;
    if (LoadInfo.size() == 0)
        setLoadInfo = true;
    do
    {
        Info info;
        pData = (PLDR_DATA_TABLE_ENTRY_SELF)((unsigned)CURLST - FrontDiff);
        if (!pData) throw "pData lost!";
        info.DllBase = pData->DllBase;
        info.FullDllName = pData->FullDllName;
        info.EntryPoint = pData->EntryPoint;
        info.ImageSize = pData->ImageSize;
        info.PLIST = &pData->InMemoryOrderLinks;
        Allinfo.push_back(info);
        if (setLoadInfo)
        {
            info.PLIST = &pData->InLoadOrderModuleList;
            LoadInfo.push_back(info);
            info.PLIST = &pData->InInitializationOrderModuleList;
            IniInfo.push_back(info);
        }
        CURLST = CURLST->Flink;
    } while (CURLST != FIRST);
    if (nowInfo.size() == 0) nowInfo = Allinfo;
    return Allinfo;
}

bool PEBInfo::HidInfo(LPVOID adrBase)
{
    for (auto& i : nowInfo)
    {
        if (!i.isHidden && i.DllBase == adrBase)
        {

            i.PLIST->Blink->Flink = i.PLIST->Flink;
            i.PLIST->Flink->Blink = i.PLIST->Blink;
            i.isHidden = true;
            break;
        }
    }
    for (auto& i : LoadInfo)
    {
        if (!i.isHidden && i.DllBase == adrBase)
        {

            i.PLIST->Blink->Flink = i.PLIST->Flink;
            i.PLIST->Flink->Blink = i.PLIST->Blink;
            i.isHidden = true;
            break;
        }
    }
    for (auto& i : IniInfo)
    {
        if (!i.isHidden && i.DllBase == adrBase)
        {

            i.PLIST->Blink->Flink = i.PLIST->Flink;
            i.PLIST->Flink->Blink = i.PLIST->Blink;
            i.isHidden = true;
            return true;
        }
    }
    return false;
}

void PEBInfo::RecoverInfo()

{
    for (auto& i : nowInfo)
    {
        if (i.isHidden)
        {
            i.PLIST->Blink->Flink = i.PLIST;
            i.PLIST->Flink->Blink = i.PLIST;
            i.isHidden = false;
            break;
        }
    }
    for (auto& i : LoadInfo)
    {
        if (i.isHidden)
        {
            i.PLIST->Blink->Flink = i.PLIST;
            i.PLIST->Flink->Blink = i.PLIST;
            i.isHidden = false;
            break;
        }
    }
    for (auto& i : IniInfo)
    {
        if (i.isHidden)
        {
            i.PLIST->Blink->Flink = i.PLIST;
            i.PLIST->Flink->Blink = i.PLIST;
            i.isHidden = false;
            break;
        }
    }
    return;
}
