#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "kuznechik.h"


// подстановка для нелинейного биективного преобразования:
static const uint8_t pi[256] = {252, 238, 221, 17, 207, 110, 49, 22, 251, 196, 250, 218, 35, 197, 4, 77,
                                233, 119, 240, 219, 147, 46, 153, 186, 23, 54, 241, 187, 20, 205, 95, 193,
                                249, 24, 101, 90, 226, 92, 239, 33, 129, 28, 60, 66, 139, 1, 142, 79,
                                5, 132, 2, 174, 227, 106, 143, 160, 6, 11, 237, 152, 127, 212, 211, 31,
                                235, 52, 44, 81, 234, 200, 72, 171, 242, 42, 104, 162, 253, 58, 206, 204,
                                181, 112, 14, 86, 8, 12, 118, 18, 191, 114, 19, 71, 156, 183, 93, 135,
                                21, 161, 150, 41, 16, 123, 154, 199, 243, 145, 120, 111, 157, 158, 178, 177,
                                50, 117, 25, 61, 255, 53, 138, 126, 109, 84, 198, 128, 195, 189, 13, 87,
                                223, 245, 36, 169, 62, 168, 67, 201, 215, 121, 214, 246, 124, 34, 185, 3,
                                224, 15, 236, 222, 122, 148, 176, 188, 220, 232, 40, 80, 78, 51, 10, 74,
                                167, 151, 96, 115, 30, 0, 98, 68, 26, 184, 56, 130, 100, 159, 38, 65,
                                173, 69, 70, 146, 39, 94, 85, 47, 140, 163, 165, 125, 105, 213, 149, 59,
                                7, 88, 179, 64, 134, 172, 29, 247, 48, 55, 107, 228, 136, 217, 231, 137,
                                225, 27, 131, 73, 76, 63, 248, 254, 141, 83, 170, 144, 202, 216, 133, 97,
                                32, 113, 103, 164, 45, 43, 9, 91, 203, 155, 37, 208, 190, 229, 108, 82,
                                89, 166, 116, 210, 230, 244, 180, 192, 209, 102, 175, 194, 57, 75, 99, 182};

// обратная к подстановке pi:
static const uint8_t inv_pi[256] = {0xA5, 0x2D, 0x32, 0x8F, 0x0E, 0x30, 0x38, 0xC0, 0x54, 0xE6, 0x9E, 0x39, 0x55, 0x7E, 0x52, 0x91,
                                    0x64, 0x03, 0x57, 0x5A, 0x1C, 0x60, 0x07, 0x18, 0x21, 0x72, 0xA8, 0xD1, 0x29, 0xC6, 0xA4, 0x3F,
                                    0xE0, 0x27, 0x8D, 0x0C, 0x82, 0xEA, 0xAE, 0xB4, 0x9A, 0x63, 0x49, 0xE5, 0x42, 0xE4, 0x15, 0xB7,
                                    0xC8, 0x06, 0x70, 0x9D, 0x41, 0x75, 0x19, 0xC9, 0xAA, 0xFC, 0x4D, 0xBF, 0x2A, 0x73, 0x84, 0xD5,
                                    0xC3, 0xAF, 0x2B, 0x86, 0xA7, 0xB1, 0xB2, 0x5B, 0x46, 0xD3, 0x9F, 0xFD, 0xD4, 0x0F, 0x9C, 0x2F,
                                    0x9B, 0x43, 0xEF, 0xD9, 0x79, 0xB6, 0x53, 0x7F, 0xC1, 0xF0, 0x23, 0xE7, 0x25, 0x5E, 0xB5, 0x1E,
                                    0xA2, 0xDF, 0xA6, 0xFE, 0xAC, 0x22, 0xF9, 0xE2, 0x4A, 0xBC, 0x35, 0xCA, 0xEE, 0x78, 0x05, 0x6B,
                                    0x51, 0xE1, 0x59, 0xA3, 0xF2, 0x71, 0x56, 0x11, 0x6A, 0x89, 0x94, 0x65, 0x8C, 0xBB, 0x77, 0x3C,
                                    0x7B, 0x28, 0xAB, 0xD2, 0x31, 0xDE, 0xC4, 0x5F, 0xCC, 0xCF, 0x76, 0x2C, 0xB8, 0xD8, 0x2E, 0x36,
                                    0xDB, 0x69, 0xB3, 0x14, 0x95, 0xBE, 0x62, 0xA1, 0x3B, 0x16, 0x66, 0xE9, 0x5C, 0x6C, 0x6D, 0xAD,
                                    0x37, 0x61, 0x4B, 0xB9, 0xE3, 0xBA, 0xF1, 0xA0, 0x85, 0x83, 0xDA, 0x47, 0xC5, 0xB0, 0x33, 0xFA,
                                    0x96, 0x6F, 0x6E, 0xC2, 0xF6, 0x50, 0xFF, 0x5D, 0xA9, 0x8E, 0x17, 0x1B, 0x97, 0x7D, 0xEC, 0x58,
                                    0xF7, 0x1F, 0xFB, 0x7C, 0x09, 0x0D, 0x7A, 0x67, 0x45, 0x87, 0xDC, 0xE8, 0x4F, 0x1D, 0x4E, 0x04,
                                    0xEB, 0xF8, 0xF3, 0x3E, 0x3D, 0xBD, 0x8A, 0x88, 0xDD, 0xCD, 0x0B, 0x13, 0x98, 0x02, 0x93, 0x80,
                                    0x90, 0xD0, 0x24, 0x34, 0xCB, 0xED, 0xF4, 0xCE, 0x99, 0x10, 0x44, 0x40, 0x92, 0x3A, 0x01, 0x26,
                                    0x12, 0x1A, 0x48, 0x68, 0xF5, 0x81, 0x8B, 0xC7, 0xD6, 0x20, 0x0A, 0x08, 0x00, 0x4C, 0xD7, 0x74};

