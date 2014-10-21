#include <eggbeater/Discovery.h>

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include <iostream>

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)

#ifndef WIN32_LEAN_AND_MEAN
  #define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>

#define SERIAL_LOOKUP_PATH  "SYSTEM\\CurrentControlSet\\Services\\usbser\\Enum"
#define DEVICE_LOOKUP_PATH  "SYSTEM\\CurrentControlSet\\Enum"

namespace EggBeater
{
  /*
    Conditions:
      2
    Exit points:
      1
    M = 2 - 1 + 2 = 3
    Cyclomatic complexity
      3
  */
  StringList split_multi_sz(const char* const multi_sz, uint32_t maxLen)
  {
    StringList strings;
    String str;
    uint32_t index = 0;
    
    while (multi_sz[index] && index < maxLen)
    {
      str.assign(&(multi_sz[index]));
      
      index += str.length() + 1;
      
      strings.push_back(str);
    }
    
    return strings;
  }
  
  HKEY reg_open(HKEY regKey, const char* subKey, DWORD opt, REGSAM access)
  {
    HKEY key;
    LONG retValue = RegOpenKeyEx(regKey,
                                 subKey,
                                 opt,
                                 access,
                                 &key);
                                
    if (retValue != ERROR_SUCCESS)
    {
      if (retValue == ERROR_FILE_NOT_FOUND || retValue == ERROR_PATH_NOT_FOUND)
      {
        return NULL;
      }
      else
      {
        char msgBuffer[129];
        
        snprintf(msgBuffer,
                 sizeof(msgBuffer),
                 "reg_open: Could not open key at %s: %d",
                 subKey,
                 GetLastError());
        
        throw Exception(msgBuffer);
      }
    }
    else
    {
      return key;
    }
  }
  
  bool reg_exist(HKEY regKey, const char* subKey, const char* valueName)
  {
    LONG retValue = RegGetValueA(regKey,
                                 subKey,
                                 valueName,
                                 RRF_RT_ANY,
                                 NULL,
                                 NULL,
                                 NULL);
    if (retValue != ERROR_SUCCESS)
    {
      if (retValue == ERROR_FILE_NOT_FOUND || retValue == ERROR_PATH_NOT_FOUND)
      {
        return false;
      }
      else
      {
        char msgBuffer[129];
        
        snprintf(msgBuffer,
                 sizeof(msgBuffer),
                 "reg_exist: Could not query value at %s\\%s: %d",
                 subKey,
                 valueName,
                 GetLastError());
        
        throw Exception(msgBuffer);
      }
    }
    else
    {
      return true;
    }
  }
  
  /*
    Conditions:
      2
    Exit points:
      1
    M = 2 - 1 + 2 = 3
    Cyclomatic complexity
      3
  */
  StringList reg_read_multi_sz(HKEY regKey,
                               const char* subKey,
                               const char* valueName)
  {
    std::vector<char> buffer;
    DWORD bufferSize;
    LONG retValue = ~ERROR_SUCCESS;
    
    buffer.resize(16);
    bufferSize = buffer.size();
    
    if (!reg_exist(regKey, subKey, valueName))
    {
      return StringList();
    }
    
    do
    {
      retValue = RegGetValueA(regKey,
                              subKey,
                              valueName,
                              RRF_RT_REG_MULTI_SZ,
                              NULL,
                              (PVOID)buffer.data(),
                              &bufferSize);
      
      if (retValue == ERROR_MORE_DATA)
      {
        buffer.resize(bufferSize);
        bufferSize = buffer.size();
      }
      else if (retValue != ERROR_SUCCESS)
      {
        char msgBuffer[129];
        
        snprintf(msgBuffer,
                 sizeof(msgBuffer),
                 "Could not get value at %s\\%s: %d",
                 subKey,
                 valueName,
                 GetLastError());
        
        throw Exception(msgBuffer);
      }
      
    } while (retValue != ERROR_SUCCESS);
    
    return split_multi_sz(buffer.data(), bufferSize);
  }
  
  String reg_read_string(HKEY regKey,
                         const char* subKey,
                         const char* valueName,
                         DWORD initialSize = 16)
  {
    String buffer;
    DWORD bufferSize;
    LONG retValue = ~ERROR_SUCCESS;
    
    buffer.resize(initialSize);
    bufferSize = buffer.size();
    
    if (!reg_exist(regKey, subKey, valueName))
    {
      return String();
    }
    
    do
    {
      retValue = RegGetValueA(regKey,
                              subKey,
                              valueName,
                              RRF_RT_REG_SZ,
                              NULL,
                              (PVOID)buffer.data(),
                              &bufferSize);
      // Don't have enough space in the buffer, yet
      if (retValue == ERROR_MORE_DATA)
      {
        buffer.resize(bufferSize);
        bufferSize = buffer.size();
      }
      // Some other error
      else if (retValue != ERROR_SUCCESS)
      {
        char msgBuffer[129];
        
        snprintf(msgBuffer,
                 sizeof(msgBuffer),
                 "Could not get value at %s\\%s: %d/%d",
                 subKey,
                 valueName,
                 GetLastError(),
                 retValue);
        
        throw Exception(msgBuffer);
      }
    } while (retValue != ERROR_SUCCESS);
    
    return buffer;
  }
  
