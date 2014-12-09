#include "gtest/gtest.h"

#include <EggBeater/Crypto.h>
#include <hex.h>

using namespace EggBeater;
using namespace CryptoPP;

class CryptoFixture : public ::testing::Test, public EggBeater::Crypto
{
public:
  CryptoFixture() : EggBeater::Crypto()
  {
  }

  virtual void SetUp()
  {
    // No-op
  }

  virtual void TearDown()
  {
    // No-op
  }
  static String toHex(const String& in)
  {
    String out;

    StringSource(in, true, new HexEncoder(new StringSink(out)));

    return out;
  }

  static String fromHex(const String& in)
  {
    String out;

    StringSource(in, true, new HexDecoder(new StringSink(out)));

    return out;
  }
};

/*
  Test vectors from: http://www.inconteam.com/software-development/41-encryption/55-aes-test-vectors#aes-cfb-256

  AES CFB128 256-bit encryption mode

  Encryption key: 603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4

  Initialization vector              Test vector                       Cipher text
  000102030405060708090A0B0C0D0E0F  6bc1bee22e409f96e93d7e117393172a  DC7E84BFDA79164B7ECD8486985D3860
  DC7E84BFDA79164B7ECD8486985D3860  ae2d8a571e03ac9c9eb76fac45af8e51  39ffed143b28b1c832113c6331e5407b
  39FFED143B28B1C832113C6331E5407B  30c81c46a35ce411e5fbc1191a0a52ef  df10132415e54b92a13ed0a8267ae2f9
  DF10132415E54B92A13ED0A8267AE2F9  f69f2445df4f9b17ad2b417be66c3710  75a385741ab9cef82031623d55b1e471
*/

TEST_F(CryptoFixture, encrypt_cfb_1)
{
  ByteArray iv      ({0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                      0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F});
  ByteArray key     ({0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
                      0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                      0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
                      0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4});
  String    plain   ("6bc1bee22e409f96e93d7e117393172a");
  String    cipher  ("DC7E84BFDA79164B7ECD8486985D3860");

  this->setCipherMode(CipherMode::CFB);
  this->setEncryptionKey(key);
  this->setInitialVector(iv);

  String plainBytes, cipherBytes, actualBytes;

  plainBytes = fromHex(plain);

  StringSource plainBytesSource(plainBytes, false);

  this->encrypt(&plainBytesSource, new StringSink(actualBytes));

  cipherBytes = fromHex(cipher);

  EXPECT_EQ(cipherBytes, actualBytes);
}

TEST_F(CryptoFixture, encrypt_cfb_2)
{
  ByteArray iv      ({0xDC, 0x7E, 0x84, 0xBF, 0xDA, 0x79, 0x16, 0x4B,
                      0x7E, 0xCD, 0x84, 0x86, 0x98, 0x5D, 0x38, 0x60});
  ByteArray key     ({0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
                      0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                      0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
                      0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4});
  String    plain   ("ae2d8a571e03ac9c9eb76fac45af8e51");
  String    cipher  ("39ffed143b28b1c832113c6331e5407b");

  this->setCipherMode(CipherMode::CFB);
  this->setEncryptionKey(key);
  this->setInitialVector(iv);

  String plainBytes, cipherBytes, actualBytes;

  plainBytes = fromHex(plain);

  StringSource plainBytesSource(plainBytes, false);

  this->encrypt(&plainBytesSource, new StringSink(actualBytes));

  cipherBytes = fromHex(cipher);

  EXPECT_EQ(cipherBytes, actualBytes);
}

TEST_F(CryptoFixture, encrypt_cfb_3)
{
  ByteArray iv      ({0x39, 0xFF, 0xED, 0x14, 0x3B, 0x28, 0xB1, 0xC8,
                      0x32, 0x11, 0x3C, 0x63, 0x31, 0xE5, 0x40, 0x7B});
  ByteArray key     ({0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
                      0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                      0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
                      0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4});
  String    plain   ("30c81c46a35ce411e5fbc1191a0a52ef");
  String    cipher  ("df10132415e54b92a13ed0a8267ae2f9");

  this->setCipherMode(CipherMode::CFB);
  this->setEncryptionKey(key);
  this->setInitialVector(iv);

  String plainBytes, cipherBytes, actualBytes;

  plainBytes = fromHex(plain);

  StringSource plainBytesSource(plainBytes, false);

  this->encrypt(&plainBytesSource, new StringSink(actualBytes));

  cipherBytes = fromHex(cipher);

  EXPECT_EQ(cipherBytes, actualBytes);
}