// константы из поля F, используемые для линейного преобразования l:
static const uint8_t l_const[16] = {1, 148, 32, 133, 16, 194, 192, 1, 251, 1, 192, 194, 16, 133, 32, 148};

// итерационные константы C:
static uint8_t iter_C[32][BLOCK_SIZE];

// итерационные ключи шифрования:
static uint8_t iter_key[10][BLOCK_SIZE]; 

// заранее вычисленные значения функции L - а именно:
// precomputed_L[n][mask] - это блок (то есть массив длины BLOCK_SIZE) = linear_L(a15, a14, ... a1, a0) - то
// есть преобразование L от блока a15, ..., a0 (ai - байты, иx BLOCK_SIZE штук), где все ai (кроме i = n)
// равны 0, a{n} == mask
// (смысл в том, что любой блок можно представить как xor (^) блоков такого вида: 15 байтов кроме одного
// равны 0, а один оставшийся принимает одно из 256 значений (так как байт = 8 битов -> 2^8=256 значений)...
// и вот все эти 16 * 256 вариантов можно заранее предподсчитать! а затем вспомнить, что L - линейно,
// а значит L(A^B) = L(A) ^ L(B) (A,B - блоки размера BLOCK_SIZE)... то есть L от любого блока считается
// как xor заранее вычисленных значений... ускорение!!!):
static uint8_t precomputed_L[16][256][BLOCK_SIZE];



// Функция, которая делает поэлементый xor двух массивов arr1 и arr2 из BLOCK_SIZE байтов, результат в res_arr (X - обозначение в документации)
static void xor_arrays_X(const uint8_t *arr1, const uint8_t *arr2, uint8_t *res_arr) {
    // важная оптимизация!
    // так как в C массивы хранятся непрерывным отрезком памяти, то в случае, когда массив состоит из 16 
    // байтов (как в случае с Кузнечиком, где каждый блок = 16 байтов), он занимает непрерывные 128 бит...
    // чтобы сделать побитовый xor таких массивов, можно не ксорить элементы поотдельности, с сразу сделать
    // xor 128 битных кусков... для этого мы просто представляем, что начало массива - это начало 
    // 128-битного числа __uint128_t (для этого просто указаетль на массив меняем на указатель этого типа),
    // а далее разыменоаваем указаетель, получая фактически эти 128 бит, и делаем их xor...
    // так получаем xor двух 128-битных кусков гораздо быстрее!
    if (BLOCK_SIZE == 16)  
        *((__uint128_t*) res_arr) = *((__uint128_t*) arr1) ^ *((__uint128_t*) arr2);
    else {
        for (int i = 0; i < BLOCK_SIZE; i ++)
            res_arr[i] = arr1[i] ^ arr2[i];  // просто поэлементный xor
    }
}


