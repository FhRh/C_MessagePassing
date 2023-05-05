#include <windows.h>
#include "CRC16CCITT.c"

void sendLargeString(HWND hWnd, const char* largeString, int index) {
    // 문자열 데이터의 크기를 계산합니다
    int dataSize = strlen(largeString) + 1; // null 문자 포함

    // 메모리를 할당하고 문자열 데이터를 복사합니다
    char* mem = (char*)malloc(dataSize);
    memcpy(mem, largeString, dataSize);


    // WM_COPYDATA 메시지를 생성합니다
    COPYDATASTRUCT cds;
    cds.dwData = 0; // 사용자 지정 데이터
    cds.cbData = dataSize;
    cds.lpData = (void*)mem;

    // 메시지를 보냅니다
    SendMessage(hWnd, WM_COPYDATA, (WPARAM)index, (LPARAM)&cds);

    // 할당된 메모리를 해제합니다
    // free(&cds);
}

int main()
{
    HWND hwndReceiver = FindWindow("class_name", "receiver_name");
    if (hwndReceiver == NULL) {
        MessageBox(NULL, "recevier not executing", "error", MB_OK | MB_ICONERROR);
        return 1;
    }

    char** codeword = create_codeword();
    for(int i=0; i<row; i++){
        sendLargeString(hwndReceiver, codeword[i], i);
    }

    return 0;
}

//그림 파일을 1k bytes단위로 잘라서 배열에 추가하고 
//그 배열의 데이터를 다시 1byte 단위로 잘라서 
//배열에 16진수로 표현하여 저장한다.