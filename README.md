# ArbitraryMemoryMapper
Basically you write code that impliments a "readMemory" and "writeMemory" function and
this lets you aggrigate those interfaces and access them from a single address space.


The code::blocks target LuaMap is a lua script processor.
  You can use it to create and host the memory mappings you define.
  
  The application expects a "MemoryMap.lua" file in the same directory as it.
  The "MemoryMap.lua" is executed with these functions defined:
    createMemoryContext(string ID)
    destroyMemoryContext(string ID)
    SysBeep(int freq,int time)
    addVirtualPage(string ID, int PageSize)
    addInterprocessPage(string ID, int PageSize,string windowName,int address)
    addMultiPage(string ID, int PageSize, int pagelistSize, string** pagelist)
    addMetaPage(string ID, int PageSize,string SubMemorySpaceName,int address)
    addLuaPage(string ID, string luafile)
    linkPageToMemorySpace(string MemorySpaceName,string PageName)
    destroyPage(string ID)
    buf{} = readMemFromContext(string ContextID,int Address,int length) --buf is a table of bytes with 'length' number of entries
    writeMemToContext(string ContextID,int Address,int length,buf{}) --buf expects a table of bytes with 'length' number of entries
    multiPageSwapBanks(string ID, int bankNum)
    
    
  Basically the LuaMap application keeps 2 global lists
    #1 A list of all "MemoryContexts" which are arrays of memory pages that you can read and write to.
      They are created with a name that can be any string. And are accessed by name.
    #2 A list of all "Pages" which are memory interfaces, they could be files or script defined math problems or simply an allocated array of bytes.
      Use "lua defined page" to create your own contexts.
      They are also created with a string for a name. 
      A page can be refrenced by any number of memory contexts link a page to a memory space with the "linkPageToMemorySpace" function.
      
    Theres a known issue where a page will be killed by the first "memoryContext" object that is deleted that refrences it.
      This bug was intended to prevent memory leaks by lost pages.
      A good fix that I havent gotten arround to as of 9/11/2020 would be to expose a function to lua that unlinks the page from a memory context,
        that way a user could unlink shared pages before destroying the memoryContext.
        
 Note: example lua scripts can be found in the "LuaScripts" subdirectory.
      If you want to play arround with this project just drop the bin/LuaMap/LuaMap.exe into the /LuaScripts directory and run it in a console.
      
        
 It's a bit buggy and I would be happy if people could fork it and improve upon it.
 Generally so long as the changes improve usability and don't constrain it I would be happy to merge them.
