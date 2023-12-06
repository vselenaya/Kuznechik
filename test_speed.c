#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "kuznechik.h"
#include "test_speed.h"


// Функция проверки скорости шифрования функцией encryption_func
void speed_of_encrypt(size_t N, void (*encryption_func) (const uint8_t *, uint8_t *)) {
    FILE* f = fopen("/dev/urandom", "rb");
    if (f == NULL) {
        printf("Невозможно открыть файл /dev/urandom!");
        return;
    }
    
    clock_t start = clock();  // запускаем таймер

    uint8_t plaintext_128[BLOCK_SIZE];
    uint8_t ciphertext_128[BLOCK_SIZE];

    for (size_t i = 0; i < N; i ++) {  // считываем N раз по BLOCK_SIZE байт из /dev/urandom - то есть случаные данные читаем
        fread(plaintext_128, BLOCK_SIZE, 1, f);
        encryption_func(plaintext_128, ciphertext_128);  // шифруем
    }

    double time = (double)(clock() - start) / CLOCKS_PER_SEC;  // вычисляем время 
    printf("--> Зашифровано %ld блоков за %.2f секунд\n", N, time);
    printf("--> Скорость шифрования ~ %.2f Мбит в секунду\n", ((double)128 * N / 1e6 / time));

    fclose(f);
}


// Функция теста скорости
void test_speed() {
    printf("Начинаем тестирование скорости...\n");
    printf("Обычное шифрование:\n");
    speed_of_encrypt(1e4, kuznechik_encrypt);
    printf("Тест ускоренного шифрования:\n");
    speed_of_encrypt(1e6, fast_kuznechik_encrypt);
}
