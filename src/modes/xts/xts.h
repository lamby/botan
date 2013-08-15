/*
* XTS mode, from IEEE P1619
* (C) 2009,2013 Jack Lloyd
*
* Distributed under the terms of the Botan license
*/

#ifndef BOTAN_MODE_XTS_H__
#define BOTAN_MODE_XTS_H__

#include <botan/transform.h>
#include <botan/block_cipher.h>
#include <memory>

namespace Botan {

/**
* IEEE P1619 XTS Mode
*/
class XTS_Mode : public Transformation
   {
   public:
      std::string name() const override;

      secure_vector<byte> start(const byte nonce[], size_t nonce_len) override;

      size_t update_granularity() const override;

      size_t minimum_final_size() const override;

      Key_Length_Specification key_spec() const override;

      size_t default_nonce_size() const override;

      bool valid_nonce_length(size_t n) const override;

      void clear();
   protected:
      XTS_Mode(BlockCipher* cipher);

      const byte* tweak() const { return &m_tweak[0]; }

      const BlockCipher& cipher() const { return *m_cipher; }

      void update_tweak(size_t last_used);

   private:
      void key_schedule(const byte key[], size_t length) override;

      std::unique_ptr<BlockCipher> m_cipher, m_tweak_cipher;
      secure_vector<byte> m_tweak;
   };

/**
* IEEE P1619 XTS Encryption
*/
class BOTAN_DLL XTS_Encryption : public XTS_Mode
   {
   public:
      XTS_Encryption(BlockCipher* cipher) : XTS_Mode(cipher) {}

      void update(secure_vector<byte>& blocks, size_t offset) override;

      void finish(secure_vector<byte>& final_block, size_t offset) override;

      size_t output_length(size_t input_length) const override;
   };

/**
* IEEE P1619 XTS Decryption
*/
class BOTAN_DLL XTS_Decryption : public XTS_Mode
   {
   public:
      XTS_Decryption(BlockCipher* cipher) : XTS_Mode(cipher) {}

      void update(secure_vector<byte>& blocks, size_t offset) override;

      void finish(secure_vector<byte>& final_block, size_t offset) override;

      size_t output_length(size_t input_length) const override;
   };

}

#endif