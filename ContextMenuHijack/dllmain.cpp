#include <windows.h>
#include <Guiddef.h>
#include "ClassFactory.h"
#include "Reg.h"

const CLSID CLSID_FileContextMenuExt = { 0xb7cdf620, 0xdb73, 0x44c0,{ 0x86, 0x11, 0x83, 0x2b, 0x26, 0x1a, 0x01, 0x07 } };

HINSTANCE   g_hInst     = NULL;
long        g_cDllRef   = 0;


BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason) {

	case DLL_PROCESS_ATTACH:
        g_hInst = hModule;
        DisableThreadLibraryCalls( hModule );
        break;

	case DLL_THREAD_ATTACH:

	case DLL_THREAD_DETACH:

	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

__declspec(dllexport) STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
    HRESULT hr = CLASS_E_CLASSNOTAVAILABLE;

    if ( IsEqualCLSID( CLSID_FileContextMenuExt, rclsid ) ) {
        hr = E_OUTOFMEMORY;

        ClassFactory *pClassFactory = new ClassFactory();
        if ( pClassFactory ) {
            hr = pClassFactory->QueryInterface( riid, ppv );
            pClassFactory->Release();
        }
    }
    return hr;
}

__declspec(dllexport) STDAPI DllCanUnloadNow(void)
{
    return g_cDllRef > 0 ? S_FALSE : S_OK;
}

__declspec(dllexport) STDAPI DllRegisterServer(void)
{
    HRESULT hr;
    wchar_t szModule[MAX_PATH];

    if ( GetModuleFileName( g_hInst, szModule, ARRAYSIZE( szModule ) ) == 0 ) {
        hr = HRESULT_FROM_WIN32( GetLastError() );
        return hr;
    }

    hr = RegisterInprocServer( szModule, CLSID_FileContextMenuExt, L"ContextMenuHijack.FileContextMenuExt Class", L"Apartment" );
    if ( SUCCEEDED( hr ) )
    {
        hr = RegisterShellExtContextMenuHandler(L"AllFilesystemObjects", CLSID_FileContextMenuExt, L"ContextMenuHijack.FileContextMenuExt");
    }
    return hr;
}


__declspec(dllexport) STDAPI DllUnregisterServer(void)
{
    HRESULT hr = S_OK;

    wchar_t szModule[MAX_PATH];
    if ( GetModuleFileName( g_hInst, szModule, ARRAYSIZE( szModule ) ) == 0 )
    {
        hr = HRESULT_FROM_WIN32( GetLastError() );
        return hr;
    }

    hr = UnregisterInprocServer( CLSID_FileContextMenuExt );
    if ( SUCCEEDED( hr ) )
    {
        hr = UnregisterShellExtContextMenuHandler( L"AllFilesystemObjects", CLSID_FileContextMenuExt );
    }
    return hr;
}
