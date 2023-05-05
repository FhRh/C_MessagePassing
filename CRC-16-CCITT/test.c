#include <stdio.h>
#include <stdint.h>

int main() {
    uint16_t value = 0x8e51;
    char str[3]; // 문자열 버퍼 크기는 2자리 문자열과 NULL 문자를 포함한 3

    sprintf(str, "%hu", value);

    char encoded_str[3];

    for (int i = 0; i < 2; i++) {
        encoded_str[i] = str[i] + '0'; // ASCII 코드로 변환
    }
    encoded_str[2] = '\0'; // 문자열 종료 문자 삽입

    printf("Encoded string: %s\n", encoded_str);

    return 0;
}