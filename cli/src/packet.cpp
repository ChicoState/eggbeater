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

Packet::Packet(CommandType cmd, const ByteArray& data)
{
  int i=0;
  int numHigh=0;
  int numLow=0;
  PacketHeader packetHd;
  packetHd.magicNum = '10001000';
  if(cmd = -1) //NoOp
    packetHd.cmd = '10000000';
  if(cmd = 0) //NoOp
    packetHd.cmd = '00000000';
  if(cmd = 1)  //read
    packetHd.cmd = '00000001';
  if(cmd = 2) //Authenticate
    packetHd.cmd = '00000010';
  if(cmd = 3) //thumbGood:
    packetHd.cmd = '00000011';
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

void Packet::CreatePacket(CommandType cmd, const ByteArray& data)
{
  Packet(CommandType cmd, data);
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

