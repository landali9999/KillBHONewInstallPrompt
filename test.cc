void main()
{
    HMODULE myCodeDLL = ::LoadLibrary(L"MyCode.dll");
    if (myCodeDLL)
    {
        typedef int(__stdcall* pfApproveAddOn)(const GUID* addOnId, int ieVer);
        pfApproveAddOn pf = (pfApproveAddOn)::GetProcAddress(myCodeDLL, "ApproveAddOn");
        if (pf)
        {
            //you bho guid
            const GUID II_BHO = { 0xBBB12AAA, 0x1337, 0x4286, 0xB9, 0x6C, 0xCB, 0x52, 0xD1, 0xE4, 0x8C, 0xD0 };
            // ie version ,  surpots 9, 10, 11  now
            pf(&II_BHO, 9);
        }
    }
}
