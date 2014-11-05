#include <eggbeater/Serial.h>

#include <stdio.h>

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
    if (!this->isOpen())
      return false;
    
    Packet::ValueType data = packet.getRawPacket();
    
    this->commPort.write((const char*)data->data(), data->size());
    
    return this->commPort.good();
  }
  
  bool SerialCommunication::receivePacket(Packet& packet)
  {
    if (!this->haveData())
      return false;
    
    Packet::ValueType data = packet.getRawPacket();
    
    data->resize(sizeof(PacketHeader));
    
    PacketHeader* header = (PacketHeader*)data->data();
    
    this->commPort.read((char*)data->data(), sizeof(PacketHeader) - 1);
    
    if (!this->commPort.good())
      return false;
    
    data->resize(packet.getDataLength() + sizeof(PacketHeader));
    
    // Make sure to get the new header location
    header = (PacketHeader*)data->data();
    
    this->commPort.read((char*)&(header->data), packet.getDataLength() + 1);
    
    return this->commPort.good();
  }
}