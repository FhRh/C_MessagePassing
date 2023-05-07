#include <windows.h>
#include "SENDER_CRC16CCITT.c"

void sendLargeString(HWND hWnd, const uint8_t* data_kbytes, int index) {
    // 문자열 데이터의 크기를 계산합니다
    int dataSize = BLOCK_SIZE+2; // null 문자 포함

    // 메모리를 할당하고 문자열 데이터를 복사합니다
    uint8_t* mem = (uint8_t*)malloc(dataSize);
    memcpy(mem, data_kbytes, dataSize);


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

    //그림 파일에 대해 CRC처리된 CodeWord를 생성합니다.
    uint8_t** codeword = create_codeword();
    for(int i=0; i<row; i++){
        sendLargeString(hwndReceiver, codeword[i], i);
    }

    return 0;
}