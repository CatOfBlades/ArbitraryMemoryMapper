

# ArbitraryMemoryMapper

ArbitraryMemoryMapper is a powerful library that provides unified memory access through its "readMemory" and "writeMemory" functions.
This library offers a convenient and efficient way to manage complex memory configurations, allowing access from a single address space.

Features:

## Code::Blocks target Lib/Lib32

This is a DLL file and Lua extension that enables other Windows projects to seamlessly set up and manage memory mappings. With this extension, you can integrate ArbitraryMemoryMapper into programs like games, even if the source code is not available.
Code::Blocks target LuaMap

## LuaMap
LuaMap is a versatile Lua script processor that allows you to create and host custom memory mappings. You can define your own memory contexts and pages, providing fine-grained control over memory access in your applications.
"MemoryMap.lua" File

The LuaMap application expects a "MemoryMap.lua" file in the same directory, and it is how you control the application. 

Heres a listing of available functions to use within your MemoryMap.lua:

    createMemoryContext(string ID)
    destroyMemoryContext(string ID)
    SysBeep(int freq, int time)
    addVirtualPage(string ID, int PageSize)
    addInterprocessPage(string ID, int PageSize, string windowName, int address)
    addMultiPage(string ID, int PageSize, int pagelistSize, string** pagelist)
    addMetaPage(string ID, int PageSize, string SubMemorySpaceName, int address)
    addLuaPage(string ID, string luafile)
    linkPageToMemorySpace(string MemorySpaceName, string PageName)
    destroyPage(string ID)
    buf{} = readMemFromContext(string ContextID, int Address, int length)
    writeMemToContext(string ContextID, int Address, int length, buf{})
    multiPageSwapBanks(string ID, int bankNum)

These functions provide a flexible and powerful way to create and manage memory contexts, pages, and memory mappings according to your specific requirements.
LuaMap Application

Example "MemoryMap.lua" files are provided for refrence.

The LuaMap application maintains two global lists:

List of Memory Contexts: These are arrays of memory pages that can be read from and written to, and are accessed by name. You can create your own memory contexts using "lua defined page" and reference them as needed.
List of Pages: These are memory interfaces, which can be files, script-defined math problems, or allocated arrays of bytes. Pages can be linked to a memory space using the "linkPageToMemorySpace" function, allowing for efficient and dynamic memory management.

## OpenGL Visualizer

(Placeholder for OpenGL visualizer screenshot)

ArbitraryMemoryMapper also includes an OpenGL visualizer that provides a graphical representation of memory mappings, making it easy to visualize and debug complex memory configurations.

## Usage

(Instructions on how to link ArbitraryMemoryMapper to different Lua binaries)

There is a powershell script that pulls the revision info and adds the build time to an automatically generated "revinfo.h"
 the powershell script is thus:
 
	powershell " Remove-Item .\revinfo.h; git show | Out-File -Encoding ascii -FilePath .\tmprevinfo.txt; '// file automatically generated on build, do not edit.' | Out-File -Encoding ascii -FilePath .\revinfo.h; '#include <string>' | Out-File -Encoding ascii -Append -FilePath .\revinfo.h ; 'std::string BuildInfo = \"' + (Get-Content .\tmprevinfo.txt | Select -First 3) + 'build at: $(NOW_L) \";' | Out-File -Encoding ascii -Append -FilePath .\revinfo.h; Remove-Item .\tmprevinfo.txt"

## Known Issues

There is a known issue where a page may be destroyed by the first "memoryContext" object that references it. Please be cautious when managing memory contexts and pages to avoid unexpected behavior.



With ArbitraryMemoryMapper, you have a powerful tool at your disposal for managing memory access in your applications. Its flexible and intuitive API, along with the LuaMap application and OpenGL visualizer, make it a valuable asset for game development, embedded systems, and other memory-intensive applications. Try it out and experience the convenience and efficiency of unified memory access with ArbitraryMemoryMapper.

