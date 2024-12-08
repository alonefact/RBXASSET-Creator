#include "Clipboard.h"

void Clipboard::Copy(const std::string& text) {
    if (!OpenClipboard(NULL)) {
        return;
    }

    EmptyClipboard();

    size_t size = (text.size() + 1) * sizeof(char);
    HGLOBAL hGlob = GlobalAlloc(GMEM_MOVEABLE, size);
    if (!hGlob) {
        CloseClipboard();
        return;
    }

    char* pText = static_cast<char*>(GlobalLock(hGlob));
    if (pText) {
        strcpy_s(pText, text.size() + 1, text.c_str());
        GlobalUnlock(hGlob);

        if (SetClipboardData(CF_TEXT, hGlob) == NULL) {
            GlobalFree(hGlob);
        }
    }

    CloseClipboard();
}
