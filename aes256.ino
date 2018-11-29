#include <bearssl/bearssl.h>
#include "base64c.h"

// buffer for doing base64 encoding operations for debugging / tracing. This generally needs to be about 4 times the size of the largest byte array you are encoding
const size_t buf_len = 1024;
static unsigned char base64_buf[buf_len];

void setup() {
  Serial.begin(9600);
  Serial.println("Setup complete");
}

void test_aes_esp8266() {
  const br_block_cbcenc_class *ve = &br_aes_ct_cbcenc_vtable;
  const br_block_cbcdec_class *vd = &br_aes_ct_cbcdec_vtable;

  // message must be multiple of 16 bytes or you will segfault
  char data[200] = "Test Message_!@ "
                   "More message dat";
  size_t data_len = strlen(data);
  unsigned char key[32] = {
    0xf6,0xd6,0x6d,0x6b,0xd5,0x2d,0x59,0xbb,0x07,0x96,0x36,0x58,
    0x79,0xef,0xf8,0x86,0xc6,0x6d,0xd5,0x1a,0x5b,0x6a,0x99,0x74,
    0x4b,0x50,0x59,0x0c,0x87,0xa2,0x38,0x84
  };
  size_t key_len = 32;
  unsigned char ive[16] = {0x00, 0xfa, 0xac, 0x24, 0xc1, 0x58, 0x5e, 0xf1f, 0x5a, 0x43, 0xd8, 0x75};
  unsigned char ivd[16] = {0x00, 0xfa, 0xac, 0x24, 0xc1, 0x58, 0x5e, 0xf1f, 0x5a, 0x43, 0xd8, 0x75};
  size_t iv_len = 16;
  memcpy(ivd, ive, iv_len);

  br_aes_gen_cbcenc_keys v_ec;
  const br_block_cbcenc_class **ec = &v_ec.vtable;

  br_aes_gen_cbcdec_keys v_dc;
  const br_block_cbcdec_class **dc = &v_dc.vtable;

  Serial.print("Encoding text: ");
  Serial.println(data);
  Serial.print("Data length: ");
  Serial.println(data_len);
  Serial.print("Key length: ");
  Serial.println(key_len);

  base64c_encode((unsigned char*)data, data_len, base64_buf, buf_len);
  Serial.print("Pre-Encoded Text base64: ");
  Serial.println((char*)base64_buf);

  base64c_encode((unsigned char*)key, key_len, base64_buf, buf_len);
  Serial.print("Key pre-encode base64: ");
  Serial.println((char*)base64_buf);

  base64c_encode((unsigned char*)ive, iv_len, base64_buf, buf_len);
  Serial.print("IV pre-encode base64: ");
  Serial.println((char*)base64_buf);

  ve->init(ec, key, key_len);
  Serial.println("Init Completed");
  ve->run(ec, ive, data, data_len);
  Serial.println("Encode Completed");

  base64c_encode((unsigned char*)data, data_len, base64_buf, buf_len);
  Serial.print("Encoded Text base64: ");
  Serial.println((char*)base64_buf);

  base64c_encode((unsigned char*)key, key_len, base64_buf, buf_len);
  Serial.print("Key pre-decode base64: ");
  Serial.println((char*)base64_buf);

  base64c_encode((unsigned char*)ive, iv_len, base64_buf, buf_len);
  Serial.print("IVE post-encode base64: ");
  Serial.println((char*)base64_buf);

  base64c_encode((unsigned char*)ivd, iv_len, base64_buf, buf_len);
  Serial.print("IVD pre-decode base64: ");
  Serial.println((char*)base64_buf);

  Serial.println("Decode Init Completed");
  vd->init(dc, key, key_len);
  vd->run(dc, ivd, data, data_len);

  base64c_encode((unsigned char*)data, data_len, base64_buf, buf_len);
  Serial.print("Decoded Text base64: ");
  Serial.println((char*)base64_buf);
  Serial.print("Decoded text: ");
  Serial.println(data);
}


void loop() {
  test_aes_esp8266();

  Serial.println("Sleeping 5 seconds");
  delay(5000);
}
