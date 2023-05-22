#include <windows.h>
#include "SENDER_CRC16CCITT.c"

#define TIMEOUT_DURATION 5000

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void sendLargeString(HWND hWnd, const uint8_t* data_kbytes, int index);
static int waitForResponse;
int currentIndex = 0;
int resendingCount = 0;
int totalResendingCount = 0;

int main()
{
    // 윈도우 클래스 등록
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "sender";
    RegisterClass(&wc);

    // 윈도우 생성
    HWND hwnd = CreateWindow(wc.lpszClassName, "sender",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, wc.hInstance, NULL);
    if (hwnd == NULL) {
        MessageBox(NULL, "failed create window", "error", MB_OK | MB_ICONERROR);
        return 1;
    }

    // 윈도우 표시
    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    HWND hwndReceiver = FindWindow("receiver", "receiver");
    if (hwndReceiver == NULL) {
        MessageBox(NULL, "recevier not executing", "error", MB_OK | MB_ICONERROR);
        return 1;
    }

    //그림 파일에 대해 CRC처리된 CodeWord를 생성합니다.
    uint8_t** codeword = create_codeword();
    // 메시지 루프
    MSG msg = { 0 };
    int totalDataSize = row; // 전체 데이터 개수
    waitForResponse = 0; //응답 대기 시그널
    while (currentIndex < totalDataSize) {
        if(currentIndex%20==0){
            if(resendingCount==0){
                resendingCount++;
            }
        }

        if(resendingCount==0){
            sendLargeString(hwndReceiver, codeword[currentIndex], currentIndex);
        }
        waitForResponse=1;
        
        DWORD startTime = GetTickCount(); // 현재 시간을 기록
        
        while (waitForResponse) {
            //메시지를 전송 받았으면
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);

                if(!waitForResponse){
                    currentIndex++;
                    if(currentIndex>=row){
                        printf("Received All\n");
                        break;
                    }
                    else{
                        printf("ok %d\n", currentIndex);
                        break;
                    }
                }
            }
            else{
                DWORD currentTime = GetTickCount();
                if (currentTime - startTime >= TIMEOUT_DURATION) {
                    // 타임아웃 발생
                    printf("Timeout occurred, resending message...\n");
                    sendLargeString(hwndReceiver, codeword[currentIndex], currentIndex);
                    totalResendingCount++;
                    startTime = GetTickCount(); // 새로운 시작 시간으로 갱신
                }
            }
        }
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_USER: {
            if((int)wParam==currentIndex+1){
                // 응답을 수신했으므로 다음 데이터를 전송할 수 있도록 플래그 변경
                waitForResponse = 0;
                resendingCount=0;
                printf("ACK received\n");
            }
            
            return 0;
        }
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

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