// Функция нелинейного биективного преобразования из in_data в out_data (S - обозначение в документации)
void unlinear_S(const uint8_t *in_data, uint8_t *out_data) {
    for (int i = 0; i < BLOCK_SIZE; i++)
        out_data[i] = pi[in_data[i]];
}


// Обратное нелинейное преобразование (S^-1 - обозначение в документации)
void inv_unlinear_S(const uint8_t *in_data, uint8_t *out_data) {
    for (int i = 0; i < BLOCK_SIZE; i++)
           out_data[i] = inv_pi[in_data[i]];
}


// Умножение чисел в поле (в начале документации описано поле F как поле вычетов по многочлену x^8 + x^7 + x^6 + x + 1, и сказано,
//                         что элемент из такого поля - число... вот эти числа в этом поле умножаем):
static uint8_t multiply_in_F(uint8_t x, uint8_t y) {
    uint8_t res = 0;
    uint8_t hi_bit;

    for (int i = 0; i < 8; i++) {
        if (y & 1)
            res ^= x;
        hi_bit = x & 0x80;
        x <<= 1;
        if (hi_bit)
            x ^= 0xc3; // Полином x^8 + x^7 + x^6 + x + 1
        y >>= 1;
    }
    return res;
}


// Функция сдвига и применения l: то есть из 128-битного массива a15, a14, ..., a1, a0 получается l(a15, a14, ..., a0), a15, ..., a1,
// где ai - это 1 байт массива, то есть как раз блок (=128-битный массив) состоит из 16 таких элементов. Это преобразование R в документации
void lshift_R(const uint8_t *arr128, uint8_t *result) {
    uint8_t temp[BLOCK_SIZE];
    for (int i = BLOCK_SIZE-1; i >= 1; i --)
        temp[i - 1] = arr128[i];  // двигаем байты в сторону младшего разряда

    uint8_t l = 0;  // значение l(a15, a14, ..., a0) - это просто линейная комбинация a15, ..., 10 с коэффициентами l_const
    for (int i = BLOCK_SIZE-1; i >= 0; i --)
        l ^= multiply_in_F(arr128[i], l_const[i]);  // считаем l (так как вычисления в поле, то вместо суммы тут xor)

    temp[BLOCK_SIZE-1] = l;  // дописываем полученный байт
    memcpy(result, temp, BLOCK_SIZE);  // результат переписываем обратно во входные данные
}


// Функция линейного преобразования (L в документации) - просто 16 раз повторить R
void linear_L(const uint8_t *in_data, uint8_t *out_data) {
    memcpy(out_data, in_data, BLOCK_SIZE);
    for (int i = 0; i < 16; i++) 
        lshift_R(out_data, out_data);
}


// Обратное к R преобразование (R^-1 в документации)
// arr128 - 128 бит в виде 16-байтового массива (16 элементов - каждый типа uint8_t (он занимает ровно 1 байт))
void inv_lshift_R(const uint8_t *arr128, uint8_t *result) {
    uint8_t temp[BLOCK_SIZE];
    for (int i = 1; i < BLOCK_SIZE; i++)
        temp[i] = arr128[i - 1];
    temp[0] = arr128[BLOCK_SIZE-1];  // arr128 из a15, ..., a0 превратился в result: a14, ..., a0, a15 - просто сдвиг

    uint8_t l = 0;  // считаем l(a14, ..., a0, a15)
    for (int i = 0; i < BLOCK_SIZE; i++)
        l ^= multiply_in_F(temp[i], l_const[i]);

    temp[0] = l;  // и дописываем l - точно как в документации (формула (7) на странице 7)
    memcpy(result, temp, BLOCK_SIZE);
}


// Функция обратного линейного преобразования (L^-1 в документации) - просто 16 раз повторить R^-1
void inv_linear_L(const uint8_t *in_data, uint8_t *out_data) {
    memcpy(out_data, in_data, BLOCK_SIZE);
    for (int i = 0; i < 16; i++) 
        inv_lshift_R(out_data, out_data);
}


