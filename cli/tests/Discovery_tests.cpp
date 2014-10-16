#include "gtest/gtest.h"

#include <eggbeater/Discovery.h>

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)

#include <Windows.h>

namespace EggBeater
{
  StringList split_multi_sz(const char* const, uint32_t);
  StringList read_multi_sz(HKEY, const char*, const char*);
  bool check_hardware_id(const String&, uint16_t, uint16_t);
}

#else
  // Linux/Mac OS X stuff
#endif

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

TEST_F(NoOpFixture, split_multi_sz_test)
{
  char data[] = "STRING1\0STRING2\0STRING3\0";
  uint32_t len = sizeof(data);
  StringList expected({"STRING1", "STRING2", "STRING3"});
  StringList actual;
  
  EXPECT_NO_THROW(actual = split_multi_sz(data, len));
  
  EXPECT_EQ(expected, actual);
}

TEST_F(NoOpFixture, check_hardware_id_test)
{
  std::string data = "USB\\VID_0483&PID_5740";
  uint16_t vid = 0x0483;
  uint16_t pid = 0x5740;
  
  EXPECT_TRUE(check_hardware_id(data, vid, pid));
  EXPECT_FALSE(check_hardware_id(data, 0, 0));
  EXPECT_FALSE(check_hardware_id("", 0, 0));
  EXPECT_FALSE(check_hardware_id("", vid, pid));
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  
  return RUN_ALL_TESTS();
}
