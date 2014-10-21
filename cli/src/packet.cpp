#include <stdint.h>
#include <vector.h>
#include <memory>
#include <bitset>

#include "Common.h"
#include "Serial.h"

//basic packet constructor
Packet::Packet()
{
  PacketHeader packetHd;
  packetHd.magicNum = '10001000';
  packetHd.cmd = NULL;
  packetHd.lenHigh = NULL;
  packetHd.lenLow = NULL;
}

Packet::Packet(CommandType, const ByteArray& data)
{
  int i=0;
  int numHigh=0;
  int numLow=0;
  PacketHeader packetHd;
  packetHd.magicNum = '10001000';
  if(CommandType = 1)  //read: need to figure what read bits are and what actually cmd will be
    packetHd.cmd = '10101010';
  if(CommandType = 2) //write: need to figure what read bits are and what actually cmd will be
    packetHd.cmd = '01010101';
  if(CommandType = 0) //NoOp
    packetHd.cmd = '11001100';
  else
	throw std::runtime_error("CreatePacket: invalid packet command type");

  while(data[i]!=NULL)
  {
    if(data[i]=='0')
	  numLow++;
	if(data[i]=='1')
	  numHigh++;
	i++;
  }
  packetHd.lenHigh = bitset<;
  packetHd.lenLow = numLow; 
}

void Packet::CreatePacket(CommandType, const ByteArray& data)
{
  Packet(CommandType, data);
}

ValueType Packet::getRawPacket()
{
}

bool Packet::isValid()
{
  if(packet.magicNum=='10001000')
   return true;
  else
   return false;
}

