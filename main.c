#include <windows.h>
#define WIDTH 200
#define HEIGHT 500

int values[WIDTH];
void initializeValues() {
    for (int i = 0; i < WIDTH; i++) {
        values[i] = rand() % WIDTH;
    }
}

void draw(HDC backBufferDC) {
    RECT clearRect = {0, 0, WIDTH, HEIGHT};
    FillRect(backBufferDC, &clearRect, (HBRUSH)(COLOR_WINDOW + 1));

    for (int i = 0; i < WIDTH; i++) {
        RECT rect = {i, HEIGHT - values[i], i + 1, HEIGHT};
        HBRUSH rbrush = CreateSolidBrush(RGB(0, 0, 255));
        FillRect(backBufferDC, &rect, rbrush);
        DeleteObject(rbrush);
    }
}
void bubbleSort(HDC hdc, int arr[], int len) {
    HDC backBufferDC = CreateCompatibleDC(hdc);
    HBITMAP backBufferBitmap = CreateCompatibleBitmap(hdc, WIDTH, HEIGHT);
    SelectObject(backBufferDC, backBufferBitmap);

    for (int i = 0; i < len - 1; i++) {
        for (int j = 0; j < len - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;

                draw(backBufferDC);

                BitBlt(hdc, 0, 0, WIDTH, HEIGHT, backBufferDC, 0, 0, SRCCOPY);
            }
        }
    }
    DeleteObject(backBufferBitmap);
    DeleteDC(backBufferDC);
}
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            initializeValues();
            return 0;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            bubbleSort(hdc, values, WIDTH);
            draw(hdc);

            EndPaint(hWnd, &ps);
            return 0;
        }

        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char* CLASS_NAME = "Sorting Algorithm Visualizer";

    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    HWND hWnd = CreateWindowEx(
            0,
            CLASS_NAME,
            "Sorting Algorithm Visualizer",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            WIDTH, HEIGHT,
            NULL, NULL,
            hInstance, NULL
    );
    ShowWindow(hWnd, nCmdShow);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}