TEST_F(CryptoFixture, encrypt_cfb_4)
{
  ByteArray iv      ({0xDF, 0x10, 0x13, 0x24, 0x15, 0xE5, 0x4B, 0x92,
                      0xA1, 0x3E, 0xD0, 0xA8, 0x26, 0x7A, 0xE2, 0xF9});
  ByteArray key     ({0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
                      0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                      0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
                      0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4});
  String    plain   ("f69f2445df4f9b17ad2b417be66c3710");
  String    cipher  ("75a385741ab9cef82031623d55b1e471");

  this->setCipherMode(CipherMode::CFB);
  this->setEncryptionKey(key);
  this->setInitialVector(iv);

  String plainBytes, cipherBytes, actualBytes;

  plainBytes = fromHex(plain);

  StringSource plainBytesSource(plainBytes, false);

  this->encrypt(&plainBytesSource, new StringSink(actualBytes));

  cipherBytes = fromHex(cipher);

  EXPECT_EQ(cipherBytes, actualBytes);
}

/*
  Test vectors from: http://www.inconteam.com/software-development/41-encryption/55-aes-test-vectors#aes-cfb-256

  AES OFB 256-bit encryption mode

  Encryption key: 603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4

  Initialization vector						  Test vector						  	        Cipher text
  000102030405060708090A0B0C0D0E0F	6bc1bee22e409f96e93d7e117393172a	dc7e84bfda79164b7ecd8486985d3860
  B7BF3A5DF43989DD97F0FA97EBCE2F4A	ae2d8a571e03ac9c9eb76fac45af8e51	4febdc6740d20b3ac88f6ad82a4fb08d
  E1C656305ED1A7A6563805746FE03EDC	30c81c46a35ce411e5fbc1191a0a52ef	71ab47a086e86eedf39d1c5bba97c408
  41635BE625B48AFC1666DD42A09D96E7	f69f2445df4f9b17ad2b417be66c3710	0126141d67f37be8538f5a8be740e484
*/

TEST_F(CryptoFixture, encrypt_ofb_1)
{
  ByteArray iv      ({0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                      0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F});
  ByteArray key     ({0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
                      0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                      0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
                      0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4});
  String    plain   ("6bc1bee22e409f96e93d7e117393172a");
  String    cipher  ("dc7e84bfda79164b7ecd8486985d3860");

  this->setCipherMode(CipherMode::CFB);
  this->setEncryptionKey(key);
  this->setInitialVector(iv);

  String plainBytes, cipherBytes, actualBytes;

  plainBytes = fromHex(plain);

  StringSource plainBytesSource(plainBytes, false);

  this->encrypt(&plainBytesSource, new StringSink(actualBytes));

  cipherBytes = fromHex(cipher);

  EXPECT_EQ(cipherBytes, actualBytes);
}

TEST_F(CryptoFixture, encrypt_ofb_2)
{
  ByteArray iv      ({0xB7, 0xBF, 0x3A, 0x5D, 0xF4, 0x39, 0x89, 0xDD,
                      0x97, 0xF0, 0xFA, 0x97, 0xEB, 0xCE, 0x2F, 0x4A});
  ByteArray key     ({0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
                      0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                      0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
                      0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4});
  String    plain   ("ae2d8a571e03ac9c9eb76fac45af8e51");
  String    cipher  ("4febdc6740d20b3ac88f6ad82a4fb08d");

  this->setCipherMode(CipherMode::CFB);
  this->setEncryptionKey(key);
  this->setInitialVector(iv);

  String plainBytes, cipherBytes, actualBytes;

  plainBytes = fromHex(plain);

  StringSource plainBytesSource(plainBytes, false);

  this->encrypt(&plainBytesSource, new StringSink(actualBytes));

  cipherBytes = fromHex(cipher);

  EXPECT_EQ(cipherBytes, actualBytes);
}

TEST_F(CryptoFixture, encrypt_ofb_3)
{
  ByteArray iv      ({0xE1, 0xC6, 0x56, 0x30, 0x5E, 0xD1, 0xA7, 0xA6,
                      0x56, 0x38, 0x05, 0x74, 0x6F, 0xE0, 0x3E, 0xDC});
  ByteArray key     ({0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
                      0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                      0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
                      0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4});
  String    plain   ("30c81c46a35ce411e5fbc1191a0a52ef");
  String    cipher  ("71ab47a086e86eedf39d1c5bba97c408");

  this->setCipherMode(CipherMode::CFB);
  this->setEncryptionKey(key);
  this->setInitialVector(iv);

  String plainBytes, cipherBytes, actualBytes;

  plainBytes = fromHex(plain);

  StringSource plainBytesSource(plainBytes, false);

  this->encrypt(&plainBytesSource, new StringSink(actualBytes));

  cipherBytes = fromHex(cipher);

  EXPECT_EQ(cipherBytes, actualBytes);
}

