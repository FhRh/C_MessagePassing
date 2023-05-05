#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include "CRC16CCITT.c"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int main()
{
    // 윈도우 클래스 등록
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "class_name";
    RegisterClass(&wc);

    // 윈도우 생성
    HWND hwnd = CreateWindow(wc.lpszClassName, "receiver_name",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, wc.hInstance, NULL);
    if (hwnd == NULL) {
        MessageBox(NULL, "failed create window", "error", MB_OK | MB_ICONERROR);
        return 1;
    }
     

    // 윈도우 표시
    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    // 메시지 루프
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_COPYDATA: {
            COPYDATASTRUCT* pcds = (COPYDATASTRUCT*)lParam;
            // 전송된 데이터가 있는지 확인하고 출력합니다
            if (pcds != NULL && pcds->cbData > 0 && pcds->lpData != NULL) {
                int index = (int)wParam;
                const char* data = (const char*)pcds->lpData;
                int datasize = pcds->cbData;

                // 전달받은 문자열 출력
                // printf("%s\n", data);

                // 문자열을 1바이트 단위로 쪼개어 uint8_t배열로 저장
                uint8_t codeword_byte[datasize];
                for(int i=0; i<datasize; i++){
                    codeword_byte[i]=(uint8_t)data[i];
                }
                
                // CRC검사 결과 출력
                // crc_check 계산값이 0이면 OK 출력 0이상이면 ERROR 출력
                printf("%s\n", crc16_ccitt(data,datasize)==0?"OK":"ERROR");
            }

            // 메모리를 해제합니다
            if (pcds != NULL && pcds->lpData == NULL) {
                MessageBox(NULL, "Received All Data", "error", MB_OK | MB_ICONERROR);
                free(pcds->lpData);
            }

            return 0;
        }
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}