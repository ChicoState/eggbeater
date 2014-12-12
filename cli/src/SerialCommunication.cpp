#include <eggbeater/Serial.h>

#include <stdio.h>
#include <iostream>

#if defined(__linux__)  && !defined(__CYGWIN__)
#include <string.h>
#include <termios.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

namespace EggBeater
{
  void set_serial_raw(const String& portPath)
  {
    struct termios termIOS;
    int fd = open(portPath.c_str(), O_RDWR | O_NONBLOCK);
    
    if (fd < 0)
      return;
    
    memset(&termIOS, 0, sizeof(struct termios));
    
    tcgetattr(fd, &termIOS);
    
    cfmakeraw(&termIOS);
    
    cfsetspeed(&termIOS, B38400);
    
    tcflush(fd, TCIFLUSH);
    
    tcsetattr(fd, TCSANOW, &termIOS);
    
    close(fd);
  }
}
#endif

namespace EggBeater
{
  SerialCommunication::SerialCommunication() : commPort()
  {
    // No-op
  }
  
  SerialCommunication::SerialCommunication(const String& portName)
  {
    this->open(portName);
  }
  
  SerialCommunication::SerialCommunication(const char* portName)
  {
    this->open(portName);
  }
  
  SerialCommunication::~SerialCommunication()
  {
    if (this->isOpen())
      this->close();
  }
  
  bool SerialCommunication::open(const String& portName)
  {
    if (this->isOpen())
    {
      this->close();
    }
    
    #if defined(__linux__) && !defined(__CYGWIN__)
    set_serial_raw(portName);
    #endif
    
    this->commPort.open(portName.c_str(), std::ios::in | std::ios::out | std::ios::binary);
    
    return this->isOpen();
  }
  
  void SerialCommunication::close()
  {
    this->commPort.close();
  }
  
  bool SerialCommunication::isOpen()
  {
    bool is_open;
    
    is_open = this->commPort.is_open();
    
    return is_open;
  }
  
  bool SerialCommunication::isValid()
  {
    bool is_open, good;
    
    is_open = this->commPort.is_open();
    
    good = this->commPort.good();
    
    return is_open && good;
  }
  
  bool SerialCommunication::haveData()
  {
    if (!this->isValid())
      return false;
    
    char c = this->commPort.peek();
    
    bool eof = this->commPort.eof();
    
    return !eof;
  }
  
  bool SerialCommunication::sendPacket(Packet& packet)
  {
    char c = 0;
    if (!this->isOpen())
      return false;
    
    Packet::ValueType data = packet.getRawPacket();
    
    this->commPort.write((const char*)data->data(), data->size());
    
    return this->commPort.good();
  }
  
  bool SerialCommunication::receivePacket(Packet& packet)
  {
    char c = 0;
    if (!this->haveData())
    {
      std::cout << "Don't have data" << std::endl;
      return false;
    }
    
    Packet::ValueType data = packet.getRawPacket();
    
    data->resize(sizeof(PacketHeader));
    
    while ((c = this->commPort.peek()) != EGGBEATER_SOF_BYTE)
    {
      if (c == -1)
        return false;
      this->commPort.ignore(1);
    }
    
    PacketHeader* header = (PacketHeader*)data->data();
    
    this->commPort.read((char*)data->data(), sizeof(PacketHeader) - 1);
    
    if (!this->commPort.good())
    {
      std::cout << "COM port failure: could not read header" << std::endl;
      return false;
    }
    
    data->resize(packet.getDataLength() + sizeof(PacketHeader));
    
    // Make sure to get the new header location
    header = (PacketHeader*)data->data();
    
    this->commPort.read((char*)&(header->data), packet.getDataLength() + 1);
    
    // EOF isn't a real error
    return (this->commPort.good() || this->commPort.eof());
  }
}