TEST_F(CryptoFixture, encrypt_ofb_4)
{
  ByteArray iv      ({0x41, 0x63, 0x5B, 0xE6, 0x25, 0xB4, 0x8A, 0xFC,
                      0x16, 0x66, 0xDD, 0x42, 0xA0, 0x9D, 0x96, 0xE7});
  ByteArray key     ({0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
                      0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                      0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
                      0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4});
  String    plain   ("f69f2445df4f9b17ad2b417be66c3710");
  String    cipher  ("0126141d67f37be8538f5a8be740e484");

  this->setCipherMode(CipherMode::CFB);
  this->setEncryptionKey(key);
  this->setInitialVector(iv);

  String plainBytes, cipherBytes, actualBytes;

  plainBytes = fromHex(plain);

  StringSource plainBytesSource(plainBytes, false);

  this->encrypt(&plainBytesSource, new StringSink(actualBytes));

  cipherBytes = fromHex(cipher);

  EXPECT_EQ(cipherBytes, actualBytes);
}

/*
  iv = 96, key = 256, plain = 128 -> taken from gcmEncryptExtIV256.rsp

  GCM 1
  Key = 31bdadd96698c204aa9ce1448ea94ae1fb4a9a0b3c9d773b51bb1822666b8f22
  IV = 0d18e06c7c725ac9e362e1ce
  PT = 2db5168e932556f8089a0622981d017d
  AAD =
  CT = fa4362189661d163fcd6a56d8bf0405a
  Tag = d636ac1bbedd5cc3ee727dc2ab4a9489

  GCM 2
  Key = 460fc864972261c2560e1eb88761ff1c992b982497bd2ac36c04071cbb8e5d99
  IV = 8a4a16b9e210eb68bcb6f58d
  PT = 99e4e926ffe927f691893fb79a96b067
  AAD =
  CT = 133fc15751621b5f325c7ff71ce08324
  Tag = ec4e87e0cf74a13618d0b68636ba9fa7

  GCM 3
  Key = f78a2ba3c5bd164de134a030ca09e99463ea7e967b92c4b0a0870796480297e5
  IV = 2bb92fcb726c278a2fa35a88
  PT = f562509ed139a6bbe7ab545ac616250c
  AAD =
  CT = e2f787996e37d3b47294bf7ebba5ee25
  Tag = 00f613eee9bdad6c9ee7765db1cb45c0

  GCM 4
  Key = 48e6af212da1386500454c94a201640c2151b28079240e40d72d2a5fd7d54234
  IV = ef0ff062220eb817dc2ece94
  PT = c7afeecec1408ad155b177c2dc7138b0
  AAD =
  CT = 9432a620e6a22307e06a321d66846fd4
  Tag = e3ea499192f2cd8d3ab3edfc55897415

  GCM 5
  Key: 00000000000000000000000000000000
  IV:  000000000000000000000000
  Plaintext: 00000000000000000000000000000000
  Ciphertext: 0388dace60b6a392f328c2b971b2fe78
  MAC: ab6e47d42cec13bdf53a67b21257bddf
*/

TEST_F(CryptoFixture, encrypt_gcm_1)
{
  ByteArray iv      ({0x0d, 0x18, 0xe0, 0x6c, 0x7c, 0x72, 0x5a, 0xc9,
                      0xe3, 0x62, 0xe1, 0xce});
  ByteArray key     ({0x31, 0xbd, 0xad, 0xd9, 0x66, 0x98, 0xc2, 0x04,
                      0xaa, 0x9c, 0xe1, 0x44, 0x8e, 0xa9, 0x4a, 0xe1,
                      0xfb, 0x4a, 0x9a, 0x0b, 0x3c, 0x9d, 0x77, 0x3b,
                      0x51, 0xbb, 0x18, 0x22, 0x66, 0x6b, 0x8f, 0x22});
  String    plain   ("2db5168e932556f8089a0622981d017d");
  String    cipher  ("fa4362189661d163fcd6a56d8bf0405a");
  String    tag     ("d636ac1bbedd5cc3ee727dc2ab4a9489");

  this->setCipherMode(CipherMode::GCM);
  this->setEncryptionKey(key);
  this->setInitialVector(iv);

  String plainBytes, cipherBytes, actualBytes;

  plainBytes = fromHex(plain);
  cipherBytes = fromHex(cipher + tag);

  StringSource plainBytesSource(plainBytes, false);

  this->encrypt(&plainBytesSource, new StringSink(actualBytes));

  EXPECT_EQ(cipherBytes, actualBytes);
}

