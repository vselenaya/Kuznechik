#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "kuznechik.h"
#include "test_correct.h"


// Функция, преврщающая шестнадцатиричную строку (вида ffeeddccbbaa99881122334455667700) (из чётного количества символов,
// в которой каждые два символа кодируют один байт) в соответствующий массив байтов...

// hexstring - строка из чётного количества символов, каждый символ - шестнадцатиричная цифра (0, 1, 2, ..., 9, a, b, c, d, e, f)
// array - массив, куда сохранится результат
// bytes - количество байтов, записанных в строке (= длина строки / 2) = размер массива

// Заметим, что строка записывает байты следующим образом: каждые два подряд идущих символа - это один байт; при этом строка записана
// от старшего бита (слева) к младшему... Итоговый массив записывается от младего байта (0-ой индекс) к старшему.
// Пример: "0affa1" - такая строка кодирует 3 байта: '0a' = 16 (страший байт), 'ff' = 255, 'a1' = 161 (младший байт). Итоговый результат -
// - это массив [161, 255, 16] (два символа переводятся в байт как обычное шестнадцатиричное число: '0a' = 0 * 16^1 + a * 16^0 = 16,
// 'ff' = f * 16 + f = 15 * 16 + 15 = 255).
static void hexstring_to_array(const char *hexstring, uint8_t *array, size_t bytes) {
    const char *pos = hexstring;
    for (size_t i = 0; i < bytes; i ++) {
        sscanf(pos, "%2hhx", &array[bytes - i - 1]);
        pos += 2;
    }
    // for(size_t count = 0; count < bytes; count++)
        // printf("%d ", array[count]);
    // printf("\n");
}