// Вычисляем итерационные коснтанты:
void compute_consts() {
    uint8_t iters[32][BLOCK_SIZE]; // сюда будем писать номер итерации от 1 до 32 (номер итерации в виде 128 битного чсила, поэтому массив)
    for (int i = 0; i < 32; i ++) {
        memset(iters[i], 0, BLOCK_SIZE);
        iters[i][0] = i+1;  // записываем номер 
    }
    
    for (int i = 0; i < 32; i++)  // делаем преобразование L -> получаем коснтанты
        linear_L(iters[i], iter_C[i]);
}


// Функция одной итерации развертывания ключа (F в документации).
// На вход функции подаем очередную пару ключей, очередную итерационную константу, на выходе получаем очередную
// промежуточную пару итерационных ключей.
static void get_next_keys_F(const uint8_t *key_1, const uint8_t *key_2,
                            uint8_t *out_key_1, uint8_t *out_key_2,
                            uint8_t *iter_const) {
    memcpy(out_key_2, key_1, BLOCK_SIZE);  // второй ключ - это копия первого (это же сеть Фейстиля, которая одну порловину оставляет неизменной)
    uint8_t temp[BLOCK_SIZE];
    xor_arrays_X(key_1, iter_const, temp);
    unlinear_S(temp, temp);
    linear_L(temp, temp);
    xor_arrays_X(temp, key_2, out_key_1);
}


// Функция развёртывания ключа, master_key - исходный ключ длины 256 (= 2 * BLOCK_SIZE) битов превращается в 10 итерационных
// ключей - каждый gj 128 битов
void expand_key(const uint8_t *master_key) {
    compute_consts(); // вычисляем итерационные константы

    // Предыдущая пара ключей
    uint8_t iter_1[BLOCK_SIZE];
    uint8_t iter_2[BLOCK_SIZE];

    // Текущая пара ключей
    uint8_t iter_3[BLOCK_SIZE];
    uint8_t iter_4[BLOCK_SIZE];

    // Первые два итерационных ключа равны половинкам мастер-ключа
    memcpy(iter_key[1], master_key, BLOCK_SIZE);  // именно в таком порядке, так как половинки отсчитываются с начала записи master_key
    memcpy(iter_key[0], master_key + BLOCK_SIZE, BLOCK_SIZE);  // (а запись в российских Гостах производится со старшего бита)
    memcpy(iter_1, iter_key[0], BLOCK_SIZE);
    memcpy(iter_2, iter_key[1], BLOCK_SIZE);

    for (int i = 0; i < 4; i++) {  // делаем по формуле (11) со страницы 8 документации
        get_next_keys_F(iter_1, iter_2, iter_3, iter_4, iter_C[0 + 8 * i]);
        get_next_keys_F(iter_3, iter_4, iter_1, iter_2, iter_C[1 + 8 * i]);
        get_next_keys_F(iter_1, iter_2, iter_3, iter_4, iter_C[2 + 8 * i]);
        get_next_keys_F(iter_3, iter_4, iter_1, iter_2, iter_C[3 + 8 * i]);
        get_next_keys_F(iter_1, iter_2, iter_3, iter_4, iter_C[4 + 8 * i]);
        get_next_keys_F(iter_3, iter_4, iter_1, iter_2, iter_C[5 + 8 * i]);
        get_next_keys_F(iter_1, iter_2, iter_3, iter_4, iter_C[6 + 8 * i]);
        get_next_keys_F(iter_3, iter_4, iter_1, iter_2, iter_C[7 + 8 * i]);
        memcpy(iter_key[2 * i + 2], iter_1, BLOCK_SIZE);
        memcpy(iter_key[2 * i + 3], iter_2, BLOCK_SIZE);
    }
}


// Функция шифрования блока plaintext_128 в блок ciphertext_128 (каждый блок - длины 128 битов)
void kuznechik_encrypt(const uint8_t *plaintext_128, uint8_t *ciphertext_128) {
    memcpy(ciphertext_128, plaintext_128, BLOCK_SIZE);

    for(int i = 0; i < 9; i ++) {  // последовательно шифруем в 10 шагов
        xor_arrays_X(ciphertext_128, iter_key[i], ciphertext_128);
        unlinear_S(ciphertext_128, ciphertext_128);
        linear_L(ciphertext_128, ciphertext_128);
    }
    xor_arrays_X(ciphertext_128, iter_key[9], ciphertext_128);
}


