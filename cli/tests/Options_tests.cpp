#include "gtest/gtest.h"
#include <memory>

#include <eggbeater/Options.h>

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

TEST_F(NoOpFixture, Options_empty)
{
  EXPECT_NO_THROW(
    Options opt;
    
    ASSERT_TRUE(opt.parseOptions(0, NULL));
  );
}

TEST_F(NoOpFixture, Options_start_session)
{
  try
  {
    Options opt;
    int argc = 2;
    const char* argv[] = {
      "eggbeater",
      "--start-session"
    };
    
    ASSERT_TRUE(opt.parseOptions(argc, argv));
    
    //EXPECT_FALSE(opt.haveErrors());
    EXPECT_EQ(opt.getAction(), CLI_Action::StartSession);
  }
  catch (...)
  {
    ASSERT_EQ("Exception thrown", nullptr);
  }
}

TEST_F(NoOpFixture, Options_refresh_session_no_session_id)
{
  try
  {
    Options opt;
    int argc = 2;
    const char* argv[] = {
      "eggbeater",
      "--refresh-session"
    };
    
    EXPECT_TRUE(opt.parseOptions(argc, argv));
    
    //EXPECT_TRUE(opt.haveErrors());
    
    EXPECT_EQ(opt.getAction(), CLI_Action::RefreshSession);
    
    EXPECT_FALSE(opt.hasSessionID());
    
    EXPECT_FALSE(opt.hasCipherMode());
  }
  catch (...)
  {
    ASSERT_EQ("Exception thrown", nullptr);
  }
}

TEST_F(NoOpFixture, Options_refresh_session_with_session_id)
{
  try
  {
    Options opt;
    int argc = 4;
    const char* argv[] = {
      "eggbeater",
      "--refresh-session",
      "--session-id",
      "4"
    };
    
    EXPECT_TRUE(opt.parseOptions(argc, argv));
    
    //EXPECT_FALSE(opt.haveErrors());
    EXPECT_EQ(opt.getAction(), CLI_Action::RefreshSession);
    EXPECT_TRUE(opt.hasSessionID());
    EXPECT_EQ(opt.getSessionID(), "4");
    EXPECT_FALSE(opt.hasCipherMode());
  }
  catch (...)
  {
    ASSERT_EQ("Exception thrown", nullptr);
  }
}

TEST_F(NoOpFixture, Options_close_session)
{
  try
  {
    Options opt;
    int argc = 4;
    const char* argv[] = {
      "eggbeater",
      "--close-session",
      "--session-id",
      "4"
    };
    
    EXPECT_TRUE(opt.parseOptions(argc, argv));
    
    //EXPECT_FALSE(opt.haveErrors());
    EXPECT_EQ(opt.getAction(), CLI_Action::CloseSession);
    EXPECT_TRUE(opt.hasSessionID());
    EXPECT_EQ(opt.getSessionID(), "4");
    EXPECT_FALSE(opt.hasCipherMode());
  }
  catch (...)
  {
    ASSERT_EQ("Exception thrown", nullptr);
  }
}

TEST_F(NoOpFixture, Options_encrypt_no_cipher_mode)
{
  try
  {
    Options opt;
    int argc = 4;
    const char* argv[] = {
      "eggbeater",
      "--encrypt",
      "--session-id",
      "4"
    };
    
    EXPECT_TRUE(opt.parseOptions(argc, argv));
    
    //EXPECT_FALSE(opt.haveErrors());
    EXPECT_EQ(opt.getAction(), CLI_Action::Encrypt);
    EXPECT_TRUE(opt.hasSessionID());
    EXPECT_EQ(opt.getSessionID(), "4");
    EXPECT_FALSE(opt.hasCipherMode());
  }
  catch (...)
  {
    ASSERT_EQ("Exception thrown", nullptr);
  }
}

TEST_F(NoOpFixture, Options_encrypt_no_cipher_mode_no_session_id)
{
  try
  {
    Options opt;
    int argc = 2;
    const char* argv[] = {
      "eggbeater",
      "--encrypt"
    };
    
    EXPECT_TRUE(opt.parseOptions(argc, argv));
    
    //EXPECT_FALSE(opt.haveErrors());
    EXPECT_EQ(opt.getAction(), CLI_Action::Encrypt);
    EXPECT_FALSE(opt.hasSessionID());
    //EXPECT_EQ(opt.getSessionID(), "4");
    EXPECT_FALSE(opt.hasCipherMode());
  }
  catch (...)
  {
    ASSERT_TRUE(false);
  }
}

TEST_F(NoOpFixture, Options_encrypt_bad_cipher_mode)
{
  try
  {
    Options opt;
    int argc = 6;
    const char* argv[] = {
      "eggbeater",
      "--encrypt",
      "--session-id",
      "4",
      "--cipher-mode",
      "abc"
    };
    
    EXPECT_FALSE(opt.parseOptions(argc, argv));
    
    //EXPECT_FALSE(opt.haveErrors());
    EXPECT_EQ(opt.getAction(), CLI_Action::Encrypt);
    EXPECT_TRUE(opt.hasSessionID());
    EXPECT_EQ(opt.getSessionID(), "4");
    EXPECT_FALSE(opt.hasCipherMode());
  }
  catch (...)
  {
    ASSERT_TRUE(false);
  }
}