// Функция, проверяющая преобразования
void test_kuznechik_LRS() {
    uint8_t data[BLOCK_SIZE];
    uint8_t result[BLOCK_SIZE];
    uint8_t answer[BLOCK_SIZE];


    // ================== ТЕСТИРОВАНИЕ ПРЕОБРАЗОВАНИЯ S ==================
    hexstring_to_array("ffeeddccbbaa99881122334455667700", data, BLOCK_SIZE);  // входные данные (записываем как массив из BLOCK_SIZE байтов)
    hexstring_to_array("b66cd8887d38e8d77765aeea0c9a7efc", answer, BLOCK_SIZE);  // ожидаемый результат после S-преобразования
    unlinear_S(data, result);  // проводим S-преобразование
    if (memcmp(result, answer, BLOCK_SIZE) != 0)  // проверяем равенство
        printf("Ошибка в S, тест 1\n");
    inv_unlinear_S(answer, result);  // теперь проводим обратное преобразование
    if (memcmp(result, data, BLOCK_SIZE) != 0)  // проверяем равенство
        printf("Ошибка в обратном S, тест 1\n");
    
    hexstring_to_array("b66cd8887d38e8d77765aeea0c9a7efc", data, BLOCK_SIZE);
    hexstring_to_array("559d8dd7bd06cbfe7e7b262523280d39", answer, BLOCK_SIZE);
    unlinear_S(data, result);
    if (memcmp(result, answer, BLOCK_SIZE) != 0)
        printf("Ошибка в S, тест 2\n");
    inv_unlinear_S(answer, result);
    if (memcmp(result, data, BLOCK_SIZE) != 0)
        printf("Ошибка в обратном S, тест 2\n");

    hexstring_to_array("559d8dd7bd06cbfe7e7b262523280d39", data, BLOCK_SIZE);
    hexstring_to_array("0c3322fed531e4630d80ef5c5a81c50b", answer, BLOCK_SIZE);
    unlinear_S(data, result);
    if (memcmp(result, answer, BLOCK_SIZE) != 0)
        printf("Ошибка в S, тест 3\n");
    inv_unlinear_S(answer, result);
    if (memcmp(result, data, BLOCK_SIZE) != 0)
        printf("Ошибка в обратном S, тест 3\n");
    
    hexstring_to_array("0c3322fed531e4630d80ef5c5a81c50b", data, BLOCK_SIZE);
    hexstring_to_array("23ae65633f842d29c5df529c13f5acda", answer, BLOCK_SIZE);
    unlinear_S(data, result);
    if (memcmp(result, answer, BLOCK_SIZE) != 0)
        printf("Ошибка в S, тест 4\n");
    inv_unlinear_S(answer, result);
    if (memcmp(result, data, BLOCK_SIZE) != 0)
        printf("Ошибка в обратном S, тест 4\n");


    // ================== ТЕСТИРОВАНИЕ ПРЕОБРАЗОВАНИЯ R ==================
    hexstring_to_array("00000000000000000000000000000100", data, BLOCK_SIZE);
    hexstring_to_array("94000000000000000000000000000001", answer, BLOCK_SIZE);
    lshift_R(data, result);
    if (memcmp(result, answer, BLOCK_SIZE) != 0)
        printf("Ошибка в R, тест 1\n");
    inv_lshift_R(answer, result);
    if (memcmp(result, data, BLOCK_SIZE) != 0)
        printf("Ошибка в обратном R, тест 1\n");

    hexstring_to_array("94000000000000000000000000000001", data, BLOCK_SIZE);
    hexstring_to_array("a5940000000000000000000000000000", answer, BLOCK_SIZE);
    lshift_R(data, result);
    if (memcmp(result, answer, BLOCK_SIZE) != 0)
        printf("Ошибка в R, тест 2\n");
    inv_lshift_R(answer, result);
    if (memcmp(result, data, BLOCK_SIZE) != 0)
        printf("Ошибка в обратном R, тест 2\n");
    
    hexstring_to_array("a5940000000000000000000000000000", data, BLOCK_SIZE);
    hexstring_to_array("64a59400000000000000000000000000", answer, BLOCK_SIZE);
    lshift_R(data, result);
    if (memcmp(result, answer, BLOCK_SIZE) != 0)
        printf("Ошибка в R, тест 3\n");
    inv_lshift_R(answer, result);
    if (memcmp(result, data, BLOCK_SIZE) != 0)
        printf("Ошибка в обратном R, тест 3\n");

    hexstring_to_array("64a59400000000000000000000000000", data, BLOCK_SIZE);
    hexstring_to_array("0d64a594000000000000000000000000", answer, BLOCK_SIZE);
    lshift_R(data, result);
    if (memcmp(result, answer, BLOCK_SIZE) != 0)
        printf("Ошибка в R, тест 4\n");
    inv_lshift_R(answer, result);
    if (memcmp(result, data, BLOCK_SIZE) != 0)
        printf("Ошибка в обратном R, тест 4\n");


    // ================== ТЕСТИРОВАНИЕ ПРЕОБРАЗОВАНИЯ L ==================
    hexstring_to_array("64a59400000000000000000000000000", data, BLOCK_SIZE);
    hexstring_to_array("d456584dd0e3e84cc3166e4b7fa2890d", answer, BLOCK_SIZE);
    linear_L(data, result);
    if (memcmp(result, answer, BLOCK_SIZE) != 0)
        printf("Ошибка в L, тест 1\n");
    inv_linear_L(answer, result);
    if (memcmp(result, data, BLOCK_SIZE) != 0)
        printf("Ошибка в обратном L, тест 1\n");

    hexstring_to_array("d456584dd0e3e84cc3166e4b7fa2890d", data, BLOCK_SIZE);
    hexstring_to_array("79d26221b87b584cd42fbc4ffea5de9a", answer, BLOCK_SIZE);
    linear_L(data, result);
    if (memcmp(result, answer, BLOCK_SIZE) != 0)
        printf("Ошибка в L, тест 2\n");
    inv_linear_L(answer, result);
    if (memcmp(result, data, BLOCK_SIZE) != 0)
        printf("Ошибка в обратном L, тест 2\n");

    hexstring_to_array("79d26221b87b584cd42fbc4ffea5de9a", data, BLOCK_SIZE);
    hexstring_to_array("0e93691a0cfc60408b7b68f66b513c13", answer, BLOCK_SIZE);
    linear_L(data, result);
    if (memcmp(result, answer, BLOCK_SIZE) != 0)
        printf("Ошибка в L, тест 3\n");
    inv_linear_L(answer, result);
    if (memcmp(result, data, BLOCK_SIZE) != 0)
        printf("Ошибка в обратном L, тест 3\n");

    hexstring_to_array("0e93691a0cfc60408b7b68f66b513c13", data, BLOCK_SIZE);
    hexstring_to_array("e6a8094fee0aa204fd97bcb0b44b8580", answer, BLOCK_SIZE);
    linear_L(data, result);
    if (memcmp(result, answer, BLOCK_SIZE) != 0)
        printf("Ошибка в L, тест 4\n");
    inv_linear_L(answer, result);
    if (memcmp(result, data, BLOCK_SIZE) != 0)
        printf("Ошибка в обратном L, тест 4\n");


    printf("--> Тестирование преобразований S, R, L завершено\n");
}