  /*
    Conditions:
      2
    Exit point:
      1
    Constant:
      2
    M
      3
  */
  bool check_hardware_id(const String& hwID, uint16_t vid, uint16_t pid)
  {
    char buffer[65] = {0};
    String hwIDlower;
    String ref;
    
    for (auto c : hwID)
      hwIDlower.append(1, (char)tolower(c));
    
    snprintf(buffer, sizeof(buffer), "usb\\vid_%04x&pid_%04x", vid, pid);
    ref.assign(buffer);
    
    return ref == hwIDlower;
  }
  
  StringList discover_devices(uint16_t vid, uint16_t pid)
  {
    StringList ports;
    StringList hwIDs;
    HKEY serialEntries;
    HKEY deviceEntry;
    DWORD countType = 0;
    DWORD countData = 0;
    DWORD dataSize = sizeof(DWORD);
    LONG retValue;
    String portName, path;
    char  buffer[16] = {0};
    
    serialEntries = reg_open(HKEY_LOCAL_MACHINE,
                             SERIAL_LOOKUP_PATH,
                             0,
                             KEY_READ & ~KEY_NOTIFY);
    
    /*
    retValue = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                             SERIAL_LOOKUP_PATH,
                             0,
                             KEY_READ & ~KEY_NOTIFY,
                             &serialEntries);
    */
    
    if (!serialEntries)
      return ports;
    
    /*
    if (retValue != ERROR_SUCCESS)
    {
      if (
      char msgBuffer[129];
      
      snprintf(msgBuffer,
               sizeof(msgBuffer),
               "Could not open key at HKEY_LOCAL_MACHINE\\%s: %d",
               SERIAL_LOOKUP_PATH,
               GetLastError());
      
      throw Exception(msgBuffer);
    }
    */
    
    retValue = RegGetValueA(serialEntries,
                            NULL,
                            "Count",
                            RRF_RT_REG_DWORD,
                            &countType,
                            (PVOID)&countData,
                            &dataSize);
    
    if (retValue == ERROR_FILE_NOT_FOUND || retValue == ERROR_PATH_NOT_FOUND)
    {
      // Expected failure
      return ports;
    }
    else if (retValue != ERROR_SUCCESS)
    {
      char msgBuffer[129];
      
      snprintf(msgBuffer,
               sizeof(msgBuffer),
               "Could not get value at HKEY_LOCAL_MACHINE\\%s\\%s: %d",
               SERIAL_LOOKUP_PATH,
               "Count",
               GetLastError());
      
      RegCloseKey(serialEntries);
      
      throw Exception(msgBuffer);
    }
    
    deviceEntry = reg_open(HKEY_LOCAL_MACHINE,
                           DEVICE_LOOKUP_PATH,
                           0,
                           KEY_READ & ~KEY_NOTIFY);
    
    if (!deviceEntry)
    {
      RegCloseKey(serialEntries);
      return ports;
    }
    
    /*
    retValue = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                             DEVICE_LOOKUP_PATH,
                             0,
                             KEY_READ & ~KEY_NOTIFY,
                             &deviceEntry);
    
    if (retValue != ERROR_SUCCESS)
    {
      char msgBuffer[129];
      
      snprintf(msgBuffer,
               sizeof(msgBuffer),
               "Could not get value at HKEY_LOCAL_MACHINE\\%s: %d",
               DEVICE_LOOKUP_PATH,
               GetLastError());
      
      RegCloseKey(serialEntries);
      
      throw Exception(msgBuffer);
    }
    */
    
    for (uint32_t i = 0; i < countData; i++)
    {
      sprintf(buffer, "%d", i);
      
      path = reg_read_string(serialEntries,
                             NULL,
                             buffer,
                             30);
      
      // Trim NULL terminators
      while (path[path.length()-1] == 0)
        path.resize(path.length() - 1);
      
      if (path != "")
      {
        hwIDs = reg_read_multi_sz(deviceEntry,
                                  path.c_str(),
                                  "HardwareID");
        
        // Strip the extra NULL terminator
        //path.resize(path.length() - 2);
        
        path = path.append("\\Device Parameters");
        
        //for (auto c : path)
        //{
        //  std::cout << (int)c << std::endl;
        //}
        
        //std::cout << path << std::endl;
        //std::cout << path.c_str() << std::endl;
        
        // C++11 for each loop
        for (auto hwID : hwIDs)
        {
          if (check_hardware_id(hwID, vid, pid))
          {
            portName = reg_read_string(deviceEntry,
                                       path.c_str(),
                                       "PortName");
            if (portName != "")
              ports.push_back(portName);
          }
        }
      }
    }
    
    RegCloseKey(deviceEntry);
    RegCloseKey(serialEntries);
    
    return ports;
  }
}

#else

namespace EggBeater
{
  // Any needed helper function should be defined here
  
  
  
  StringList discover_devices(uint16_t vid, uint16_t pid)
  {
    StringList ports;
    
/*******************************************************************************
              Linux and Mac OS X discovery code goes here.
*******************************************************************************/
    
    return ports;
  }
}

  #error Not yet implemented
#endif