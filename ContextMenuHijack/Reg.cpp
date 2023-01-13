#include "Reg.h"
#include <strsafe.h>

HRESULT SetHKCRRegistryKeyAndValue(PCWSTR pszSubKey, PCWSTR pszValueName, PCWSTR pszData)
{
    HKEY hKey = NULL;

    HRESULT hr = HRESULT_FROM_WIN32( RegCreateKeyEx( HKEY_CLASSES_ROOT, pszSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL ) );

    if ( SUCCEEDED( hr ) ) {
        if ( pszData != NULL ) {
            DWORD cbData = lstrlen(pszData) * sizeof(*pszData);
            hr = HRESULT_FROM_WIN32(RegSetValueEx(hKey, pszValueName, 0, REG_SZ, reinterpret_cast<const BYTE *>(pszData), cbData));
        }
        RegCloseKey( hKey );
    }
    return hr;
}

HRESULT GetHKCRRegistryKeyAndValue(PCWSTR pszSubKey, PCWSTR pszValueName, PWSTR pszData, DWORD cbData)
{
    HKEY hKey = NULL;

    HRESULT hr = HRESULT_FROM_WIN32( RegOpenKeyEx( HKEY_CLASSES_ROOT, pszSubKey, 0, KEY_READ, &hKey ) );

    if ( SUCCEEDED( hr ) ) {
        hr = HRESULT_FROM_WIN32( RegQueryValueEx( hKey, pszValueName, NULL, NULL, ( LPBYTE )( pszData ), &cbData ) );
        RegCloseKey( hKey );
    }
    return hr;
}


HRESULT RegisterInprocServer(PCWSTR pszModule, const CLSID& clsid, PCWSTR pszFriendlyName, PCWSTR pszThreadModel)
{
    if ( pszModule == NULL || pszThreadModel == NULL ) {
        return E_INVALIDARG;
    }

    HRESULT hr;
    wchar_t szCLSID[ MAX_PATH ];

    StringFromGUID2( clsid, szCLSID, ARRAYSIZE( szCLSID ) );

    wchar_t szSubkey[ MAX_PATH ];

    hr = StringCchPrintf( szSubkey, ARRAYSIZE( szSubkey ), L"CLSID\\%s", szCLSID );
    if ( SUCCEEDED( hr ) ) {
        hr = SetHKCRRegistryKeyAndValue( szSubkey, NULL, pszFriendlyName );

        if ( SUCCEEDED( hr ) ) {
            hr = StringCchPrintf( szSubkey, ARRAYSIZE( szSubkey ), L"CLSID\\%s\\InprocServer32", szCLSID );
            if ( SUCCEEDED( hr ) ) {
                hr = SetHKCRRegistryKeyAndValue( szSubkey, NULL, pszModule );
                if ( SUCCEEDED( hr ) ) {
                    hr = SetHKCRRegistryKeyAndValue( szSubkey, L"ThreadingModel", pszThreadModel );
                }
            }
        }
    }
    return hr;
}


HRESULT UnregisterInprocServer(const CLSID& clsid)
{
    HRESULT hr = S_OK;
    wchar_t szCLSID[ MAX_PATH ];

    StringFromGUID2( clsid, szCLSID, ARRAYSIZE( szCLSID ) );

    wchar_t szSubkey[ MAX_PATH ];

    hr = StringCchPrintf( szSubkey, ARRAYSIZE( szSubkey ), L"CLSID\\%s", szCLSID );
    if ( SUCCEEDED( hr ) ) {
        hr = HRESULT_FROM_WIN32( RegDeleteTree( HKEY_CLASSES_ROOT, szSubkey ) );
    }
    return hr;
}


HRESULT RegisterShellExtContextMenuHandler(PCWSTR pszFileType, const CLSID& clsid, PCWSTR pszFriendlyName)
{
    if ( pszFileType == NULL ) {
        return E_INVALIDARG;
    }

    HRESULT hr;
    wchar_t szCLSID[ MAX_PATH ];

    StringFromGUID2( clsid, szCLSID, ARRAYSIZE( szCLSID ) );

    wchar_t szSubkey[ MAX_PATH ];

    if ( *pszFileType == L'.' ) {
        wchar_t szDefaultVal[ 260 ];
        hr = GetHKCRRegistryKeyAndValue( pszFileType, NULL, szDefaultVal, sizeof( szDefaultVal ) );

        if ( SUCCEEDED( hr ) && szDefaultVal[ 0 ] != L'\0' ) {
            pszFileType = szDefaultVal;
        }
    }

    hr = StringCchPrintf( szSubkey, ARRAYSIZE( szSubkey ), L"%s\\shellex\\ContextMenuHandlers\\%s", pszFileType, szCLSID );
    if ( SUCCEEDED( hr ) ) {
        hr = SetHKCRRegistryKeyAndValue( szSubkey, NULL, pszFriendlyName );
    }
    return hr;
}


HRESULT UnregisterShellExtContextMenuHandler(PCWSTR pszFileType, const CLSID& clsid)
{
    if (pszFileType == NULL) {
        return E_INVALIDARG;
    }

    HRESULT hr;
    wchar_t szCLSID[ MAX_PATH ];

    StringFromGUID2( clsid, szCLSID, ARRAYSIZE( szCLSID ) );

    wchar_t szSubkey[ MAX_PATH ];

    if (*pszFileType == L'.') {
        wchar_t szDefaultVal[ 260 ];
        hr = GetHKCRRegistryKeyAndValue( pszFileType, NULL, szDefaultVal, sizeof( szDefaultVal ) );

        if ( SUCCEEDED( hr ) && szDefaultVal[ 0 ] != L'\0' ) {
            pszFileType = szDefaultVal;
        }
    }

    hr = StringCchPrintf( szSubkey, ARRAYSIZE( szSubkey ), L"%s\\shellex\\ContextMenuHandlers\\%s", pszFileType, szCLSID );
    if ( SUCCEEDED( hr ) ) {
        hr = HRESULT_FROM_WIN32( RegDeleteTree( HKEY_CLASSES_ROOT, szSubkey ) );
    }
    return hr;
}
