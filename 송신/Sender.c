#include <windows.h>

int main()
{
    HWND hwndReceiver = FindWindow("class_name", "receiver_name");
    if (hwndReceiver == NULL) {
        MessageBox(NULL, "recevier not executing", "error", MB_OK | MB_ICONERROR);
        return 1;
    }

    // 보낼 메시지
    UINT msg = WM_USER + 1;
    WPARAM wParam = 0;
    LPARAM lParam = 0;

    // 메시지 보내기
    LRESULT result = PostMessage(hwndReceiver, msg, wParam, lParam);
    if (result == 0) {
        MessageBox(NULL, "failed send", "error", MB_OK | MB_ICONERROR);
        return 1;
    }

    return 0;
}