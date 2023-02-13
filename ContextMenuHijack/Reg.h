#pragma once

#include <windows.h>

HRESULT RegisterInprocServer(PCWSTR pszModule, const CLSID& clsid, PCWSTR pszFriendlyName, PCWSTR pszThreadModel);
HRESULT UnregisterInprocServer(const CLSID& clsid);
HRESULT RegisterShellExtContextMenuHandler(PCWSTR pszFileType, const CLSID& clsid, PCWSTR pszFriendlyName);
HRESULT UnregisterShellExtContextMenuHandler(PCWSTR pszFileType, const CLSID& clsid);


