#include "gtest/gtest.h"
#include <memory>

#include <eggbeater/Serial.h>
#include <eggbeater/Discovery.h>

#include <iostream>

using namespace EggBeater;

class NoOpFixture : public ::testing::Test
{
public:
  NoOpFixture()
  {
    // No-op
  }
  
  virtual ~NoOpFixture()
  {
    // No-op
  }
  
  virtual void SetUp()
  {
    // No-op
  }
  
  virtual void TearDown()
  {
    // No-op
  }
};

TEST_F(NoOpFixture, SerialCommPort_test_echo)
{
  uint16_t vid = 0x0483;
  uint16_t pid = 0x5740;
  SerialCommunication serial;
  ByteArray data({0xff, 0x8f, 0x6f, 0x11});
  
  ASSERT_NO_THROW(
    Packet sendPacket(CommandType::Echo, data);
    Packet recvPacket;
    StringList ports;
    
    try
    {
      StringList ports2(discover_devices(vid, pid));
      
      for (auto port : ports2)
        ports.push_back(port);
      
//      ports = discover_devices(vid, pid);
    }
    catch (Exception e)
    {
      std::cout << e.msg() << std::endl;
      throw e;
    }
    
    std::cout << "num ports: " << ports.size() << std::endl;
    
    for (auto port : ports)
    {
      std::cout << "serial.open(" << port << ")" << std::endl;
      
      std::flush(std::cout);
      
      ASSERT_TRUE(serial.open(port));
      
      std::cout << "send packet" << std::endl;
      
      ASSERT_TRUE(serial.sendPacket(sendPacket));
      
      std::cout << "receive packet" << std::endl;
      
      ASSERT_TRUE(serial.receivePacket(recvPacket));
      
      std::cout << "check command" << std::endl;
      
      ASSERT_EQ(sendPacket.getCommandType(), recvPacket.getCommandType());
      
      std::cout << "check data length" << std::endl;
      ASSERT_EQ(sendPacket.getDataLength(), recvPacket.getDataLength());
      
      std::cout << "check packet data" << std::endl;
      //const uint8_t* sendData = sendPacket.getPacketData();
      //const uint8_t* recvData = recvPacket.getPacketData();
      
      Packet::ValueType sendData = sendPacket.getRawPacket();
      Packet::ValueType recvData = recvPacket.getRawPacket();
      
      std::cout << "get pointers from shared_ptr" << std::endl;
      ByteArray* sendArray = sendData.get();
      ByteArray* recvArray = sendData.get();
      
      std::cout << "check for NULL pointers" << std::endl;
      ASSERT_NE(sendArray, nullptr);
      ASSERT_NE(recvArray, nullptr);
      
      std::cout << "get packet length" << std::endl;
      uint32_t len = sendData->size();
      std::cout << "send packet length: " << len << std::endl;
      std::cout << "recv packet length: " << recvData->size() << std::endl;
      
      for (uint32_t i = 0; i < len; i++)
      {
        printf("%0.2x %0.2x\n", (int)((*sendData)[i]), (int)((*recvData)[i]));
        EXPECT_EQ((*sendData)[i], (*recvData)[i]);
      }
      
      std::cout << "done" << std::endl;
      
      serial.close();
    }
  );
  
  std::cout << "outside ASSERT_NO_THROW" << std::endl;
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  
  return RUN_ALL_TESTS();
}