TEST_F(NoOpFixture, Options_encrypt_cipher_mode_cfb)
{
  try
  {
    Options opt;
    int argc = 6;
    const char* argv[] = {
      "eggbeater",
      "--encrypt",
      "--session-id",
      "4",
      "--cipher-mode",
      "cfb"
    };
    
    EXPECT_TRUE(opt.parseOptions(argc, argv));
    
    //EXPECT_FALSE(opt.haveErrors());
    EXPECT_EQ(opt.getAction(), CLI_Action::Encrypt);
    EXPECT_TRUE(opt.hasSessionID());
    EXPECT_EQ(opt.getSessionID(), "4");
    EXPECT_TRUE(opt.hasCipherMode());
    EXPECT_EQ(opt.getCipherMode(), CipherMode::CFB);
  }
  catch (...)
  {
    ASSERT_TRUE(false);
  }
}

TEST_F(NoOpFixture, Options_encrypt_cipher_mode_ofb)
{
  try
  {
    Options opt;
    int argc = 6;
    const char* argv[] = {
      "eggbeater",
      "--encrypt",
      "--session-id",
      "4",
      "--cipher-mode",
      "ofb"
    };
    
    EXPECT_TRUE(opt.parseOptions(argc, argv));
    
    //EXPECT_FALSE(opt.haveErrors());
    EXPECT_EQ(opt.getAction(), CLI_Action::Encrypt);
    EXPECT_TRUE(opt.hasSessionID());
    EXPECT_EQ(opt.getSessionID(), "4");
    EXPECT_TRUE(opt.hasCipherMode());
    EXPECT_EQ(opt.getCipherMode(), CipherMode::OFB);
  }
  catch (...)
  {
    ASSERT_TRUE(false);
  }
}

TEST_F(NoOpFixture, Options_encrypt_cipher_mode_gcm)
{
  try
  {
    Options opt;
    int argc = 6;
    const char* argv[] = {
      "eggbeater",
      "--encrypt",
      "--session-id",
      "4",
      "--cipher-mode",
      "gcm"
    };
    
    EXPECT_TRUE(opt.parseOptions(argc, argv));
    
    //EXPECT_FALSE(opt.haveErrors());
    EXPECT_EQ(opt.getAction(), CLI_Action::Encrypt);
    EXPECT_TRUE(opt.hasSessionID());
    EXPECT_EQ(opt.getSessionID(), "4");
    EXPECT_TRUE(opt.hasCipherMode());
    EXPECT_EQ(opt.getCipherMode(), CipherMode::GCM);
  }
  catch (...)
  {
    ASSERT_TRUE(false);
  }
}

TEST_F(NoOpFixture, Options_encrypt_3_files)
{
  try
  {
    Options opt;
    int argc = 9;
    const char* argv[] = {
      "eggbeater",
      "--encrypt",
      "--session-id",
      "4",
      "--cipher-mode",
      "gcm",
      "A",
      "g",
      "e"
    };
    StringList str({"A", "g", "e"});
    
    EXPECT_TRUE(opt.parseOptions(argc, argv));
    
    //EXPECT_FALSE(opt.haveErrors());
    EXPECT_EQ(opt.getAction(), CLI_Action::Encrypt);
    EXPECT_TRUE(opt.hasSessionID());
    EXPECT_EQ(opt.getSessionID(), "4");
    EXPECT_TRUE(opt.hasCipherMode());
    EXPECT_EQ(opt.getCipherMode(), CipherMode::GCM);
    
    EXPECT_EQ(opt.getFileList(), str);
  }
  catch (...)
  {
    ASSERT_TRUE(false);
  }
}

TEST_F(NoOpFixture, Options_decrypt_bad_cipher_mode)
{
  try
  {
    Options opt;
    int argc = 6;
    const char* argv[] = {
      "eggbeater",
      "--decrypt",
      "--session-id",
      "4",
      "--cipher-mode",
      "abc"
    };
    
    EXPECT_FALSE(opt.parseOptions(argc, argv));
    
    //EXPECT_FALSE(opt.haveErrors());
    EXPECT_EQ(opt.getAction(), CLI_Action::Decrypt);
    EXPECT_TRUE(opt.hasSessionID());
    EXPECT_EQ(opt.getSessionID(), "4");
    EXPECT_FALSE(opt.hasCipherMode());
  }
  catch (...)
  {
    ASSERT_TRUE(false);
  }
}

TEST_F(NoOpFixture, Options_decrypt_cipher_mode_cfb)
{
  try
  {
    Options opt;
    int argc = 6;
    const char* argv[] = {
      "eggbeater",
      "--decrypt",
      "--session-id",
      "4",
      "--cipher-mode",
      "cfb"
    };
    
    EXPECT_TRUE(opt.parseOptions(argc, argv));
    
    //EXPECT_FALSE(opt.haveErrors());
    EXPECT_EQ(opt.getAction(), CLI_Action::Decrypt);
    EXPECT_TRUE(opt.hasSessionID());
    EXPECT_EQ(opt.getSessionID(), "4");
    EXPECT_TRUE(opt.hasCipherMode());
    EXPECT_EQ(opt.getCipherMode(), CipherMode::CFB);
  }
  catch (...)
  {
    ASSERT_TRUE(false);
  }
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  
  return RUN_ALL_TESTS();
}
