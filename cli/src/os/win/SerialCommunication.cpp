#include <eggbeater/Serial.h>
#include <iostream>
#include <iomanip>

#ifndef WIN32_LEAN_AND_MEAN
  #define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN

#include <tchar.h>
#include <Windows.h>

#include "Serial.h"

#define SERIAL_BUFFER_SIZE 1024
#define SERIAL_USE_OVERLAP true

namespace EggBeater
{
  SerialCommunication::SerialCommunication() : dev(new CSerial())
  {
  }
  
  SerialCommunication::SerialCommunication(const String& device) : dev(new CSerial())
  {
    open(device);
  }
  
  SerialCommunication::SerialCommunication(const char* device) : dev(new CSerial())
  {
    open(device);
  }
  
  SerialCommunication::~SerialCommunication()
  {
    close();
  }
  
  bool SerialCommunication::open(const String& str)
  {
    LONG ret = dev->Open(str.c_str(),
                         SERIAL_BUFFER_SIZE,
                         SERIAL_BUFFER_SIZE,
                         SERIAL_USE_OVERLAP);

    if (ret != ERROR_SUCCESS)
      return false;
    
    ret = dev->SetEventChar(EGGBEATER_SOF_BYTE);
    
    if (ret != ERROR_SUCCESS)
      return false;
    
    return true;
  }
  
  void SerialCommunication::close()
  {
    dev->Close();
  }
  
  bool SerialCommunication::isOpen()
  {
    return dev->IsOpen();
  }
  
  bool SerialCommunication::isValid()
  {
    return dev->IsOpen();
  }
  
  bool SerialCommunication::haveData()
  {
    return dev->IsOpen(); // && (dev->WaitEvent(NULL) == ERROR_SUCCESS);
  }
  
  bool SerialCommunication::sendPacket(Packet& p)
  {
    if (!dev->IsOpen())
      return false;
      
    auto d = p.getRawPacket();
    
    LONG ret = dev->Write((void*)d->data(), d->size());
    
    return ret == ERROR_SUCCESS;
  }
  
  bool SerialCommunication::receivePacket(Packet& p)
  {
    LONG ret;
    Packet::ValueType d = p.getRawPacket();
    
    //std::cout << "Waiting for SOF byte" << std::endl;
    ret = dev->WaitEvent(NULL, 5000);
    
    if (ret != ERROR_SUCCESS)
    {
      char c = 0;
      ret = dev->Read((void*)&c, sizeof(char), NULL, NULL, 50);
      
      if (ret == ERROR_SUCCESS)
      {
        //std::cout << "Found unexpected byte: " << std::hex << std::setw(2) << std::setfill('0') << (((int)c) & 0xff) << std::endl;
      }
    
      //std::cout << "Waiting for SOF byte - failed" << std::endl;
      return false;
    }
    
    d->resize(sizeof(PacketHeader));
    
    //std::cout << "Reading packet header" << std::endl;
    ret = dev->Read((void*)d->data(), sizeof(PacketHeader));
    
    if (ret != ERROR_SUCCESS)
    {
      //std::cout << "Reading packet header - failed" << std::endl;
      return false;
    }
    
    PacketHeader* head = (PacketHeader*)d->data();
    PacketError err = p.isValid();
    
    // Bad size isn't really an error for here
    if (err != PacketError::None && err != PacketError::BadSize)
    {
      //std::cout << "Received packet is invalid" << std::endl;
      return false;
    }
    
    //std::cout << "Checking for additional data" << std::endl;
    if (p.getDataLength() > 0)
    {
      //std::cout << "Reading packet header - has data" << std::endl;
      d->resize(sizeof(PacketHeader) + p.getDataLength());
      
      // Read some more data
      ret = dev->Read(&(d->at(sizeof(PacketHeader))), p.getDataLength());
      
      if (ret != ERROR_SUCCESS)
      {
        //std::cout << "Reading packet header - has data - failed" << std::endl;
        return false;
      }
    }
    else
    {
      //std::cout << "Reading packet header - no data" << std::endl;
    }
    
    return true;
  }
}
