#include <eggbeater/Serial.h>

#include <stdio.h>

namespace EggBeater
{
  SerialCommunication::SerialCommunication() : commPort(NULL)
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
      this->close();
    
    this->commPort.open(portName.c_str(), std::ios::in | std::ios::out | std::ios::binary);
    
    return this->isOpen();
  }
  
  void SerialCommunication::close()
  {
    this->commPort.close();
  }
  
  bool SerialCommunication::isOpen()
  {
    return this->commPort.is_open();
  }
  
  bool SerialCommunication::isValid()
  {
    return this->commPort.is_open() && this->commPort.good();
  }
  
  bool SerialCommunication::haveData()
  {
    if (!this->isValid())
      return false;
    
    char c = this->commPort.peek();
    
    return !this->commPort.eof();
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
    
    this->commPort.read((char*)&(header->data), packet.getDataLength() + 1);
    
    return this->commPort.good();
  }
}