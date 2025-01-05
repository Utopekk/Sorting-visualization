// Sorting Algorithm Visualizer
// This program visualizes sorting algorithms (Bubble Sort, Selection Sort, Quick Sort, Merge Sort and Heap Sort)
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
    const char* menuText1 = "Press '1' for Bubble Sort";
    const char* menuText2 = "Press '2' for Selection Sort";
    const char* menuText3 = "Press '3' for Quick Sort";
    const char* menuText4 = "Press '4' for Merge Sort";
    const char* menuText5 = "Press '5' for Heap Sort";

    TextOut(hdc, 10, 10, menuText1, strlen(menuText1));
    TextOut(hdc, 10, 30, menuText2, strlen(menuText2));
    TextOut(hdc, 10, 50, menuText3, strlen(menuText3));
    TextOut(hdc, 10, 70, menuText4, strlen(menuText4));
    TextOut(hdc, 10, 90, menuText5, strlen(menuText5));
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

void updateVisualization(HDC hdc, HDC backBufferDC,COLORREF color) {
    draw(backBufferDC, color);
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
                updateVisualization(hdc, backBufferDC, RGB(0, 0, 255));
            }
        }
    }
    updateVisualization(hdc, backBufferDC, RGB(0, 225, 0));
    DeleteObject(backBufferBitmap);
    DeleteDC(backBufferDC);
}

void selectionSort(HDC hdc, int arr[], int len) {
    HDC backBufferDC = CreateCompatibleDC(hdc);
    HBITMAP backBufferBitmap = CreateCompatibleBitmap(hdc, WIDTH, HEIGHT);
    SelectObject(backBufferDC, backBufferBitmap);

    for (int i = 0; i < len - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < len; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            swap(&arr[i], &arr[minIndex]);
            updateVisualization(hdc, backBufferDC, RGB(0, 0, 255));
            Sleep(1); // Add a small delay to reduce lag
        }
    }
    updateVisualization(hdc, backBufferDC, RGB(0, 255, 0));
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
        updateVisualization(hdc, backBufferDC, RGB(0, 0, 255));
    }

    swap(&arr[i + 1], &arr[high]);
    updateVisualization(hdc, backBufferDC, RGB(0, 0, 255));

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

void merge(HDC hdc, int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;

    HDC backBufferDC = CreateCompatibleDC(hdc);
    HBITMAP backBufferBitmap = CreateCompatibleBitmap(hdc, WIDTH, HEIGHT);
    SelectObject(backBufferDC, backBufferBitmap);

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
        updateVisualization(hdc, backBufferDC, RGB(0, 0, 255));
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
        updateVisualization(hdc, backBufferDC, RGB(0, 0, 255));
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
        updateVisualization(hdc, backBufferDC, RGB(0, 0, 255));
    }

    DeleteObject(backBufferBitmap);
    DeleteDC(backBufferDC);
}

void mergeSort(HDC hdc, int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        mergeSort(hdc, arr, l, m);
        mergeSort(hdc, arr, m + 1, r);

        merge(hdc, arr, l, m, r);
    }
}

void heapify(HDC hdc, int arr[], int len, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < len && arr[left] > arr[largest]) {
        largest = left;
    }

    if (right < len && arr[right] > arr[largest]) {
        largest = right;
    }

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        updateVisualization(hdc, CreateCompatibleDC(hdc), RGB(0, 0, 255));
        heapify(hdc, arr, len, largest);
    }
}

void heapSort(HDC hdc, int arr[], int len) {
    HDC backBufferDC = CreateCompatibleDC(hdc);
    HBITMAP backBufferBitmap = CreateCompatibleBitmap(hdc, WIDTH, HEIGHT);
    SelectObject(backBufferDC, backBufferBitmap);

    for (int i = len / 2 - 1; i >= 0; i--) {
        heapify(hdc, arr, len, i);
    }

    for (int i = len - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        updateVisualization(hdc, backBufferDC, RGB(0, 0, 255));
        Sleep(1); // Add a small delay to reduce lag
        heapify(hdc, arr, i, 0);
    }

    updateVisualization(hdc, backBufferDC, RGB(0, 255, 0));
    DeleteObject(backBufferBitmap);
    DeleteDC(backBufferDC);
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
            else if(wParam == '4'){
                sortChoice = 4;
                InvalidateRect(hWnd, NULL, TRUE);
            }
            else if(wParam == '5'){
                sortChoice = 5;
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

                updateVisualization(hdc, backBufferDC, RGB(0, 255, 0));

                DeleteObject(backBufferBitmap);
                DeleteDC(backBufferDC);
            }
            if(sortChoice == 4) {
                mergeSort(hdc, values, 0, WIDTH - 1);
                HDC backBufferDC = CreateCompatibleDC(hdc);
                HBITMAP backBufferBitmap = CreateCompatibleBitmap(hdc, WIDTH, HEIGHT);
                SelectObject(backBufferDC, backBufferBitmap);

                updateVisualization(hdc, backBufferDC, RGB(0, 255, 0));

                DeleteObject(backBufferBitmap);
                DeleteDC(backBufferDC);
            }
            if(sortChoice == 5) {
                heapSort(hdc, values, WIDTH);
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
