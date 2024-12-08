#include "FileExplorer.h"

namespace FileExplorer {

    std::wstring Search() {
        auto WindowDeleter = [](HWND hwnd) { if (hwnd) DestroyWindow(hwnd); };
        std::unique_ptr<std::remove_pointer<HWND>::type, decltype(WindowDeleter)> OwnerWindow(
            CreateWindowExW(
                WS_EX_TOPMOST, L"STATIC", L"File Explorer", WS_POPUP,
                CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                NULL, NULL, NULL, NULL
            ), WindowDeleter
        );

        wchar_t FileName[MAX_PATH] = L"";
        OPENFILENAMEW Ofn = { 0 };
        Ofn.lStructSize = sizeof(Ofn);
        Ofn.hwndOwner = OwnerWindow.get();
        Ofn.lpstrFile = FileName;
        Ofn.nMaxFile = MAX_PATH;
        Ofn.lpstrFilter = L"All Files\0*.*\0";
        Ofn.lpstrTitle = L"Select a file";
        Ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (!GetOpenFileNameW(&Ofn))
            return L"";

        std::wstring SelectedFile = Ofn.lpstrFile;

        auto HandleDeleter = [](HANDLE handle) { if (handle != INVALID_HANDLE_VALUE && handle != NULL) CloseHandle(handle); };
        std::unique_ptr<std::remove_pointer<HANDLE>::type, decltype(HandleDeleter)> File(
            CreateFileW(
                SelectedFile.c_str(),
                GENERIC_READ,
                FILE_SHARE_READ,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL
            ), HandleDeleter
        );

        if (File.get() == INVALID_HANDLE_VALUE)
            return L"";

        wchar_t UserPath[MAX_PATH];
        if (!GetEnvironmentVariableW(L"USERPROFILE", UserPath, MAX_PATH))
            return L"";
        std::wstring BasePath = std::wstring(UserPath) + L"\\AppData\\Local\\Roblox\\Versions\\";

        WIN32_FIND_DATAW FindData;
        HANDLE HFind = FindFirstFileW((BasePath + L"version-*").c_str(), &FindData);
        if (HFind == INVALID_HANDLE_VALUE)
            return L"";

        std::wstring DestinationPath;
        do {
            if (!(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                continue;

            std::wstring VersionPath = BasePath + FindData.cFileName + L"\\";
            std::wstring ExePath = VersionPath + L"RobloxStudioBeta.exe";
            std::wstring ContentPath = VersionPath + L"content\\";

            if (GetFileAttributesW(ExePath.c_str()) == INVALID_FILE_ATTRIBUTES ||
                GetFileAttributesW(ContentPath.c_str()) == INVALID_FILE_ATTRIBUTES)
                continue;

            std::wstring Extension = SelectedFile.substr(SelectedFile.find_last_of(L"."));
            std::wstring RandomPrefix = Random::New(100);
            DestinationPath = ContentPath + RandomPrefix + Extension;

            if (CopyFileW(SelectedFile.c_str(), DestinationPath.c_str(), FALSE))
                break;

        } while (FindNextFileW(HFind, &FindData));
        FindClose(HFind);

        return DestinationPath.empty() ? L"" : DestinationPath;
    }

}
