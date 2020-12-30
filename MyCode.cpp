#include <windows.h>


__declspec(dllexport) int __stdcall ApproveAddOn(const GUID* addOnId, int ieVer)
{
    HMODULE ieFrameDLL = ::LoadLibrary(L"ieframe.dll");
    if (!ieFrameDLL)
    {
        return 1;
    }
    const DWORD dllSize = 16 * 1024 * 1024;
    const WCHAR* subKeyString = L"Software\\Microsoft\\Internet Explorer\\Approved Extensions";
    const BYTE cmdPush80000001[] = { 0x68, 0x01, 0x00, 0x00, 0x80 };
    //const BYTE cmdPushSubkey[] = { 0x68, 0xB8, 0xED, 0x10, 0x10 };
    const BYTE cmdFunctionBegin[] = { 0x8B, 0xFF, 0x55, 0x8B, 0xEC };
    void *pfn = NULL;
    BYTE* begin = (BYTE*)ieFrameDLL;
    BYTE* end = begin + dllSize;

    __try
    {
        for (; begin < end; ++begin)
        {
            if (::memcmp(cmdPush80000001, begin, sizeof(cmdPush80000001)) == 0)
            {
                if (*(begin - 5) == 0x68)//push
                {
                    WCHAR* subKey = *(WCHAR**)(begin - 4);
                    if ((BYTE*)subKey > (BYTE*)ieFrameDLL && (BYTE*)subKey < end)
                    {
                        if (::wcscmp(subKeyString, subKey) == 0)
                        {
                            BYTE* p = begin - 10;
                            while (p > begin - 1000)
                            {
                                if (::memcmp(cmdFunctionBegin, p, sizeof(cmdFunctionBegin)) == 0)
                                {
                                    pfn = p;
                                    break;
                                }
                                --p;
                            }
                            if (pfn)
                            {
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        
    }

    if (pfn)
    {
        typedef int(__stdcall* Ext_ApproveFrameAddOnIE9)(bool);
        Ext_ApproveFrameAddOnIE9 pfnIE9 = (Ext_ApproveFrameAddOnIE9)pfn;

        typedef int(__stdcall* Ext_ApproveFrameAddOnIE10)(const struct _GUID *, bool);
        Ext_ApproveFrameAddOnIE10 pfnIE10 = (Ext_ApproveFrameAddOnIE10)pfn;

        typedef int(__fastcall* Ext_ApproveFrameAddOnIE11)(const struct _GUID *, bool);
        Ext_ApproveFrameAddOnIE11 pfnIE11 = (Ext_ApproveFrameAddOnIE11)pfn;

        __try
        {
            if (ieVer == 9)
            {
                __asm
                {
                    push esi;
                    mov esi, addOnId;
                }
                pfnIE9(true);
                __asm pop esi;
            }
            else if (ieVer == 10)
            {
                pfnIE10(addOnId, true);
            }
            else if (ieVer == 11)
            {
                pfnIE11(addOnId, true);
            }
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {
            return 2;
        }
        return 0;
    }

    return 1;
}


