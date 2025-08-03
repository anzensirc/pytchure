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

    std::string sql = "CREATE TABLE IF NOT EXISTS USERS (ID INTEGER PRIMARY KEY AUTOINCREMENT, USERNAME TEXT, PASSWORD TEXT);";
    sqlite3_exec(db, sql.c_str(), nullptr, 0, nullptr);

    std::string insert = "INSERT INTO USERS (USERNAME, PASSWORD) "
                         "SELECT 'admin', '1234' WHERE NOT EXISTS (SELECT 1 FROM USERS WHERE USERNAME='admin');";
    sqlite3_exec(db, insert.c_str(), nullptr, 0, nullptr);

    sql = "SELECT * FROM USERS WHERE USERNAME=? AND PASSWORD=?";
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

void CheckWindow(HWND hwnd, const char* name) {
    if (hwnd == NULL) {
        MessageBoxA(NULL, name, "CreateWindowA FAILED", MB_ICONERROR);
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            CheckWindow(CreateWindowA("STATIC", "Username:", WS_VISIBLE | WS_CHILD,
                                      20, 20, 80, 20, hwnd, NULL, NULL, NULL), "STATIC Username");

            hUsername = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
                                      100, 20, 150, 20, hwnd, (HMENU)ID_EDIT_USERNAME, NULL, NULL);
            CheckWindow(hUsername, "EDIT Username");

            CheckWindow(CreateWindowA("STATIC", "Password:", WS_VISIBLE | WS_CHILD,
                                      20, 60, 80, 20, hwnd, NULL, NULL, NULL), "STATIC Password");

            hPassword = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD,
                                      100, 60, 150, 20, hwnd, (HMENU)ID_EDIT_PASSWORD, NULL, NULL);
            CheckWindow(hPassword, "EDIT Password");

            CheckWindow(CreateWindowA("BUTTON", "Login", WS_VISIBLE | WS_CHILD,
                                      100, 100, 150, 25, hwnd, (HMENU)ID_BUTTON_LOGIN, NULL, NULL), "BUTTON");

            hOutput = CreateWindowA("STATIC", "", WS_VISIBLE | WS_CHILD,
                                    20, 140, 250, 20, hwnd, NULL, NULL, NULL);
            CheckWindow(hOutput, "STATIC Output");
            break;

        case WM_COMMAND:
            if (LOWORD(wParam) == ID_BUTTON_LOGIN) {
                char username[100], password[100];
                GetWindowTextA(hUsername, username, 100);
                GetWindowTextA(hPassword, password, 100);

                if (checkLogin(username, password)) {
                    SetWindowTextA(hOutput, "✅ Login berhasil!");
                } else {
                    SetWindowTextA(hOutput, "❌ Login gagal.");
                }
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProcA(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int main() {
    HINSTANCE hInst = GetModuleHandle(NULL);

    WNDCLASSA wc = {};
    wc.lpszClassName = "LoginWindowClass";
    wc.hInstance = hInst;
    wc.lpfnWndProc = WindowProc;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.hCursor = LoadCursorA(NULL, IDC_ARROW);
    wc.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClassA(&wc);

    HWND hwnd = CreateWindowA("LoginWindowClass", "Login GUI SQLite",
                              WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                              CW_USEDEFAULT, CW_USEDEFAULT, 320, 240,
                              NULL, NULL, hInst, NULL);

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessageA(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    return 0;
}
