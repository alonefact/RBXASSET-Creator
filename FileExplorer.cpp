#include "FileExplorer.h"
#include "Random.h"
#include <windows.h>
#include <commdlg.h>
#include <string>
#include <random>

namespace FileExplorer {
    std::wstring Search() {
        wchar_t FileName[MAX_PATH] = L"";
        HWND HwndOwner = CreateWindowEx(WS_EX_TOPMOST, L"STATIC", L"File Explorer", WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, NULL, NULL);

        OPENFILENAMEW Ofn = { 0 };
        Ofn.lStructSize = sizeof(Ofn);
        Ofn.hwndOwner = HwndOwner;
        Ofn.lpstrFile = FileName;
        Ofn.nMaxFile = MAX_PATH;
        Ofn.lpstrFilter = L"All Files\0*.*\0";
        Ofn.lpstrTitle = L"Select a file";
        Ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetOpenFileNameW(&Ofn)) {
            std::wstring SelectedFile = Ofn.lpstrFile;
            HANDLE FileHandle = CreateFileW(SelectedFile.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

            if (FileHandle != INVALID_HANDLE_VALUE) {
                wchar_t UserPath[MAX_PATH];
                if (GetEnvironmentVariableW(L"USERPROFILE", UserPath, MAX_PATH)) {
                    std::wstring BasePath = std::wstring(UserPath) + L"\\AppData\\Local\\Roblox\\Versions\\";
                    WIN32_FIND_DATAW FindData;
                    HANDLE hFind = FindFirstFileW((BasePath + L"version-*").c_str(), &FindData);

                    if (hFind != INVALID_HANDLE_VALUE) {
                        do {
                            if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                                std::wstring VersionFolderPath = BasePath + FindData.cFileName + L"\\";
                                std::wstring ExePath = VersionFolderPath + L"RobloxStudioBeta.exe";

                                if (GetFileAttributesW(ExePath.c_str()) != INVALID_FILE_ATTRIBUTES) {
                                    std::wstring ContentFolderPath = VersionFolderPath + L"content\\";
                                    if (GetFileAttributesW(ContentFolderPath.c_str()) != INVALID_FILE_ATTRIBUTES) {
                                        std::wstring RandomPrefix = Random::New(100);
                                        std::wstring DestinationPath = ContentFolderPath + RandomPrefix +
                                                                        SelectedFile.substr(SelectedFile.find_last_of(L"."));

                                        if (CopyFileW(SelectedFile.c_str(), DestinationPath.c_str(), FALSE)) {
                                            FindClose(hFind);
                                            CloseHandle(FileHandle);
                                            DestroyWindow(HwndOwner);
                                            return DestinationPath;
                                        }
                                    }
                                }
                            }
                        } while (FindNextFileW(hFind, &FindData));
                        FindClose(hFind);
                    }
                }
                CloseHandle(FileHandle);
            }
        }

        DestroyWindow(HwndOwner);
        return L"";
    }
}