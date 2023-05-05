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

    // 결과 출력. 
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
        if(crc_byte[i]^0x80==0){
            sprintf(temp[i], "%hu", crc_byte[i]<<1);
        }
        else{
            sprintf(temp[i], "%hu", crc_byte[i]);
        }
        for (int j = 0; j < 2; j++) {
            crc_str[i][j] = temp[i][j] + '0'; // ASCII 코드로 변환
        }
        crc_str[i][2] = '\0'; 
        printf("Encoded string: %s\n", crc_str[i]);
    }

    //문자열을 이어붙여 CodeWord만들고 반환
    //codeword 메모리 동적 할당 및 초기화
    char **codeword = (char**)malloc((row) * sizeof(char *));;
    for (int i = 0; i < row; i++) {
        codeword[i] = (char *)malloc(col * sizeof(char));
        strcpy(codeword[i],data_str[i]);
        strcat(codeword[i],crc_str[i]);
        // printf("cut\n");
        // printf("%s\n",codeword[i]);

        uint8_t codeword_byte[1027];
        for(int j=0; j<1027; j++){
            codeword_byte[j]=(uint8_t)codeword[j];
            printf("%d ", codeword_byte[j]);
        }
        printf("\ncut\n");
    }

    return codeword;
}

/*
int main()
{
    // CRC를 계산할 데이터
    uint8_t data[] = {205, 75, 151, 95, 104, 74, 172, 155, 64, 66, 160, 202, 188, 204, 16, 174, 82, 5, 58, 10, 59, 131, 23, 177, 45, 69, 178, 196, 160, 187, 121, 184, 48, 231, 71, 152, 125, 16, 206, 242, 195, 93, 41, 49, 70, 107, 38, 166, 51, 10, 218, 97, 214, 176, 246, 75, 83, 139, 228, 226, 0, 157, 212, 49, 47, 100, 174, 14, 160, 70, 234, 118, 92, 40, 90, 124, 8, 27, 203, 115, 33, 134, 39, 152, 3, 19, 49, 106, 11, 84, 93, 73, 113, 138, 65, 26, 43, 242, 194, 138, 76, 177, 44, 28, 27, 35, 138, 123, 93, 202, 148, 237, 117, 60, 99, 167, 82, 175, 97, 216, 184, 27, 102, 151, 2, 25, 52, 240, 194, 141, 218, 251, 130, 205, 143, 114, 230, 130, 31, 115, 93, 43, 119, 113, 146, 88, 228, 131, 32, 139, 106, 160, 81, 112, 194, 183, 82, 162, 192, 24, 198, 166, 136, 215, 136, 151, 37, 113, 108, 114, 8, 55, 146, 27, 56, 60, 18, 243, 133, 129, 76, 212, 44, 106, 224, 87, 152, 160, 123, 117, 113, 181, 81, 160, 180, 74, 183, 180, 40, 22, 254, 25, 99, 72, 130, 160, 141, 114, 86, 139, 22, 33, 54, 120, 66, 192, 162, 156, 113, 41, 72, 58, 107, 152, 183, 181, 151, 144, 52, 0, 212, 178, 53, 32, 160, 12, 227, 8, 96, 18, 171, 51, 66, 110, 56, 133, 169, 136, 112, 18, 94, 241, 24, 93, 110, 20, 138, 129, 127, 240, 124, 74, 92, 81, 157, 171, 150, 120, 34, 186, 157, 170, 87, 168, 123, 203, 94, 97, 218, 99, 255, 0, 80, 191, 17, 27, 19, 88, 75, 37, 116, 148, 233, 137, 174, 37, 248, 248, 32, 246, 148, 115, 47, 241, 25, 169, 150, 230, 19, 27, 18, 135, 4, 190, 101, 241, 45, 99, 110, 229, 242, 129, 135, 49, 78, 8, 84, 26, 171, 134, 90, 123, 82, 147, 204, 203, 191, 219, 224, 20, 192, 126, 165, 209, 212, 181, 66, 184, 95, 75, 100, 67, 99, 163, 81, 247, 45, 81, 116, 37, 159, 100, 27, 13, 142, 160, 170, 138, 181, 101, 204, 236, 30, 165, 155, 149, 192, 196, 124, 32, 57, 38, 29, 89, 48, 211, 29, 234, 101, 20, 67, 241, 5, 237, 143, 169, 103, 95, 193, 151, 241, 255, 217, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    size_t length = sizeof(data) / sizeof(data[0]);

    // CRC를 계산하고 출력
    uint16_t crc = crc16_ccitt(data, length);
    printf("CRC: 0x%04X\n", crc);

    return 0;
}
*/
