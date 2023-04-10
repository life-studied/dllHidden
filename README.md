# dllHidden
> hide the injected dll by PEB

## Environment

* OS:win10
* x86

## Interface Introduction

the class PEBInfo provide the interface:

* GetInfo: return the vector of dllInfo（the struct defined in class）
* HidInfo: given the dllName（wide string）, the function will hide it, return the result
* RecoverInfo: the function will recover the hidden dllInfo

## Example

```C++
#include "PEBEnumMod.h"
#include "iostream"

int main()
{
    PEBInfo info;
    std::vector<PEBInfo::Info> Allinfo;
    try {
        Allinfo = info.GetInfo();
    }
    catch (const char* s)
    {
        std::cout << s << std::endl;
        return -1;
    }

    for (auto const& i : Allinfo)
    {

        std::wcout
            << "ImageSize:" << i.ImageSize
            << "\tDllBase:" << i.DllBase
            << "\tEntryPoint" << i.EntryPoint
            << "\tFullPath:" << i.FullDllName.Buffer
            << std::endl;
    }

    std::cout << "Hid-----------------------------------------------" << std::endl;
    if (!info.HidInfo(L"ntdll.dll")) std::cout << "could not find the following dll:" << std::endl;
    std::wcout << "HidInfo:" << L"ntdll.dll" << std::endl;
    getchar();
    info.RecoverInfo();
    while (1);
}

```

