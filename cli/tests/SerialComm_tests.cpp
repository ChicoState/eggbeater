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

TEST_F(NoOpFixture, SerialComm_open_file)
{
  EXPECT_NO_THROW(
    SerialCommunication serial;
    
    serial.open("SerialComm_test_file1.txt");
    
    ASSERT_TRUE(serial.isOpen());
    
    EXPECT_TRUE(serial.isValid());
  );
}

TEST_F(NoOpFixture, SerialComm_write_file)
{
  String fileName = "SerialComm_test_file1.txt";
  EXPECT_NO_THROW(
    SerialCommunication serial;
    ByteArray data({0x55, 0x03, 0x00, 0x04, 0xff, 0x8f, 0x6f, 0x11, 0xff});
    Packet packet(data);
    
    ASSERT_TRUE(serial.open(fileName));
    
    EXPECT_TRUE(serial.isOpen());
    
    EXPECT_TRUE(serial.isValid());
    
    ASSERT_TRUE(serial.sendPacket(packet));
    
    serial.close();
    
    ASSERT_TRUE(serial.open(fileName));
    
    EXPECT_TRUE(serial.isOpen());
    
    EXPECT_TRUE(serial.isValid());
    
    ASSERT_TRUE(serial.receivePacket(packet));
    
    EXPECT_EQ(*(packet.getRawPacket()), data);
  );
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  
  return RUN_ALL_TESTS();
}
