#include <stdint.h>
#include <vector>
#include <memory>
#include <bitset>
#include <string.h>
#include <iomanip>

#include <eggbeater/Common.h>
#include <eggbeater/Serial.h>

#define COMMAND_VALID(cmd)  ( ( ( cmd ) == CommandType::NoOp ) \
                           || ( ( cmd ) == CommandType::Echo ) \
                           || ( ( cmd ) == CommandType::OpenSession ) \
                           || ( ( cmd ) == CommandType::NewSession ) \
                           || ( ( cmd ) == CommandType::RefreshSession ) \
                           || ( ( cmd ) == CommandType::CloseSession ) \
                           || ( ( cmd ) == CommandType::GetFileKey ) \
                            )

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

  Packet::~Packet()
  {
  }
  
  void Packet::createPacket(CommandType cmd, const ByteArray& inputData)
  {
    data->resize(sizeof(PacketHeader) + inputData.size());
    
    PacketHeader* header = (PacketHeader*)data->data();
    header->magicNum = EGGBEATER_SOF_BYTE;
    
    if (COMMAND_VALID(cmd))
      header->cmd = cmd;
    else
      header->cmd = CommandType::Bad;
    
    if (inputData.size() < (1 << 16))
    {
      header->lenLow = inputData.size() & 0xff;
      header->lenHigh = (inputData.size() >> 8) & 0xff;
      
      memcpy(&(header->data), inputData.data(), inputData.size());
      
      data->back() = EGGBEATER_EOF_BYTE;
    }
  }
  
  void Packet::createPacket(const ByteArray& inputPacket)
  {
    data->resize(inputPacket.size());
    
    memcpy(data->data(), inputPacket.data(), inputPacket.size());
    
    data->back() = EGGBEATER_EOF_BYTE;
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

  PacketError Packet::isValid() const
  {
    PacketHeader header = getPacketHeader();
    
    if (header.magicNum != EGGBEATER_SOF_BYTE)
    {
      //std::cout << "Bad magic number" << std::endl;
      return PacketError::BadSOF;
    }
    
    if (!COMMAND_VALID(header.cmd))
    {
      //std::cout << "Bad command" << std::endl;
      return PacketError::BadCommand;
    }
    
    if (data->size() != (sizeof(PacketHeader) + ((header.lenHigh << 8) | (header.lenLow))))
    {
      //std::cout << "Bad packet size" << std::endl;
      //std::cout << "data->size(): " << data->size() << std::endl;
      //std::cout << "calc  size  : " << (sizeof(PacketHeader) + ((header.lenHigh << 8) | (header.lenLow))) << std::endl;
      return PacketError::BadSize;
    }
    
    if (data->back() != EGGBEATER_EOF_BYTE)
    {
      //std::cout << "Bad EOF number" << std::endl;
      //std::cout << "Found unexpected byte: " << std::hex << std::setw(2) << std::setfill('0') << (((int)data->back()) & 0xff) << std::endl;
      return PacketError::BadEOF;
    }
    
    return PacketError::Success;
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
    const PacketHeader* header;

    header = (const PacketHeader* const)data->data();
    
    const uint8_t* packetData = &(header->data);
  
    return packetData;
  }
}
