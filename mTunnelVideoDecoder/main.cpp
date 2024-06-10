#include <iostream>
#include <string>
#include <windows.h>
#include <winuser.h>

#include "Decryptor.h"

std::string KeyWord = "";  // Write (part of) the name of the window you want to decode here 

static std::string title;
static int widthClient = 0;
static int heightClient = 0;


// Find the target window and calculate its position and size 
BOOL CALLBACK EnumWindowsProc(
        _In_ HWND   hwnd,
        _In_ LPARAM lParam
){
        auto cTxtLen = GetWindowTextLength(hwnd);
        auto pszMem = (PSTR)VirtualAlloc((LPVOID) NULL,(DWORD) (cTxtLen + 10), MEM_COMMIT, PAGE_READWRITE);
        GetWindowTextA(hwnd, pszMem, cTxtLen + 10);
        if (std::string(pszMem).find(KeyWord)!=std::string::npos) {
            RECT rcClient;
            GetClientRect(hwnd, &rcClient);
            auto wid = rcClient.right - rcClient.left;
            auto hei = rcClient.bottom - rcClient.top;
            if (wid >= widthClient && hei >= heightClient) {
                title = pszMem;
                widthClient = rcClient.right - rcClient.left;
                heightClient = rcClient.bottom - rcClient.top;
            }
        }
        VirtualFree(pszMem,(DWORD)cTxtLen + 10 ,MEM_DECOMMIT);
    return true;
}



int main(int argc, char* argv[]) {
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            KeyWord += argv[i];
            KeyWord += " ";
        }
        KeyWord.erase(KeyWord.end() - 1);
    }
    std::cout << "Target Window: " << KeyWord << std::endl;
    HWND hwnd = GetConsoleWindow();
    RECT rect = { 0, 0, 600, 400 };
    MoveWindow(hwnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);

    SetProcessDPIAware();

    EnumDesktopWindows(NULL, EnumWindowsProc, NULL);
    if (title == "" || widthClient == 0 || heightClient == 0) {
        std::cout << "Target not detected!" << std::endl;
        return -1;
    }
    std::cout << "title: " << title << "\twidth: " << widthClient << "\theight: " << heightClient << std::endl;

    start(title, widthClient, heightClient);

    return 0;
}