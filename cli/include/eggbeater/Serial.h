#ifndef _EGGBEATER_SERIAL_H_
#define _EGGBEATER_SERIAL_H_

#include <stdint.h>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>

#include <eggbeater/Common.h>

namespace EggBeater
{
  //! @todo Add to as needed
  enum class CommandType : int8_t
  {
    Bad = -1,
    NoOp = 0,
    Echo = 1,
    
    OpenSession = 4,
    NewSession = 5,
    RefreshSession = 6,
    CloseSession = 7,
    
    GetFileKey = 9
  };
  
  struct PacketHeader
  {
    uint8_t     magicNum;
    CommandType cmd;
    uint8_t     lenHigh;
    uint8_t     lenLow;
    uint8_t     data;
  };
  
  class Packet
  {
  public:
    typedef std::shared_ptr<ByteArray> ValueType;
    //! Create an empty packet
    Packet();
    //! Create a packet from the specified command type and data array.
    Packet(CommandType, const ByteArray&);
    //! Create a packet from the specified byte stream
    explicit Packet(const ByteArray&);
    
    //! Create a packet from the specified command type and data array.
    void createPacket(CommandType, const ByteArray&);
    //! Create a packet from the specified byte stream
    void createPacket(const ByteArray&);
    
    ~Packet(); // As needed
    
    /**
      Get the packet as a series of bytes.
    **/
    ValueType       getRawPacket();
    /**
      Get the decoded packet header.
    **/
    PacketHeader    getPacketHeader() const;
    /**
      Validate the packet header.
    **/
    bool            isValid() const;
    /**
      Extract only the command type from the packet header
    **/
    CommandType     getCommandType() const;
    /**
      Extract only the data length from the packet header
    **/
    uint32_t        getDataLength() const;
    /**
      Retrieve a pointer to the data section of the packet.
      @note This data should be treated as immutable.
    **/
    const uint8_t*  getPacketData() const;
  private:
    ValueType data;
  };

  class SerialCommunication
  {
  public:
    //! Initialize without a starting port.
    SerialCommunication();
    //! Open the specified serial port.
    explicit SerialCommunication(const String&);
    explicit SerialCommunication(const char*);
    
    //! Ensure commPort is closed properly.
    ~SerialCommunication();
    
    //! Open the specified serial port.
    bool open(const String&);
    //! Close the serial port, if one is open.
    void close();
    //! Check if the serial port is open
    bool isOpen();
    //! Check if the serial port is in a valid state.
    bool isValid();
    //! Check if the serial port has data waiting.
    bool haveData();
    //! Send a packet on the serial port
    bool sendPacket(Packet&);
    //! Read a packet from the serial port
    bool receivePacket(Packet&);
  private:
    std::fstream commPort;
  };
}

#endif // _EGGBEATER_SERIAL_H_
