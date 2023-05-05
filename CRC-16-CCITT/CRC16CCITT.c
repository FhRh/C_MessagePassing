#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define POLY 0x1021 // CCITT 다항식
#define INIT 0xFFFF // 초기값
#define XOR_OUT 0x0000 // 최종 XOR 값
#define BLOCK_SIZE 1024
int row, col;

int **FileIntConversion()
{
    FILE* fp;
    int file_size, num_blocks;
    int** blocks;
    int temp;

    // 파일 열기
    fp = fopen("crysis_3_concrete_skeletons_by_pino44io_d8a5041-pre.jpg", "rb");
    if (fp == NULL) {
        printf("Failed to open file.");
        return 1;
    }

    // 파일 크기 계산
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // 블록 개수 계산
    num_blocks = (file_size + BLOCK_SIZE - 1) / BLOCK_SIZE;

    // 2차원 배열 동적 할당
    blocks = (int**)malloc(num_blocks * sizeof(int*));
    for (row = 0; row < num_blocks; ++row) {
        blocks[row] = (int*)malloc(BLOCK_SIZE * sizeof(int));
    }

    // 파일 읽기
    for (row = 0; row < num_blocks; ++row) {
        for (col = 0; col < BLOCK_SIZE; ++col) {
            temp = fgetc(fp);
            if(temp!=EOF){
                blocks[row][col] = temp;
            }
            else {
                break;
            }
        }
    }
    
    col=BLOCK_SIZE;

    // 파일 닫기
    fclose(fp); 

    // 결과 출력
    // for (row = 0; row < num_blocks; ++row) {
    //     for (col = 0; col < BLOCK_SIZE; ++col) {
    //         printf("%d, ", blocks[row][col]);
    //     }
    //     printf("\n");
    // }
    
    
    
    return blocks;
}

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

char** create_codeword()
{
    int **data_int = FileIntConversion();

    //byte로 바꾸기
    uint8_t **data_byte= (uint8_t **)malloc((row) * sizeof(uint8_t *)); // uint8_t의 2차원 배열 동적할당
    for (int i = 0; i < row; i++) {
        data_byte[i] = (uint8_t *)malloc(col * sizeof(uint8_t));
        for (int j = 0; j < col; j++) {
            data_byte[i][j] = (uint8_t)data_int[i][j]; // 정수형 배열의 값을 uint8_t 배열로 변환하여 저장
        }
    }

    //data_byte를 문자열로 변환
    char data_str[row][col+1];
    for(int i=0; i<row; i++){
        sprintf(data_str[i], "%s", (char*)data_byte[i]); // 각 요소를 2자리 16진수로 변환하여 문자열에 추가
    }
    //결과 출력
    // for(int i=0; i<row; i++){
    //         printf("%s\n",data_str[i]);
    // }
    
    
    
    // CRC를 계산
    // 0x를 제외한 데이터로 저장됨
    uint16_t crc_byte[row]; 
    for(int i=0; i<row; i++){
        crc_byte[i] = crc16_ccitt(data_byte[i], col);
        //printf("CRC: %04x\n", crc_byte[i]);
    }

    //crc_byte를 문자열로 변환
    char crc_str[row][3];
    char temp[row][3];
    for(int i=0; i<row; i++){
        if((crc_byte[i]>>7)==0){
            sprintf(temp[i], "%hu", crc_byte[i]<<1);
        }
        else{
            sprintf(temp[i], "%hu", crc_byte[i]);
        }
        
        for (int j = 0; j < 2; j++) {
            crc_str[i][j] = temp[i][j] + '0'; // ASCII 코드로 변환
        }
        crc_str[i][2] = '\0'; 
        //printf("Encoded string: %s\n", crc_str[i]);
    }

    //문자열을 이어붙여 CodeWord만들고 반환
    char codeword[row][col+3];
    for(int i=0; i<row; i++){
        strcpy(codeword[i],data_str[i]);
        strcat(codeword[i],crc_str[i]);
        // printf("cut\n");
        // printf("%s\n",codeword[i]);
    }

    return codeword;
}