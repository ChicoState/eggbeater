#include <eggbeater/Discovery.h>

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <sstream>

#include <iostream>

#ifndef DEBUG
#define DEBUG 0
#endif

#if DEBUG > 0
  #define D_RUN(...) { __VA_ARGS__; }
#else
  #define D_RUN(...)
#endif

//! @todo Figure out which symbols are defined on Windows
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
      else
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
      else
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
    
    retValue = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                             SERIAL_LOOKUP_PATH,
                             0,
                             KEY_READ & ~KEY_NOTIFY,
                             &serialEntries);
    
    if (retValue != ERROR_SUCCESS)
    {
      char msgBuffer[129];
      
      snprintf(msgBuffer,
               sizeof(msgBuffer),
               "Could not get value at HKEY_LOCAL_MACHINE\\%s: %d",
               SERIAL_LOOKUP_PATH,
               GetLastError());
      
      throw Exception(msgBuffer);
    }
    
    retValue = RegGetValueA(serialEntries,
                            NULL,
                            "Count",
                            RRF_RT_REG_DWORD,
                            &countType,
                            (PVOID)&countData,
                            &dataSize);
    
    if (retValue != ERROR_SUCCESS)
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
    
    for (uint32_t i = 0; i < countData; i++)
    {
      sprintf(buffer, "%d", i);
      
      path = reg_read_string(serialEntries,
                             NULL,
                             buffer,
                             36);
      
      if (path != "")
      {
        hwIDs = reg_read_multi_sz(deviceEntry,
                                  path.c_str(),
                                  "HardwareID");
        
        // C++11 for each loop
        for (auto hwID : hwIDs)
        {
          if (check_hardware_id(hwID, vid, pid))
          {
            portName = reg_read_string(deviceEntry,
                                       path.c_str(),
                                       "PathName");
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

#include <fstream>
#include <iomanip>

#define SHELLSCRIPT "lsusb -d %0.4x:$0.4x > serialinfo.txt"
//#/bin/bash \n\ 
//                     echo \"cd /dev/serial/by-id\" \n\ 
//                     echo \"lsusb -d 0483:5740 > serialinfo.txt\" \n"
#define PORTSCRIPT "ls /dev/serial/by-id > portinfo.txt"

namespace EggBeater
{
  uint32_t string_get_number(const std::string& str)
  {
    uint32_t i;
    std::stringstream ss(str);
    
    ss << std::hex;
    ss >> i;
    
    return i;
  }
  
  std::string get_ls_script(uint16_t vid, uint16_t pid)
  {
    std::stringstream ss;
    
    ss << "lsusb -d" << std::hex;
    
    ss.width(4);
    ss.fill('0');
    ss << vid;
    
    ss << ":";
    
    ss.width(4);
    ss.fill('0');
    ss << pid;
    
    ss << " > serialinfo.txt";
    
    //<< std::setw(4) << std::fill('0')
    //ss << vid << std::set ":" << pid << " > serialinfo.txt";
    
    return ss.str();
  }

  StringList discover_devices(uint16_t vid, uint16_t pid)
  {
    uint32_t str_vid, str_pid;
    StringList ports;
    
    std::string shell_script = get_ls_script(vid, pid);
    
    D_RUN(std::cout << "system(" << shell_script << ")" << std::endl);
    
    system(shell_script.c_str());
    
    std::string filename = "serialinfo.txt";
    std::ifstream input(filename.c_str());  
    std::string temp;
    
    if(input.is_open())
    {
      D_RUN(std::cout << filename << " is open" << std::endl);
      
      while(getline(input, temp))
      {
        D_RUN(std::cout << "read line: " << temp << std::endl);
        
        int index = temp.find(": ID ") + 5;
        temp = temp.substr(index);
        
        std::stringstream ss(temp); 
        std::string firstNumString;
        std::string secondNumString;
        std::getline(ss, firstNumString, ':');
        ss >> secondNumString;
        
        str_vid = string_get_number(firstNumString);
        str_pid = string_get_number(secondNumString);
        
        D_RUN(std::cout << "found vid=" << std::hex << str_vid << std::endl);
        D_RUN(std::cout << "found pid=" << std::hex << str_pid << std::endl);
        
        if(vid == str_vid && pid == str_pid)
        {
          D_RUN(std::cout << "system(" << PORTSCRIPT << ")" << std::endl);
          
          system(PORTSCRIPT);
          
          std::string filename2 = "portinfo.txt";
          std::ifstream input2(filename2.c_str());
          std::string portpath;
          
          if(input2.is_open())
          {
            D_RUN(std::cout << filename2 << " is open" << std::endl);
            
            std::getline(input2, portpath);
            std::string full_path = "/dev/serial/by-id/";
            full_path.append(portpath);
            
            ports.push_back(full_path);
            input2.close();
          }
          else
          {
            std::cout << "Unable to open file" << std::endl << std::endl;
          }     
        } 
      }
      input.close();
    }
    else
    {
      std::cout << "Unable to open file" << std::endl << std::endl;
    }
    
    
    
    //Take in the command line stuff and process it
    //Check to see if PID and VID match
    //Throw exceptions if no device is on the serial bus 
    
/*******************************************************************************
              Linux and Mac OS X discovery code goes here.
*******************************************************************************/
    
    return ports;
  }
}

  //#error Not yet implemented
#endif