// Функция, проверяющая генерацию ключей
void test_kuznechik_KEYS() {
    uint8_t answer[BLOCK_SIZE];
    uint8_t (*iter_C)[BLOCK_SIZE] = get_ptr_iter_C();
    uint8_t (*iter_key)[BLOCK_SIZE] = get_ptr_iter_key();
    
    // ================== ТЕСТИРОВАНИЕ ИТЕРАЦИОННЫХ КОНСТАНТ ==================
    compute_consts();
    const char *samples[8] = {"6ea276726c487ab85d27bd10dd849401",
                              "dc87ece4d890f4b3ba4eb92079cbeb02",
                              "b2259a96b4d88e0be7690430a44f7f03",
                              "7bcd1b0b73e32ba5b79cb140f2551504",
                              "156f6d791fab511deabb0c502fd18105",
                              "a74af7efab73df160dd208608b9efe06",
                              "c9e8819dc73ba5ae50f5b570561a6a07",
                              "f6593616e6055689adfba18027aa2a08"};
    for (int i = 0; i < 8; i ++) {
        hexstring_to_array(samples[i], answer, BLOCK_SIZE);
        if (memcmp(answer, iter_C[i], BLOCK_SIZE) != 0)
            printf("Ошибка в итерационной константе %d\n", i);
    }

    // ================== ТЕСТИРОВАНИЕ КЛЮЧЕЙ ==================
    uint8_t master_key[2 * BLOCK_SIZE];
    hexstring_to_array("8899aabbccddeeff0011223344556677fedcba98765432100123456789abcdef", master_key, 2 * BLOCK_SIZE);
    expand_key(master_key);

    const char *key_samples[10] = {"8899aabbccddeeff0011223344556677",
                                   "fedcba98765432100123456789abcdef",
                                   "db31485315694343228d6aef8cc78c44",
                                   "3d4553d8e9cfec6815ebadc40a9ffd04",
                                   "57646468c44a5e28d3e59246f429f1ac",
                                   "bd079435165c6432b532e82834da581b",
                                   "51e640757e8745de705727265a0098b1",
                                   "5a7925017b9fdd3ed72a91a22286f984",
                                   "bb44e25378c73123a5f32f73cdb6e517",
                                   "72e9dd7416bcf45b755dbaa88e4a4043"};

    for (int i = 0; i < 10; i ++) {
        hexstring_to_array(key_samples[i], answer, BLOCK_SIZE);
        if (memcmp(answer, iter_key[i], BLOCK_SIZE) != 0)
            printf("Ошибка в ключе %d\n", i);
    }

    printf("--> Тестирование ключей завершено\n");
}


