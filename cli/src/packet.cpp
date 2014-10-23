#include <stdint.h>
#include <vector>
#include <memory>
#include <bitset>
#include <string.h>

#include <eggbeater/Common.h>
#include <eggbeater/Serial.h>

#define EGGBEATER_SOF_BYTE  0x55
#define EGGBEATER_EOF_BYTE  0xff

namespace EggBeater
{
  //basic packet constructor
  Packet::Packet() : data(new ByteArray())
  {
    data->resize(sizeof(PacketHeader));

    PacketHeader* header = (PacketHeader*)data->data();
    header->magicNum = EGGBEATER_SOF_BYTE;
    header->cmd = CommandType::Bad;
    header->lenHigh = 0;
    header->lenLow = 0;
    header->data = EGGBEATER_EOF_BYTE;
  }

  Packet::Packet(CommandType cmd, const ByteArray& inputData) : data(new ByteArray())
  {
    createPacket(cmd, inputData);
  }
  
  Packet::Packet(const ByteArray& packet) : data(new ByteArray())
  {
    createPacket(packet);
  }

  void Packet::createPacket(CommandType cmd, const ByteArray& inputData)
  {
    data->resize(sizeof(PacketHeader) + 1 + inputData.size());
    
    PacketHeader* header = (PacketHeader*)data->data();
    header->magicNum = EGGBEATER_SOF_BYTE;
    
    if (cmd == CommandType::NoOp
     || cmd == CommandType::Read
     || cmd == CommandType::Echo
     || cmd == CommandType::StartSession
     || cmd == CommandType::RefreshSession
     || cmd == CommandType::CloseSession
     || cmd == CommandType::GenerateKey)
      header->cmd = cmd;
    else
      header->cmd = CommandType::Bad;
    
    if (inputData.size() < (1 << 16))
    {
      header->lenLow = inputData.size() & 0xff;
      header->lenHigh = (inputData.size() >> 8) & 0xff;
      
      memcpy(&(header->data), inputData.data(), inputData.size());
      
      (*data)[data->size()-1] = EGGBEATER_EOF_BYTE;
    }
  }
  
  void Packet::createPacket(const ByteArray& inputPacket)
  {
    data->resize(inputPacket.size() + 1);
    
    memcpy(data->data(), inputPacket.data(), inputPacket.size());
      
    (*data)[data->size()-1] = EGGBEATER_EOF_BYTE;
    /*
    data->resize(sizeof(PacketHeader) + 1 + inputData.size());
    
    PacketHeader* header = (PacketHeader*)data->data();
    header->magicNum = EGGBEATER_SOF_BYTE;
    
    if (cmd == CommandType::NoOp
     || cmd == CommandType::Read
     || cmd == CommandType::Write
     || cmd == CommandType::Echo)
      header->cmd = cmd;
    else
      header->cmd = CommandType::Bad;
    
    if (inputData.size() < (1 << 16))
    {
      header->lenLow = inputData.size() & 0xff;
      header->lenHigh = (inputData.size() >> 8) & 0xff;
      
      memcpy(&(header->data), inputData.data(), inputData.size());
    }
    */
  }

  Packet::ValueType Packet::getRawPacket()
  {
    return this->data;
  }

  PacketHeader Packet::getPacketHeader() const
  {
    const PacketHeader* const header = (const PacketHeader* const)data->data();
    
    return *header;
  }

  bool Packet::isValid() const
  {
    PacketHeader header = getPacketHeader();
    
    if (header.magicNum != EGGBEATER_SOF_BYTE)
      return false;
    
    if (header.cmd != CommandType::NoOp
     && header.cmd != CommandType::Read
     && header.cmd != CommandType::Echo
     && header.cmd != CommandType::StartSession
     && header.cmd != CommandType::RefreshSession
     && header.cmd != CommandType::CloseSession
     && header.cmd != CommandType::GenerateKey)
      return false;
    
    if (data->size() != (sizeof(PacketHeader) + ((header.lenHigh < 8) | (header.lenLow))))
      return false;
    
    if ((*data)[data->size() - 1] != EGGBEATER_EOF_BYTE)
      return false;
    
    return true;
  }

  CommandType Packet::getCommandType() const
  {
    PacketHeader header = getPacketHeader();
    
    return header.cmd;
  }

  uint32_t Packet::getDataLength() const
  {
    PacketHeader header = getPacketHeader();
    
    return (header.lenHigh << 8) | (header.lenLow);
  }

  const uint8_t* Packet::getPacketData() const
  {
    const PacketHeader* const header = (const PacketHeader* const)data->data();
    
    const uint8_t* packetData = &(header->data);
  
    return packetData;
  }
}
