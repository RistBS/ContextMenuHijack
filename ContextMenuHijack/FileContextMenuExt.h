#pragma once

#include <windows.h>
#include <shlobj.h>


class FileContextMenuExt : public IShellExtInit, public IContextMenu3
{
public:
    // IUnknown
    IFACEMETHODIMP QueryInterface(REFIID riid, void** ppv);
    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();

    // IShellExtInit
    IFACEMETHODIMP Initialize(LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hKeyProgID);

    // IContextMenu
    IFACEMETHODIMP QueryContextMenu(HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags);
    IFACEMETHODIMP InvokeCommand(LPCMINVOKECOMMANDINFO pici);
    IFACEMETHODIMP GetCommandString(UINT_PTR idCommand, UINT uFlags, UINT* pwReserved, LPSTR pszName, UINT cchMax);

    // IContextMenu2 IContextMenu3
    IFACEMETHODIMP HandleMenuMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);
    IFACEMETHODIMP HandleMenuMsg2(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* plResult);

    FileContextMenuExt(void);

protected:
    ~FileContextMenuExt(void);

private:
    long    m_cRef;
    wchar_t m_szSelectedFile[MAX_PATH];
};