// Функция, проверяющая шифроввание
void test_encryption() {
    uint8_t data[BLOCK_SIZE];
    uint8_t result[BLOCK_SIZE];
    uint8_t answer[BLOCK_SIZE];

    hexstring_to_array("1122334455667700ffeeddccbbaa9988", data, BLOCK_SIZE);
    hexstring_to_array("7f679d90bebc24305a468d42b9d4edcd", answer, BLOCK_SIZE);
    kuznechik_encrypt(data, result);
    if (memcmp(result, answer, BLOCK_SIZE) != 0)
            printf("Ошибка в зашифровании\n");

    hexstring_to_array("7f679d90bebc24305a468d42b9d4edcd", data, BLOCK_SIZE);
    hexstring_to_array("1122334455667700ffeeddccbbaa9988", answer, BLOCK_SIZE);
    kuznechik_decrypt(data, result);
    if (memcmp(result, answer, BLOCK_SIZE) != 0)
            printf("Ошибка в расшифровании\n");

    printf("--> Тесты шифрования завершены\n");
}


// Функция проверки функций для ускоренного шифрования
void test_fast_functions() {
    uint8_t data[BLOCK_SIZE];
    uint8_t result[BLOCK_SIZE];
    uint8_t answer[BLOCK_SIZE];

    compute_values_L();

    // ================== ТЕСТИРОВАНИЕ УСКОРЕННОГО ПРЕОБРАЗОВАНИЯ L ==================
    hexstring_to_array("64a59400000000000000000000000000", data, BLOCK_SIZE);
    hexstring_to_array("d456584dd0e3e84cc3166e4b7fa2890d", answer, BLOCK_SIZE);
    fast_linear_L(data, result);
    if (memcmp(result, answer, BLOCK_SIZE) != 0)
        printf("Ошибка в БЫСТРОЙ версии L, тест 1\n");

    hexstring_to_array("d456584dd0e3e84cc3166e4b7fa2890d", data, BLOCK_SIZE);
    hexstring_to_array("79d26221b87b584cd42fbc4ffea5de9a", answer, BLOCK_SIZE);
    fast_linear_L(data, result);
    if (memcmp(result, answer, BLOCK_SIZE) != 0)
        printf("Ошибка в БЫСТРОЙ версии L, тест 2\n");

    hexstring_to_array("79d26221b87b584cd42fbc4ffea5de9a", data, BLOCK_SIZE);
    hexstring_to_array("0e93691a0cfc60408b7b68f66b513c13", answer, BLOCK_SIZE);
    fast_linear_L(data, result);
    if (memcmp(result, answer, BLOCK_SIZE) != 0)
        printf("Ошибка в БЫСТРОЙ версии L, тест 3\n");

    hexstring_to_array("0e93691a0cfc60408b7b68f66b513c13", data, BLOCK_SIZE);
    hexstring_to_array("e6a8094fee0aa204fd97bcb0b44b8580", answer, BLOCK_SIZE);
    fast_linear_L(data, result);
    if (memcmp(result, answer, BLOCK_SIZE) != 0)
        printf("Ошибка в БЫСТРОЙ версии L, тест 4\n");

    // ================== ТЕСТИРОВАНИЕ УСКОРЕННОГО ШИФРОВАНИЯ ==================
    hexstring_to_array("1122334455667700ffeeddccbbaa9988", data, BLOCK_SIZE);
    hexstring_to_array("7f679d90bebc24305a468d42b9d4edcd", answer, BLOCK_SIZE);
    fast_kuznechik_encrypt(data, result);
    if (memcmp(result, answer, BLOCK_SIZE) != 0)
            printf("Ошибка в БЫСТРОМ зашифровании\n");

    printf("--> Тесты ускоренного шифрования завершены\n");
}


// ====== Запуск тестов ======


// Функция проверки корректности кода на основе примеров в документации
void test_correct() {
    printf("Начинается тестирование корректности кода...\n");
    test_kuznechik_LRS();
    test_kuznechik_KEYS();
    test_encryption();
    test_fast_functions();
    printf("Тестирование корректности окончено. Если не было выведено сообщений об ошибках, значит тесты пройдены верно!\n\n");
}