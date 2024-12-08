#include "FileExplorer.h"

namespace FileExplorer {

    std::string Search() {
        auto WindowDeleter = [](HWND hwnd) {
            if (hwnd) DestroyWindow(hwnd);
            };

        std::unique_ptr<std::remove_pointer<HWND>::type, decltype(WindowDeleter)> OwnerWindow(
            CreateWindowExA(
                WS_EX_TOPMOST, "STATIC", "File Explorer", WS_POPUP,
                CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                NULL, NULL, NULL, NULL
            ),
            WindowDeleter
        );

        char FileName[MAX_PATH] = "";

        OPENFILENAMEA Ofn = { 0 };
        Ofn.lStructSize = sizeof(Ofn);
        Ofn.hwndOwner = OwnerWindow.get();
        Ofn.lpstrFile = FileName;
        Ofn.nMaxFile = MAX_PATH;
        Ofn.lpstrFilter = "All Files\0*.*\0";
        Ofn.lpstrTitle = "Select a file";
        Ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (!GetOpenFileNameA(&Ofn))
            return "";

        std::string SelectedFile = Ofn.lpstrFile;

        auto HandleDeleter = [](HANDLE handle) {
            if (handle != INVALID_HANDLE_VALUE && handle != NULL) CloseHandle(handle);
            };

        std::unique_ptr<std::remove_pointer<HANDLE>::type, decltype(HandleDeleter)> File(
            CreateFileA(
                SelectedFile.c_str(),
                GENERIC_READ,
                FILE_SHARE_READ,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL
            ),
            HandleDeleter
        );

        if (File.get() == INVALID_HANDLE_VALUE)
            return "";

        char UserPath[MAX_PATH];
        if (!GetEnvironmentVariableA("USERPROFILE", UserPath, MAX_PATH))
            return "";

        std::string BasePath = std::string(UserPath) + "\\AppData\\Local\\Roblox\\Versions\\";

        WIN32_FIND_DATAA FindData;
        HANDLE HFind = FindFirstFileA((BasePath + "version-*").c_str(), &FindData);
        if (HFind == INVALID_HANDLE_VALUE)
            return "";

        std::string DestinationPath;
        do {
            if (!(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                continue;

            std::string VersionPath = BasePath + FindData.cFileName + "\\";
            std::string ExePath = VersionPath + "RobloxStudioBeta.exe";
            std::string ContentPath = VersionPath + "content\\";

            if (GetFileAttributesA(ExePath.c_str()) == INVALID_FILE_ATTRIBUTES ||
                GetFileAttributesA(ContentPath.c_str()) == INVALID_FILE_ATTRIBUTES)
                continue;

            size_t extPos = SelectedFile.find_last_of(".");
            std::string Extension = (extPos != std::string::npos) ? SelectedFile.substr(extPos) : "";

            std::string RandomPrefix = Random::New(100);
            DestinationPath = ContentPath + RandomPrefix + Extension;

            if (CopyFileA(SelectedFile.c_str(), DestinationPath.c_str(), FALSE))
                break;

        } while (FindNextFileA(HFind, &FindData));

        FindClose(HFind);

        return DestinationPath.empty() ? "" : DestinationPath;
    }

}
