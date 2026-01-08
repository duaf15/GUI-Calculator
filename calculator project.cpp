#include <windows.h>

HWND hEdit;
int first = 0;
int second = 0;
char op = 0;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void UpdateEdit(int value)
{
    char buf[50];
    wsprintfA(buf, "%d", value);
    SetWindowTextA(hEdit, buf);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nShow)
{
    WNDCLASSA wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = "CalcApp";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClassA(&wc);

    HWND hwnd = CreateWindowA(
        "CalcApp", "Calculator",
        WS_OVERLAPPEDWINDOW,
        200, 200, 300, 350,
        NULL, NULL, hInst, NULL);

    ShowWindow(hwnd, nShow);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        hEdit = CreateWindowA("EDIT", "0",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
            20, 20, 240, 30,
            hwnd, NULL, NULL, NULL);

        int x = 20, y = 70, id = 1;

        for (int i = 1; i <= 9; i++)
        {
            char t[2] = { char('0' + i), 0 };
            CreateWindowA("BUTTON", t,
                WS_CHILD | WS_VISIBLE,
                x, y, 50, 40,
                hwnd, (HMENU)(INT_PTR)i, NULL, NULL);

            x += 60;
            if (i % 3 == 0) { x = 20; y += 50; }
        }

        CreateWindowA("BUTTON", "0",
            WS_CHILD | WS_VISIBLE,
            80, y, 50, 40,
            hwnd, (HMENU)(INT_PTR)10, NULL, NULL);

        const char* ops[] = { "+", "-", "*", "/" };
        for (int i = 0; i < 4; i++)
            CreateWindowA("BUTTON", ops[i],
                WS_CHILD | WS_VISIBLE,
                200, 70 + i * 50, 50, 40,
                hwnd, (HMENU)(INT_PTR)(11 + i), NULL, NULL);

        CreateWindowA("BUTTON", "=",
            WS_CHILD | WS_VISIBLE,
            140, y, 50, 40,
            hwnd, (HMENU)(INT_PTR)15, NULL, NULL);

        CreateWindowA("BUTTON", "C",
            WS_CHILD | WS_VISIBLE,
            20, y, 50, 40,
            hwnd, (HMENU)(INT_PTR)16, NULL, NULL);
    }
    break;

    case WM_COMMAND:
    {
        int id = LOWORD(wParam);

        if (id >= 1 && id <= 9)
        {
            first = first * 10 + id;
            UpdateEdit(first);
        }
        else if (id == 10)
        {
            first = first * 10;
            UpdateEdit(first);
        }
        else if (id >= 11 && id <= 14)
        {
            op = "+-*/"[id - 11];
            second = first;
            first = 0;
            UpdateEdit(0);
        }
        else if (id == 15)
        {
            int result = 0;
            if (op == '+') result = second + first;
            if (op == '-') result = second - first;
            if (op == '*') result = second * first;
            if (op == '/' && first != 0) result = second / first;

            first = result;
            UpdateEdit(result);
        }
        else if (id == 16)
        {
            first = second = 0;
            op = 0;
            UpdateEdit(0);
        }
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}




   