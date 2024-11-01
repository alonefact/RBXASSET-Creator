#include "Clipboard.h"
#include <windows.h>
#include <iostream>

// Annoying error handling because it randomly errors??

namespace Clipboard {

    void Copy(const std::wstring& text) {
        HGLOBAL hGlob = GlobalAlloc(GMEM_MOVEABLE, (text.size() + 1) * sizeof(wchar_t));
        if (!hGlob) {
            std::cerr << "GlobalAlloc failed!" << std::endl;
            return;
        }

        wchar_t* pText = static_cast<wchar_t*>(GlobalLock(hGlob));
        if (!pText) {
            std::cerr << "GlobalLock failed!" << std::endl;
            GlobalFree(hGlob);
            return;
        }

        wcscpy_s(pText, text.size() + 1, text.c_str());
        GlobalUnlock(hGlob);

        if (OpenClipboard(NULL)) {
            EmptyClipboard();
            if (SetClipboardData(CF_UNICODETEXT, hGlob) == NULL) {
                std::cerr << "SetClipboardData failed!" << std::endl;
                GlobalFree(hGlob);
            }
            CloseClipboard();
        }
        else {
            std::cerr << "OpenClipboard failed!" << std::endl;
            GlobalFree(hGlob);
        }
    }
}