// Функция дешифрования блока
void kuznechik_decrypt(const uint8_t *ciphertext_128, uint8_t *plaintext_128) {
    memcpy(plaintext_128, ciphertext_128, BLOCK_SIZE);

    xor_arrays_X(plaintext_128, iter_key[9], plaintext_128);
    for(int i = 8; i >= 0; i --) {
        inv_linear_L(plaintext_128, plaintext_128);
        inv_unlinear_S(plaintext_128, plaintext_128);
        xor_arrays_X(iter_key[i], plaintext_128, plaintext_128);
    }
}


// ============= дополнительный код для ускорения шифрования =============


// Функция для вычисления значений L
void compute_values_L() {
    uint8_t curr_block[BLOCK_SIZE];  // создаём 128-битный блок
    for (size_t i = 0; i < BLOCK_SIZE; i ++)  // полностью из нулей
        curr_block[i] = 0;
    
    for (size_t n = 0; n < 16; n ++) {  // перебираем номер байта в блоке
        for (size_t mask = 0; mask < 256; mask ++) {  // перебираем все значения байта
            curr_block[n] = mask;  // берём блок, у которого только n-ый байт равен mask, а остальное нули
            linear_L(curr_block, precomputed_L[n][mask]);  // считаем от него L
        }
        curr_block[n] = 0;  // не забываем занулить n-ый байт перед следующей итерацией
    }
}


// Ускоренная версия функции линейного преобразования L с ипользованием заранее вычисленных значений L
void fast_linear_L(const uint8_t *in_data, uint8_t *out_data) {
    uint8_t in_data_copy[BLOCK_SIZE];  // копируем входные данные, чтобы не потерять их (если на вход функции
    for (int i = 0; i < BLOCK_SIZE; i ++) {  // в качетсве in_data и out_data подали один и тот же указаетль)
        in_data_copy[i] = in_data[i];
        out_data[i] = 0;  // попутно зануляем выходные данные
    }

    // а далее очень просто: in_data = a15, ... a0 - 16 байтов данных, 
    // их можно записать: in_data = (a15,0,...,0) ^ (0,a14,0,...,0) ^ ... ^ (0,0,...,a0) - просто xor блоков,
    // в которых только один исходных байт незанулён (а остальные = 0) 
    // а далее линейность L: L(in_data) = L(a15,0,...,0) ^ ... ^ L(0,0,...,a0)
    // теперь получили, что L(in_data) - это просто xor 16-ти значений L на таких вот блоках с одним ненулевым байтом...
    // а все эти значения мы посчитали ранее! для каждого n ( = 0, ..., 15) и всех mask (= 0, ..., 255 - значения
    // байта) в массиве сохранено значение L на блоке, где все байты равны 0, кроме n-ого, который = mask.
    for (int n = 0; n < BLOCK_SIZE; n ++)
        xor_arrays_X(out_data, precomputed_L[n][in_data_copy[n]], out_data);  // делаем xor посчитанных значений L
}


// Та же функция шифрования блока, что и до этого, только теперь используется быстрая версия L
void fast_kuznechik_encrypt(const uint8_t *plaintext_128, uint8_t *ciphertext_128) {
    memcpy(ciphertext_128, plaintext_128, BLOCK_SIZE);

    for(int i = 0; i < 9; i ++) {  // последовательно шифруем в 10 шагов
        xor_arrays_X(ciphertext_128, iter_key[i], ciphertext_128);
        unlinear_S(ciphertext_128, ciphertext_128);
        fast_linear_L(ciphertext_128, ciphertext_128);
    }
    xor_arrays_X(ciphertext_128, iter_key[9], ciphertext_128);
}


// Две дополнительные функции для доступа к глобальным массивам констант и ключей:
uint8_t (*get_ptr_iter_C(void))[BLOCK_SIZE] {
   return iter_C;
}

uint8_t (*get_ptr_iter_key(void))[BLOCK_SIZE] {
    return iter_key;
}