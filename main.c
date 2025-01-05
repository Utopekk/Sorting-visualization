// Sorting Algorithm Visualizer
// This program visualizes sorting algorithms (Bubble Sort, Selection Sort, and Quick Sort)
// in a graphical interface using the Win32 API.

#include <windows.h>
#define WIDTH 700
#define HEIGHT 700

int values[WIDTH];
int sortChoice = 0;

void initializeValues() {
    for (int i = 0; i < WIDTH; i++) {
        values[i] = rand() % HEIGHT;
    }
}

void drawMenu(HDC hdc) {
    const char* menuText = "Press '1' for Bubble Sort\nPress '2' for Selection Sort\n Press '3' for Quick Sort\n";
    TextOut(hdc, 10, 10, menuText, strlen(menuText));
}

void draw(HDC backBufferDC,COLORREF color) {
    RECT clearRect = {0, 0, WIDTH, HEIGHT};
    FillRect(backBufferDC, &clearRect, (HBRUSH)(COLOR_WINDOW + 1));

    for (int i = 0; i < WIDTH; i+=2) {
        RECT rect = {i, HEIGHT - values[i], i + 2, HEIGHT};
        HBRUSH rbrush = CreateSolidBrush(color);
        FillRect(backBufferDC, &rect, rbrush);
        DeleteObject(rbrush);
    }
}

void updateVisualization(HDC hdc, HDC backBufferDC, HBITMAP backBufferBitmap) {
    draw(backBufferDC, RGB(0, 0, 255));
    BitBlt(hdc, 0, 0, WIDTH, HEIGHT, backBufferDC, 0, 0, SRCCOPY);
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void bubbleSort(HDC hdc, int arr[], int len) {
    HDC backBufferDC = CreateCompatibleDC(hdc);
    HBITMAP backBufferBitmap = CreateCompatibleBitmap(hdc, WIDTH, HEIGHT);
    SelectObject(backBufferDC, backBufferBitmap);

    for (int i = 0; i < len - 1; i++) {
        for (int j = 0; j < len - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j],&arr[j+1]);
            }
            updateVisualization(hdc, backBufferDC, backBufferBitmap);
        }

    }
    draw(backBufferDC, RGB(0, 255, 0));
    BitBlt(hdc, 0, 0, WIDTH, HEIGHT, backBufferDC, 0, 0, SRCCOPY);
    DeleteObject(backBufferBitmap);
    DeleteDC(backBufferDC);
}

void selectionSort(HDC hdc, int arr[], int len) {
    HDC backBufferDC = CreateCompatibleDC(hdc);
    HBITMAP backBufferBitmap = CreateCompatibleBitmap(hdc, WIDTH, HEIGHT);
    SelectObject(backBufferDC, backBufferBitmap);

    for (int i = 0; i < len; i++) {
        int current = i;
        for (int j = i + 1; j < len; j++) {
            if (arr[j] < arr[current]) {
                current = j;
                updateVisualization(hdc, backBufferDC, backBufferBitmap);
            }
        }
        swap(&arr[i], &arr[current]);


    }
    draw(backBufferDC, RGB(0, 255, 0));
    BitBlt(hdc, 0, 0, WIDTH, HEIGHT, backBufferDC, 0, 0, SRCCOPY);
    DeleteObject(backBufferBitmap);
    DeleteDC(backBufferDC);
}

int partition(HDC hdc, int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    HDC backBufferDC = CreateCompatibleDC(hdc);
    HBITMAP backBufferBitmap = CreateCompatibleBitmap(hdc, WIDTH, HEIGHT);
    SelectObject(backBufferDC, backBufferBitmap);

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
        updateVisualization(hdc, backBufferDC, backBufferBitmap);
    }

    swap(&arr[i + 1], &arr[high]);

    draw(backBufferDC, RGB(0, 0, 255));
    Sleep(5);
    BitBlt(hdc, 0, 0, WIDTH, HEIGHT, backBufferDC, 0, 0, SRCCOPY);

    DeleteObject(backBufferBitmap);
    DeleteDC(backBufferDC);

    return i + 1;
}

void quickSort(HDC hdc, int arr[], int low, int high) {
    if (low < high) {
        int pivot = partition(hdc, arr, low, high);

        quickSort(hdc, arr, low, pivot - 1);
        quickSort(hdc, arr, pivot + 1, high);
    }
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_KEYDOWN:
            if(wParam == '1'){
                sortChoice = 1;
                InvalidateRect(hWnd, NULL, TRUE);
            }
            else if(wParam == '2'){
                sortChoice = 2;
                InvalidateRect(hWnd, NULL, TRUE);
            }
            else if(wParam == '3'){
                sortChoice = 3;
                InvalidateRect(hWnd, NULL, TRUE);
            }
            break;
        case WM_CREATE: {
            initializeValues();
            return 0;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            drawMenu(hdc);
            if(sortChoice == 1){
                bubbleSort(hdc, values, WIDTH);
            }
            if(sortChoice == 2) {
                selectionSort(hdc, values, WIDTH);
            }
            if(sortChoice == 3) {
                quickSort(hdc, values, 0, WIDTH - 1);
                HDC backBufferDC = CreateCompatibleDC(hdc);
                HBITMAP backBufferBitmap = CreateCompatibleBitmap(hdc, WIDTH, HEIGHT);
                SelectObject(backBufferDC, backBufferBitmap);

                draw(backBufferDC, RGB(0, 255, 0));
                BitBlt(hdc, 0, 0, WIDTH, HEIGHT, backBufferDC, 0, 0, SRCCOPY);

                DeleteObject(backBufferBitmap);
                DeleteDC(backBufferDC);
            }

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
            WIDTH+100, HEIGHT+100,
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
