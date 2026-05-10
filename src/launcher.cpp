#include <windows.h>
#include <shellapi.h>
#include <cstring>

#define WINDOW_W 450
#define WINDOW_H 350

#define ID_SNAKE   1001
#define ID_SHOOTER 1002
#define ID_EXIT    1003

static const COLORREF YELLOW_BG  = RGB(255, 215, 0);
static const COLORREF DARK_GOLD  = RGB(92, 74, 10);

static HBRUSH g_brushBg = NULL;

// -------------------------------------------------------------------
// Launch a game exe (same dir, fallback to .\bin\)
// -------------------------------------------------------------------
static void launch_game(const char *exe)
{
    char dir[MAX_PATH], path[MAX_PATH];
    GetModuleFileNameA(NULL, dir, MAX_PATH);
    char *p = strrchr(dir, '\\');
    if (p) *p = '\0';

    wsprintfA(path, "%s\\%s", dir, exe);
    if (GetFileAttributesA(path) == INVALID_FILE_ATTRIBUTES)
        wsprintfA(path, "%s\\bin\\%s", dir, exe);

    ShellExecuteA(NULL, "open", path, NULL, dir, SW_SHOW);
}

// -------------------------------------------------------------------
// Window procedure
// -------------------------------------------------------------------
static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM w, LPARAM lp)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        static const struct {
            const char *text;
            int id;
            int y, h;
            int font_sz;
            int bold;
        } items[] = {
            // labels
            { "Game Launcher",       0,      30, 38, 26, 1 },
            { "Select a game to play:", 0,   75, 20, 13, 0 },
            // buttons
            { "Snake Game",   ID_SNAKE,   125, 45, 16, 1 },
            { "Shooter Game", ID_SHOOTER, 190, 45, 16, 1 },
            { "Exit",         ID_EXIT,    270, 35, 12, 0 },
        };

        for (auto &it : items)
        {
            int cx = (it.id == 0) ? 0 : 125;
            int cw = (it.id == 0) ? WINDOW_W : 200;
            if (it.id == ID_EXIT) { cx = 165; cw = 120; }

            HFONT f = CreateFontA(it.font_sz, 0, 0, 0,
                        it.bold ? FW_BOLD : FW_NORMAL,
                        0, 0, 0, DEFAULT_CHARSET,
                        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");

            HWND ctl = CreateWindowA(
                it.id ? "BUTTON" : "STATIC",
                it.text,
                (it.id ? WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
                       : WS_CHILD | WS_VISIBLE | SS_CENTER),
                cx, it.y, cw, it.h,
                hwnd, (HMENU)(INT_PTR)it.id, NULL, NULL);
            SendMessage(ctl, WM_SETFONT, (WPARAM)f, TRUE);
        }
        break;
    }

    case WM_CTLCOLORSTATIC:
    {
        HDC dc = (HDC)w;
        SetBkColor(dc, YELLOW_BG);
        SetTextColor(dc, DARK_GOLD);
        return (LRESULT)g_brushBg;
    }

    case WM_CTLCOLORBTN:
    {
        HDC dc = (HDC)w;
        SetBkColor(dc, YELLOW_BG);
        SetDCBrushColor(dc, YELLOW_BG);
        return (LRESULT)GetStockObject(DC_BRUSH);
    }

    case WM_COMMAND:
        switch (LOWORD(w))
        {
        case ID_SNAKE:   launch_game("dyg.exe");    break;
        case ID_SHOOTER: launch_game("shooter.exe"); break;
        case ID_EXIT:    PostQuitMessage(0);         break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, w, lp);
    }
    return 0;
}

// -------------------------------------------------------------------
// Entry point
// -------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nShow)
{
    g_brushBg = CreateSolidBrush(YELLOW_BG);

    const char CLASS[] = "GameLauncherClass";
    WNDCLASS wc = {};
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInst;
    wc.hbrBackground = g_brushBg;
    wc.lpszClassName = CLASS;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClassA(&wc);

    int x = (GetSystemMetrics(SM_CXSCREEN) - WINDOW_W) / 2;
    int y = (GetSystemMetrics(SM_CYSCREEN) - WINDOW_H) / 2;

    HWND hwnd = CreateWindowExA(0, CLASS, "Game Launcher",
                    WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                    x, y, WINDOW_W, WINDOW_H,
                    NULL, NULL, hInst, NULL);
    if (!hwnd) return 1;

    ShowWindow(hwnd, nShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    DeleteObject(g_brushBg);
    return 0;
}
