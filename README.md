# dllHidden
> hide the injected dll by PEB

## Environment

* OS:win10
* x86

## Interface Introduction

the class PEBInfo provide the interface:

* GetInfo: return the vector of dllInfo（the struct defined in class）
* HidInfo: given the dllBase, the function will hide it
* RecoverInfo: the function will recover the hidden dllInfo

## Example

```C++
#include "PEBEnumMod.h"
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

    for (auto const & i : Allinfo)
    {
        
        std::wcout
            << "ImageSize:" << i.ImageSize
            << "\tDllBase:" << i.DllBase
            << "\tEntryPoint" << i.EntryPoint
            << "\tFullPath:" << i.FullDllName.Buffer
            << std::endl;
    }

    std::cout << "Hid-----------------------------------------------" << std::endl;
    info.HidInfo(Allinfo[2].DllBase);
    std::wcout << "HidInfo:" << Allinfo[2].FullDllName.Buffer << std::endl;
    getchar();
    info.RecoverInfo();
    while (1);
}

```

