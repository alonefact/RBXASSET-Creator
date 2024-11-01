#include <iostream>
#include <windows.h>
#include <vector>
#include "FileExplorer.h"
#include "Clipboard.h"

int main() {
    SetConsoleTitle(L"RBXASSET Creator");

    std::vector<std::wstring> ConvertedAssets;

    while (true) {
        system("cls");

        std::cout << "Press Enter to Import a New File..." << std::endl;
        std::cout << std::endl;

        for (const auto& Asset : ConvertedAssets) {
            std::wcout << Asset << std::endl;
        }

        std::cin.get();

        HWND ConsoleWindow = GetConsoleWindow();
        ShowWindow(ConsoleWindow, SW_MINIMIZE);

        std::wstring CopiedFilePath = FileExplorer::Search();

        ShowWindow(ConsoleWindow, SW_RESTORE);

        if (!CopiedFilePath.empty()) {
            std::wstring FileName = CopiedFilePath.substr(CopiedFilePath.find_last_of(L"\\") + 1);
            std::wstring RbxAsset = L"rbxasset://" + FileName;

            ConvertedAssets.push_back(RbxAsset);

            std::wcout << RbxAsset << std::endl;

            Clipboard::Copy(RbxAsset);
            std::wcout << L"Copied to Clipboard!" << std::endl;
        }
        else {
            std::cout << "Failed to Create Rbxasset." << std::endl;
            return 0;
        }
    }

    return 0;
}