TEST_F(CryptoFixture, encrypt_gcm_2)
{
  ByteArray iv      ({0x8a, 0x4a, 0x16, 0xb9, 0xe2, 0x10, 0xeb, 0x68,
                      0xbc, 0xb6, 0xf5, 0x8d});
  ByteArray key     ({0x46, 0x0f, 0xc8, 0x64, 0x97, 0x22, 0x61, 0xc2,
                      0x56, 0x0e, 0x1e, 0xb8, 0x87, 0x61, 0xff, 0x1c,
                      0x99, 0x2b, 0x98, 0x24, 0x97, 0xbd, 0x2a, 0xc3,
                      0x6c, 0x04, 0x07, 0x1c, 0xbb, 0x8e, 0x5d, 0x99});
  String    plain   ("99e4e926ffe927f691893fb79a96b067");
  String    cipher  ("133fc15751621b5f325c7ff71ce08324");
  String    tag     ("ec4e87e0cf74a13618d0b68636ba9fa7");

  this->setCipherMode(CipherMode::GCM);
  this->setEncryptionKey(key);
  this->setInitialVector(iv);

  String plainBytes, cipherBytes, actualBytes;

  plainBytes = fromHex(plain);
  cipherBytes = fromHex(cipher + tag);

  StringSource plainBytesSource(plainBytes, false);

  this->encrypt(&plainBytesSource, new StringSink(actualBytes));

  EXPECT_EQ(cipherBytes, actualBytes);
}

TEST_F(CryptoFixture, encrypt_gcm_3)
{
  ByteArray iv      ({0x2b, 0xb9, 0x2f, 0xcb, 0x72, 0x6c, 0x27, 0x8a,
                      0x2f, 0xa3, 0x5a, 0x88});
  ByteArray key     ({0xf7, 0x8a, 0x2b, 0xa3, 0xc5, 0xbd, 0x16, 0x4d,
                      0xe1, 0x34, 0xa0, 0x30, 0xca, 0x09, 0xe9, 0x94,
                      0x63, 0xea, 0x7e, 0x96, 0x7b, 0x92, 0xc4, 0xb0,
                      0xa0, 0x87, 0x07, 0x96, 0x48, 0x02, 0x97, 0xe5});
  String    plain   ("f562509ed139a6bbe7ab545ac616250c");
  String    cipher  ("e2f787996e37d3b47294bf7ebba5ee25");
  String    tag     ("00f613eee9bdad6c9ee7765db1cb45c0");

  this->setCipherMode(CipherMode::GCM);
  this->setEncryptionKey(key);
  this->setInitialVector(iv);

  String plainBytes, cipherBytes, actualBytes;

  plainBytes = fromHex(plain);
  cipherBytes = fromHex(cipher + tag);

  StringSource plainBytesSource(plainBytes, false);

  this->encrypt(&plainBytesSource, new StringSink(actualBytes));

  EXPECT_EQ(cipherBytes, actualBytes);
}

TEST_F(CryptoFixture, encrypt_gcm_4)
{
  ByteArray iv      ({0xef, 0x0f, 0xf0, 0x62, 0x22, 0x0e, 0xb8, 0x17,
                      0xdc, 0x2e, 0xce, 0x94});
  ByteArray key     ({0x48, 0xe6, 0xaf, 0x21, 0x2d, 0xa1, 0x38, 0x65,
                      0x00, 0x45, 0x4c, 0x94, 0xa2, 0x01, 0x64, 0x0c,
                      0x21, 0x51, 0xb2, 0x80, 0x79, 0x24, 0x0e, 0x40,
                      0xd7, 0x2d, 0x2a, 0x5f, 0xd7, 0xd5, 0x42, 0x34});
  String    plain   ("c7afeecec1408ad155b177c2dc7138b0");
  String    cipher  ("9432a620e6a22307e06a321d66846fd4");
  String    tag     ("e3ea499192f2cd8d3ab3edfc55897415");

  this->setCipherMode(CipherMode::GCM);
  this->setEncryptionKey(key);
  this->setInitialVector(iv);

  String plainBytes, cipherBytes, actualBytes;

  plainBytes = fromHex(plain);
  cipherBytes = fromHex(cipher + tag);

  StringSource plainBytesSource(plainBytes, false);

  this->encrypt(&plainBytesSource, new StringSink(actualBytes));

  EXPECT_EQ(cipherBytes, actualBytes);
}

TEST_F(CryptoFixture, encrypt_gcm_5)
{
  ByteArray iv      ({0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                      0x00, 0x00, 0x00, 0x00});
  ByteArray key     ({0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
  String    plain   ("00000000000000000000000000000000");
  String    cipher  ("0388dace60b6a392f328c2b971b2fe78ab6e47d42cec13bdf53a67b21257bddf");

  this->setCipherMode(CipherMode::GCM);
  this->setEncryptionKey(key);
  this->setInitialVector(iv);

  String plainBytes, cipherBytes, actualBytes;

  plainBytes = fromHex(plain);
  cipherBytes = fromHex(cipher);

  StringSource plainBytesSource(plainBytes, false);

  this->encrypt(&plainBytesSource, new StringSink(actualBytes));

  EXPECT_EQ(cipherBytes, actualBytes);
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
