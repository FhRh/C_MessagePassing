#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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