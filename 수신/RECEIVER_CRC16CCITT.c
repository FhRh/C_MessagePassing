#include <stdio.h>
#include <stdint.h>

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

/*
int main()
{
    // CRC를 계산할 데이터
    uint8_t data[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};
    size_t length = sizeof(data) / sizeof(data[0]);

    // CRC를 계산하고 출력
    uint16_t crc = crc16_ccitt(data, length);
    printf("CRC: 0x%04X\n", crc);

    return 0;
}
*/