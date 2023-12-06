#pragma once

#include <stdint.h>

#define BLOCK_SIZE 16  // размер блока Кузнечика равен 128 бит = 16 байт (мы будем хранить блок как массив из 16 байт, байт - это unt8_t)

void unlinear_S(const uint8_t *in_data, uint8_t *out_data);
void inv_unlinear_S(const uint8_t *in_data, uint8_t *out_data);

void lshift_R(const uint8_t *arr128, uint8_t *result);
void inv_lshift_R(const uint8_t *arr128, uint8_t *result);

void linear_L(const uint8_t *in_data, uint8_t *out_data);
void inv_linear_L(const uint8_t *in_data, uint8_t *out_data);

void compute_consts();
void compute_values_L();
void expand_key(const uint8_t *master_key);

void kuznechik_encrypt(const uint8_t *plaintext_128, uint8_t *ciphertext_128);
void kuznechik_decrypt(const uint8_t *ciphertext_128, uint8_t *plaintext_128);

void fast_linear_L(const uint8_t *in_data, uint8_t *out_data);
void fast_kuznechik_encrypt(const uint8_t *plaintext_128, uint8_t *ciphertext_128);

uint8_t (*get_ptr_iter_C(void))[BLOCK_SIZE];
uint8_t (*get_ptr_iter_key(void))[BLOCK_SIZE];