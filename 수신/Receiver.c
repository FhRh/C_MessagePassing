#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include "RECEIVER_CRC16CCITT.c"
int error_count=0;
int receiveCount=0;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int main()
{
    // 윈도우 클래스 등록
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "receiver";
    RegisterClass(&wc);

    // 윈도우 생성
    HWND hwnd = CreateWindow(wc.lpszClassName, "receiver",
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
            // 전송된 데이터가 있는지 확인하고 처리 결과를 출력합니다.
            if (pcds != NULL && pcds->cbData > 0 && pcds->lpData != NULL) {
                int index = (int)wParam;
                const uint8_t* data = (const uint8_t*)pcds->lpData;
                int datasize = pcds->cbData;

                //전송받은 데이터 출력
                // printf("index : %d\n", index);
                // for(int i=0; i<datasize; i++){
                //     printf("%x ", data[i]);
                // }
                
                // CRC검사 결과 출력
                // crc_check 계산값이 0이면 OK 출력 0이상이면 ERROR 출력
                int result = crc16_ccitt(data,datasize);
                if(result==1){
                    error_count++;
                }
                printf("index : %d %s ",index, result==0?"OK":"ERROR");
                printf("/ error_count : %d ", error_count);
                printf("/ receiveCount : %d\n",receiveCount);

                HWND hwndSender = FindWindow("sender", "sender");
                if (hwndSender == NULL) {
                    MessageBox(NULL, "sender not executing", "error", MB_OK | MB_ICONERROR);
                    return 1;
                }

                //10개 단위로 응답을 한번씩 안 보내는 로직 구현
                if(index%10==0){
                    if(receiveCount==0){
                        receiveCount++;
                    }
                    else{
                        receiveCount=0;
                    }
                }

                // 데이터를 두번째 받았거나, 10단위의 데이터가 아니면 응답 전송
                // 응답 데이터 전송
                if(receiveCount==0){
                    int wparam = index+1;
                    PostMessage(hwndSender, WM_USER, (WPARAM)wparam, 0);
                    printf("ACK sended\n");
                }
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