#include <eggbeater/Common.h>

using namespace EggBeater;

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
  // Running on Windows
  
  #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
  #endif

  #include <Windows.h>
  
  String EggBeater::getTempPath()
  {
    char tempPath[MAX_PATH];
    
    DWORD retCode = GetTempPathA(MAX_PATH, tempPath);
    
    if (retCode == 0)
    {
      // Error
      return "";
    }
    else
    {
      // Successful
      String path(tempPath);
      
      // Does the path end in a slash?
      if (path.back() == '\\' || path.back() == '/')
        // Then remove it
        path.pop_back();
        
      return path;
    }
  }
  
#else
  // Unix or Linux
  
  String EggBeater::getTempPath()
  {
    String path;
    
    path = getenv("TMPDIR");
    
    if (path.empty())
    {
      path = "/tmp";
    }
    
    if (path.back() == '\\' || path.back == '/')
      path.pop_back();
    
    return path;
  }
#endif
