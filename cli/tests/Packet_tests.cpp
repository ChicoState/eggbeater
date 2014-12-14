#include "gtest/gtest.h"
#include <memory>

#include <eggbeater/Serial.h>

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

TEST_F(NoOpFixture, Packet_create_empty)
{
  EXPECT_NO_THROW(
    std::shared_ptr<Packet> packet(new Packet());
  
    EXPECT_EQ(packet->getCommandType(), CommandType::Bad);
    
    EXPECT_EQ(packet->getDataLength(), 0);
    
    EXPECT_NE(packet->isValid(), PacketError::None);
    //EXPECT_FALSE(packet->isValid());
  );
}

TEST_F(NoOpFixture, Packet_create_nop)
{
  CommandType cmd = CommandType::NoOp;
  ByteArray   data({0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F});
  const uint8_t* packetData = NULL;
  EXPECT_NO_THROW(
    std::shared_ptr<Packet> packet(new Packet(cmd, data));
  
    EXPECT_EQ(packet->getCommandType(), cmd);
    
    EXPECT_EQ(packet->getDataLength(), data.size());
    
    packetData = packet->getPacketData();
    
    ASSERT_NE(packetData, nullptr);
    
    for (auto i = 0u; i < data.size(); i++)
      EXPECT_EQ(packetData[i], data[i]);
    
    EXPECT_EQ(packet->isValid(), PacketError::None);
    //EXPECT_TRUE(packet->isValid());
  );
}

TEST_F(NoOpFixture, Packet_create_echo)
{
  CommandType cmd = CommandType::Echo;
  ByteArray   data({0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F});
  const uint8_t* packetData = NULL;
  EXPECT_NO_THROW(
    std::shared_ptr<Packet> packet(new Packet(cmd, data));
  
    EXPECT_EQ(packet->getCommandType(), cmd);
    
    EXPECT_EQ(packet->getDataLength(), data.size());
    
    packetData = packet->getPacketData();
    
    ASSERT_NE(packetData, nullptr);
    
    for (auto i = 0u; i < data.size(); i++)
      EXPECT_EQ(packetData[i], data[i]);
    
    EXPECT_EQ(packet->isValid(), PacketError::None);
    //EXPECT_TRUE(packet->isValid());
  );
}

TEST_F(NoOpFixture, Packet_create_buffer)
{
  CommandType cmd = CommandType::Echo;
  ByteArray buffer({0x55, (uint8_t)cmd, 0x00, 0x04, 0xff, 0x8f, 0x6f, 0x11, 0xff});
  ByteArray data({0xff, 0x8f, 0x6f, 0x11});
  const uint8_t* packetData = NULL;
  
  EXPECT_NO_THROW(
    std::shared_ptr<Packet> packet(new Packet(buffer));
    
    EXPECT_EQ(packet->isValid(), PacketError::None);
    //EXPECT_TRUE(packet->isValid());
  
    EXPECT_EQ(packet->getCommandType(), cmd);
    
    EXPECT_EQ(packet->getDataLength(), data.size());
    
    packetData = packet->getPacketData();
    
    ASSERT_NE(packetData, nullptr);
    
    for (auto i = 0u; i < data.size(); i++)
      EXPECT_EQ(packetData[i], data[i]);
  );
}

TEST_F(NoOpFixture, Packet_create_buffer_bad1)
{
  CommandType cmd = CommandType::Echo;
  ByteArray buffer({0x5a, (uint8_t)cmd, 0x00, 0x04, 0xff, 0x8f, 0x6f, 0x11, 0xff, 0x8f, 0x6f, 0x11, 0xff, 0x8f, 0x6f, 0x11, 0xff});
  ByteArray data({0xff, 0x8f, 0x6f, 0x11, 0xff, 0x8f, 0x6f, 0x11, 0xff, 0x8f, 0x6f, 0x11});
  const uint8_t* packetData = NULL;
  
  EXPECT_NO_THROW(
    std::shared_ptr<Packet> packet(new Packet(buffer));
    
    EXPECT_EQ(packet->isValid(), PacketError::BadSOF);
    //EXPECT_FALSE(packet->isValid());
  );
}

TEST_F(NoOpFixture, Packet_create_buffer_bad2)
{
  CommandType cmd = CommandType::Echo;
  ByteArray buffer({0x55, 0x07, 0x00, 0x04, 0xff, 0x8f, 0x6f, 0x11, 0xff, 0x8f, 0x6f, 0x11, 0xff, 0x8f, 0x6f, 0x11, 0xff});
  ByteArray data({0xff, 0x8f, 0x6f, 0x11, 0xff, 0x8f, 0x6f, 0x11, 0xff, 0x8f, 0x6f, 0x11});
  const uint8_t* packetData = NULL;
  
  EXPECT_NO_THROW(
    std::shared_ptr<Packet> packet(new Packet(buffer));
    
    EXPECT_EQ(packet->isValid(), PacketError::BadCommand);
    //EXPECT_FALSE(packet->isValid());
  );
}

TEST_F(NoOpFixture, Packet_create_buffer_bad3)
{
  CommandType cmd = CommandType::Echo;
  ByteArray buffer({0x55, (uint8_t)cmd, 0x80, 0x04, 0xff, 0x8f, 0x6f, 0x11, 0xff, 0x8f, 0x6f, 0x11, 0xff, 0x8f, 0x6f, 0x11, 0xff});
  ByteArray data({0xff, 0x8f, 0x6f, 0x11, 0xff, 0x8f, 0x6f, 0x11, 0xff, 0x8f, 0x6f, 0x11});
  const uint8_t* packetData = NULL;
  
  EXPECT_NO_THROW(
    std::shared_ptr<Packet> packet(new Packet(buffer));
    
    EXPECT_EQ(packet->isValid(), PacketError::BadSize);
    //EXPECT_FALSE(packet->isValid());
  );
}

TEST_F(NoOpFixture, Packet_create_buffer_bad4)
{
  CommandType cmd = CommandType::Echo;
  ByteArray buffer({0x55, (uint8_t)cmd, 0x80, 0x04, 0xff, 0x8f, 0x6f, 0x11, 0x0f});
  ByteArray data({0xff, 0x8f, 0x6f, 0x11});
  const uint8_t* packetData = NULL;
  
  EXPECT_NO_THROW(
    std::shared_ptr<Packet> packet(new Packet(buffer));
    
    EXPECT_EQ(packet->isValid(), PacketError::BadEOF);
    //EXPECT_FALSE(packet->isValid());
  );
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  
  return RUN_ALL_TESTS();
}
