#include <windows.h>
#include <sqlite3.h>
#include <string>

#define ID_EDIT_USERNAME 101
#define ID_EDIT_PASSWORD 102
#define ID_BUTTON_LOGIN  103

HWND hUsername, hPassword, hOutput;

bool checkLogin(const std::string& username, const std::string& password) {
    sqlite3* db;
    sqlite3_stmt* stmt;
    bool success = false;

    int rc = sqlite3_open("users.db", &db);
    if (rc != SQLITE_OK) return false;

    std::string sql = "SELECT * FROM USERS WHERE USERNAME=? AND PASSWORD=?";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);

    if (rc == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        success = true;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return success;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            CreateWindow("STATIC", "Username:", WS_VISIBLE | WS_CHILD,
                         20, 20, 80, 20, hwnd, NULL, NULL, NULL);
            hUsername = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
                                     100, 20, 150, 20, hwnd, (HMENU)ID_EDIT_USERNAME, NULL, NULL);

            CreateWindow("STATIC", "Password:", WS_VISIBLE | WS_CHILD,
                         20, 60, 80, 20, hwnd, NULL, NULL, NULL);
            hPassword = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD,
                                     100, 60, 150, 20, hwnd, (HMENU)ID_EDIT_PASSWORD, NULL, NULL);

            CreateWindow("BUTTON", "Login", WS_VISIBLE | WS_CHILD,
                         100, 100, 150, 25, hwnd, (HMENU)ID_BUTTON_LOGIN, NULL, NULL);

            hOutput = CreateWindow("STATIC", "", WS_VISIBLE | WS_CHILD,
                                   20, 140, 250, 20, hwnd, NULL, NULL, NULL);
            break;

        case WM_COMMAND:
            if (LOWORD(wParam) == ID_BUTTON_LOGIN) {
                char username[100], password[100];
                GetWindowText(hUsername, username, 100);
                GetWindowText(hPassword, password, 100);

                if (checkLogin(username, password)) {
                    SetWindowText(hOutput, "✅ Login berhasil!");
                } else {
                    SetWindowText(hOutput, "❌ Login gagal.");
                }
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow) {
    WNDCLASS wc = {};
    wc.lpszClassName = "LoginWindowClass";
    wc.hInstance = hInst;
    wc.lpfnWndProc = WindowProc;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    RegisterClass(&wc);

    HWND hwnd = CreateWindow("LoginWindowClass", "Login GUI SQLite",
                             WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
                             100, 100, 320, 240,
                             NULL, NULL, hInst, NULL);

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
