#pragma once

void speed_of_encrypt(size_t N, void (*encryption_func) (const uint8_t *, uint8_t *));
void test_speed();