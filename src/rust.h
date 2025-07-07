#ifndef RUST_H
#define RUST_H
#include <stdint.h>

void aes_encrypt(uint8_t* data, uint64_t len, const uint8_t key[32]);
void aes_decrypt(uint8_t* data, uint64_t len, const uint8_t key[32]);
void sandbox_process(uint64_t pid);

#endif