#include <iostream>
#include <windows.h>
#include <vector>
#include "FileExplorer.h"
#include "Clipboard.h"

int main() {
    SetConsoleTitleA("RBXASSET Creator");

    std::vector<std::string> ConvertedAssets;

    while (true) {
        system("cls");

        std::cout << "Press Enter to Import a New File..." << std::endl;
        std::cout << std::endl;

        for (const auto& Asset : ConvertedAssets) {
            std::cout << Asset << std::endl;
        }

        std::cin.get();

        HWND ConsoleWindow = GetConsoleWindow();
        ShowWindow(ConsoleWindow, SW_MINIMIZE);

        std::string CopiedFilePath = FileExplorer::Search();

        ShowWindow(ConsoleWindow, SW_RESTORE);

        if (!CopiedFilePath.empty()) {
            std::string FileName = CopiedFilePath.substr(CopiedFilePath.find_last_of("\\") + 1);
            std::string RbxAsset = "rbxasset://" + FileName;

            ConvertedAssets.push_back(RbxAsset);

            std::cout << RbxAsset << std::endl;

            Clipboard::Copy(RbxAsset);
            std::cout << "Copied to Clipboard!" << std::endl;
        }
        else {
            std::cout << "Failed to Create Rbxasset." << std::endl;
            return 0;
        }
    }

    return 0;
}
