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

TEST_F(NoOpFixture, discover_device_absent)
{
  uint16_t vid = 0x0483;
  uint16_t pid = 0x5740;
  
  StringList devs;
  
  EXPECT_NO_THROW(
  try
  {
    devs = discover_devices(vid, pid);
  }
  catch (Exception e)
  {
    std::cout << e.msg() << std::endl;
    throw e;
  });
  
  EXPECT_EQ(devs.size(), 0);
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  
  return RUN_ALL_TESTS();
}
