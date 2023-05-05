#include <stdio.h>
#include <stdint.h>
#include "FileIntConversion.c"

#define POLY 0x1021 // CCITT 다항식
#define INIT 0xFFFF // 초기값
#define XOR_OUT 0x0000 // 최종 XOR 값

uint16_t crc16_ccitt(const uint8_t *data, size_t length)
{
    uint16_t crc = INIT;

    for (size_t i = 0; i < length; i++) {
        crc ^= ((uint16_t)data[i] << 8);

        for (size_t j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ POLY;
            } else {
                crc <<= 1;
            }
        }
    }

    return crc ^ XOR_OUT;
}

int main()
{
    int **data_int = FileIntConversion();
    uint8_t **data_byte;
    size_t length;

    //byte로 바꾸기
    data_byte= (uint8_t **)malloc(row * sizeof(uint8_t *)); // uint8_t의 2차원 배열 동적할당
    for (int i = 0; i < row; i++) {
        data_byte[i] = (uint8_t *)malloc(col * sizeof(uint8_t));
        for (int j = 0; j < col; j++) {
            data_byte[i][j] = (uint8_t)data_int[i][j]; // 정수형 배열의 값을 uint8_t 배열로 변환하여 저장
        }
    }

    // CRC를 계산할 데이터
    length = sizeof(data_byte[0]) / sizeof(data_byte[0][1]);

    // CRC를 계산하고 출력
    uint16_t crc = crc16_ccitt(data_byte[0], length);
    printf("CRC: 0x%04X\n", crc);
    printf("%d",length);

    return 0;
}