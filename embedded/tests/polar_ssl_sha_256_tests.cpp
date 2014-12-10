#include "gtest/gtest.h"

#include <iostream>
#include <polarssl/sha256.h>

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

TEST_F(NoOpFixture, SHA256_self_test)
{
  ASSERT_EQ(sha256_self_test(1), 0);
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  
  return RUN_ALL_TESTS();
}
