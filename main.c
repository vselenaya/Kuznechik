#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "kuznechik.h"
#include "test_speed.h"
#include "test_correct.h"

int main() {
    test_correct();
    test_speed();
}
